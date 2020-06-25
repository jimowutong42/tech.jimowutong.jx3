#include "push.hpp"

std::string today() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t seasuntt = std::chrono::system_clock::to_time_t(now - std::chrono::hours(7));
    struct tm t;
    localtime_s(&t, &seasuntt);
    return (cq::to_string(t.tm_year + 1900) + '-' + cq::to_string(t.tm_mon + 1) + '-' + cq::to_string(t.tm_mday));
}

std::string get_card(const int64_t &qq) {
    std::map<int64_t, std::string> cards = {
        {544654867, "【纨石】"},
        {1946225331, "【炮太】"},
        {490897031, "【改改】"},
        {405460725, "【师姐】"},
        {913406536, "【北七】"},
        {2, "【test】"}
    };
    return cards[qq];
}

std::unordered_map<int64_t, std::unordered_map<std::string, std::string>> push_read() {
    std::unordered_map<int64_t, std::unordered_map<std::string, std::string>> res;
    auto dir = cq::get_app_directory() + "push.json";
    std::ifstream f_push(std::filesystem::u8path(dir));
    if (!f_push.is_open()) {
        cq::logging::error("push", "无法读取用户数据！");
    }
    try {
        auto j_push = nlohmann::json::parse(f_push);
        for (auto &[key, value] : j_push.items()) {
            std::unordered_map<std::string, std::string> r;
            for (auto &[k, v] : value.items()) {
                r[k] = v;
            }
            res[atoi(key.c_str())] = r;
        }
        f_push.close();
    } catch (nlohmann::json::exception &e) {
        cq::logging::error("json", cq::to_string(e.what()) + "; exception id: " + cq::to_string(e.id));
    }
    return res;
}

std::unordered_map<std::string, std::string> push_write(const int64_t &qq, const std::string &card,
                                                        const std::stringstream &sshour) {
    std::unordered_map<std::string, std::string> res;
    auto dir = cq::get_app_directory() + "push.json";
    std::ifstream if_push(std::filesystem::u8path(dir));
    if (!if_push.is_open()) {
        cq::logging::error("push", "无法读取用户数据！");
    }
    try {
        auto j_push = nlohmann::json::parse(if_push);
        for (auto &[key, value] : j_push.items()) {
            if (atoi(key.c_str()) == qq) {
                for (auto &[k, v] : value.items()) {
                    res[k] = v;
                }
                // res["card"] = card;
                if (res["last"] != today()) {
                    res["hour"] = sshour.str();
                    res["days"] = cq::to_string(atoi(res["days"].c_str()) + 1);
                    std::stringstream sshours;
                    sshours << (atof(res["hours"].c_str()) + atof(sshour.str().c_str()));
                    res["hours"] = sshours.str();
                    res["last"] = today();
                } else {
                    auto bk_hour = res["hour"];
                    res["hour"] = sshour.str();
                    std::stringstream sshours;
                    sshours << (atof(res["hours"].c_str()) - atof(bk_hour.c_str()) + atof(sshour.str().c_str()));
                    res["hours"] = sshours.str();
                }
                break;
            }
        }
        if (res.empty()) {
            res["card"] = card;
            res["hour"] = sshour.str();
            res["days"] = "1";
            res["hours"] = sshour.str();
            res["last"] = today();
        }
        if_push.close();
        std::ofstream of_push(std::filesystem::u8path(dir));
        if (!of_push.is_open()) {
            cq::logging::error("push", "无法读取用户数据！");
        }
        nlohmann::json jr(res);
        j_push[cq::to_string(qq)] = jr;
        of_push << j_push.dump(4) << std::endl;
        of_push.close();
    } catch (nlohmann::json::exception &e) {
        cq::logging::error("json", cq::to_string(e.what()) + "; exception id: " + cq::to_string(e.id));
    }
    return res;
}
