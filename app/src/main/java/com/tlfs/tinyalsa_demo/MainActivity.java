package com.tlfs.tinyalsa_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.jakewharton.rxbinding4.view.RxView;
import com.tbruyelle.rxpermissions3.RxPermissions;
import com.tlfs.tinyalsa_demo.databinding.ActivityMainBinding;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity implements IJump {

    private static final String TAG = "MainActivity";


    private ActivityMainBinding binding;

    Player mPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        final RxPermissions rxPermissions = new RxPermissions(this);

        mPlayer = new Player(MainActivity.this);

        Button btn = binding.sampleText;
        Button btnProp = binding.prop;
        Button btnClose = binding.close;
        Button btnSetPath = binding.setPath;
        Button btnPrepare = binding.prepare;
        Button btnComplete = binding.complete;
        Button btnSetMix = binding.setMix;
        Button btnPause = binding.pause;
        Button btnContinue = binding.continuePlay;
        Button btnReset = binding.reset;
        Button btnStatus = binding.getStatus;

        RxView.clicks(btn)
                .compose(rxPermissions.ensure(Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE))
                .subscribe(granted -> {
                    if (granted) {
                        mPlayer.startPlay();
                    } else {
                        Toast.makeText(MainActivity.this, "权限被拒绝，无法录制音频！", Toast.LENGTH_SHORT).show();
                    }
                });

        btnProp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String customPropertyValue = getCustomProperty("voice.mode");
                if (!TextUtils.isEmpty(customPropertyValue)) {
                    // 在这里处理获取到的自定义属性值
                    Toast.makeText(MainActivity.this, "属性值: " + customPropertyValue, Toast.LENGTH_SHORT).show();
//                    Log.e(TAG, "属性值: "+customPropertyValue);
                } else {
                    // 自定义属性不存在或无法获取其值
                    Toast.makeText(MainActivity.this, "属性值: null", Toast.LENGTH_SHORT).show();
//                    Log.e(TAG, "属性值: null");
                }
            }
        });

        btnClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.closePCM();
            }
        });

        btnSetPath.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path = "/mnt/sdcard/Documents/Records/" + Utils.getCurrentTime() + ".wav";
                mPlayer.setFilePath(path);
            }
        });

        btnPrepare.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.prepare();
            }
        });

        btnComplete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.complete();
            }
        });

        btnSetMix.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.setMixArgs();
            }
        });

        btnPause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.pause();
            }
        });

        btnContinue.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.continuePlay();
            }
        });

        btnReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.reset();
            }
        });

        btnStatus.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int r = mPlayer.getStatus();
                if (r == Player.STATUS_COMPLETE) {
                    Toast.makeText(MainActivity.this, "STATUS_COMPLETE", Toast.LENGTH_SHORT).show();
                } else if (r == Player.STATUS_PAUSE) {
                    Toast.makeText(MainActivity.this, "STATUS_PAUSE", Toast.LENGTH_SHORT).show();
                } else if (r == Player.STATUS_PLAYING) {
                    Toast.makeText(MainActivity.this, "STATUS_PLAYING", Toast.LENGTH_SHORT).show();
                } else if (r == Player.STATUS_UNPLAY) {
                    Toast.makeText(MainActivity.this, "STATUS_UNPLAY", Toast.LENGTH_SHORT).show();
                } else if (r == Player.STATUS_UNINIT) {
                    Toast.makeText(MainActivity.this, "STATUS_UNINIT", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    public String getCustomProperty(String key) {
        try {
            Class<?> systemPropertiesClass = Class.forName("android.os.SystemProperties");
            Method getMethod = systemPropertiesClass.getMethod("get", String.class);
            return (String) getMethod.invoke(null, key);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }


    @Override
    protected void onDestroy() {

        super.onDestroy();
    }

    @Override
    public void jumpActivity(String path) {
        Intent intent = new Intent(MainActivity.this, TargetActivity.class);
        intent.putExtra("path", path);
        startActivity(intent);
    }
}