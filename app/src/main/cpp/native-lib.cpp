#include <jni.h>
#include <string>
#include "Player.h"
#include "util.h"
#include "log4c.h"
#include "JNICallbackHelper.h"

Player *player = 0;
JavaVM *vm = 0;

jint JNI_OnLoad(JavaVM *vm, void *args) {
    ::vm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_startPlay(JNIEnv *env, jobject thiz) {
    if (player) {
        player->start();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_closePCM(JNIEnv *env, jobject thiz) {
    if (player) {
        player->closePcm();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_setFilePath(JNIEnv *env, jobject thiz, jstring path) {
    const char *file_path = env->GetStringUTFChars(path, 0);
    if (player) {
        player->setPath(file_path);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_prepare(JNIEnv *env, jobject thiz) {
    auto *helper = new JNICallbackHelper(vm, env, thiz);
    player = new Player(helper);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_complete(JNIEnv *env, jobject thiz) {
    if (player) {
        player->status = STATUS_COMPLETE;
        LOGE("set status suceess %d\n", player->status);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_setMixArgs(JNIEnv *env, jobject thiz) {
    if (player) {
        player->setMixArgs();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_pause(JNIEnv *env, jobject thiz) {
    if (player) {
        player->pause();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_continuePlay(JNIEnv *env, jobject thiz) {
    if (player) {
        player->continuePlay();
    }
}