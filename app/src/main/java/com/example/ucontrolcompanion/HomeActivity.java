package com.example.ucontrolcompanion;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.String;

public class HomeActivity extends AppCompatActivity {
    private static final int CONTROLLER_ACTIVITY_REQUEST_CODE = 0;
    private static final int CONSOLE_ACTIVITY_REQUEST_CODE = 1;

    private BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();
    private BluetoothLeScanner bluetoothLeScanner = btAdapter.getBluetoothLeScanner();
    private boolean scanning;
    private Handler handler = new Handler();
    private String address;
    // Stops scanning after 10 seconds
    private static final long SCAN_PERIOD = 10000;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        Button controllerButton = findViewById(R.id.button4);
        controllerButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openControllerActivity();
            }
        });
        Button consoleButton = findViewById(R.id.button2);
        consoleButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openConsoleActivity();
            }
        });
        Button startButton = findViewById(R.id.button);
        startButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openStartActivity();
            }
        });
        Button scanButton = findViewById(R.id.button9);
        scanButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                scanLeDevice();
            }
        });
        Button pairControllerButton = findViewById(R.id.button11);
        pairControllerButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) { openPairControllerActivity(); }
        });
    }
    public void openPairControllerActivity(){
        if (address == null)
            Toast.makeText(this, "Please perform a scan for your uControl device.", Toast.LENGTH_LONG).show();
        else {
            Intent intent = new Intent(this, PairController.class);
            intent.putExtra("Address", address);
            startActivity(intent);
        }
    }
    public void openControllerActivity(){
        Intent intent = new Intent(this, ControllerActivity.class);
        startActivityForResult(intent, CONTROLLER_ACTIVITY_REQUEST_CODE);
    }
    public void openConsoleActivity(){
        Intent intent = new Intent(this, ConsoleActivity.class);
        startActivityForResult(intent, CONSOLE_ACTIVITY_REQUEST_CODE);
    }
    public void openStartActivity(){
        TextView textView = findViewById(R.id.textView3);
        if(!textView.getText().equals("Emulated"))
            Toast.makeText(this, "Please select the emulated controller.", Toast.LENGTH_LONG).show();
        else if (address == null)
            Toast.makeText(this, "Please perform a scan for your uControl device.", Toast.LENGTH_LONG).show();
        else {
            Intent intent = new Intent(this, emulatedController.class);
            intent.putExtra("Address", address);
            startActivity(intent);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode == RESULT_OK){
            if(requestCode == CONTROLLER_ACTIVITY_REQUEST_CODE){
                String returnString = data.getStringExtra(Intent.EXTRA_TEXT);
                TextView textView = findViewById(R.id.textView3);
                textView.setText(returnString);
                textView = findViewById(R.id.xboxAddress);
                textView.setText("");
            }
            else if(requestCode == CONSOLE_ACTIVITY_REQUEST_CODE){
                String returnString = data.getStringExtra(Intent.EXTRA_TEXT);
                TextView textView = findViewById(R.id.textView4);
                textView.setText(returnString);
            }
        }

    }

    private void scanLeDevice() {
        if (!scanning) {
            //Stops scanning after a predefined scan period.
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    scanning = false;
                    bluetoothLeScanner.stopScan(leScanCallback);
                }
            }, SCAN_PERIOD);

            scanning = true;
            bluetoothLeScanner.startScan(leScanCallback);
        }
        else
        {
            scanning = false;
            bluetoothLeScanner.stopScan(leScanCallback);
        }
    }

    // Device scan callback.
    private ScanCallback leScanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            TextView addrText = findViewById(R.id.textView7);
            if (addrText.getText().equals("No uControl found") && result.getDevice().getName() != null && result.getDevice().getName().contains("Adafruit")) {
                address = result.getDevice().getAddress();
                addrText.setText("Address: " + address);
                scanning = false;
            }
            else if (result.getDevice().getName() != null){
                TextView target = findViewById(R.id.textView3);
                if (target.getText().equals("Xbox") && result.getDevice().getName().contains("Xbox")) {
                    address = result.getDevice().getAddress();
                    TextView textView = findViewById(R.id.xboxAddress);
                    textView.setText(address);
                    scanning = false;
                }
                else if (target.getText().equals("Playstation") && result.getDevice().getName().contains("Wireless Controller")) {
                    address = result.getDevice().getAddress();
                    TextView textView = findViewById(R.id.xboxAddress);
                    textView.setText(address);
                    scanning = false;
                }
                else if (target.getText().toString().contains("Switch") && (result.getDevice().getName().contains("Joy-Con") || result.getDevice().getName().contains("Pro Controller"))) {
                    address = result.getDevice().getAddress();
                    TextView textView = findViewById(R.id.xboxAddress);
                    textView.setText(address);
                    scanning = false;
                }
            }
        }
    };
}