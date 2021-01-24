#ifndef PSYSFETCHER_H
#define PSYSFETCHER_H

#include <cstdint>
#include <string>
#include <functional>
#include <vector>

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

    bool unpackJsonFile();

    friend size_t read_header(char* header, size_t size, size_t nitems, void *userdata);
    friend size_t write_func(void *data, size_t size, size_t nmemb, void *userp);
};


#endif //PSYSFETCHER_H
