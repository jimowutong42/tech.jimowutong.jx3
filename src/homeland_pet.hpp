#pragma once

#include <map>
#include <sstream>
#include <string>

struct Furniture {
    std::string name;
    std::string type;
    int guanshang;
    int shiyong;
    int jiangu;
    int fengshui;
    int quwei;
};

std::string homeland_pet_query(const std::string& map_msg);
