#ifndef PSYSFETCHER_H
#define PSYSFETCHER_H

#include "common/spaceobjects.h"

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

    std::function<void(int32_t current, int32_t length)> DownloadingProgress;
    std::function<void(StarSystem &system)> SystemReceived;

private:
    //Marker to break download process
    bool needDlBreak;

    z_stream m_strm{};

    PSysParser *m_psysParser;

    size_t writeFunc(void *data, size_t size, size_t nmemb);
    void progressCallback(int64_t dltotal, int64_t dlnow);
    //Function for initialize inflate process
    bool initZlib();

    friend size_t write_func(void *data, size_t size, size_t nmemb, void *userdata);
    friend int progress_callback(void *userdata, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};


#endif //PSYSFETCHER_H
