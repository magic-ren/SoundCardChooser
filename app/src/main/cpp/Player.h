//
// Created by Administrator on 2023/6/8.
//

#ifndef TINYALSA_DEMO_PLAYER_H
#define TINYALSA_DEMO_PLAYER_H

#include "log4c.h"
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

#include "util.h"

extern "C" {
#include "asoundlib.h"
};

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
    FILE *file = 0;
    pthread_t pid_play;
    struct pcm *pcm_in=0;
    struct pcm *pcm_out=0;
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
    enum pcm_format format=PCM_FORMAT_S16_LE;

    struct pcm_config config;
    char *buffer=0;
    unsigned int size=0;

    unsigned int bytes_read = 0;

    //**********************************合成代码*************************************//
    char *buffer2;
    char *buffer3;
    struct pcm *pcm_in_2;
    //**********************************合成代码*************************************//

public:
    int status = STATUS_UNPLAY;

    Player();

    ~Player();

    void start();

    void start_();

    void closePcm();

    void setPath(const char *file_path);

};


#endif //TINYALSA_DEMO_PLAYER_H
