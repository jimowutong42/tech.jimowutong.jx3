#include <cqcppsdk/cqcppsdk.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <sstream>

#include "my_curl.hpp"

// QQ号、群号
static int MY_QQ;
static int G_beijiu;
static int G_test;

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
        MY_QQ = conf["MY_QQ"];
        G_beijiu = conf["G_beijiu"];
        G_test = conf["G_test"];
        logging::debug("启用", to_string(MY_QQ) + "," + to_string(G_beijiu) + "," + to_string(G_test));
        logging::info("启用", "剑网3小助手已启用");
    });

    // on_private_message([](const PrivateMessageEvent &event) {
    //     try {
    //         auto msgid = send_private_message(event.user_id, event.message); // 直接复读消息
    //         logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " + to_string(msgid));
    //         send_message(event.target,
    //                      MessageSegment::face(111) + "这是通过 message 模块构造的消息~"); // 使用 message模块构造消息
    //     } catch (ApiError &err) {
    //         logging::warning("私聊", "私聊消息复读失败, 错误码: " + to_string(err.code));
    //     }
    // });

    cq::on_private_message([](const auto &event) {
        try {
            // send_message(event.target, get_content("http://www.httpbin.org/get"));
            auto res = get_content(
                "https://next.jx3box.com/api/flower/price/"
                "rank?server=%E5%A4%A9%E9%B9%85%E5%9D%AA&flower=%E7%BB%A3%E7%90%83%E8%8A%B1");
            send_message(event.target, res);
            auto j = json::parse(res);
            std::cout << j.dump(4) << std::endl;
        } catch (cq::ApiError &) {
        }
    });

    // on_message([](const MessageEvent &event) {
    //     logging::debug("消息", "收到消息: " + event.message + "\n实际类型: " + typeid(event).name());
    // });

    on_group_message([](const GroupMessageEvent &event) {
        static const set<int64_t> ENABLED_GROUPS = {G_beijiu, G_test};
        if (ENABLED_GROUPS.count(event.group_id) == 0) return;  // 不在启用的群中, 忽略

        if (event.message == "查看签到表") {
            try {
                auto appdir = get_app_directory();
                string msg = MessageSegment::image("tech.jimowutong.jx3/chakanqiandaobiao.png");
                send_group_message(event.group_id, msg);  // 发送群消息
            } catch (ApiError &err) {
                logging::warning("群聊", "发送签到表失败, 错误码: " + to_string(err.code));
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

CQ_MENU(menu_demo_2) { send_private_message(MY_QQ, "测试"); }