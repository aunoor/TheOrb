#include "psysfetcher.h"

#include <curl/curl.h>
#include <zlib.h>
#include <cstdint>
#include <string>
#include <vector>

#define DEFL_CHUNK 1024*1024

static std::string contentLength( "Content-Length:");

//--------------------------------------------------------------------------------------------------------------------//

size_t read_header(char* header, size_t size, size_t nmemb, void *userdata) {
    auto sysFetcher = static_cast<PSysFetcher*>(userdata);
    size_t realSize = size * nmemb;
    std::string hdr(header, realSize);
    std::size_t found = hdr.find(contentLength);
    if (found == 0) {
        std::string header_in_string = hdr.replace(found, contentLength.size(), "");
        auto sizeValue = std::stoi(header_in_string);
        if (sizeValue > 0) {
            sysFetcher->m_fileSize = sizeValue;
            sysFetcher->m_fileData.reserve(sizeValue);
        }
    }
    return realSize;
}

//--------------------------------------------------------------------------------------------------------------------//

size_t write_func(void *data, size_t size, size_t nmemb, void *userdata) {
    auto sysFetcher = static_cast<PSysFetcher*>(userdata);
    size_t realSize = size * nmemb;

    auto oldSize = sysFetcher->m_fileData.size();
    sysFetcher->m_fileData.resize(oldSize + realSize);
    memcpy(sysFetcher->m_fileData.data()+oldSize, data, realSize);

    if (sysFetcher->DownloadingProgress) {
        sysFetcher->DownloadingProgress(sysFetcher->m_fileData.size(), sysFetcher->m_fileSize);
    }

    return realSize;
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

    CURL *curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    CURLcode res;
    do {
        //curl_easy_setopt(curl, CURLOPT_URL, "https://www.edsm.net/dump/systemsPopulated.json.gz");
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/systemsPopulated.json.gz");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, read_header);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            break;
        }
    } while(false);

    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
        unpackJsonFile();
    }

    return (res == CURLE_OK);
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
