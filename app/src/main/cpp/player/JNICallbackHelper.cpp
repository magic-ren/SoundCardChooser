//
// Created by Administrator on 2023/7/31.
//
#include "JNICallbackHelper.h"

JNICallbackHelper::JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job) {
    this->vm = vm;
    this->env_main = env;
    //获取全局引用，解决jobject不能跨线程的问题
    this->job = env->NewGlobalRef(job);
    jclass jclass1 = env->GetObjectClass(job);
    this->jmd_callback = env->GetMethodID(jclass1, "onAudioDataCallback", "([BI)V");
    this->jmd_jump_callback = env->GetMethodID(jclass1, "onCompleteCallback",
                                               "(Ljava/lang/String;)V");
    this->jmd_error = env->GetMethodID(jclass1, "onError", "(I)V");
}

JNICallbackHelper::~JNICallbackHelper() {
    vm = 0;
    env_main->DeleteGlobalRef(job);
    job = 0;
    env_main = 0;
}

void JNICallbackHelper::onCallback(char *data, int size) {
    JNIEnv *env_child;
    //用vm->AttachCurrentThread(&env_child, 0)获取子线程的JNIEnv
    if (vm->AttachCurrentThread(&env_child, 0) == JNI_OK) {
        //用JNIEnv初始化jni的byte数组
        jbyteArray byteArray = env_child->NewByteArray(size);
        //在C中byte数组就用char*来表示，这里将C的byte数组转为jni的byte数组
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

void JNICallbackHelper::onError(int thread_mode, int error_code) {
    if (thread_mode == THREAD_MAIN) {
        env_main->CallVoidMethod(job, jmd_error, error_code);
    } else if (thread_mode == THREAD_CHILD) {
        JNIEnv *env_child;
        vm->AttachCurrentThread(&env_child, 0);
        env_child->CallVoidMethod(job, jmd_error, error_code);
        vm->DetachCurrentThread();
    }
}