//
// Created by Administrator on 2023/6/8.
//
#define PCM_CARD 0
#define PCM_DEVICE_IN 0
#define PCM_DEVICE_OUT 0
#include "Player.h"

 Player::Player() {}

void Player::start() {
    struct pcm_config config;
    struct pcm *pcm_in;
    struct pcm *pcm_out;
    char *buffer;
    unsigned int size;


    memset(&config, 0, sizeof(config));
    config.channels = 2;
    config.rate = 44100;
    config.period_size = 16*(44100/1000);   //16ms算一帧，16ms内有多少采样点
    config.period_count = 4;    //一个周期内采集几帧？
    config.format = PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;

    pcm_in = pcm_open(2, 0, PCM_IN, &config);
    if (!pcm_in || !pcm_is_ready(pcm_in)) {
//        fprintf(stderr, "Unable to open PCM device (%s)\n",
//                pcm_get_error(pcm_in));
        LOGE("Unable to open PCM device (%s)\n",pcm_get_error(pcm_in));
        return;
    } else{
        LOGE("pcmC0D0c打开啦");
    }

    pcm_out = pcm_open(0, 0, PCM_OUT, &config);
    if (!pcm_out || !pcm_is_ready(pcm_out)) {
//        fprintf(stderr, "Unable to open PCM device (%s)\n",
//                pcm_get_error(pcm_in));
        LOGE("Unable to open PCM device (%s)\n",pcm_get_error(pcm_out));
        return;
    } else{
        LOGE("pcmC0D0p打开啦");
    }
    LOGE("采样点个数：%u",pcm_get_buffer_size(pcm_in));
    size = pcm_frames_to_bytes(pcm_in, pcm_get_buffer_size(pcm_in));    //pcm_get_buffer_size()是获取一个周期内有多少采样点；
                                                                        // pcm_frames_to_bytes()是获取一个周期内占用多少字节。
    LOGE("采样点大小计算：%u",pcm_get_buffer_size(pcm_in)*4);
    LOGE("采样点大小size：%u",size);
//    LOGE("录音buffer的大小是：%u",size);
//    int j = pcm_frames_to_bytes(pcm_out, pcm_get_buffer_size(pcm_out));
//    LOGE("播放buffer的大小是：%u",j);
    buffer = static_cast<char *>(malloc(size));
    if (!buffer) {
        LOGE("Unable to allocate %u bytes\n",size);
        free(buffer);
        pcm_close(pcm_in);
        pcm_close(pcm_out);
        return ;
    } else{
        LOGE("buffer初始化啦");
    }
    LOGE("Capturing sample: %u ch, %u hz, %u bit\n", 2, 44100,
           pcm_format_to_bits(PCM_FORMAT_S16_LE));

    while (!pcm_read(pcm_in, buffer, size)) {
        LOGE("录制成功");
//        std::string output;
//        for (int i = 0; i < size; ++i) {
////            output += std::to_string(static_cast<unsigned char>(*(buffer+i))) + " ";
//            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("%s", output.c_str());
        int i = pcm_write(pcm_out, buffer, size);
        LOGE("播放结果：%d\n",i);
    }

}
