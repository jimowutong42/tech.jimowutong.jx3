#pragma once

#include <cqcppsdk/cqcppsdk.h>

#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <sstream>
#include <string>

#include "curl.hpp"
#include "flower.hpp"

struct MYCONFIG {
    int MY_QQ;
    int G_beijiu;
    int G_nanqi;
    int G_test;
} config;

std::map<int64_t, std::string> server;