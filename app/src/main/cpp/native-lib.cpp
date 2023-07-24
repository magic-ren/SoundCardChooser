#include <jni.h>
#include <string>
#include "Player.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_startPlay(JNIEnv *env, jobject thiz) {
    Player *player = new Player();
    player->start();
}