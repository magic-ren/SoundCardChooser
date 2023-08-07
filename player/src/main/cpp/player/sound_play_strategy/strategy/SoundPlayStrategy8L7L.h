//
// Created by Administrator on 2023/8/2.
//
#include "SoundPlayStrategy.h"

class SoundPlayStrategy8L7L : public SoundPlayStrategy {
public:
    SoundPlayStrategy8L7L(Player *player);

    ~SoundPlayStrategy8L7L();

    /**
     * 817左声道和7202左声道合成播放——7202左声道复制到817的右声道（听诊音、环境音同时播放）。
     * **/
    int playSound() override;
};