#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

extern std::string get_content(const std::string &url);

std::string flower_query(const int64_t &group_id, const std::string &flower_msg, const std::string &server);