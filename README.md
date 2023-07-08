# monitor-opencv
基于http-opencv的监控系统

# 需求分析
使用uvc摄像头实时获取视频，通过http协议传输视频数据，并实时显示到web浏览器或者客户端 \
在ubuntu系统架设视频服务器(Mjpg-streamer)，实现抓取视频图像。分析视频服务器响应的数据格式，从返回的数据中剥离并保存一帧JPEG图像。 \
使用Qt网络编程框架向视频服务器发送获取视频或截图请求，剥离出JPEG图像并显示到Qt界面，通过不断刷新显示JPEG图像帧格式 

# 项目设计
远程视频监控系统结构框图 \
![1688645120175](https://github.com/X9475/monitor-opencv/assets/102901627/2554c734-0294-4608-83f8-a3c8b0b024f6)

网络视频服务器(Mjpg-streamer)，采用多线程的方式获取UVC摄像头的视频数据 \
Mjpg-streamer采用高内聚低耦合系统结构，将输入插件和输出插件分离，编译维护。 \
输入插件可以是intput_uvc.so、测试图片插件intput_testpicture.so；输出插件可以是多个，如：output_http.so可以同时发送给多个客户端。

JPEG图片格式标记：\
![1688645087567](https://github.com/X9475/monitor-opencv/assets/102901627/34fe259b-62c1-4512-b5e5-3122895437d7)

从视频服务器获取图像帧分析：hexdump -C /temp/test.mjpeg | less
![1688645069831](https://github.com/X9475/monitor-opencv/assets/102901627/69dd48d0-083a-4826-9bb1-a739eed2aae3)

  

