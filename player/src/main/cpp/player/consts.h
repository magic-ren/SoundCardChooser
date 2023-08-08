//
// Created by rendedong on 2023/7/26.
//

#ifndef TINYALSA_DEMO_CONSTS_H
#define TINYALSA_DEMO_CONSTS_H

/**
 * 播放器状态码
 * **/
#define STATUS_UNPLAY 1 //第一次进入播放页面，或者重置时
#define STATUS_PLAYING 2 //正在播放
#define STATUS_PAUSE 3 //暂停播放
#define STATUS_COMPLETE 4 //结束播放并保存音频
#define STATUS_PLAYER_UNINIT -1 //播放器未初始化

/**
 * 播放的结果。
 * 播放时出错就会结束播放：打开设备错误、java层播放错误、报错音频错误。
 * **/
#define PLAY_SUCCESS 1
#define PLAY_FAIL -1

/**
 * 音频保存文件打开是否成功
 * **/
#define FILE_OPEN_SUCCESS 1
#define FILE_OPEN_FAIL -1

/**
 * 播放器的错误码
 * **/
#define ERROR_OPEN_PCMC2D0C_FAIL 1
#define ERROR_INIT_PCMC2D0C_BUFFER_FAIL 2
#define ERROR_SAVE_AUDIO_FAIL 3
#define ERROR_READ_PCMC2D0C_FAIL 4
#define ERROR_OPEN_PCMC0D0C_FAIL 5
#define ERROR_INIT_PCMC0D0C_BUFFER_FAIL 6
#define ERROR_READ_PCMC0D0C_FAIL 7
#define ERROR_INIT_MERGE_BUFFER_FAIL 8
#define ERROR_READ_PCMC0D0C_PCMC2D0C_FAIL 9
#define ERROR_CREATE_FILE_FAIL 10
#define ERROR_CARD_STRATEGY_SETTING 11
#define ERROR_OPEN_MIXER_FAIL 12
#define ERROR_SET_PCMC0D0C_MIC_FAIL 13

#define THREAD_MAIN 1   //主线程
#define THREAD_CHILD 2  //子线程

#endif //TINYALSA_DEMO_CONSTS_H
