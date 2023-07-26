#include <jni.h>
#include <string>
#include "Player.h"

Player *player;

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_startPlay(JNIEnv *env, jobject thiz) {
    player->start();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_closePCM(JNIEnv *env, jobject thiz) {
    player->closePcm();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_setFilePath(JNIEnv *env, jobject thiz, jstring path) {
    const char* file_path = env->GetStringUTFChars(path, 0);
    player = new Player(file_path);
}