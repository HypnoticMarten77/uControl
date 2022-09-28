package com.example.ucontrolcompanion;

import android.Manifest;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;

import java.util.List;

// This service will interact with the BLE device through BLE API
public class BluetoothLeService extends Service {
    private Binder binder = new LocalBinder();

    public static final String TAG = "BluetoothLeService";
    private BluetoothAdapter bluetoothAdapter;

    public BluetoothGatt bluetoothGatt;
    public final static String ACTION_GATT_CONNECTED = "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED = "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED = "com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE = "com.example.bluetooth.le.ACTION_DATA_AVAILABLE";

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

    public boolean initialize() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Log.e(TAG, "Unable to obtain a BluetoothAdapter.");
            return false;
        }
        return true;
    }

    private void close() {
        if (bluetoothGatt == null) {
            return;
        }
        if (ActivityCompat.checkSelfPermission(BluetoothLeService.this, Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED) {
            return;
        } else {
            bluetoothGatt.close();
        }
        bluetoothGatt = null;
    }

    // Function to connect to device with address
    public boolean connect(final String address) {
        if (bluetoothAdapter == null || address == null) {
            Log.w(TAG, "BluetoothAdapter not initialized or address invalid.");
            return false;
        }
        try {
            final BluetoothDevice device = bluetoothAdapter.getRemoteDevice((address));

            // Connect to GATT server on device
            if (ActivityCompat.checkSelfPermission(BluetoothLeService.this, Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED) {
                return false;
            } else {
                bluetoothGatt = device.connectGatt(this, false, bluetoothGattCallback);
            }
            return true;
        } catch (IllegalArgumentException exception) {
            Log.w(TAG, "Device not found with provided address.");
            return false;
        }
    }

    // Function to write a characteristic to remote device
    public int writeCharacteristic(BluetoothGattCharacteristic characteristic, byte[] value, int writeType) {
        if (bluetoothGatt == null) {
            Log.w(TAG, "Bluetooth not Initialized");
            return 1;
        }
        characteristic.setValue(value);
        bluetoothGatt.writeCharacteristic(characteristic);
        Log.w(TAG, "Writing characteristic.");
        return 0;
    }

    public void readCharacteristic(BluetoothGattCharacteristic characteristic) {
        if (bluetoothGatt == null) {
            Log.w(TAG, "Bluetooth not Initialized");
            return;
        }
        // read from remote device
        Log.w(TAG, "Perms are granted, attempt to read.");
        if (!bluetoothGatt.readCharacteristic(characteristic));
        Log.e(TAG, "Failed to read characteristic " + characteristic.getPermissions());
    }

    // GATT Callback
    private final BluetoothGattCallback bluetoothGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                // Successfully connected to the GATT Server
                connectionState = 2;
                broadcastUpdate(ACTION_GATT_CONNECTED, null);
                // Attempts to discover services after successful connection
                bluetoothGatt.discoverServices();
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                // Disconnected from the GATT Server
                connectionState = 0;
                broadcastUpdate(ACTION_GATT_DISCONNECTED, null);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED, null);
            } else {
                Log.w(TAG, "onServicesDiscovered received: " + status);
            }
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status){
            if(status != BluetoothGatt.GATT_SUCCESS){
                Log.e(TAG, "Characteristic Write Failed");
            }
            // Value of characteristic represents the value reported by remote device
            Log.w(TAG, "Characteristic Write Success!!");

        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status){
            //super.onCharacteristicRead(gatt, characteristic, status);
            if(status == BluetoothGatt.GATT_SUCCESS){
                Log.w(TAG, "Characteristic read success!!");
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
            }
            else
                Log.w(TAG, "Characteristic read failed.");
        }
    };

    // Broadcast
    private void broadcastUpdate(final String action, BluetoothGattCharacteristic characteristic) {
        final Intent intent = new Intent(action);
        if (characteristic != null)
        {
            final byte[] data = characteristic.getValue();
            intent.putExtra("DATA", data.toString());
        }
        else
            intent.putExtra("DATA", "No data.");
        sendBroadcast(intent);
    }

    class LocalBinder extends Binder {
        public BluetoothLeService getService() {
            return BluetoothLeService.this;
        }
    }

    public List<BluetoothGattService> getSupportedGattServices() {
        if (bluetoothGatt == null) return null;
        return bluetoothGatt.getServices();
    }
}
