//
// Created by Administrator on 2023/6/8.
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
#include "../tinyalsa/asoundlib.h"
};

extern "C" int tinymix_set_value(struct mixer *mixer, const char *control,
                                 char **values, unsigned int num_values);

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
    char *file_path = 0;

    pthread_t pid_play;

    struct pcm *pcm_out = 0;
    struct wav_header header;

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
    void saveAndJump(pcm *pcm_target);

    void resouceReset();

public:

    Player(JNICallbackHelper *jniCallbackHelper);

    ~Player();

    void start();

    void start_();

    int setPath(const char *file_path);

    void setMixArgs();

    void pause();

    void continuePlay();

    void reset();

    void afterPlay(pcm *pcm_target);

    void setPlayStrategy(int card_mode);

};


#endif //TINYALSA_DEMO_PLAYER_H
