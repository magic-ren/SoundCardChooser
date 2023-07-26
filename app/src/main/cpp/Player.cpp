//
// Created by Administrator on 2023/6/8.
//
#define PCM_CARD 0
#define PCM_DEVICE_IN 0
#define PCM_DEVICE_OUT 0

#include "Player.h"


Player::Player() {
    header.riff_id = ID_RIFF;
    header.riff_sz = 0;
    header.riff_fmt = ID_WAVE;
    header.fmt_id = ID_FMT;
    header.fmt_sz = 16;
    header.audio_format = FORMAT_PCM;
    header.num_channels = channels;
    header.sample_rate = rate;
    header.bits_per_sample = pcm_format_to_bits(format);
    header.byte_rate = (header.bits_per_sample / 8) * channels * rate;
    header.block_align = channels * (header.bits_per_sample / 8);
    header.data_id = ID_DATA;

    memset(&config, 0, sizeof(config));
    config.channels = 2;
    config.rate = 44100;
    config.period_size = 16 * (44100 / 1000);   //16ms算一帧，16ms内有多少采样点
    config.period_count = 4;    //一个周期内采集几帧？
    config.format = PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;
}

void Player::setPath(const char *file_path) {
    this->file_path = new char[strlen(file_path) + 1];
    strcpy(this->file_path, file_path);
    LOGE("file path is (%s)\n", file_path);
    if (file) {
        fclose(file);
    }
    file = fopen(file_path, "wb");
    if (!file) {
        LOGE("Unable to create file '%s'\n", file_path);
    } else {
        LOGE("open file success '%s'\n", file_path);
        /* leave enough room for header */
        fseek(file, sizeof(struct wav_header), SEEK_SET);
    }
}

void *task_start(void *args) {
    Player *player = static_cast<Player *>(args);
    player->start_();
    return 0;
}

void Player::start() {
    pthread_create(&pid_play, 0, task_start, this);
}

