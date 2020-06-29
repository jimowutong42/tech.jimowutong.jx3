# tech.jimowutong.jx3
基于酷Q的剑网3小助手（自用）

## 构建：
### Git 子模块
`git submodule update --init` # 初始化 Git 子模块


### CMake (已加入Curl的配置)
CMakeLists.txt中设置项目名称、APP_ID、install 路径

x86编译；所有文件使用 UTF-8 无签名编码


### OpenSSL (Curl的HTTPS支持)
下载、配置Perl、Nasm、nmake、dmake (详见官方文档)

`perl Configure no-shared VC-WIN32 --prefix=D:\openssl\build --openssldir=D:\openssl\build`

在x86 Native Tools Command Prompt for VS 2017中：

`nmake`

`nmake install`

`nmake clean`

VS code配置cmake
```
"cmake.configureArgs": [
    "-DCMAKE_USE_OPENSSL=ON",
    "-DOPENSSL_ROOT_DIR=D:\\openssl\\build"
]
```

下载cacert.pem证书，并确认路径



## 功能：
### 个人信息配置
res/config.json

res/python/config.ini

将res中的文件放在"酷Q"\data\app\tech.jimowutong.jx3下


### 腾讯在线Excel文档 团队签到
> 查看签到表

TODO: 消息触发之后再截图

TODO: Cookie免登录

TODO: 图像处理裁剪表格

TODO: 不同表格页的跳转

TODO: 签到功能


### 花价查询
> 花价 菌

> 花价 玫瑰


### 器物谱查询
> 器物谱 长歌门


### QQ转发
七七官方新闻公告转发

TODO: 自定义私聊群发


### 打卡小助手
> 打卡 1.5

> 打卡

> 查看

> 统计

TODO: 打卡情况统计按累计学习时间排序

TODO: 每日打卡提醒


### 微博监控转发（咕咕咕）
剑网3客户服务



## 感谢：
### 酷Q
https://cqp.cc/

https://docs.cqp.im/


### A C++ SDK for CoolQ
https://github.com/cqmoe/cqcppsdk

https://cqcppsdk.cqp.moe/


### command line tool and library for transferring data with URLs
https://github.com/curl/curl

https://curl.haxx.se/


### TLS/SSL and crypto library
https://github.com/openssl/openssl

https://www.openssl.org


### JSON for Modern C++
https://github.com/nlohmann/json

https://nlohmann.github.io/json/



## 联系：
### 剑网3
双线一区 天鹅坪 寂寞梧桐@雪絮金屏