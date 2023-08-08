#include <jni.h>
#include <string>
#include "player/Player.h"
#include "player/consts.h"
#include "utils/log4c.h"
#include "player/JNICallbackHelper.h"

//java有java的数据类型；C有C的数据类型；jni也有自己的数据类型；
//在java调用C时，会先到jni层，此时java的数据都已经变为了jni相应的类型，此时用JNIEnv将它们转为C的数据类型，就可以传给C层代码了。
//同样的，C层调用java时，也需要先将C的数据通过JNIEnv转为jni类型，然后通过JNIEnv调用java代码

Player *player = 0;
JavaVM *vm = 0;

/**
 * 在JNI_OnLoad方法中保存vm变量，因为vm可以跨线程；而JNIEnv不能跨线程，所以用vm在子线程初始化子线程的JNIEnv。
 * 注：
 * vm可以跨线程、跨函数；
 * JNIEnv不能跨线程、可以跨函数；   解决：vm->AttachCurrentThread(&env_child, 0)
 * jobject不可以跨线程、也不可以跨函数。  解决：this->job = env->NewGlobalRef(job);
 * **/
jint JNI_OnLoad(JavaVM *vm, void *args) {
    ::vm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_rdd_player_Player_setFilePathN(JNIEnv *env, jobject thiz, jstring path) {
    const char *file_path = env->GetStringUTFChars(path, 0);//从jni的String类型拿到C++的String类型
    int r = FILE_OPEN_FAIL;
    if (player) {
        r = player->setPath(file_path);
    }
    env->ReleaseStringUTFChars(path, file_path);//从jni的String类型拿到C++的String类型，使用完要释放
    return r;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_prepareN(JNIEnv *env, jobject thiz) {
    auto *helper = new JNICallbackHelper(vm, env, thiz);//auto关键字可以自动推断类型，不用自己写了
    player = new Player(helper);
    LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>播放器已经初始化\n");
    player->setMixArgs();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_completeN(JNIEnv *env, jobject thiz) {
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
Java_com_rdd_player_Player_setMixArgsN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->setMixArgs();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_pauseN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->pause();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_continueeN(JNIEnv *env, jobject thiz) {
    if (player) {
        player->continuePlay();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_resetN(JNIEnv *env, jobject thiz) {
    if (player) {
        if (player->status == STATUS_PAUSE) {
            player->continuePlay();
        }
        player->reset();
    }
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_rdd_player_Player_getStatusN(JNIEnv *env, jobject thiz) {
    if (player) {
        return player->status;
    } else {
        return STATUS_PLAYER_UNINIT;
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_startN(JNIEnv *env, jobject thiz, jint card_mode) {
    if (player) {
        player->setPlayStrategy(card_mode);
        player->start();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rdd_player_Player_releaseN(JNIEnv *env, jobject thiz) {
    if (player) {
        //当正在播放时不能直接释放player，因为播放线程还在用它。通过reset将播放线程停止，子线程中会依据finishByWorkThread标识在结束前释放player
        if (player->status == STATUS_PLAYING) {
            player->finishByWorkThread = true;
            player->reset();
            //比上面多了一步操作，要先唤醒线程
        } else if (player->status == STATUS_PAUSE) {
            player->finishByWorkThread = true;
            player->continuePlay();
            player->reset();
            //如果子线程没有在运行，那么直接释放player就可以了
        } else {
            delete player;
            player = 0;
            LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>播放器已经释放\n");
        }
    }
}