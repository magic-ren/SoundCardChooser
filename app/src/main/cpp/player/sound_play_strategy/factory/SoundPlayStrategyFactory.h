//
// Created by Administrator on 2023/8/2.
//

#ifndef TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H
#define TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H


class Player;

#include "../strategy/SoundPlayStrategy.h"

class SoundPlayStrategyFactory {
public:
    static SoundPlayStrategy *createStrategy(int v, Player *player);
};

#endif //TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H
