package com.tlfs.tinyalsa_demo;

import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * @Author rendedong
 * @Date 2023/7/31
 */
public class Player {
    private static final String TAG = "Player";
    public static final int STATUS_UNPLAY = 1;
    public static final int STATUS_PLAYING = 2;
    public static final int STATUS_PAUSE = 3;
    public static final int STATUS_COMPLETE = 4;
    public static final int STATUS_UNINIT = -1;

    private AudioTrack audioTrack;
    private IJump iJump;
    private IErrorListener iErrorListener;

    static {
        System.loadLibrary("tinyalsa_demo");
    }

    public Player(IJump iJump, IErrorListener iErrorListener) {
        this.iJump = iJump;
        this.iErrorListener = iErrorListener;
    }

    public native void startPlay(int cardMode);

    public native void closePCM();

    public native void setFilePath(String path);

    public native void prepare();

    public native void complete();

    public native void setMixArgs();

    public native void pause();

    public native void continuePlay();

    public native void reset();

    public native int getStatus();

    private void onAudioDataCallback(byte[] data, int size) {

        if (audioTrack == null) {
            audioTrack = new AudioTrack(AudioManager.STREAM_VOICE_CALL, 44100, AudioFormat.CHANNEL_OUT_STEREO,
                    AudioFormat.ENCODING_PCM_16BIT, size, AudioTrack.MODE_STREAM);
        }
        audioTrack.play();
        audioTrack.write(data, 0, size);
    }

    private void onCompleteCallback(String path) {
        Handler handler = new Handler(Looper.getMainLooper());
        handler.post(new Runnable() {
            @Override
            public void run() {
                iJump.jumpActivity(path);
            }
        });
    }

    private static final int ERROR_OPEN_PCMC2D0C_FAIL = 1;
    private static final int ERROR_INIT_PCMC2D0C_BUFFER_FAIL = 2;
    private static final int ERROR_SAVE_AUDIO_FAIL = 3;
    private static final int ERROR_READ_PCMC2D0C_FAIL = 4;
    private static final int ERROR_OPEN_PCMC0D0C_FAIL = 5;
    private static final int ERROR_INIT_PCMC0D0C_BUFFER_FAIL = 6;
    private static final int ERROR_READ_PCMC0D0C_FAIL = 7;
    private static final int ERROR_INIT_MERGE_BUFFER_FAIL = 8;
    private static final int ERROR_READ_PCMC0D0C_PCMC2D0C_FAIL = 9;
    private static final int ERROR_CREATE_FILE_FAIL = 10;
    private static final int ERROR_CARD_STRATEGY_SETTING = 11;
    private static final int ERROR_OPEN_MIXER_FAIL = 12;
    private static final int ERROR_SET_PCMC0D0C_MIC_FAIL = 13;

    private void onError(int errorCode) {
        String msg = "";
        switch (errorCode) {
            case ERROR_OPEN_PCMC2D0C_FAIL:
                msg = "打开7202声卡设备文件失败";
                break;
            case ERROR_INIT_PCMC2D0C_BUFFER_FAIL:
                msg = "初始化7202声卡使用的缓冲失败";
                break;
            case ERROR_SAVE_AUDIO_FAIL:
                msg = "边录边播时：向保存文件写入音频数据失败";
                break;
            case ERROR_READ_PCMC2D0C_FAIL:
                msg = "7202声卡录制失败";
                break;
            case ERROR_OPEN_PCMC0D0C_FAIL:
                msg = "打开817声卡设备文件失败";
                break;
            case ERROR_INIT_PCMC0D0C_BUFFER_FAIL:
                msg = "初始化817声卡使用的缓冲失败";
                break;
            case ERROR_READ_PCMC0D0C_FAIL:
                msg = "817声卡录制失败";
                break;
            case ERROR_INIT_MERGE_BUFFER_FAIL:
                msg = "初始化合成使用的缓冲失败";
                break;
            case ERROR_READ_PCMC0D0C_PCMC2D0C_FAIL:
                msg = "声卡合成录制失败";
                break;
            case ERROR_CREATE_FILE_FAIL:
                msg = "创建录音文件失败";
                break;
            case ERROR_CARD_STRATEGY_SETTING:
                msg = "播放策略设置错误";
                break;
            case ERROR_OPEN_MIXER_FAIL:
                msg = "混音器打开失败";
                break;
            case ERROR_SET_PCMC0D0C_MIC_FAIL:
                msg = "混音器设置失败";
                break;
        }
        if (iErrorListener != null) {
            iErrorListener.onError(msg);
        }
    }
}
