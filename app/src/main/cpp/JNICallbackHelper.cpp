//
// Created by Administrator on 2023/7/31.
//
#include "JNICallbackHelper.h"

JNICallbackHelper::JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job) {
    this->vm = vm;
    this->env_main = env;
    this->job = env->NewGlobalRef(job);
    jclass jclass1 = env->GetObjectClass(job);
    this->jmd_callback = env->GetMethodID(jclass1, "onAudioDataCallback", "([BI)V");
    this->jmd_jump_callback = env->GetMethodID(jclass1, "onCompleteCallback",
                                               "(Ljava/lang/String;)V");
}

JNICallbackHelper::~JNICallbackHelper() {
    vm = 0;
    env_main->DeleteGlobalRef(job);
    job = 0;
    env_main = 0;
}

void JNICallbackHelper::onCallback(char *data, int size) {
    JNIEnv *env_child;
    if (vm->AttachCurrentThread(&env_child, 0) == JNI_OK) {
        jbyteArray byteArray = env_child->NewByteArray(size);
        env_child->SetByteArrayRegion(byteArray, 0, size, reinterpret_cast<jbyte *>(data));
        env_child->CallVoidMethod(job, jmd_callback, byteArray, size);
        vm->DetachCurrentThread();
    }
}

void JNICallbackHelper::onJumpCallback(char *path) {
    JNIEnv *env_child;
    if (vm->AttachCurrentThread(&env_child, 0) == JNI_OK) {
        jstring javaPath = env_child->NewStringUTF(path);
        env_child->CallVoidMethod(job, jmd_jump_callback, javaPath);
        env_child->DeleteLocalRef(javaPath);
        vm->DetachCurrentThread();
    }
}