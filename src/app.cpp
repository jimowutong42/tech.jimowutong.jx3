#include "app.hpp"

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;
using json = nlohmann::json;

CQ_INIT {
    on_enable([] {
        auto dir = get_app_directory() + "config.json";
        ifstream cfgfile(filesystem::u8path(dir));
        if (!cfgfile.is_open()) {
            logging::error("启用", "无法读取用户配置文件！");
        }
        auto conf = json::parse(cfgfile);
        config.Q_ME = conf["Q_ME"];
        config.Q_qiqi = conf["Q_qiqi"];
        config.G_beijiu = conf["G_beijiu"];
        config.G_nanqi = conf["G_nanqi"];
        config.G_test = conf["G_test"];
        config.G_push = conf["G_push"];
        server.insert(std::pair<int64_t, std::string>(config.G_beijiu, "绝代天骄"));
        server.insert(std::pair<int64_t, std::string>(config.G_nanqi, "天鹅坪"));
        server.insert(std::pair<int64_t, std::string>(config.G_test, "天鹅坪"));
        logging::info("启用", "剑网3小助手已启用");
    });

    on_private_message([](const MessageEvent &event) {                          //测试
        if (event.message.substr(0, (to_string(" 花价")).size()) == " 花价") {  // 更简易的花价查询
            auto flower_msg = event.message.substr((to_string(" 花价 ")).size(), size(event.message));
            try {
                string msg = flower_query_easy(flower_msg, server.find(config.G_test)->second);
                send_private_message(event.user_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "花价查询失败, 错误码: " + to_string(err.code));
            }
        }
    });

    // on_message([](const MessageEvent &event) {
    //     logging::debug("消息", "收到消息: " + event.message + "\n实际类型: " + typeid(event).name());
    // });

    on_group_message([](const GroupMessageEvent &event) {
        if (event.message == "查看签到表") {  // 查看签到表（TODO）
            static const set<int64_t> ENABLED_GROUPS = {config.G_beijiu, config.G_test};
            if (ENABLED_GROUPS.count(event.group_id) == 0) return;  // 不在启用的群中, 忽略
            try {
                string msg = MessageSegment::image("tech.jimowutong.jx3/chakanqiandaobiao.png");
                send_group_message(event.group_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "发送签到表失败, 错误码: " + to_string(err.code));
            }
        } else if (event.message.substr(0, (to_string("花价 ")).size()) == "花价 ") {  // 更简易的花价查询
            auto flower_msg = event.message.substr((to_string("花价 ")).size(), size(event.message));
            if (server.find(event.group_id) == server.end()) return;
            try {
                string msg = flower_query_easy(flower_msg, server.find(event.group_id)->second);
                send_group_message(event.group_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "花价查询失败, 错误码: " + to_string(err.code));
            }
        } else if (event.message.substr(0, (to_string(" 花价")).size()) == " 花价") {  // 原版花价查询
            auto flower_msg = event.message.substr((to_string(" 花价 ")).size(), size(event.message));
            if (server.find(event.group_id) == server.end()) return;
            try {
                string msg = flower_query(flower_msg, server.find(event.group_id)->second);
                send_group_message(event.group_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "花价查询失败, 错误码: " + to_string(err.code));
            }
        } else if (event.group_id == config.G_nanqi && event.user_id == config.Q_qiqi) {  // 七七转发（TODO）
            if ((event.message.substr(0, (to_string("官方")).size()) == "官方") &&
                (event.message.substr((to_string("官方新闻")).size(), (to_string("来辣")).size()) == "来辣")) {
                try {
                    send_group_message(config.G_beijiu, event.message);
                    send_group_message(config.G_test, event.message);
                } catch (ApiError &err) {
                    logging::warning("群聊", "七七转发失败, 错误码: " + to_string(err.code));
                }
            }
        } else if (event.message.substr(0, (to_string("器物谱 ")).size()) == "器物谱 ") {  // 器物谱查询
            auto map_msg = event.message.substr((to_string("器物谱 ")).size(), size(event.message));
            try {
                string msg = homeland_pet_query(map_msg);
                if (msg.size()) send_group_message(event.group_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "器物谱查询失败, 错误码: " + to_string(err.code));
            }
        } else if (event.group_id == config.G_push) {  // 打卡push
            stringstream msg;
            auto id = event.user_id;
            if (event.message.substr(0, (to_string("打卡")).size()) == "打卡") {
                logging::debug("push", "打卡; 日期：" + today());
                auto push_msg = event.message.substr((to_string("打卡")).size(), size(event.message));
                stringstream sshour;
                sshour << atof(push_msg.c_str());
                cout << "hour = " << sshour.str() << endl;
                auto card = get_card(id);
                // FIXME: 获取群昵称 有问题
                // auto card = get_group_member_info(event.group_id, id, true).card;
                // logging::debug("push", "打卡; card = " + card);
                msg << string(MessageSegment::at(id)) << " ";
                if (atof(sshour.str().c_str()) <= 0) {
                    msg << "打卡失败" << endl << "未记录今日学习时间";
                } else {
                    auto res = push_write(id, card, sshour);
                    if (res.empty()) {
                        msg << "打卡失败" << endl << "数据写入出错";
                    } else {
                        msg << "打卡成功" << endl;
                        msg << "今日学习" << res["hour"] << "小时" << endl;
                        msg << "已打卡" << res["days"] << "天，学习" << res["hours"] << "小时";
                    }
                }
            } else if (event.message == "查看") {
                logging::debug("push", "查看; 日期：" + today());
                msg << string(MessageSegment::at(id)) << " ";
                auto res = push_read();
                if (res[id]["last"] == today()) {
                    msg << "今天已打卡，累计打卡" << res[id]["days"] << "天" << endl;
                    msg << "今日学习" << res[id]["hour"] << "小时，累计学习" << res[id]["hours"] << "小时";
                } else {
                    msg << "今天还没有打卡哦" << endl;
                    if (res[id]["days"].empty()) {
                        msg << "已打卡0天，学习0小时";
                    } else {
                        msg << "已打卡" << res[id]["days"] << "天，学习" << res[id]["hours"] << "小时";
                    }
                }
            } else if (event.message == "统计") {
                logging::debug("push", "统计; 日期：" + today());
                msg << "【打卡情况统计】";
                auto res = push_read();
                for (auto &r : res) {
                    msg << endl << r.second["card"];
                    msg << " 已打卡" << r.second["days"] << "天，学习" << r.second["hours"] + "小时";
                    msg << endl << "上次打卡时间：" << r.second["last"];
                }
            }
            try {
                if (msg.str().size()) send_group_message(event.group_id, msg.str());
            } catch (ApiError &err) {
                logging::warning("群聊", "打卡失败, 错误码: " + to_string(err.code));
            }
        }

        if (event.is_anonymous()) {
            logging::info("群聊", "消息是匿名消息, 匿名昵称: " + event.anonymous.name);
        }
        event.block();  // 阻止当前事件传递到下一个插件
    });

    // on_group_upload([](const auto &event) {  // 可以使用 auto 自动推断类型
    //     stringstream ss;
    //     ss << "您上传了一个文件, 文件名: " << event.file.name << ", 大小(字节): " << event.file.size;
    //     try {
    //         send_message(event.target, ss.str());
    //     } catch (ApiError &) {
    //     }
    // });
}

CQ_MENU(menu_demo_1) { logging::info("菜单", "点击菜单1"); }

CQ_MENU(menu_demo_2) { send_private_message(config.Q_ME, "测试"); }
