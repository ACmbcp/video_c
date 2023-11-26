#工具

1.nginx(包含rtmp-module)
    配置nginx.conf
    地址：https://blog.csdn.net/u014552102/article/details/100906058
2.ffmpeg(支持h265格式flv)
    重新编译：由于FLV规范不支持H.265,需要修改增加新的编码ID以支持h265格式flv

    地址：https://github.com/runner365/ffmpeg_rtmp_h265

3.vlc

#步骤

1.录制并编码后的视频output.265

2.配置并启动nginx

3.ffmpeg 推流到nginx
    ffmpeg -re -i output.265 -f flv rtmp://localhost/live/test1
4.vlc播放
    打开网络串流->rtmp://localhost/live/test1->播放