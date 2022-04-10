package com.example.ucontrolcompanion;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

public class emulatedController extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.emulated_controller);
        Button upButton = findViewById(R.id.button3);
        upButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                Log.e("TAG", "Up button pressed");
                return false;
            }
        });
        Button downButton = findViewById(R.id.button5);
        downButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                Log.e("TAG", "Down button pressed");
                return false;
            }
        });
        Button leftButton = findViewById(R.id.button6);
        leftButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                Log.e("TAG", "Left button pressed");
                return false;
            }
        });
        Button rightButton = findViewById(R.id.button7);
        rightButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                Log.e("TAG", "Right button pressed");
                return false;
            }
        });
        Button aButton = findViewById(R.id.button8);
        aButton.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                Log.e("TAG", "A button pressed");
                return false;
            }
        });
    }
}