#include "psysfetcher.h"

#include "psysparser.h"

#include <cstdint>
#include <string>
#include <vector>

#define DEFL_CHUNK 1024*1024

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
    needDlBreak = false;
    m_psysParser = nullptr;
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

//    curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
//    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, read_header);

    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    m_psysParser = new PSysParser();
    m_psysParser->SystemReceived = [this](StarSystem &system) {
        if (this->SystemReceived) {
            SystemReceived(system);
        }
    };
    m_psysParser->StartParse();

    curlRes = curl_easy_perform(curl);
//    if (curlRes != CURLE_OK) {
//        break;
//    }

    inflateEnd(&m_strm);
    curl_easy_cleanup(curl);
    delete m_psysParser;

    return (curlRes == CURLE_OK);
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
        return 0;
    }
    auto actual = DEFL_CHUNK - m_strm.avail_out;

    bool res = m_psysParser->AddData(out, actual);
    if (!res) {
        printf("Download canceled by AddData\n");
        return 0;
    }

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
