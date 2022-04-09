package com.example.ucontrolcompanion;

import androidx.annotation.Nullable;

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
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class DeviceControlActivity extends BluetoothTest {

    private Button readBtn;
    private BluetoothLeService bluetoothLeService;
    private boolean isGattConnected;
    private TextView connectionState, dataValue;
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
            Toast.makeText(DeviceControlActivity.this, "Adapter disconnected.", Toast.LENGTH_SHORT).show();
        }
    };

    private final BroadcastReceiver gattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            final String data = intent.getStringExtra("DATA");
            Log.e(TAG, "Action: " + action);
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                isGattConnected = true;
                updateConnectionState("Receiver Connected");
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                isGattConnected = false;
                updateConnectionState("Receiver Disconnected");
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                updateConnectionState("Services Found");
                displayGattServices(bluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                updateDataValue(data);
            }
        }
    };

    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) {
            Log.e(TAG, "No services found.");
            //Toast.makeText(DeviceControlActivity.this, "No services found.", Toast.LENGTH_SHORT).show();
            return;
        }
        for (BluetoothGattService gattService : gattServices) {
            Log.w(TAG, "Service UUID: " + gattService.getUuid().toString());
            //Toast.makeText(DeviceControlActivity.this, "S UUID: " + gattService.getUuid().toString(), Toast.LENGTH_LONG).show();
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

    private void updateConnectionState(final String state) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                connectionState.setText(state);
            }
        });
    }

    private void updateDataValue(final String value) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                dataValue.setText(value);
            }
        });
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_control);

        // Set UI references
        connectionState = (TextView) findViewById(R.id.connection_state);
        dataValue = (TextView) findViewById(R.id.data_value);

        Bundle bundle = getIntent().getExtras();
        address = bundle.getString("Address");

        Log.w(TAG, "Passed in address: " + address);

        // We start service using bindService
        // ServiceConnection listens for connection/disconnection
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, serviceConnection, Context.BIND_AUTO_CREATE);

        readBtn = (Button) findViewById(R.id.readBtn);
        readBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.w(TAG, "Getting services...");
                List<BluetoothGattService> services = bluetoothLeService.getSupportedGattServices();
                for (BluetoothGattService service : services)
                {
                    Log.w(TAG, "Service " + service.getUuid().toString());
                    if (service.getUuid().toString().contains(serviceUUID))
                    {
                        Log.w(TAG, "Service found.");
                        List<BluetoothGattCharacteristic> characteristics = service.getCharacteristics();
                        for (BluetoothGattCharacteristic characteristic : characteristics)
                        {
                            if (characteristic.getUuid().toString().contains(characteristicUUID))
                            {
                                Log.w(TAG, "Characteristic found.");
                                byte[] testChar = new byte[1];
                                testChar[0] = 'E';
                                bluetoothLeService.writeCharacteristic(characteristic, testChar, 2);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        });
    }
}

