//
// Created by Administrator on 2023/8/2.
//
#include "SoundPlayStrategy.h"

class SoundPlayStrategy8L : public SoundPlayStrategy {
public:
    SoundPlayStrategy8L(Player *player);

    int playSound() override;
};