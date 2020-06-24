#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

// {
//     "1": {
//         "card": "test1",
//         "hour": "1",
//         "days": "2",
//         "hours": "3",
//         "last": "2020-6-23"
//     },
//     "3": {
//         "card": "test3",
//         "hour": "1",
//         "days": "1",
//         "hours": "1.1",
//         "last": "2020-6-23"
//     }
// }

std::string today();

std::string get_card(const int64_t &qq);

std::unordered_map<int64_t, std::unordered_map<std::string, std::string>> push_read();

std::unordered_map<std::string, std::string> push_write(const int64_t &qq, const std::string &card,
                                                        const std::stringstream &sshour);