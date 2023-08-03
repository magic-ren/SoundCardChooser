//
// Created by Administrator on 2023/8/2.
//

#include "SoundPlayStrategy8L.h"
#include "../../Player.h"

SoundPlayStrategy8L::SoundPlayStrategy8L(Player *player) : SoundPlayStrategy(player) {}

int SoundPlayStrategy8L::playSound() {
    LOGE("播放策略：817左");
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
    LOGE("一个周期内有多少采样点：%u", pcm_get_buffer_size(playerPtr->pcm_in));

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

    LOGE("Capturing sample: %u ch, %u hz, %u bit\n", 2, 44100,
         pcm_format_to_bits(PCM_FORMAT_S16_LE));

    playerPtr->status = STATUS_PLAYING;

    while (playerPtr->status != STATUS_COMPLETE && playerPtr->status != STATUS_UNPLAY &&
           !pcm_read(playerPtr->pcm_in, playerPtr->buffer, playerPtr->size)) {

        pthread_mutex_lock(&playerPtr->mutex);
        if (playerPtr->status == STATUS_PAUSE) {
            pthread_cond_wait(&playerPtr->cond, &playerPtr->mutex);
        }
        pthread_mutex_unlock(&playerPtr->mutex);


        std::string output2;
        for (int i = 0; i < playerPtr->size; ++i) {
            output2 += std::to_string(static_cast<unsigned char>(*(playerPtr->buffer + i))) + " ";
        }
        LOGE("817取左声道前：%s", output2.c_str());


        //3,4复制1，2。
        //声卡是817时：
        //817是左声道录制的听诊音，所以这里等于是将听诊音复制到了右声道，可以播放成功。
        int i = 0;
        int j = 2;
        int n = playerPtr->size / 4;
        for (int k = 1; k <= n; ++k) {
            playerPtr->buffer[j] = playerPtr->buffer[i];
            playerPtr->buffer[j + 1] = playerPtr->buffer[i + 1];
            i += 4;
            j += 4;
        }


        if (playerPtr->jniCallbackHelper) {
            playerPtr->jniCallbackHelper->onCallback(playerPtr->buffer, playerPtr->size);
        } else {
            LOGE("未初始化播放回调");
            return PLAY_FAIL;
        }

        std::string output;
        for (int i = 0; i < playerPtr->size; ++i) {
            output += std::to_string(static_cast<unsigned char>(*(playerPtr->buffer + i))) + " ";
        }
        LOGE("817取左声道后：%s", output.c_str());


        if (fwrite(playerPtr->buffer, 1, playerPtr->size, playerPtr->file) != playerPtr->size) {
            LOGE("保存音频失败\n");
            return PLAY_FAIL;
        }
        playerPtr->bytes_read += playerPtr->size;

    }

    LOGE("播放结束后的状态%d", playerPtr->status);

    playerPtr->afterPlay(playerPtr->pcm_in);

    return PLAY_SUCCESS;

}