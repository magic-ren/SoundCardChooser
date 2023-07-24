//
// Created by Administrator on 2023/6/8.
//

#ifndef TINYALSA_DEMO_PLAYER_H
#define TINYALSA_DEMO_PLAYER_H

#include "log4c.h"
#include <string.h>
#include <cstdlib>
#include <string>

extern "C" {
#include "asoundlib.h"
};

class Player {
public:
    Player();

    ~Player();

    void start();


};


#endif //TINYALSA_DEMO_PLAYER_H
