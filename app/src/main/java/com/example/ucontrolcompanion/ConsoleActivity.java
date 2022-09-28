package com.example.ucontrolcompanion;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

public class ConsoleActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.console_select);
        Intent intent = getIntent();
        ImageView ps = findViewById(R.id.psLogo);
        ps.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String stringToPassBack = "Playstation";
                Intent intent = new Intent();
                intent.putExtra(Intent.EXTRA_TEXT, stringToPassBack);
                setResult(RESULT_OK, intent);
                finish();
            }
        });
        ImageView ns = findViewById(R.id.nsLogo);
        ns.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String stringToPassBack = "Nintendo Switch";
                Intent intent = new Intent();
                intent.putExtra(Intent.EXTRA_TEXT, stringToPassBack);
                setResult(RESULT_OK, intent);
                finish();
            }
        });
        ImageView xb = findViewById(R.id.xbLogo);
        xb.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String stringToPassBack = "Xbox";
                Intent intent = new Intent();
                intent.putExtra(Intent.EXTRA_TEXT, stringToPassBack);
                setResult(RESULT_OK, intent);
                finish();
            }
        });
    }
}