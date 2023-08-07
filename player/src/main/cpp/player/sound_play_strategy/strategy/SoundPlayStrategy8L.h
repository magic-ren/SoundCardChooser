//
// Created by Administrator on 2023/8/2.
//
#include "SoundPlayStrategy.h"

class SoundPlayStrategy8L : public SoundPlayStrategy {
public:
    SoundPlayStrategy8L(Player *player);

    ~SoundPlayStrategy8L();

    /**
     * 817声卡左声道播放（听诊音播放）；
     * **/
    int playSound() override;
};