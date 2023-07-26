#include <jni.h>
#include <string>
#include "Player.h"

Player *player=0;

extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_startPlay(JNIEnv *env, jobject thiz) {
    if(player){
        player->start();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_closePCM(JNIEnv *env, jobject thiz) {
    if(player){
        player->closePcm();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_setFilePath(JNIEnv *env, jobject thiz, jstring path) {
    const char* file_path = env->GetStringUTFChars(path, 0);
    if(player){
        player->setPath(file_path);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tlfs_tinyalsa_1demo_MainActivity_prepare(JNIEnv *env, jobject thiz) {
    player = new Player();
}