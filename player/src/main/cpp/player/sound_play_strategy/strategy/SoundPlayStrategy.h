//
// Created by rendedong on 2023/8/2.
//

#ifndef TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H
#define TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H

extern "C" {
#include "../../../tinyalsa/asoundlib.h"
};

class Player;

class SoundPlayStrategy {
public:
    Player *playerPtr;
public:
    SoundPlayStrategy(Player *player) : playerPtr(player) {}

    /**
     * 设计模式：策略模式。
     *
     * 现在的硬件情况是：听诊mic连接在817的左声道，环境mic连接在7202的左声道
     *三种不同的播放策略：
     * 1、817声卡左声道播放（听诊音播放）；
     * 2、7202声卡左声道播放（环境音播放）；
     * 3、817左声道和7202左声道合成播放——7202左声道复制到817的右声道（听诊音、环境音同时播放）。
     * **/
    virtual int playSound() = 0;
};

/**
 * 备用算法
 * **/
//1,2复制3,4
//声卡是7202时：
//7202是左声道录制的环境音，所以这里等于是将听诊音复制到了左声道，而此时听诊器是插到817上的，听诊音是没有声音的，所以播放失败。
//声卡是817时：
//817是左声道录制的听诊音，所以这里等于是将环境音复制到了左声道，而此时环境麦是插到7202上的，环境音是没有声音的，所以播放失败。
//        int i=0;
//        int j =2;
//        int n = size/4;
//        for(int k = 1;k<=n;++k){
//            buffer[i]=buffer[j];
//            buffer[i+1]=buffer[j+1];
//            i+=4;
//            j+=4;
//        }

#endif //TINYALSA_DEMO_SOUNDPLAYSTRATEGY_H
