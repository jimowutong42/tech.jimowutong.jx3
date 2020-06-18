#include <iostream>
#include <set>
#include <sstream>

#include <cqcppsdk/cqcppsdk.h>
#include <curl/curl.h>

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

std::string get_content(const std::string &url) {
    std::string body;

    auto curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "res/cacert.pem");

        auto receive = [](char *buf, size_t size, size_t count, void *data) {
            (*static_cast<std::string *>(data)) += std::string(buf, count);
            return size * count;
        };
        typedef size_t (*WriteFunction)(char *, size_t, size_t, void *);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<WriteFunction>(receive));
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

        curl_easy_perform(curl);

        auto res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            logging::error("curl", "curl_easy_perform() failed: " + to_string(curl_easy_strerror(res)) + "\n");
        }
        curl_easy_cleanup(curl);
    }

    return body;
}

CQ_INIT {
    on_enable([] { logging::info("启用", "插件已启用"); });

    // on_private_message([](const PrivateMessageEvent &event) {
    //     try {
    //         auto msgid = send_private_message(event.user_id, event.message); // 直接复读消息
    //         logging::info_success("私聊", "私聊消息复读完成, 消息 Id: " + to_string(msgid));
    //         send_message(event.target,
    //                      MessageSegment::face(111) + "这是通过 message 模块构造的消息~"); // 使用 message 模块构造消息
    //     } catch (ApiError &err) {
    //         logging::warning("私聊", "私聊消息复读失败, 错误码: " + to_string(err.code));
    //     }
    // });

    cq::on_private_message([](const auto &event) {
        try {
            // send_message(event.target, get_content("http://www.httpbin.org/get"));
            send_message(event.target, get_content("https://next.jx3box.com/api/flower/price/rank?server=%E5%A4%A9%E9%B9%85%E5%9D%AA&flower=%E7%BB%A3%E7%90%83%E8%8A%B1"));
        } catch (cq::ApiError &) {
        }
    });

    on_message([](const MessageEvent &event) {
        logging::debug("消息", "收到消息: " + event.message + "\n实际类型: " + typeid(event).name());
    });

    on_group_message([](const GroupMessageEvent &event) {
        static const set<int64_t> ENABLED_GROUPS = {123456, 123457};
        if (ENABLED_GROUPS.count(event.group_id) == 0) return; // 不在启用的群中, 忽略

        try {
            send_message(event.target, event.message); // 复读
            auto mem_list = get_group_member_list(event.group_id); // 获取群成员列表
            string msg;
            for (auto i = 0; i < min(10, static_cast<int>(mem_list.size())); i++) {
                msg += "昵称: " + mem_list[i].nickname + "\n"; // 拼接前十个成员的昵称
            }
            send_group_message(event.group_id, msg); // 发送群消息
        } catch (ApiError &) { // 忽略发送失败
        }
        if (event.is_anonymous()) {
            logging::info("群聊", "消息是匿名消息, 匿名昵称: " + event.anonymous.name);
        }
        event.block(); // 阻止当前事件传递到下一个插件
    });

    on_group_upload([](const auto &event) { // 可以使用 auto 自动推断类型
        stringstream ss;
        ss << "您上传了一个文件, 文件名: " << event.file.name << ", 大小(字节): " << event.file.size;
        try {
            send_message(event.target, ss.str());
        } catch (ApiError &) {
        }
    });
}

CQ_MENU(menu_demo_1) {
    logging::info("菜单", "点击菜单1");
}

CQ_MENU(menu_demo_2) {
    send_private_message(10000, "测试");
}