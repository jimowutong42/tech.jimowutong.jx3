#include "flower.hpp"

struct hua {
    int max;
    // int min;
    std::vector<std::string> maxLine;
};

void from_json(const nlohmann::json& j, hua& h) {
    h.max = j.at("max").get<int>();
    // h.min = j.at("min").get<int>();
    h.maxLine = j.at("maxLine").get<std::vector<std::string>>();
}

std::string flower_query(const int64_t& group_id, const std::string& flower_msg, const std::string& server) {
    std::string msg = "";
    std::string xiuqiuhua = "绣球花";
    std::string yujinxiang = "郁金香";
    std::string qianniuhua = "牵牛花";
    std::string meigui = "玫瑰花";
    std::string baihe = "百合花";
    std::string yingguangjun = "荧光菌, 蘑菇";
    std::string flower = "";
    if (xiuqiuhua.find(flower_msg) != xiuqiuhua.npos) {
        flower = "绣球花";
    } else if (yujinxiang.find(flower_msg) != yujinxiang.npos) {
        flower = "郁金香";
    } else if (qianniuhua.find(flower_msg) != qianniuhua.npos) {
        flower = "牵牛花";
    } else if (meigui.find(flower_msg) != meigui.npos) {
        flower = "玫瑰";
    } else if (baihe.find(flower_msg) != baihe.npos) {
        flower = "百合";
    } else if (yingguangjun.find(flower_msg) != yingguangjun.npos) {
        flower = "荧光菌";
    } else {
        flower = "绣球花";
    }
    static std::string api = "https://next.jx3box.com/api/flower/price/rank?";
    std::string url = api + "server=" + server + "&flower=" + flower;
    auto res = get_content(url);
    cq::logging::debug("消息", "res: " + res + "\n");
    if (res.size() == 0) {
        cq::logging::warning("消息", "查询返回为空");
    }
    std::string hua1, hua2;
    int size = res.size(), start = 0, len = 0;
    for (int i = 0; i < size; i++) {
        if (res[i] == '\"') {
            if (start == 0) {
                start = i + 1;
            } else {
                len = i - start;
                break;
            }
        }
    }
    cq::logging::debug("消息", "start: " + cq::to_string(start) + "len: " + cq::to_string(len) + "\n");
    hua1.assign(res, start, len);
    hua2 = "二级" + hua1.substr(strlen("二级"), hua1.size() - strlen("二级"));
    cq::logging::debug("消息", "hua1: " + hua1 + "\n");
    cq::logging::debug("消息", "hua2: " + hua2 + "\n");
    auto j = nlohmann::json::parse(res);
    // std::cout << res.dump(4) << std::endl;
    try {
        auto h1 = j.at(hua1).get<hua>();
        auto h2 = j.at(hua2).get<hua>();
        msg = "收花线路-" + server + "\n" + hua1 + " 最高价" + cq::to_string(h1.max) + "\n" + h1.maxLine[0] + " " +
              h1.maxLine[1] + " " + h1.maxLine[2] + " " + h1.maxLine[3] + " " + h1.maxLine[4] + "\n" + hua2 +
              " 最高价" + cq::to_string(h2.max) + "\n" + h2.maxLine[0] + " " + h2.maxLine[1] + " " + h2.maxLine[2] +
              " " + h2.maxLine[3] + " " + h2.maxLine[4];
        // logging::debug("消息", "最高价: " + to_string(h1.max) + "\n");
        // logging::debug("消息", "收花线路: " + h1.maxLine[0] + " " + h1.maxLine[1] + " " + h1.maxLine[2] + " " +
        //                                      h1.maxLine[3] + " " + h1.maxLine[4] + "\n");
    } catch (nlohmann::json::exception& e) {
        cq::logging::error("json", cq::to_string(e.what()) + "; exception id: " + cq::to_string(e.id));
    }
    return msg;
}