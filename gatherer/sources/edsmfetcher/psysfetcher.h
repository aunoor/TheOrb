#ifndef PSYSFETCHER_H
#define PSYSFETCHER_H

#include <curl/curl.h>
#include <zlib.h>

#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class PSysParser;

///Class for fetching EDMS populated systems json file
class PSysFetcher {
public:
    PSysFetcher();
    ///Fetch json file from EDSM
    bool FetchPopulatedSystems();
    ///Fill
    void GetJSON(std::string &json);

    std::function<void(int32_t current, int32_t length)> DownloadingProgress;

private:
    size_t m_fileSize;
    std::vector<char> m_fileData;
    std::string m_json;

    std::string m_error;

    bool needDlBreak;

    z_stream m_strm;

    PSysParser *m_psysParser;

    bool unpackJsonFile();

    size_t readHeader(char* header, size_t size, size_t nmemb);
    size_t writeFunc(void *data, size_t size, size_t nmemb);
    void progressCallback(int64_t dltotal, int64_t dlnow);

    bool initZlib();



    friend size_t read_header(char* header, size_t size, size_t nmemb, void *userdata);
    friend size_t write_func(void *data, size_t size, size_t nmemb, void *userdata);
    friend int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};


#endif //PSYSFETCHER_H
