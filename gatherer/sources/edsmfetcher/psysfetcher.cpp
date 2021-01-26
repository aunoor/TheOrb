#include "psysfetcher.h"

#include "psysparser.h"

#include <cstdint>
#include <string>
#include <vector>

#define DEFL_CHUNK 1024*1024

static std::string contentLength( "Content-Length:");

//--------------------------------------------------------------------------------------------------------------------//

size_t read_header(char* header, size_t size, size_t nmemb, void *userdata) {
    auto sysFetcher = static_cast<PSysFetcher*>(userdata);
    return sysFetcher->readHeader(header, size, nmemb);
}

//--------------------------------------------------------------------------------------------------------------------//

size_t write_func(void *data, size_t size, size_t nmemb, void *userdata) {
    auto sysFetcher = static_cast<PSysFetcher*>(userdata);
    return sysFetcher->writeFunc(data, size, nmemb);
}

//--------------------------------------------------------------------------------------------------------------------//
int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    auto sysFetcher = static_cast<PSysFetcher*>(userdata);
    sysFetcher->progressCallback(dltotal, dlnow);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------//

PSysFetcher::PSysFetcher() {
    m_fileSize = -1;
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysFetcher::GetJSON(std::string &json) {
    json = m_json;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysFetcher::FetchPopulatedSystems() {

    needDlBreak = false;
    bool res = initZlib();
    if (!res) {
        return false;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    CURLcode curlRes;
    //curl_easy_setopt(curl, CURLOPT_URL, "https://www.edsm.net/dump/systemsPopulated.json.gz");
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/systemsPopulated.json.gz");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, read_header);

    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    m_psysParser = new PSysParser();
    m_psysParser->StartParse();

    curlRes = curl_easy_perform(curl);
//    if (curlRes != CURLE_OK) {
//        break;
//    }

    inflateEnd(&m_strm);
    curl_easy_cleanup(curl);

    char c = 0;
    m_psysParser->AddData(&c, 1);
    delete m_psysParser;

//    if (curlRes == CURLE_OK) {
//        unpackJsonFile();
//    }

    //TODO: check for errors

    return (curlRes == CURLE_OK);
}

//--------------------------------------------------------------------------------------------------------------------//
size_t PSysFetcher::readHeader(char *header, size_t size, size_t nmemb) {
    size_t realSize = size * nmemb;
    std::string hdr(header, realSize);
    std::size_t found = hdr.find(contentLength);
    if (found == 0) {
        std::string header_in_string = hdr.replace(found, contentLength.size(), "");
        auto sizeValue = std::stoi(header_in_string);
        if (sizeValue > 0) {
            m_fileSize = sizeValue;
            m_fileData.reserve(sizeValue);
        }
    }
    return realSize;
}

//--------------------------------------------------------------------------------------------------------------------//

size_t PSysFetcher::writeFunc(void *data, size_t size, size_t nmemb) {
    if (needDlBreak) {
        return 0;
    }

    size_t realSize = size * nmemb;

    char out[DEFL_CHUNK];

    m_strm.avail_out = DEFL_CHUNK;
    m_strm.next_out = (Bytef*)out;

    m_strm.avail_in = realSize;
    m_strm.next_in = (Bytef*)data;

    int ret = inflate(&m_strm, Z_NO_FLUSH);
    if (ret == Z_STREAM_END) {
        ret = Z_OK;
        //return 0;
    }
    if (ret != Z_OK) {
        m_error = std::string(m_strm.msg);
        return 0;
    }
    auto actual = DEFL_CHUNK - m_strm.avail_out;

    m_psysParser->AddData(out, actual);

    //m_json += std::string(out, actual);

//    auto oldSize = m_fileData.size();
//    m_fileData.resize(oldSize + realSize);
//    memcpy(m_fileData.data()+oldSize, data, realSize);

    //return CURL_READFUNC_ABORT;
    return realSize;
}

//--------------------------------------------------------------------------------------------------------------------//

void PSysFetcher::progressCallback(int64_t dltotal, int64_t dlnow) {
    if (DownloadingProgress) {
        DownloadingProgress(dlnow, dltotal);
    }
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysFetcher::initZlib() {
    m_strm = z_stream();
    m_strm.zalloc = Z_NULL;
    m_strm.zfree = Z_NULL;
    m_strm.opaque = Z_NULL;
    m_strm.avail_in = 0;
    m_strm.next_in = Z_NULL;
    int ret = inflateInit2(&m_strm, 15 + 32);
    if (ret != Z_OK) {
        return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PSysFetcher::unpackJsonFile() {

    m_json.clear();

    char out[DEFL_CHUNK];

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = m_fileSize;
    strm.next_in = (Bytef*)m_fileData.data();
    //Need to set window size to enable gzip header detecting
    int ret = inflateInit2(&strm, 15 + 32);
    if (ret != Z_OK) {
        return false;
    }

    do {
        strm.avail_out = DEFL_CHUNK;
        strm.next_out = (Bytef*)out;

        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            ret = Z_OK;
            break;
        }
        if (ret != Z_OK) {
            m_error = std::string(strm.msg);
            break;
        }
        auto actual = DEFL_CHUNK - strm.avail_out;
        m_json += std::string(out, actual);
    } while (strm.avail_out == 0);
    inflateEnd(&strm);

    return (ret == Z_OK);
}
