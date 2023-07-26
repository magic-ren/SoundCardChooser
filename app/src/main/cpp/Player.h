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

extern "C" {
#include "asoundlib.h"
};

class Player {
private:
    char *file_path = 0;
    FILE *file = 0;
    pthread_t pid_play;
    struct pcm *pcm_in=0;
    struct pcm *pcm_out=0;
public:
    Player();

    ~Player();

    void start();

    void start_();

    void closePcm();

    void setPath(const char *file_path);


};


#endif //TINYALSA_DEMO_PLAYER_H
