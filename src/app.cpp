#include "app.hpp"

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;
using json = nlohmann::json;

CQ_INIT {
    on_enable([] {
        auto dir = get_app_directory() + "config.json";
        ifstream cfgfile(utils::s2ws(dir));
        if (!cfgfile.is_open()) {
            logging::error("启用", "无法读取用户配置文件！");
        }
        auto conf = json::parse(cfgfile);
        config.Q_ME = conf["Q_ME"];
        config.Q_qiqi = conf["Q_qiqi"];
        config.G_beijiu = conf["G_beijiu"];
        config.G_nanqi = conf["G_nanqi"];
        config.G_test = conf["G_test"];
        server.insert(std::pair<int64_t, std::string>(config.G_beijiu, "绝代天骄"));
        server.insert(std::pair<int64_t, std::string>(config.G_nanqi, "天鹅坪"));
        server.insert(std::pair<int64_t, std::string>(config.G_test, "天鹅坪"));
        logging::info("启用", "剑网3小助手已启用");
    });

    on_private_message([](const MessageEvent &event) {  //测试
        if (event.message.substr(0, (to_string("花价 ")).size()) == "花价 ") {  // 花价查询（TODO）
            auto flower_msg = event.message.substr((to_string("花价 ")).size(), size(event.message));
            try {
                string msg = flower_query(flower_msg, server.lower_bound(config.G_test)->second);
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
        } else if (event.message.substr(0, (to_string("花价 ")).size()) == "花价 ") {  // 花价查询（TODO）
            auto flower_msg = event.message.substr((to_string("花价 ")).size(), size(event.message));
            if (server.lower_bound(event.group_id) == server.end()) return;
            try {
                string msg = flower_query(flower_msg, server.lower_bound(event.group_id)->second);
                send_group_message(event.group_id, msg);
            } catch (ApiError &err) {
                logging::warning("群聊", "花价查询失败, 错误码: " + to_string(err.code));
            }
        } else if(event.group_id == config.G_nanqi && event.user_id == config.Q_qiqi) { // 七七转发（TODO）
            if ((event.message.substr(0, (to_string("官方")).size()) == "官方") &&
                (event.message.substr((to_string("官方新闻")).size(), (to_string("来辣")).size()) == "来辣")) {
                try {
                    send_group_message(config.G_beijiu, event.message);
                    send_group_message(config.G_test, event.message);
                } catch (ApiError &err) {
                    logging::warning("群聊", "七七转发失败, 错误码: " + to_string(err.code));
                }
            }
        }

        if (event.is_anonymous()) {
            logging::info("群聊", "消息是匿名消息, 匿名昵称: " + event.anonymous.name);
        }
        // event.block();  // 阻止当前事件传递到下一个插件
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