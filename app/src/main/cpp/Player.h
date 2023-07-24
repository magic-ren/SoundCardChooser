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

extern "C" {
#include "asoundlib.h"
};

class Player {
private:
    pthread_t pid_play;
public:
    Player();

    ~Player();

    void start();

    void start_();


};


#endif //TINYALSA_DEMO_PLAYER_H
