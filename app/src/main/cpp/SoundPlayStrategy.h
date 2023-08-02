//
// Created by Administrator on 2023/8/2.
//

#ifndef TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H
#define TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H

extern "C" {
#include "asoundlib.h"
};

class Player;

class SoundPlayStrategy {
public:
    Player *playerPtr;
public:
    SoundPlayStrategy(Player *player) : playerPtr(player) {}

    virtual int playSound() = 0;
};

#endif //TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H
