#include "my_curl.hpp"

std::string get_content(const std::string &url) {
    std::string body;
    auto appdir = cq::get_app_directory();
    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, appdir + "cacert.pem");
        auto receive = [](char *buf, size_t size, size_t count, void *data) {
            (*static_cast<std::string *>(data)) += std::string(buf, count);
            return size * count;
        };
        typedef size_t (*WriteFunction)(char *, size_t, size_t, void *);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<WriteFunction>(receive));
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
        auto res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cq::logging::error("curl", "curl_easy_perform() failed: " + cq::to_string(curl_easy_strerror(res)) + "\n");
        }
        curl_easy_cleanup(curl);
    }
    return body;
}