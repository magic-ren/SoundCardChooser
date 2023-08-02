//
// Created by Administrator on 2023/8/2.
//

#include "SoundPlayStrategy.h"

class SoundPlayStrategy7L : public SoundPlayStrategy {
public:
    SoundPlayStrategy7L(Player *player);

    int playSound() override;
};
