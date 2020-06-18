# tech.jimowutong.jx3
基于酷Q的剑网3小助手（自用）

### 构建：
#### Git 子模块
`git submodule update --init` # 初始化 Git 子模块

#### CMake (已加入Curl的配置)
CMakeLists.txt中设置项目名称、APP_ID、install路径

x86

UTF-8 无签名编码

#### OpenSSL (Curl的HTTPS支持)
下载、配置Perl、Nasm、nmake、dmake (详见官方文档)

`perl Configure no-shared VC-WIN32 --prefix=D:\OpenSSL --openssldir=D:\OpenSSL`

`nmake`

`nmake install`

`nmake clean`

VS code配置cmake
```
"cmake.configureArgs": [
    "-DCMAKE_USE_OPENSSL=ON",
    "-DOPENSSL_ROOT_DIR=D:\\OpenSSL"
]
```


### 感谢：
#### 酷Q
https://cqp.cc/

https://docs.cqp.im/

#### A C++ SDK for CoolQ
https://github.com/cqmoe/cqcppsdk

https://cqcppsdk.cqp.moe/

#### command line tool and library for transferring data with URLs
https://github.com/curl/curl

https://curl.haxx.se/

#### TLS/SSL and crypto library
https://github.com/openssl/openssl

https://www.openssl.org


### 联系：
#### 剑网3
双线一区 天鹅坪 寂寞梧桐@雪絮金屏