package com.tlfs.tinyalsa_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

public class TargetActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_target);

        TextView tv = (TextView) findViewById(R.id.tv);
        String path = "";

        Intent intent = getIntent();
        if (intent != null) {
            path = intent.getStringExtra("path");
        }
        tv.setText(path);
    }
}