void Player::start_() {




    //**********************************合成代码*************************************//
//    char *buffer2;
//    char *buffer3;
//    struct pcm *pcm_in_2;
    //**********************************合成代码*************************************//




    if (!pcm_in) {
        pcm_in = pcm_open(0, 0, PCM_IN, &config);
        if (!pcm_in || !pcm_is_ready(pcm_in)) {
//        fprintf(stderr, "Unable to open PCM device (%s)\n",
//                pcm_get_error(pcm_in));
            LOGE("Unable to open PCM device (%s)\n", pcm_get_error(pcm_in));
            return;
        } else {
            LOGE("pcmC0D0c打开啦");
        }
    }

    //**********************************合成代码*************************************//
//    pcm_in_2 = pcm_open(2, 0, PCM_IN, &config);
//    if (!pcm_in_2 || !pcm_is_ready(pcm_in_2)) {
////        fprintf(stderr, "Unable to open PCM device (%s)\n",
////                pcm_get_error(pcm_in));
//        LOGE("Unable to open PCM device (%s)\n",pcm_get_error(pcm_in_2));
//        return;
//    } else{
//        LOGE("pcmC0D2c打开啦");
//    }
    //**********************************合成代码*************************************//


    if (!pcm_out) {
        pcm_out = pcm_open(0, 0, PCM_OUT, &config);
        if (!pcm_out || !pcm_is_ready(pcm_out)) {
//        fprintf(stderr, "Unable to open PCM device (%s)\n",
//                pcm_get_error(pcm_in));
            LOGE("Unable to open PCM device (%s)\n", pcm_get_error(pcm_out));
            return;
        } else {
            LOGE("pcmC0D0p打开啦");
        }
    }
    LOGE("采样点个数：%u", pcm_get_buffer_size(pcm_in));

    if (!size) {
        size = pcm_frames_to_bytes(pcm_in, pcm_get_buffer_size(
                pcm_in));    //pcm_get_buffer_size()是获取一个周期内有多少采样点；
    }
    // pcm_frames_to_bytes()是获取一个周期内占用多少字节。
    LOGE("采样点大小计算：%u", pcm_get_buffer_size(pcm_in) * 4);
    LOGE("采样点大小size：%u", size);
//    LOGE("录音buffer的大小是：%u",size);
//    int j = pcm_frames_to_bytes(pcm_out, pcm_get_buffer_size(pcm_out));
//    LOGE("播放buffer的大小是：%u",j);

    if (!buffer) {
        buffer = static_cast<char *>(malloc(size));
    }
    //**********************************合成代码*************************************//
//    buffer2 = static_cast<char *>(malloc(size));
//    buffer3 = static_cast<char *>(malloc(size));
    //**********************************合成代码*************************************//
    if (!buffer) {
        LOGE("Unable to allocate %u bytes\n", size);
        free(buffer);
        pcm_close(pcm_in);
        pcm_close(pcm_out);
        return;
    } else {
        LOGE("buffer初始化啦");
    }
    LOGE("Capturing sample: %u ch, %u hz, %u bit\n", 2, 44100,
         pcm_format_to_bits(PCM_FORMAT_S16_LE));

    status=STATUS_PLAYING;

    while (status==STATUS_PLAYING&&!pcm_read(pcm_in, buffer, size)) {
        LOGE("此时状态%d",status);
        //**********************************合成代码*************************************//
//    while (!pcm_read(pcm_in, buffer, size)&&!pcm_read(pcm_in_2, buffer2, size)) {
        //**********************************合成代码*************************************//
//        LOGE("录制成功");
//        std::string output;
//        for (int i = 0; i < size; ++i) {
//            output += std::to_string(static_cast<unsigned char>(*(buffer+i))) + " ";
////            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("%s", output.c_str());


//**********************************合成代码*************************************//
//        std::string output1;
//        for (int i = 0; i < size; ++i) {
//            output1 += std::to_string(static_cast<unsigned char>(*(buffer+i))) + " ";
////            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("817合成前：%s", output1.c_str());
//
//        std::string output2;
//        for (int i = 0; i < size; ++i) {
//            output2 += std::to_string(static_cast<unsigned char>(*(buffer2+i))) + " ";
////            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("7202合成前：%s", output2.c_str());
//
//
//        //将817的左声道和7202的左声道合成
////        int i=2;
////        int j =0;
////        int n = size/4;
////        for(int k = 1;k<=n;++k){
////            buffer[i]=buffer2[j];
////            buffer[i+1]=buffer[j+1];
////            i+=4;
////            j+=4;
////        }
//        int a = 0;
//        int b = 0;
//        int c = 0;
//        int n = size/4;
//        for(int k = 1;k<=n;++k){
//            buffer3[c]=buffer[a];
//            buffer3[c+1]=buffer[a+1];
//            buffer3[c+2]=buffer2[b];
//            buffer3[c+3]=buffer2[b+1];
//            a+=4;
//            b+=4;
//            c+=4;
//        }
//
//        int result = pcm_write(pcm_out, buffer3, size);
//
//        std::string output;
//        for (int i = 0; i < size; ++i) {
//            output += std::to_string(static_cast<unsigned char>(*(buffer3+i))) + " ";
////            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("合成后：%s", output.c_str());
//
//        LOGE("播放结果：%d\n",result);
//**********************************合成代码*************************************//





        //3,4复制1，2。
        //声卡是7202时：
        //7202是左声道录制的环境音，所以这里等于是将环境音复制到了右声道，可以播放成功。
        //声卡是817时：
        //817是左声道录制的听诊音，所以这里等于是将听诊音复制到了右声道，可以播放成功。
//        int i=0;
//        int j =2;
//        int n = size/4;
//        for(int k = 1;k<=n;++k){
//            buffer[j]=buffer[i];
//            buffer[j+1]=buffer[i+1];
//            i+=4;
//            j+=4;
//        }

        //1,2复制3,4
        //声卡是7202时：
        //7202是左声道录制的环境音，所以这里等于是将听诊音复制到了左声道，而此时听诊器是插到817上的，听诊音是没有声音的，所以播放失败。
        //声卡是817时：
        //817是左声道录制的听诊音，所以这里等于是将环境音复制到了左声道，而此时环境麦是插到7202上的，环境音是没有声音的，所以播放失败。
//        int i=0;
//        int j =2;
//        int n = size/4;
//        for(int k = 1;k<=n;++k){
//            buffer[i]=buffer[j];
//            buffer[i+1]=buffer[j+1];
//            i+=4;
//            j+=4;
//        }

        int result = pcm_write(pcm_out, buffer, size);
//
//        std::string output;
//        for (int i = 0; i < size; ++i) {
//            output += std::to_string(static_cast<unsigned char>(*(buffer+i))) + " ";
////            LOGE("%d",static_cast<int>(static_cast<unsigned char>(buffer[i])));
//        }
//        LOGE("合成后：%s", output.c_str());
//
//        LOGE("播放结果：%d\n",result);


        if (fwrite(buffer, 1, size, file) != size) {
            LOGE("Error capturing sample\n");
            break;
        }
        bytes_read += size;


    }
    LOGE("此时状态%d",status);

    if(status==STATUS_COMPLETE){
        unsigned int recFrames = pcm_bytes_to_frames(pcm_in, bytes_read);
        header.data_sz = recFrames * header.block_align;
        header.riff_sz = header.data_sz + sizeof(header) - 8;
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(struct wav_header), 1, file);

        fclose(file);
    }

}

void Player::closePcm() {
    if (pcm_in) {
        pcm_close(pcm_in);
    }
    if (pcm_out) {
        pcm_close(pcm_out);
    }
}

