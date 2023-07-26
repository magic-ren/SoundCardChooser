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

extern "C" {
#include "asoundlib.h"
};

class Player {
private:
    char *file_path = 0;
    pthread_t pid_play;
    struct pcm *pcm_in=0;
    struct pcm *pcm_out=0;
public:
    Player(const char *file_path);

    ~Player();

    void start();

    void start_();

    void closePcm();


};


#endif //TINYALSA_DEMO_PLAYER_H
