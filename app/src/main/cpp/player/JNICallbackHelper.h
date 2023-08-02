//
// Created by Administrator on 2023/7/31.
//

#ifndef TINYALSA_DEMO_JNICALLBACKHELPER_H
#define TINYALSA_DEMO_JNICALLBACKHELPER_H


#include <jni.h>
#include "../utils/log4c.h"


class JNICallbackHelper {
private:
    JavaVM *vm = 0;
    JNIEnv *env_main = 0;
    jobject job;
    jmethodID jmd_callback;
    jmethodID jmd_jump_callback;
public:
    JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job);

    ~JNICallbackHelper();

    void onCallback(char *data, int size);

    void onJumpCallback(char *path);
};


#endif //TINYALSA_DEMO_JNICALLBACKHELPER_H
