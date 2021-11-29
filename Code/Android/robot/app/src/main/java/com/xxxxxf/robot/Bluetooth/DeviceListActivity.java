package com.xxxxxf.robot.Bluetooth;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.xxxxxf.robot.R;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class DeviceListActivity extends Activity {
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;
    private String TAG = "MainActivity";
    private BluetoothAdapter mBluetoothAdapter;

    private Button bt;
    private ListView listViewDevice;
    private ListView listViewNewDevice;

    private ArrayAdapter<String> mListAdapter;
    private ArrayAdapter<String> mListNewAdapter;
    private List<String> listData=new ArrayList<>();
    private List<String> listNewData=new ArrayList<>();

    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.bluetooth_connect);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bt=findViewById(R.id.bt_scan);

        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanDevice();
            }
        });


        listViewDevice=findViewById(R.id.listView_Devices);
        mListAdapter= new ArrayAdapter<String>(this,R.layout.device_item,listData);
        listViewDevice.setAdapter(mListAdapter);
        listViewDevice.setOnItemClickListener(mDeviceClickListener);

        listViewNewDevice=findViewById(R.id.listView_NewDevices);
        mListNewAdapter= new ArrayAdapter<String>(this,R.layout.device_item,listNewData);
        listViewNewDevice.setAdapter(mListNewAdapter);
        listViewNewDevice.setOnItemClickListener(mDeviceClickListener);

        initBlueTooth();
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.registerReceiver(mReceiver, filter);

        /*filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(mReceiver, filter);*/

        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device: pairedDevices) {
                mListAdapter.add(device.getName()+'\n'+device.getAddress());
            }
        }

    }
    void initBlueTooth(){
        if (mBluetoothAdapter == null) {
            Log.e(TAG, "设备不支持蓝牙");
        }else {
            Log.e(TAG, "蓝牙可用");
            if (!mBluetoothAdapter.isEnabled()){
                Log.e(TAG, "蓝牙未开启" );
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }else {
                Log.e(TAG, "蓝牙已开启" );
                if(ContextCompat.checkSelfPermission(DeviceListActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION)
                        != PackageManager.PERMISSION_GRANTED);{
                    ActivityCompat.requestPermissions(DeviceListActivity.this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},1);
                }
            }
        }
    }



    void scanDevice(){
        if (mBluetoothAdapter.isDiscovering())
        {
            mBluetoothAdapter.cancelDiscovery();
        }

        mBluetoothAdapter.startDiscovery();

    }

    private final BroadcastReceiver mReceiver=new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                mListNewAdapter.add(device.getName()+'\n'+device.getAddress());
            }
        }
    };

    private AdapterView.OnItemClickListener mDeviceClickListener=new AdapterView.OnItemClickListener(){
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            mBluetoothAdapter.cancelDiscovery();
            String macAddr;
            String info;
            info=((TextView) view).getText().toString();
            macAddr = info.substring(info.length() - 17);
            Intent intent = new Intent();
            intent.putExtra(EXTRA_DEVICE_ADDRESS, macAddr);

            // Set result and finish this Activity
            setResult(Activity.RESULT_OK, intent);
            finish();

            /*BluetoothSocket socket = null;
            BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(macAddr);
            UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
            try {
                socket = device.createRfcommSocketToServiceRecord(uuid);
            } catch (IOException e) {
                e.printStackTrace();
            }


            *//*try {
                socket=(BluetoothSocket) device.getClass().getDeclaredMethod("createRfcommSocket", new Class[]{int.class}).invoke(device, 1);
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            } catch (NoSuchMethodException e) {
                e.printStackTrace();
            }*//*


            BluetoothSocket finalSocket = socket;
            new Thread(){
                @Override
                public void run() {
                    mBluetoothAdapter.cancelDiscovery();

                    try {
                        finalSocket.connect();
                        if(finalSocket.isConnected()){
                            Log.e(TAG, "connect to : " + macAddr);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                        try {
                            finalSocket.close();
                            Log.e(TAG, "连接失败！");
                        } catch (IOException ioException) {
                            ioException.printStackTrace();
                        }
                    }
                    super.run();
                }

            }.start();*/
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mBluetoothAdapter!=null){
            mBluetoothAdapter.cancelDiscovery();
        }
        unregisterReceiver(mReceiver);
    }
}
