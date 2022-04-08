package com.example.ucontrolcompanion;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.widget.TextView;

public class DeviceControlActivity extends BluetoothTest {

    private BluetoothLeService bluetoothLeService;
    private boolean isGattConnected;
    private TextView connectionState;

    private ServiceConnection serviceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            bluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (bluetoothLeService != null) {
                // Assume Bluetooth Adapter is initialized and ready to connect

                // Connect to device with address: F1:ED:88:DE:69:1C
                bluetoothLeService.connect("F1:ED:88:DE:69:1C");

            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            bluetoothLeService = null;
        }
    };

    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String gattStatus = intent.getAction();
            if(BluetoothLeService.ACTION_GATT_CONNECTED.equals(gattStatus)){
                isGattConnected = true;
                updateConnectionState("Receiver Connected");
            } else if(BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(gattStatus)){
                isGattConnected = false;
                updateConnectionState("Receiver Disconnected");
            }
        }
    };

    private void updateConnectionState(final String state){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                connectionState.setText(state);
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_control);

        // Set UI references
        connectionState = (TextView) findViewById(R.id.connectionState);

        // We start service using bindService
        // ServiceConnection listens for connection/disconnection
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, Context.BIND_AUTO_CREATE);
    }
}

// This service will interact with the BLE device through BLE API
class BluetoothLeService extends Service {
    private Binder binder = new LocalBinder();

    public static final String TAG = "BluetoothLeService";
    private BluetoothAdapter bluetoothAdapter;

    private BluetoothGatt bluetoothGatt;
    public final static String ACTION_GATT_CONNECTED = "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED = "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    private int connectionState;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        close();
        return super.onUnbind(intent);
    }

    private void close() {
        if (bluetoothGatt == null) {
            return;
        }
        if (ActivityCompat.checkSelfPermission(BluetoothLeService.this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            return;
        } else {
            bluetoothGatt.close();
        }
        bluetoothGatt = null;
    }

    // Function to connect to device with address
    public boolean connect(final String address) {
        if (bluetoothAdapter == null | address == null) {
            Log.w(TAG, "BluetoothAdapter not initialized or unspecified address.");
            return false;
        }
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice((address));

            // Connect to GATT server on device
            if (ActivityCompat.checkSelfPermission(BluetoothLeService.this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                return false;
            } else{
                bluetoothGatt = device.connectGatt(this, false, bluetoothGattCallback);
            }
            return true;
        } catch (IllegalArgumentException exception){
            Log.w(TAG, "Device not found with provided address.");
            return false;
        }
    }

    // GATT Callback
    private final BluetoothGattCallback bluetoothGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if(newState == BluetoothProfile.STATE_CONNECTED){
                // Successfully connected to the GATT Server
                connectionState = 2;
                broadcastUpdate(ACTION_GATT_CONNECTED);
            } else if(newState == BluetoothProfile.STATE_DISCONNECTED){
                // Disconnected from the GATT Server
                connectionState = 0;
                broadcastUpdate(ACTION_GATT_DISCONNECTED);
            }
        }
    };

    // Broadcast
    private void broadcastUpdate(final String action){
        final Intent intent = new Intent(action);
        sendBroadcast(intent);
    }

    class LocalBinder extends Binder{
        public BluetoothLeService getService(){
            return BluetoothLeService.this;
        }
    }
}

