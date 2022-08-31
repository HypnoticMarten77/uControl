package com.example.ucontrolcompanion;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;
import java.util.UUID;

public class PairController extends AppCompatActivity {
    private BluetoothLeService bluetoothLeService;
    private boolean isGattConnected;
    private String address;
    private static final String TAG = "BluetoothLeService";
    private static final String serviceUUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
    private static final String characteristicUUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            //Log.w("1","THIS IS A TEST");
            bluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (bluetoothLeService != null) {
                if (!bluetoothLeService.initialize()) {
                    Log.e(TAG, "Unable to initialize Bluetooth.");
                }
                // Assume Bluetooth Adapter is initialized and ready to connect
                //Toast.makeText(DeviceControlActivity.this, "Adapter ready to connect.", Toast.LENGTH_SHORT).show();
                // Connect to device with address: F1:ED:88:DE:69:1C
                bluetoothLeService.connect(address);

            }
            //else
            //Toast.makeText(DeviceControlActivity.this, "Adapter is NOT ready to connect.", Toast.LENGTH_SHORT).show();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            bluetoothLeService = null;
            Toast.makeText(PairController.this, "Adapter disconnected.", Toast.LENGTH_SHORT).show();
        }
    };

    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            final String data = intent.getStringExtra("DATA");
            Log.w(TAG, "Action: " + action);
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                isGattConnected = true;
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                isGattConnected = false;
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                displayGattServices(bluetoothLeService.getSupportedGattServices());
            }
        }
    };

    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) {
            Log.e(TAG, "No services found.");
            return;
        }
        for (BluetoothGattService gattService : gattServices) {
            Log.w(TAG, "Service UUID: " + gattService.getUuid().toString());
            List<BluetoothGattCharacteristic> gattCharacteristics = gattService.getCharacteristics();
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics)
            {
                Log.w(TAG, "Characteristic UUID: " + gattCharacteristic.getUuid().toString());
                //Toast.makeText(DeviceControlActivity.this, "C UUID: " + gattCharacteristic.getUuid().toString(), Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        registerReceiver(gattUpdateReceiver, makeGattUpdateIntentFilter());
        if (bluetoothLeService != null) {
            final boolean result = bluetoothLeService.connect(address);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(gattUpdateReceiver);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }

    private void sendInput(byte[] data)
    {
        BluetoothGattService service = bluetoothLeService.bluetoothGatt.getService(UUID.fromString(serviceUUID));
        BluetoothGattCharacteristic characteristic = service.getCharacteristic(UUID.fromString(characteristicUUID));
        bluetoothLeService.writeCharacteristic(characteristic, data, 2);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pair_controller);

        Bundle bundle = getIntent().getExtras();
        address = bundle.getString("Address");
        Log.w(TAG, "Passed in address: " + address);

        // We start service using bindService
        // ServiceConnection listens for connection/disconnection
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, Context.BIND_AUTO_CREATE);
        //List<BluetoothGattService> serviceList = bluetoothLeService.getSupportedGattServices();
        /*TextView service = findViewById(R.id.textView6);
        for(int i = 0; i < serviceList.size(); i++){
            service.setText(service + serviceList.get(i).toString() + '\n');
        }*/
    }

}

