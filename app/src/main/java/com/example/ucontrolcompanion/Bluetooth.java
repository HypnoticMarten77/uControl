package com.example.ucontrolcompanion;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class Bluetooth extends AppCompatActivity {
    // Variable Declaration
    Button buttonOn, buttonOff;
    BluetoothAdapter myBluetoothAdapter;

    Intent btEnablingIntent;
    int btEnableRequestCode;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);
        // Variable
        buttonOn = (Button) findViewById(R.id.btOn);
        buttonOff = (Button) findViewById(R.id.btOff);
        myBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        btEnablingIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        btEnableRequestCode = 1;

        bluetoothEnabled();
        bluetoothDisabled();

    }
    private void bluetoothDisabled(){
        buttonOff.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("MissingPermission")
            @Override
            public void onClick(View view) {
                if(myBluetoothAdapter.isEnabled()){
                    myBluetoothAdapter.disable();
                }
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == btEnableRequestCode) {
            if (resultCode == RESULT_OK) {
                Toast.makeText(getApplicationContext(), "Bluetooth is enabled", Toast.LENGTH_LONG).show();
            } else if (resultCode == RESULT_CANCELED) {
                Toast.makeText(getApplicationContext(), "Bluetooth enabling cancelled", Toast.LENGTH_LONG).show();
            }
        }
    }

    private void bluetoothEnabled(){
        buttonOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(myBluetoothAdapter == null){
                    Toast.makeText(getApplicationContext(), "Bluetooth not supported on device", Toast.LENGTH_LONG).show();
                }else{
                    if(!myBluetoothAdapter.isEnabled()){
                        startActivityForResult(btEnablingIntent, btEnableRequestCode);
                    }
                }
            }
        });
    }
}