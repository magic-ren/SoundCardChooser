//
// Created by Administrator on 2023/8/2.
//
#include "SoundPlayStrategy.h"

class SoundPlayStrategy8L7L : public SoundPlayStrategy {
public:
    SoundPlayStrategy8L7L(Player *player);

    int playSound() override;
};