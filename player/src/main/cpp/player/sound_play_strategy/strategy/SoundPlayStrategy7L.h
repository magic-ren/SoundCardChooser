//
// Created by rendedong on 2023/8/2.
//

#include "SoundPlayStrategy.h"

class SoundPlayStrategy7L : public SoundPlayStrategy {
public:
    SoundPlayStrategy7L(Player *player);

    ~SoundPlayStrategy7L();

    /**
     * 7202声卡左声道播放（环境音播放）；
     * **/
    int playSound() override;
};
