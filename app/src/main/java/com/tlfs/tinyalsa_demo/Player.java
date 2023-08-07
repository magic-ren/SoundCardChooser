package com.tlfs.tinyalsa_demo;

import android.content.Context;
import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

/**
 * @Author rendedong
 * @Date 2023/7/31
 */
public class Player implements IPlayer {
    //播放器状态码
    public static final int STATUS_UNPLAY = 1;
    public static final int STATUS_PLAYING = 2;
    public static final int STATUS_PAUSE = 3;
    public static final int STATUS_COMPLETE = 4;
    public static final int STATUS_UNINIT = -1;

    private AudioTrack audioTrack;//用于java层播放音频数据
    private IJump iJump;//播放完成后的跳转回调
    private IErrorListener iErrorListener;//C层发生错误的错误回调

    private String rootFilePath = "";//由客户端传入的文件根路径，用以确定保存文件的路径。  格式：根路径+时间.wav
    private Context context;

    static {
        System.loadLibrary("tinyalsa_demo");
    }

    public Player(Context context, String rootFilePath, IJump iJump, IErrorListener iErrorListener) {
        this.context = context;
        this.rootFilePath = rootFilePath;
        this.iJump = iJump;
        this.iErrorListener = iErrorListener;
    }

    @Override
    public void prepare() {
        prepareN();
    }

    //音频保存文件打开是否成功
    private static final int FILE_OPEN_SUCCESS = 1;
    private static final int FILE_OPEN_FAIL = -1;

    @Override
    public void start() {

        //确保播放策略已被正确设置
        int cardMode = Utils.getCustomProperty("voice.mode");
        //2:只取7202的左声道数据。（只取环境音）
        //8:只取817的左声道数据。（只取听诊音）
        //10:取817的左声道和7202的左声道。（同时取听诊音和环境音）
        if (cardMode != 2 && cardMode != 8 && cardMode != 10) {
            Toast.makeText(context, "播放策略设置错误", Toast.LENGTH_SHORT).show();
            return;
        }

        //在某些情况下不能播放并提示
        if (getStatus() == STATUS_PLAYING) {
            Toast.makeText(context, "正在播放", Toast.LENGTH_SHORT).show();
            return;
        }
        if (getStatus() == STATUS_PAUSE) {
            Toast.makeText(context, "播放暂停中，请使用“继续”功能", Toast.LENGTH_SHORT).show();
            return;
        }
        if (getStatus() == STATUS_UNINIT) {
            Toast.makeText(context, "播放器未初始化", Toast.LENGTH_SHORT).show();
            return;
        }

        //设置此次播放录音的保存文件路径并打开文件
        String path = rootFilePath + Utils.getCurrentTime() + ".wav";
        if (setFilePathN(path) == FILE_OPEN_FAIL) return;

        startN(cardMode);
    }

    @Override
    public void pause() {
        pauseN();
    }

    @Override
    public void continuee() {
        continueeN();
    }

    @Override
    public void reset() {
        resetN();
    }

    @Override
    public void complete() {
        //未开始播放时，不能完成并保存
        if (getStatus() == STATUS_UNPLAY || getStatus() == STATUS_COMPLETE) {
            Toast.makeText(context, "未开始播放", Toast.LENGTH_SHORT).show();
            return;
        }
        completeN();
    }

    @Override
    public int getStatus() {
        return getStatusN();
    }

    @Override
    public void release() {
        releaseN();
    }

    private native void startN(int cardMode);

    private native void releaseN();

    private native int setFilePathN(String path);

    private native void prepareN();

    private native void completeN();

    private native void setMixArgsN();

    private native void pauseN();

    private native void continueeN();

    private native void resetN();

    private native int getStatusN();

    /**
     * C层的播放回调。
     * 此操作在子线程中。
     **/
    private void onAudioDataCallback(byte[] data, int size) {
        if (audioTrack == null) {
            audioTrack = new AudioTrack(AudioManager.STREAM_VOICE_CALL, 44100, AudioFormat.CHANNEL_OUT_STEREO,
                    AudioFormat.ENCODING_PCM_16BIT, size, AudioTrack.MODE_STREAM);
        }
        audioTrack.play();
        audioTrack.write(data, 0, size);
    }

    /**
     * C层的播放录音完成后的回调。
     * 此操作在子线程中。
     **/
    private void onCompleteCallback(String path) {
        Handler handler = new Handler(Looper.getMainLooper());
        handler.post(new Runnable() {
            @Override
            public void run() {
                iJump.jumpActivity(path);
            }
        });
    }

    //C层报错的错误码
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

    /**
     * C层报错的回调。
     * 此操作在子线程中。
     **/
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
            Handler handler = new Handler(Looper.getMainLooper());
            String finalMsg = msg;
            handler.post(new Runnable() {
                @Override
                public void run() {
                    iErrorListener.onError(finalMsg);
                }
            });
        }
    }


}
