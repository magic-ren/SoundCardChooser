#include <jni.h>
#include <string>
#include "player/Player.h"
#include "player/consts.h"
#include "utils/log4c.h"
#include "player/JNICallbackHelper.h"

Player *player = 0;
JavaVM *vm = 0;

jint JNI_OnLoad(JavaVM *vm, void *args) {
    ::vm = vm;
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_closePCMN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->closePcm();
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_setFilePathN(JNIEnv *env, jobject thiz, jstring path) {
    const char *file_path = env->GetStringUTFChars(path, 0);
    int r = FILE_OPEN_FAIL;
    if (player) {
        r = player->setPath(file_path);
    }
    return r;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_prepareN(JNIEnv *env, jobject thiz) {
    auto *helper = new JNICallbackHelper(vm, env, thiz);
    player = new Player(helper);
    player->setMixArgs();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_completeN(JNIEnv *env, jobject thiz) {
    if (player) {
        if (player->status == STATUS_PAUSE) {
            player->continuePlay();
        }
        player->status = STATUS_COMPLETE;
        LOGE("set status suceess %d\n", player->status);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_setMixArgsN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->setMixArgs();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_pauseN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->pause();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_continueeN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->continuePlay();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_resetN(JNIEnv *env, jobject thiz) {
    if (player) {
        if (player->status == STATUS_PAUSE) {
            player->continuePlay();
        }
        player->reset();
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_getStatusN(JNIEnv *env, jobject thiz) {
    if (player) {
        return player->status;
    } else {
        return STATUS_PLAYER_UNINIT;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_Player_startN(JNIEnv *env, jobject thiz, jint card_mode) {
    if (player) {
        player->setPlayStrategy(card_mode);
        player->start();
    }
}
