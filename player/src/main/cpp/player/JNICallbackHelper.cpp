//
// Created by rendedong on 2023/7/31.
//
#include "JNICallbackHelper.h"

JNICallbackHelper::JNICallbackHelper(JavaVM *vm, JNIEnv *env, jobject job) {
    this->vm = vm;//JavaVM可以跨线程
    this->env_main = env;//JNIEnv不可以跨线程，所以之后在子线程中要用JavaVM新建子线程的JNIEnv
    this->job = env->NewGlobalRef(job);//获取全局引用，解决jobject不能跨线程的问题
    jclass jclass1 = env->GetObjectClass(job);//由jobject获取jclass
    this->jmd_callback = env->GetMethodID(jclass1, "onAudioDataCallback",
                                          "([BI)V");//获取java类的方法id，注意“方法签名”
    this->jmd_jump_callback = env->GetMethodID(jclass1, "onCompleteCallback",
                                               "(Ljava/lang/String;)V");
    this->jmd_error = env->GetMethodID(jclass1, "onError", "(I)V");
}

JNICallbackHelper::~JNICallbackHelper() {
    if (finishByWorkThread) {
        //可能是从子线程析构Player，从而析构JNICallbackHelper的，所以这里的JNIEnv要用子线程的
        JNIEnv *env_child;
        vm->AttachCurrentThread(&env_child, 0);//创建子线程JNIEnv
        env_child->DeleteGlobalRef(job);//全局引用一定要释放
        vm->DetachCurrentThread();//创建子线程JNIEnv
    } else {
        env_main->DeleteGlobalRef(job);
    }
}

void JNICallbackHelper::onCallback(char *data, int size) {
    JNIEnv *env_child;
    //用vm->AttachCurrentThread(&env_child, 0)获取子线程的JNIEnv
    if (vm->AttachCurrentThread(&env_child, 0) == JNI_OK) {
        //用JNIEnv初始化jni的byte数组
        jbyteArray byteArray = env_child->NewByteArray(size);
        //在C中byte数组就用char*来表示，这里将C的byte数组转为jni的byte数组
        env_child->SetByteArrayRegion(byteArray, 0, size, reinterpret_cast<jbyte *>(data));
        env_child->CallVoidMethod(job, jmd_callback, byteArray, size);//调用java层的方法，注意这里的参数一定都是jni的类型
        vm->DetachCurrentThread();
    }
}

void JNICallbackHelper::onJumpCallback(char *path) {
    JNIEnv *env_child;
    if (vm->AttachCurrentThread(&env_child, 0) == JNI_OK) {
        jstring javaPath = env_child->NewStringUTF(path);//将C++的String类型转为jni的String类型
        env_child->CallVoidMethod(job, jmd_jump_callback, javaPath);
        env_child->DeleteLocalRef(javaPath);//jni的String类型用完后要释放（本地引用要释放）
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