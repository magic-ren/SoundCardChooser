//
// Created by rendedong on 2023/8/2.
//

#ifndef TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H
#define TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H


class Player;

#include "../strategy/SoundPlayStrategy.h"

class SoundPlayStrategyFactory {
public:
    /**
     * 设计模式：静态简单工厂
     *
     * 策略工厂。
     * 依据系统属性设置的声卡模式，创建相应的播放策略。
     * **/
    static SoundPlayStrategy *createStrategy(int v, Player *player);
};

#endif //TINYALSA_DEMO_SOUNDPLAYSTRATEGYFACTORY_H
