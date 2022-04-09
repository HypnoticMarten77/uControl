package com.example.ucontrolcompanion;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.RecyclerView;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Button;

import java.util.ArrayList;

public class BluetoothTest extends AppCompatActivity {

    // Button and permission codes to identify which permission to check.
    private Button enableBtn, findBtn;
    public static final int BT_PERM_CODE = 100;
    public static final int ACCESS_FINELOC_PERM_CODE = 101;
    private int REQUEST_ENABLE_BT;
    public boolean btGranted = false;
    public boolean accLocGranted = false;
    BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_test);

        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "Your device does not support Bluetooth Low Energy.", Toast.LENGTH_LONG).show();
        }

        enableBtn = (Button) findViewById(R.id.enableBtn);
        enableBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (btAdapter == null)
                    Toast.makeText(BluetoothTest.this, "No Bluetooth adapter detected.", Toast.LENGTH_SHORT).show();
                else {
                    if (!btAdapter.isEnabled()) {
                        checkPermission(Manifest.permission.BLUETOOTH, BT_PERM_CODE);
                        checkPermission(Manifest.permission.ACCESS_FINE_LOCATION, ACCESS_FINELOC_PERM_CODE);
                        if (btGranted) {
                            Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                            startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);
                        }
                    }
                }
            }
        });

        findBtn = (Button) findViewById(R.id.findBtn);
        findBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                checkPermission(Manifest.permission.BLUETOOTH, BT_PERM_CODE);
                if (btGranted)
                {
                    Intent myIntent = new Intent(BluetoothTest.this, DeviceScanActivity.class);
                    startActivityForResult(myIntent, 0);
                }
                else
                    Toast.makeText(BluetoothTest.this, "Please give BT perm.", Toast.LENGTH_SHORT).show();
            }
        });

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK)
            Toast.makeText(BluetoothTest.this, "BT enabled.", Toast.LENGTH_SHORT).show();
        else
            Toast.makeText(BluetoothTest.this, "BT was not enabled.", Toast.LENGTH_SHORT).show();
    }

    // Check and request for permission.
    public void checkPermission(String permission, int requestCode)
    {
        if (ContextCompat.checkSelfPermission(BluetoothTest.this, permission) == PackageManager.PERMISSION_DENIED) {

            // Requesting the permission
            ActivityCompat.requestPermissions(BluetoothTest.this, new String[] { permission }, requestCode);
        }
        else {
            if (requestCode == BT_PERM_CODE)
                btGranted = true;
            if (requestCode == ACCESS_FINELOC_PERM_CODE)
                accLocGranted = true;
        }
    }

    // This function is called when the user accepts or decline the permission.
    // Request Code is used to check which permission called this function.
    // This request code is provided when the user is prompt for permission.

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions,
                                           @NonNull int[] grantResults)
    {
        super.onRequestPermissionsResult(requestCode,
                permissions,
                grantResults);

        if (requestCode == BT_PERM_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                btGranted = true;
            }
            else {
                btGranted = false;
            }
        }
        else if (requestCode == ACCESS_FINELOC_PERM_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                accLocGranted = true;
            }
            else {
                accLocGranted = false;
            }
        }
    }
}