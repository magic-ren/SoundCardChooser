//
// Created by rendedong on 2023/6/8.
//

#ifndef TINYALSA_DEMO_PLAYER_H
#define TINYALSA_DEMO_PLAYER_H

#include "../utils/log4c.h"
#include <string.h>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>

#include <errno.h>
#include <ctype.h>
#include <getopt.h>

#include "consts.h"

#include "JNICallbackHelper.h"


#include "sound_play_strategy/factory/SoundPlayStrategyFactory.h"


extern "C" {
#include "../tinyalsa/asoundlib.h" //这个头文件里声明的都是C文件里的方法，在C++中调用C，需要加上extern "C"
};

//想引用tinymix.c文件中的方法，需要先像这样声明一下（否则调用不了），会自动帮你找到实现方法
extern "C" int tinymix_set_value(struct mixer *mixer, const char *control,
                                 char **values, unsigned int num_values);


//wav头信息相关参数
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

struct wav_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t riff_fmt;
    uint32_t fmt_id;
    uint32_t fmt_sz;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    uint32_t data_id;
    uint32_t data_sz;
};

class Player {
private:
    char *file_path = 0;//所有的指针类型初始化时最好都指定为0，不然默认会指向一个随机地址，也就是野指针

    pthread_t pid_play;

    struct pcm *pcm_out = 0;
    struct wav_header header;

    //这些参数都是从tinyalsa库的示例代码里copy过来的
    unsigned int card = 0;
    unsigned int device = 0;
    unsigned int channels = 2;
    unsigned int rate = 44100;
    unsigned int bits = 16;
    unsigned int frames;
    unsigned int period_size = 1024;
    unsigned int period_count = 4;
    unsigned int cap_time = 0;
    enum pcm_format format = PCM_FORMAT_S16_LE;

    SoundPlayStrategy *soundPlayStrategy = 0;
    int lastMode = -1;

    struct mixer *mixer = 0;


public:
    int status = STATUS_UNPLAY;
    struct pcm *pcm_in = 0;
    struct pcm_config config;
    struct pcm *pcm_in_2 = 0;
    unsigned int size = 0;
    char *buffer = 0;
    char *buffer2 = 0;
    char *buffer3 = 0;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    JNICallbackHelper *jniCallbackHelper = 0;
    unsigned int bytes_read = 0;
    FILE *file = 0;
    bool finishByWorkThread = false;//false是未结束，true是播放器已经结束了。播放过程中退出，子线程中依据此变量来释放Player，因为此时不能在主线程释放Player，不然会出错（子线程还在用Player）


private:
    /**
     * 播放完成后保存录音并跳转页面
     * **/
    void saveAndJump(pcm *pcm_target);

    /**
     * 播放重置时重置相关资源
     * **/
    void resouceReset();

public:

    Player(JNICallbackHelper *jniCallbackHelper);

    ~Player();

    /**
     * 开始播放（主线程中调用）
     * **/
    void start();

    /**
     * 开始播放（子线程中调用）
     * **/
    void start_();

    /**
     * 设置保存录音文件的路径
     * **/
    int setPath(const char *file_path);

    /**
     * 设置817声卡录制通道的mic为主mic（默认是关闭的）
     * **/
    void setMixArgs();

    /**
     * 暂停播放
     * **/
    void pause();

    /**
     * 继续播放
     * **/
    void continuePlay();

    /**
     * 重置播放
     * **/
    void reset();

    /**
     * 播放完毕后进行的处理（子线程调用）。例如保存录音、跳转页面、重置资源。
     * **/
    void afterPlay(pcm *pcm_target);

    /**
     * 设置播放器的播放策略。目前有三种：
     * 1、7202声卡左声道播放（环境音播放）；
     * 2、817声卡左声道播放（听诊音播放）；
     * 3、817左声道和7202左声道合成播放——7202左声道复制到817的右声道（听诊音、环境音同时播放）。
     * **/
    void setPlayStrategy(int card_mode);

};


#endif //TINYALSA_DEMO_PLAYER_H
