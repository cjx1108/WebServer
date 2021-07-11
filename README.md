# WebServer
本项目为 Linux 下 C++编写的轻量级高性能的B/S模型Web服务器，可实现 web 端用户注册，登录功能，可以请求服务器图片和视频文件。
1.使用线程池、套接字（socket）、epoll（ET）和事件处理（reactor）的并发模型。
2.使用状态机解析 HTTP 请求报文，支持解析 GET 和 POST 请求。
3.实现数据库连接池。
4.用定时器处理非活动链接，定时器容器利用升序链表进行设计。
5.实现异步日志系统，记录服务器运行状态。


// 修改为root文件夹所在路径
const char* doc_root="/home/c/WebServer/root";

#编译方式
make server
./server port
浏览器端  ip:port
