//
// Created by rendedong on 2023/6/8.
//
#define PCM_CARD 0
#define PCM_DEVICE_IN 0
#define PCM_DEVICE_OUT 0

#include "Player.h"

Player::Player(JNICallbackHelper *jniCallbackHelper) {
    pthread_mutex_init(&mutex, 0);//初始化互斥锁，用于同步操作
    pthread_cond_init(&cond, 0);//初始化条件变量，用于线程间的等待、唤醒

    this->jniCallbackHelper = jniCallbackHelper;

    //这些参数都是从tinyalsa库的示例代码里copy过来的
    header.riff_id = ID_RIFF;
    header.riff_sz = 0;
    header.riff_fmt = ID_WAVE;
    header.fmt_id = ID_FMT;
    header.fmt_sz = 16;
    header.audio_format = FORMAT_PCM;
    header.num_channels = channels;
    header.sample_rate = rate;
    header.bits_per_sample = pcm_format_to_bits(format);
    header.byte_rate = (header.bits_per_sample / 8) * channels * rate;
    header.block_align = channels * (header.bits_per_sample / 8);
    header.data_id = ID_DATA;

    memset(&config, 0, sizeof(config));
    config.channels = 2;
    config.rate = 44100;
    config.period_size = 16 * (44100 / 1000);   //16ms算一帧，16ms内有多少采样点
    config.period_count = 4;    //一个周期内采集几帧？
    config.format = PCM_FORMAT_S16_LE;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;
}

Player::~Player() {
    pthread_mutex_destroy(&mutex);//不要忘记互斥锁的释放
    pthread_cond_destroy(&cond);//不要忘记条件变量的释放
    if (file_path) {
        delete[] file_path;//数组的释放方式
        file_path = nullptr;//防止悬空指针，用nullptr挺好
    }
    if (soundPlayStrategy) {
        delete soundPlayStrategy;//new出来的对象用delete
        soundPlayStrategy = nullptr;
    }
    if (mixer) {
        mixer_close(mixer);//一定要注意，有些资源的释放一定要调用库指定的释放方法，否则会报错
        mixer = nullptr;
    }
    if (pcm_out) {
        pcm_close(pcm_out);
        pcm_out = nullptr;
    }
    if (pcm_in) {
        pcm_close(pcm_in);
        pcm_in = nullptr;
    }
    if (pcm_in_2) {
        pcm_close(pcm_in_2);
        pcm_in_2 = nullptr;
    }
    if (buffer) {
        free(buffer);//malloc的内存用free
        buffer = nullptr;
    }
    if (buffer2) {
        free(buffer2);
        buffer2 = nullptr;
    }
    if (buffer3) {
        free(buffer3);
        buffer3 = nullptr;
    }
    if (jniCallbackHelper) {
        delete jniCallbackHelper;
        jniCallbackHelper = nullptr;
    }
    if (file) {
        fclose(file);
        file = nullptr;
    }
}

int Player::setPath(const char *file_path) {
    //C++中创建数组。注意这里创建的是字符串（字符数组），而字符串的最后一个字符是'\0'，所以长度要+1
    this->file_path = new char[strlen(file_path) + 1];
    strcpy(this->file_path, file_path);//拷贝字符串不是像java那样直接=，而是要先创建新的字符数组，然后调用strcpy将内容拷贝过啦
    LOGI("file path is (%s)\n", file_path);
    if (file) {
        fclose(file);
    }
    file = fopen(file_path, "wb");//打开文件。参数是模式，关系到读、写、是否新建文件等
    if (!file) {
        LOGE("Unable to create file '%s'\n", file_path);
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_MAIN, ERROR_CREATE_FILE_FAIL);
        }
        return FILE_OPEN_FAIL;
    } else {
        LOGI("open file success '%s'\n", file_path);
        /* leave enough room for header */
        fseek(file, sizeof(struct wav_header), SEEK_SET);//定位到文件的指定位置
        return FILE_OPEN_SUCCESS;
    }
}

/**
 * 函数指针。
 * 函数名是task_start。
 * 返回值是void*。
 * 参数有一个，类型是void*。
 * 它是依据pthread_create里的函数指针类型定义的函数，void* (*__start_routine)(void*)。
 * **/
void *task_start(void *args) {
    Player *player = static_cast<Player *>(args);//只能传一个参数，所以传的是Player实例，这一就可以在子线程中调用Player的方法。
    player->start_();
    return 0;//这里不要忘记返回，不然程序会出错。
}

void Player::start() {
    //void* (*__start_routine)(void*)：*__start_routine代表着这个参数是一个函数指针，两个void*分别代表着返回值类型和参数类型。
    //其他的例子：void(*method)(int,int)：代表着函数指针类型，类型名称是method（可以用这个名称来调用），返回值是void，两个int参数。
    //传入函数指针类型，就相当于传入一个回调。实际函数的写法必须和函数指针的声明一一对应。
    //函数被传入时会退化成指针。
    pthread_create(&pid_play, 0, task_start,
                   this);//创建线程，参数1是线程id，参数3是线程中运行的方法，参数4是void* (*__start_routine)(void*)中的参数。
}

