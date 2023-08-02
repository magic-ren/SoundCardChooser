//
// Created by Administrator on 2023/8/2.
//

#include "SoundPlayStrategy8L7L.h"
#include "Player.h"

SoundPlayStrategy8L7L::SoundPlayStrategy8L7L(Player *player) : SoundPlayStrategy(player) {}

int SoundPlayStrategy8L7L::playSound() {
    LOGE("播放策略：817左+7202左");
    if (!playerPtr->pcm_in) {
        playerPtr->pcm_in = pcm_open(0, 0, PCM_IN, &playerPtr->config);
        if (!playerPtr->pcm_in || !pcm_is_ready(playerPtr->pcm_in)) {
            LOGE("Unable to open PCM device (%s)\n", pcm_get_error(playerPtr->pcm_in));
            return PLAY_FAIL;
        } else {
            LOGE("pcmC0D0c打开啦");
        }
    } else {
        LOGE("pcmC0D0c之前已结打开啦");
    }

    if (!playerPtr->pcm_in_2) {
        playerPtr->pcm_in_2 = pcm_open(2, 0, PCM_IN, &playerPtr->config);
        if (!playerPtr->pcm_in_2 || !pcm_is_ready(playerPtr->pcm_in_2)) {
            LOGE("Unable to open PCM device (%s)\n", pcm_get_error(playerPtr->pcm_in_2));
            return PLAY_FAIL;
        } else {
            LOGE("pcmC0D2c打开啦");
        }
    } else {
        LOGE("pcmC0D2c之前已结打开啦");
    }

    LOGE("一个周期内有多少采样点：%u", pcm_get_buffer_size(playerPtr->pcm_in));

    // pcm_frames_to_bytes()是获取一个周期内占用多少字节。
    //pcm_get_buffer_size()是获取一个周期内有多少采样点；
    //817和7202的size是一样的，所以这里写一个就行了
    if (!playerPtr->size) {
        playerPtr->size = pcm_frames_to_bytes(playerPtr->pcm_in, pcm_get_buffer_size(
                playerPtr->pcm_in));
    }
    LOGE("一个周期内占用多少字节：%u", playerPtr->size);


    if (!playerPtr->buffer) {
        playerPtr->buffer = static_cast<char *>(malloc(playerPtr->size));
        if (!playerPtr->buffer) {
            LOGE("buffer:Unable to allocate %u bytes\n", playerPtr->size);
            free(playerPtr->buffer);
            return PLAY_FAIL;
        } else {
            LOGE("buffer初始化啦");
        }
    } else {
        LOGE("buffer之前已结初始化啦");
    }
    if (!playerPtr->buffer2) {
        playerPtr->buffer2 = static_cast<char *>(malloc(playerPtr->size));
        if (!playerPtr->buffer2) {
            LOGE("buffer2:Unable to allocate %u bytes\n", playerPtr->size);
            free(playerPtr->buffer2);
            return PLAY_FAIL;
        } else {
            LOGE("buffer2初始化啦");
        }
    } else {
        LOGE("buffer2之前已结初始化啦");
    }
    if (!playerPtr->buffer3) {
        playerPtr->buffer3 = static_cast<char *>(malloc(playerPtr->size));
        if (!playerPtr->buffer3) {
            LOGE("buffer3:Unable to allocate %u bytes\n", playerPtr->size);
            free(playerPtr->buffer3);
            return PLAY_FAIL;
        } else {
            LOGE("buffer3初始化啦");
        }
    } else {
        LOGE("buffer3之前已结初始化啦");
    }

    LOGE("Capturing sample: %u ch, %u hz, %u bit\n", 2, 44100,
         pcm_format_to_bits(PCM_FORMAT_S16_LE));

    playerPtr->status = STATUS_PLAYING;

    while (playerPtr->status != STATUS_COMPLETE && playerPtr->status != STATUS_UNPLAY &&
           !pcm_read(playerPtr->pcm_in, playerPtr->buffer, playerPtr->size) &&
           !pcm_read(playerPtr->pcm_in_2, playerPtr->buffer2, playerPtr->size)) {

        pthread_mutex_lock(&playerPtr->mutex);
        if (playerPtr->status == STATUS_PAUSE) {
            pthread_cond_wait(&playerPtr->cond, &playerPtr->mutex);
        }
        pthread_mutex_unlock(&playerPtr->mutex);

        std::string output1;
        for (int i = 0; i < playerPtr->size; ++i) {
            output1 += std::to_string(static_cast<unsigned char>(*(playerPtr->buffer + i))) + " ";
        }
        LOGE("817合成前：%s", output1.c_str());

        std::string output2;
        for (int i = 0; i < playerPtr->size; ++i) {
            output2 += std::to_string(static_cast<unsigned char>(*(playerPtr->buffer2 + i))) + " ";
        }
        LOGE("7202合成前：%s", output2.c_str());


        int a = 0;
        int b = 0;
        int c = 0;
        int n = playerPtr->size / 4;
        for (int k = 1; k <= n; ++k) {
            playerPtr->buffer3[c] = playerPtr->buffer[a];
            playerPtr->buffer3[c + 1] = playerPtr->buffer[a + 1];
            playerPtr->buffer3[c + 2] = playerPtr->buffer2[b];
            playerPtr->buffer3[c + 3] = playerPtr->buffer2[b + 1];
            a += 4;
            b += 4;
            c += 4;
        }

        if (playerPtr->jniCallbackHelper) {
            playerPtr->jniCallbackHelper->onCallback(playerPtr->buffer3, playerPtr->size);
        } else {
            LOGE("未初始化播放回调");
            return PLAY_FAIL;
        }

        std::string output;
        for (int i = 0; i < playerPtr->size; ++i) {
            output += std::to_string(static_cast<unsigned char>(*(playerPtr->buffer3 + i))) + " ";
        }
        LOGE("合成后：%s", output.c_str());

        if (fwrite(playerPtr->buffer3, 1, playerPtr->size, playerPtr->file) != playerPtr->size) {
            LOGE("保存音频失败\n");
            return PLAY_FAIL;
        }
        playerPtr->bytes_read += playerPtr->size;


    }
    LOGE("播放结束后的状态%d", playerPtr->status);

    playerPtr->afterPlay(playerPtr->pcm_in);

    return PLAY_SUCCESS;
}