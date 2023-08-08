//
// Created by rendedong on 2023/7/31.
//

#ifndef TINYALSA_DEMO_JNICALLBACKHELPER_H
#define TINYALSA_DEMO_JNICALLBACKHELPER_H


#include <jni.h>
#include "../utils/log4c.h"
#include "consts.h"


class JNICallbackHelper {
private:
    JavaVM *vm = 0;
    JNIEnv *env_main = 0;
    jobject job;
    jmethodID jmd_callback;
    jmethodID jmd_jump_callback;
    jmethodID jmd_error;

public:
    bool finishByWorkThread = false;//当由子线程执行Player的析构函数时，对调用到helper的析构函数，要用子线程的JNIEnv来释放全局引用jobject

public:
    JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job);

    ~JNICallbackHelper();

    /**
     * C层到Java层的播放回调（C层获取录音数据，Java层播放）（通过JNI完成）
     * **/
    void onCallback(char *data, int size);

    /**
     * 完成播放时回调java层代码
     * **/
    void onJumpCallback(char *path);

    /**
     * 播放器内部发生错误时通知java层
     * **/
    void onError(int thread_mode, int error_code);
};


#endif //TINYALSA_DEMO_JNICALLBACKHELPER_H
