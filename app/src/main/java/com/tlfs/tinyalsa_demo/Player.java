package com.tlfs.tinyalsa_demo;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

/**
 * @Author rendedong
 * @Date 2023/7/31
 */
public class Player {
    private static final String TAG = "Player";
    private static final int STATUS_UNPLAY = 1;
    private static final int STATUS_PLAYING = 2;
    private static final int STATUS_PAUSE = 3;
    private static final int STATUS_COMPLETE = 4;

    private AudioTrack audioTrack;

    static {
        System.loadLibrary("tinyalsa_demo");
    }

    public native void startPlay();

    public native void closePCM();

    public native void setFilePath(String path);

    public native void prepare();

    public native void complete();

    public native void setMixArgs();

    public native void pause();

    public native void continuePlay();

    public native void reset();

    private void onAudioDataCallback(byte[] data, int size) {

        if (audioTrack == null) {
            audioTrack = new AudioTrack(AudioManager.STREAM_VOICE_CALL, 44100, AudioFormat.CHANNEL_OUT_STEREO,
                    AudioFormat.ENCODING_PCM_16BIT, size, AudioTrack.MODE_STREAM);
        }
        Log.e("RDD", "onAudioDataCallback: " + audioTrack.hashCode());
        audioTrack.play();
        audioTrack.write(data, 0, size);
    }
}