void Player::start_() {
    if (!soundPlayStrategy) {
        LOGE("声卡策略设置错误！\n");
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_CHILD, ERROR_CARD_STRATEGY_SETTING);
        }
        return;
    }

    //使用策略模式播放声音。将播放模式解耦，方便动态更改策略和维护代码。
    int r = soundPlayStrategy->playSound();

    //播放失败时也要将相关资源重置。
    if (r == PLAY_FAIL) {
        resouceReset();
    }

}

/**
 * 结束播放后的操作。子线程运行。
 * **/
void Player::afterPlay(pcm *pcm_target) {
    if (status == STATUS_COMPLETE) {
        //完成了播放，那么保存音频和跳转页面。
        LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>已完成\n");
        saveAndJump(pcm_target);
    }
    if (status == STATUS_UNPLAY) {
        //重置了播放，那么重置相关资源。
        LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>已重置\n");
        resouceReset();
        //如果finishByWorkThread是true，那么根据业务设定就是播放线程在跑着时需要释放player（见native-lib.cpp的releaseN方法），
        //可是播放线程中又在使用player，所以只能在线程跑完结束时释放player，
        //释放的时候又会涉及到jniCallbackHelper的释放，其里面又会涉及到线程（见JniCallbackHelper的析构方法），
        //所以需要给jniCallbackHelper一个在子线程调用的标记。
        if (finishByWorkThread) {
            jniCallbackHelper->finishByWorkThread = true;
            delete this;
            LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>播放器已经释放\n");
        }
    }
}

void Player::saveAndJump(pcm *pcm_target) {
    //保存代码从tinyalsa库的示例代码里copy过来的
    unsigned int recFrames = pcm_bytes_to_frames(pcm_target, bytes_read);
    header.data_sz = recFrames * header.block_align;
    header.riff_sz = header.data_sz + sizeof(header) - 8;
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(struct wav_header), 1, file);
    fclose(file);
    bytes_read = 0;
    if (jniCallbackHelper) {
        jniCallbackHelper->onJumpCallback(file_path);
    }
    delete[] file_path;
    file_path = 0;
    file = 0;
}

void Player::resouceReset() {
    status = STATUS_UNPLAY;
    bytes_read = 0;
    fclose(file);
    remove(file_path);//删除文件（因为这个文件未录制完成，不要了）
    delete[] file_path;
    file_path = 0;
    file = 0;
}

void Player::setMixArgs() {
    if (!mixer) {
        //打开混音器，这里用来设置声卡的播放通道和录音通道所用的设备
        mixer = mixer_open(0);
    }
    if (!mixer) {
        LOGE("%s", "Failed to open mixer\n");
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_MAIN, ERROR_OPEN_MIXER_FAIL);
        }
        return;
    }

    //这里必须这种写法，不能用char *value1 = "1";不然会报错
    char *value1 = new char[2];
    strcpy(value1, "1");
    int ret1 = 0;

    //对应命令行里输入tinymix -D 0 1 1。意思是将0号声卡的1号通道设置为1这个类型
    ret1 = tinymix_set_value(mixer, "1", &value1, 1);

    if (ret1 != 0) {
        LOGE("%s", "Failed to set value : tinymix -D 0 1 1\n");
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_MAIN, ERROR_SET_PCMC0D0C_MIC_FAIL);
        }
    } else {
        LOGI("%s", "Success to set value : tinymix -D 0 1 1\n");
    }

}

void Player::pause() {
    pthread_mutex_lock(&mutex);
    status = STATUS_PAUSE;
    pthread_mutex_unlock(&mutex);
}

void Player::continuePlay() {
    pthread_mutex_lock(&mutex);
    status = STATUS_PLAYING;
    pthread_cond_signal(&cond);//用条件变量在主线程中唤醒播放线程
    pthread_mutex_unlock(&mutex);
}

void Player::reset() {
    pthread_mutex_lock(&mutex);
    status = STATUS_UNPLAY;
    pthread_mutex_unlock(&mutex);
}

void Player::setPlayStrategy(int card_mode) {
    if (!soundPlayStrategy) {
        soundPlayStrategy = SoundPlayStrategyFactory::createStrategy(card_mode, this);
        lastMode = card_mode;//如果是页面内第一次播放，那么记录下播放策略
        return;
    }
    //如果不是页面内第一次播放，而且此次播放的策略和之前的不同，那么就重新设置策略。
    //策略没有改变的话就什么都不用做。
    if (lastMode != card_mode) {
        delete soundPlayStrategy;
        soundPlayStrategy = 0;
        soundPlayStrategy = SoundPlayStrategyFactory::createStrategy(card_mode, this);
        lastMode = card_mode;
        return;
    }
}
