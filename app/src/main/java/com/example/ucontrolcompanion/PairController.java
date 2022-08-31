package com.example.ucontrolcompanion;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class PairController extends AppCompatActivity {
    private BluetoothLeService bluetoothLeService;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pair_controller);
    }
}

