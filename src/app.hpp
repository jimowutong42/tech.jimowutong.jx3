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
#include "homeland_pet.hpp"
#include "push.hpp"

struct MYCONFIG {
    int Q_ME;
    int Q_qiqi;
    int G_beijiu;
    int G_nanqi;
    int G_test;
    int G_push;
} config;

std::map<int64_t, std::string> server;