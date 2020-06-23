#include "flower.hpp"

void from_json(const nlohmann::json& j, hua& h) {
    h.max = j.at("max").get<int>();
    h.maxLine = j.at("maxLine").get<std::vector<std::string>>();
}

Flower xiuqiuhua("绣球花");
Flower yujinxiang("郁金香");
Flower qianniuhua("牵牛花");
Flower meigui("玫瑰, 玫瑰花");
Flower baihe("百合, 百合花");
Flower yingguangjun("荧光菌, 蘑菇, 菌菇");

std::map<std::string, std::string> color = {
    {"一级绣球花", "(白/红/紫)"},     {"二级绣球花", "(粉/黄/蓝)"},
    {"一级郁金香", "(粉/红/黄)"},     {"二级郁金香", "(白/混)"},
    {"一级牵牛花", "(红/绯/紫)"},     {"二级牵牛花", "(黄/蓝)"},
    {"一级玫瑰", "(粉/红/橙/黄/蓝)"}, {"二级玫瑰", "(白/紫/黑)"},   {"三级玫瑰", "(绿/混)"},
    {"一级百合", "(白/粉/黄)"},       {"二级百合", "(橙/绿)"},
    {"一级荧光菌", "(白/红/黄)"},     {"二级荧光菌", "(蓝/紫)"}};

std::string flower_query(const std::string& flower_msg, const std::string& server) {
    std::stringstream msg;
    msg << "收花线路-" << server;
    Flower flower;
    if (xiuqiuhua.nickname.find(flower_msg) != xiuqiuhua.nickname.npos) {
        flower = xiuqiuhua;
    } else if (yujinxiang.nickname.find(flower_msg) != yujinxiang.nickname.npos) {
        flower = yujinxiang;
    } else if (qianniuhua.nickname.find(flower_msg) != qianniuhua.nickname.npos) {
        flower = qianniuhua;
    } else if (meigui.nickname.find(flower_msg) != meigui.nickname.npos) {
        flower = meigui;
    } else if (baihe.nickname.find(flower_msg) != baihe.nickname.npos) {
        flower = baihe;
    } else if (yingguangjun.nickname.find(flower_msg) != yingguangjun.nickname.npos) {
        flower = yingguangjun;
    } else {
        return "呐呐呐没有" + flower_msg + "这种花哦！";
    }
    static std::string api = "https://next.jx3box.com/api/flower/price/rank?";
    std::string url = api + "server=" + server + "&flower=" + flower.name;
    auto res = get_content(url);
    cq::logging::debug("花价", "res: " + res + "\n");
    if (res.size() == 0) {
        cq::logging::warning("花价", "查询返回为空");
    }
    auto j = nlohmann::json::parse(res);
    try {
        for (auto& [key, value] : j.items()) {
            flower.items.push_back({key, value});
        }
        for (auto& sh : flower.items) {
            auto h = j.at(sh.sub_name).get<hua>();
            msg << '\n' << sh.sub_name << color.find(sh.sub_name)->second << " 最高价" << h.max << '\n';
            if (h.maxLine.size() <= 5) {
                for (auto& i : h.maxLine) {
                    msg << i << " ";
                }
            } else {
                msg << (h.maxLine[0] + " " + h.maxLine[1] + " " + h.maxLine[2] + " " + h.maxLine[3] + " " +
                        h.maxLine[4]);
            }
        }
    } catch (nlohmann::json::exception& e) {
        cq::logging::error("json", cq::to_string(e.what()) + "; exception id: " + cq::to_string(e.id));
    }
    return msg.str();
}