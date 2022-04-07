package com.example.ucontrolcompanion;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;
import android.widget.Button;

public class BluetoothTest extends AppCompatActivity {

    // Button and permission codes to identify which permission to check.
    private Button enableBtn;
    private static final int BT_ADVERT_PERM_CODE = 100;
    private int REQUEST_ENABLE_BT;
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
                        checkPermission(Manifest.permission.BLUETOOTH_CONNECT, BT_ADVERT_PERM_CODE);
                        Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);
                    }
                }
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
            Toast.makeText(BluetoothTest.this, "Permission already granted", Toast.LENGTH_SHORT).show();
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

        if (requestCode == BT_ADVERT_PERM_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(BluetoothTest.this, "BT Advertise perm granted.", Toast.LENGTH_SHORT) .show();
            }
            else {
                Toast.makeText(BluetoothTest.this, "BT Advertise perm denied.", Toast.LENGTH_SHORT) .show();
            }
        }
    }

    // Register the permissions callback, which handles the user's response to the
    // system permissions dialog. Save the return value, an instance of
    // ActivityResultLauncher, as an instance variable.
    /*private ActivityResultLauncher<String> requestPermissionLauncher =
                registerForActivityResult(new ActivityResultContracts.RequestPermission(), isGranted -> {

                })*/
}