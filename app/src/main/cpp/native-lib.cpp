#include <jni.h>
#include <string>
#include "Player.h"

Player *player;

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_startPlay(JNIEnv *env, jobject thiz) {
    player = new Player();
    player->start();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_closePCM(JNIEnv *env, jobject thiz) {
    player->closePcm();
}