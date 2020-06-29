#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <algorithm>
#include <nlohmann/json.hpp>
#include <sstream>

struct hua {
    int max;
    std::vector<std::string> maxLine;
};

struct item {
    std::string sub_name;
    nlohmann::json sub_json;
};

class Flower {
  public:
    std::string name;
    std::string nickname;
    Flower(std::string n = "") {
        nickname = n;
        name = get_name(nickname);
    }
    std::vector<item> items;

  private:
    std::string get_name(std::string nickname) {
        if (nickname == "") return "";
        const char *split = ",";
        char *res = NULL;
        auto p = strtok_s((char *)nickname.c_str(), split, &res);
        return cq::to_string(p);
    }
};

extern std::string get_content(const std::string &url);

std::string get_flower_content(const std::string& flower_msg, const std::string& server, Flower& flower);

std::string flower_query(const std::string &flower_msg, const std::string &server);

std::string flower_query_easy(const std::string &flower_msg, const std::string &server);