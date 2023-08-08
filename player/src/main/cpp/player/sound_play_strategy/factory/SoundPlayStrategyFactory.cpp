//
// Created by rendedong on 2023/8/2.
//

#include "SoundPlayStrategyFactory.h"
#include "../strategy/SoundPlayStrategy7L.h"
#include "../strategy/SoundPlayStrategy8L.h"
#include "../strategy/SoundPlayStrategy8L7L.h"

SoundPlayStrategy *SoundPlayStrategyFactory::createStrategy(int v, Player *player) {
    switch (v) {
        case 8:
            return new SoundPlayStrategy8L(player);
        case 2:
            return new SoundPlayStrategy7L(player);
        case 10:
            return new SoundPlayStrategy8L7L(player);
        default:
            return 0;
    }
}