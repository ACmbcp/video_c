#include <iostream>
#include <fstream>
#include <cstring>

#include <opencv2/opencv.hpp>
#include "cilent.h"

//extern "C" {
//#include <libavformat/avformat.h>
//}


void test_encoder();

int main() {
    // 打开摄像头
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头" << std::endl;
        return -1;
    }

    // 获取摄像头的宽高信息
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // 打开输出文件
    FILE* outFile = fopen("output.yuv", "wb");
    if (!outFile) {
        std::cerr << "无法创建输出文件" << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame, yuvFrame;

        cap >> frame; // 从摄像头读取一帧图像

        if (frame.empty()) {
            std::cerr << "摄像头数据流中断" << std::endl;
            break;
        }

        // 将opencv的bgr图像转换为yuv420p格式
        cv::cvtColor(frame, yuvFrame, cv::COLOR_BGR2YUV_I420);

        // 将YUV数据写入文件
        fwrite(yuvFrame.data, 1, yuvFrame.total() * yuvFrame.elemSize(), outFile);

        // 显示原始帧
        cv::imshow("CameraWin", frame);

        // 按ESC退出录制
        if (cv::waitKey(1) == 27)
            break;
    }

    // 释放资源
    fclose(outFile);
    cap.release();
    cv::destroyAllWindows();

    std::cout << "视频录制完成，已保存为output.yuv" << std::endl;
    std::cout << "高度: " << frame_height << std::endl;
    std::cout << "宽度: " << frame_width << std::endl;

    //编码视频
    test_encoder();

    //通过UDP传输,当前路径下output.265
    if (FileSend() != 0) {
        std::cerr << "socket send failed" << std::endl;
    }

    return 0;
}








/*int main() {

    // 打开摄像头
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头" << std::endl;
        return -1;
    }

    // 获取摄像头的宽高信息
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // 初始化ffmpeg
    avformat_network_init();

    // 创建avformatcontext
    AVFormatContext* pformatctx = avformat_alloc_context();
    if (!pformatctx) {
        std::cerr << "无法创建avformatcontext" << std::endl;
        return -1;
    }

    // 设置输出视频格式为yuv420p
    AVOutputFormat* outfmt = const_cast<AVOutputFormat*>(av_guess_format("rawvideo", NULL, NULL));
    pformatctx->oformat = outfmt;

    // 打开输出文件
    if (avio_open(&pformatctx->pb, "output.yuv", AVIO_FLAG_WRITE) < 0) {
        std::cerr << "无法创建输出文件" << std::endl;
        return -1;
    }

    // 创建avstream
    AVStream* pstream = avformat_new_stream(pformatctx, NULL);
    if (!pstream) {
        std::cerr << "创建avstream失败" << std::endl;
        return -1;
    }

    // 设置帧率
    int fps = 30;

    // 设置avstream的时间基准
    AVRational time_base;
    time_base.num = 1;
    time_base.den = fps;

    // 设置avstream的时间基准
    pstream->time_base = time_base;

    // 创建avcodeccontext
    AVCodecParameters* pcodecparams = pstream->codecpar;
    pcodecparams->codec_id = AV_CODEC_ID_RAWVIDEO;
    pcodecparams->codec_type = AVMEDIA_TYPE_VIDEO;
    pcodecparams->format = AV_PIX_FMT_YUV420P;
    pcodecparams->width = frame_width;
    pcodecparams->height = frame_height;

    // 写入流头信息
    avformat_write_header(pformatctx, NULL);

    // 录制视频并保存为yuv格式
    std::chrono::steady_clock::time_point next_frame_time = std::chrono::steady_clock::now();

   
    while (true) {
            cv::Mat frame;
            cap >> frame; // 从摄像头读取一帧图像

            if (frame.empty()) {
                std::cerr << "摄像头数据流中断" << std::endl;
                break;
            }

            // 将opencv的bgr图像转换为yuv420p格式
            cvtColor(frame, frame, cv::COLOR_BGR2YUV_I420);

            // 将avframe写入输出文件
            

            AVPacket pkt;
            av_new_packet(&pkt, frame.total() * frame.elemSize());
            pkt.data = frame.data;
            pkt.size = frame.total() * frame.elemSize();
            pkt.stream_index = pstream->index;

            // 让ffmpeg自动生成dts和pts
            pkt.pts = av_rescale_q_rnd(pstream->nb_frames, time_base, pstream->time_base, (enum AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = pkt.pts;

            // 更新nb_frames计数器
            pstream->nb_frames++;

            av_interleaved_write_frame(pformatctx, &pkt);
            imshow("CameraWin1", frame);

            // 计算下一帧的时间戳
            next_frame_time += std::chrono::milliseconds(1000 / fps);

            // esc键退出录制
            /*if (cv::getWindowProperty("CameraWin1", WND_PROP_VISIBLE)==-1)
                break;
           

            if (cv::waitKey(1) == 27)
                break;
            
        }
   
    // 写入文件尾
    av_write_trailer(pformatctx);

    // 释放资源

    avio_close(pformatctx->pb);
    avformat_free_context(pformatctx);
    cap.release();
    

    std::cout << "视频录制完成，已保存为output.yuv" << std::endl;
    std::cout << frame_height << "高度" << std::endl;
    std::cout << frame_width << "宽度" << std::endl;
    //test_encoder();

    cv::destroyAllWindows();

    return 0;
}
*/