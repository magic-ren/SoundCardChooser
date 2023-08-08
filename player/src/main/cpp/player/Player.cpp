//
// Created by Administrator on 2023/6/8.
//
#define PCM_CARD 0
#define PCM_DEVICE_IN 0
#define PCM_DEVICE_OUT 0

#include "Player.h"

Player::Player(JNICallbackHelper *jniCallbackHelper) {
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&cond, 0);

    this->jniCallbackHelper = jniCallbackHelper;

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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    if (file_path) {
        delete[] file_path;
        file_path = nullptr;
    }
    if (soundPlayStrategy) {
        delete soundPlayStrategy;
        soundPlayStrategy = nullptr;
    }
    if (mixer) {
        mixer_close(mixer);
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
        free(buffer);
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
    this->file_path = new char[strlen(file_path) + 1];
    strcpy(this->file_path, file_path);
    LOGI("file path is (%s)\n", file_path);
    if (file) {
        fclose(file);
    }
    file = fopen(file_path, "wb");
    if (!file) {
        LOGE("Unable to create file '%s'\n", file_path);
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_MAIN, ERROR_CREATE_FILE_FAIL);
        }
        return FILE_OPEN_FAIL;
    } else {
        LOGI("open file success '%s'\n", file_path);
        /* leave enough room for header */
        fseek(file, sizeof(struct wav_header), SEEK_SET);
        return FILE_OPEN_SUCCESS;
    }
}

void *task_start(void *args) {
    Player *player = static_cast<Player *>(args);
    player->start_();
    return 0;
}

void Player::start() {
    pthread_create(&pid_play, 0, task_start, this);
}

void Player::start_() {
    if (!soundPlayStrategy) {
        LOGE("声卡策略设置错误！\n");
        if (jniCallbackHelper) {
            jniCallbackHelper->onError(THREAD_CHILD, ERROR_CARD_STRATEGY_SETTING);
        }
        return;
    }
    int r = soundPlayStrategy->playSound();

    if (r == PLAY_FAIL) {
        resouceReset();
    }

}

void Player::afterPlay(pcm *pcm_target) {
    if (status == STATUS_COMPLETE) {
        LOGI("已完成\n");
        saveAndJump(pcm_target);
    }
    if (status == STATUS_UNPLAY) {
        LOGI("已重置\n");
        resouceReset();
        if (finishByWorkThread) {
            jniCallbackHelper->finishByWorkThread = true;
            delete this;
        }
    }
}

void Player::saveAndJump(pcm *pcm_target) {
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
    delete file_path;
    file_path = 0;
    file = 0;
}

void Player::resouceReset() {
    status = STATUS_UNPLAY;
    bytes_read = 0;
    fclose(file);
    remove(file_path);
    delete file_path;
    file_path = 0;
    file = 0;
}

void Player::setMixArgs() {
    if (!mixer) {
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
    pthread_cond_signal(&cond);
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
        lastMode = card_mode;
        return;
    }
    if (lastMode != card_mode) {
        delete soundPlayStrategy;
        soundPlayStrategy = 0;
        soundPlayStrategy = SoundPlayStrategyFactory::createStrategy(card_mode, this);
        lastMode = card_mode;
        return;
    }
}
