package com.rdd.playeruser2;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.jakewharton.rxbinding4.view.RxView;
import com.tbruyelle.rxpermissions3.RxPermissions;
import com.rdd.player.IErrorListener;
import com.rdd.player.IJump;
import com.rdd.player.Player;
import com.rdd.player.Utils;

public class MainActivity extends AppCompatActivity implements IJump, IErrorListener {

    private static final String TAG = "MainActivity";

    Player mPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        final RxPermissions rxPermissions = new RxPermissions(this);

        mPlayer = new Player.Builder()
                .context(this)
                .rootFilePath("/mnt/sdcard/Documents/")
                .jumpCallback(this)
                .errorMsgCallback(this)
                .build();
        mPlayer.prepare();

        Button btnProp = (Button) findViewById(R.id.prop);
        Button btnStart = (Button) findViewById(R.id.start);
        Button btnComplete = (Button) findViewById(R.id.complete);
        Button btnPause = (Button) findViewById(R.id.pause);
        Button btnContinue = (Button) findViewById(R.id.continue_play);
        Button btnReset = (Button) findViewById(R.id.reset);
        Button btnStatus = (Button) findViewById(R.id.get_status);

        RxView.clicks(btnStart)
                .compose(rxPermissions.ensure(Manifest.permission.RECORD_AUDIO, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE))
                .subscribe(granted -> {
                    if (granted) {
                        mPlayer.start();
                    } else {
                        Toast.makeText(MainActivity.this, "权限被拒绝，无法录制音频！", Toast.LENGTH_SHORT).show();
                    }
                });

        btnProp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, Utils.getCustomProperty("voice.mode") + "", Toast.LENGTH_SHORT).show();
            }
        });


        btnComplete.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPlayer.complete();
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
                mPlayer.continuee();
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


    @Override
    protected void onDestroy() {
        mPlayer.release();
        super.onDestroy();
    }

    @Override
    public void jumpActivity(String path) {
        Intent intent = new Intent(MainActivity.this, TargetActivity.class);
        intent.putExtra("path", path);
        startActivity(intent);
    }

    @Override
    public void onError(String msg) {
        Toast.makeText(MainActivity.this, msg, Toast.LENGTH_SHORT).show();
    }
}