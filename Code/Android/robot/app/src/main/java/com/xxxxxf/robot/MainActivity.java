package com.xxxxxf.robot;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.camera2.CameraManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import com.xxxxxf.robot.Bluetooth.BluetoothChatService;
import com.xxxxxf.robot.Bluetooth.DeviceListActivity;
import com.xxxxxf.robot.Camera.ColorRecognition;
import com.xxxxxf.robot.Solution.RubikRobot;
import com.xxxxxf.robot.Solution.Search;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private CameraManager cameraManager;
    private SurfaceView surfaceView;
    private SurfaceView lineSurface;
    private Camera camera;
    private Camera.Parameters parameters;
    private Bundle bundle = null;
    private String TAG="MainActivity";
    private ColorRecognition colorRecognition;
    private Button button;
    private Button takeBt;
    private Button connectBt;
    private TextView mTitle;
    private Search search;

    private int photoID=0;

    private BluetoothChatService mChatService = null;

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;

    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;


    private String mConnectedDeviceName = null;
    private StringBuffer mOutStringBuffer;

    private BluetoothAdapter mBluetoothAdapter = null;

    private double time1;
    private double time2;

    private int time10;//开始时间
    private int time11;//结束时间

    RubikRobot rubikrobot = new RubikRobot();

    String readMessage;
    byte[] readchars = new byte[10];

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        colorRecognition=new ColorRecognition();


        camera=Camera.open();
        parameters=camera.getParameters();
        parameters.setPictureFormat(PixelFormat.JPEG);
        parameters.setPictureSize(1280,720);
        parameters.setPreviewSize(1280,720);
        parameters.setJpegQuality(100);
        camera.setParameters(parameters);
        /*预览尺寸
        *1920*1440 1920*1080 1600*1200 1440*1080 1280*960 1560*720 1440*720
        *1280*720 800*600 720*480 640*480 640*360 352*288 320*240 176*144
        */
        /*照片尺寸
        * 4000*3000 4000*2250 3840*2160 3000*3000 2688*1512 2592*1940
        * 1920*1440 2340*1080 2160*1080 1920*1080 1600*1200 1440*1080
        * 1280*960 1560*720 1440*720 1280*720 800*600 720*480 640*480
        * 640*360 352*288 320*240 176*144
        * */

        surfaceView=findViewById(R.id.surfaceView);
        //surfaceView.getHolder().setFixedSize(1280,720);
        surfaceView.getHolder().setKeepScreenOn(true);
        surfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
        surfaceView.getHolder().addCallback(new MysurfaceCallBack());
        surfaceView.setSecure(true);

        lineSurface=findViewById(R.id.surfaceViewLine);
        lineSurface.getHolder().setFixedSize(1280,720);
        lineSurface.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {
                DrawLine(holder);
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

            }
        });
        lineSurface.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        lineSurface.setZOrderMediaOverlay(true);
        button =findViewById(R.id.button);
        button.setOnClickListener(this);
        takeBt=findViewById(R.id.takeBt);
        takeBt.setOnClickListener(this);
        connectBt=findViewById(R.id.connectBt);
        connectBt.setOnClickListener(this);

        mTitle=findViewById(R.id.tv_title);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.button:
                //camera.autoFocus(new MyAutoFocusBack(MyAutoFocusBack.MODE_TAKE_PICTURE));
                colorRecognition.Get_RubikTotalColor(0,0);
                Log.e(TAG, colorRecognition.look_color());
                getColorResult();
                break;
            case R.id.takeBt:
                photoID++;
                if (photoID<7) {
                    camera.autoFocus(new MyAutoFocusBack(MyAutoFocusBack.MODE_TAKE_PICTURE));
                }else{
                    Toast.makeText(MainActivity.this,"已拍完全部",Toast.LENGTH_SHORT).show();
                }
                break;

            case R.id.connectBt:
                Intent serverIntent = new Intent(this, DeviceListActivity.class);
                startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
                break;

        }
    }

    private class  MyPictureCallBack implements Camera.PictureCallback {
        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            bundle = new Bundle();
            bundle.putByteArray("bytes", data);
            Change_Readchars();
            File jpgFile = new File("/storage/emulated/0/DCIM/Camera/"+photoID+".jpg");
            FileOutputStream outputStream = null;
            try {
                outputStream = new FileOutputStream(jpgFile);
                outputStream.write(data);
                outputStream.close();
                Toast.makeText(MainActivity.this,"进度 "+photoID,Toast.LENGTH_SHORT).show();
            } catch (IOException e) {
                e.printStackTrace();
            }
            camera.startPreview();
            time11 = (int) (System.nanoTime() / 1.0e6 - time10);
        }
    }
    private class MyAutoFocusBack implements Camera.AutoFocusCallback{
        private int mode;
        public static final int MODE_TAKE_PICTURE=1;
        public static final int MODE_FOCUS=0;
         MyAutoFocusBack(int mode){
            this.mode=mode;
        }
        @Override
        public void onAutoFocus(boolean success, Camera camera) {
            if (mode==MODE_FOCUS){

            }else {
                camera.takePicture(null,null,new MyPictureCallBack());
            }
        }
    }

    private class MysurfaceCallBack implements SurfaceHolder.Callback{
        @Override
        public void surfaceCreated(@NonNull SurfaceHolder holder) {
            try {
                camera.setPreviewDisplay(holder);
                camera.startPreview();
                camera.autoFocus(new MyAutoFocusBack(MyAutoFocusBack.MODE_FOCUS));

            } catch (IOException e) {
                e.printStackTrace();
            }

        }

        @Override
        public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

        }

        @Override
        public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
            if (camera != null) {
                camera.release();
                camera = null;
            }
        }
    }

    /**
     * DrawLine
     * 给预览界面画上辅助的点和线，方便放置手机
     * */
    private void DrawLine(SurfaceHolder holder){
        Canvas canvas = holder.lockCanvas();
        ColorRecognition cr=new ColorRecognition();
        int x[]=cr.getColor_x_position();
        int y[]=cr.getColor_y_position();
        int position;
        if (canvas == null)
        {
            Log.e(TAG, "Cannot draw onto the canvas as it's null");
        }
        else
        {
            Log.i(TAG, "Drawing...");
            Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
            paint.setColor(Color.GREEN);
            paint.setStyle(Paint.Style.FILL);
            paint.setStrokeWidth(2); //画笔粗细
            //画点
            float[][] points = {{x[2],y[0]},{x[2],y[2]},{x[2],y[4]},{x[0],y[2]},{x[4],y[2]},{x[1],y[1]},{x[1],y[3]},{x[3],y[1]},{x[3],y[3]}};
            for (int i = 0; i<9; i++){
                canvas.drawCircle(points[i][0],points[i][1],4,paint);
            }

            paint.setColor(Color.BLACK);

            /*画线
            //paint.setPathEffect ( new DashPathEffect( new float [ ] { 20, 5,5,5 }, 0 ) ) ;
            //paint.setShadowLayer(50 /2 + 1, 0, 0, Color.RED);
            //canvas.drawCircle(screenWidth/2,screenHeight/2, 50, paint);
            for (position=0;position<5;position++){
                canvas.drawLine(x[position],0 ,x[position],720,paint);
                canvas.drawLine(0,y[position],1280,y[position],paint);
            }*/

            //画框，
            canvas.drawLine(x[0]-110,y[2] ,x[2],y[0]-110,paint);
            canvas.drawLine(x[0]-110,y[2] ,x[2],y[4]+110,paint);
            canvas.drawLine(x[4]+110,y[2] ,x[2],y[4]+110,paint);
            canvas.drawLine(x[4]+110,y[2] ,x[2],y[0]-110 ,paint);
            holder.unlockCanvasAndPost(canvas);
        }

    }

    //调试用，解算按钮已隐藏
    private void getColorResult(){
        Log.e(TAG, colorRecognition.color_mask);
        String rubik_solution;
        search=new Search();
        rubik_solution=search.solution(colorRecognition.color_mask,21,5000,0,0);
        Log.e(TAG, rubik_solution);
        Toast.makeText(MainActivity.this,rubik_solution,Toast.LENGTH_SHORT).show();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case REQUEST_CONNECT_DEVICE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {
                    // Get the device MAC address
                    String address = data.getExtras()
                            .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    // Get the BLuetoothDevice object
                    BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                    // Attempt to connect to the device
                    mChatService.connect(device);
                }
                break;
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) {
                    // Bluetooth is now enabled, so set up a chat session
                    setupChat();
                } else {
                    // User did not enable Bluetooth or an error occured
                    Log.d(TAG, "BT not enabled");
                    //Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                    finish();
                }
        }
    }

    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MESSAGE_STATE_CHANGE:
                    switch (msg.arg1) {
                        case BluetoothChatService.STATE_CONNECTED:
                            mTitle.setText(R.string.title_connected_to);
                            mTitle.append(mConnectedDeviceName);
                            break;
                        case BluetoothChatService.STATE_CONNECTING:
                            mTitle.setText(R.string.title_connecting);
                            break;
                        case BluetoothChatService.STATE_LISTEN:
                        case BluetoothChatService.STATE_NONE:
                            mTitle.setText(R.string.title_not_connected);
                            break;
                    }
                    break;
                case MESSAGE_WRITE:
                    byte[] writeBuf = (byte[]) msg.obj;
                    // construct a string from the buffer
                    String writeMessage = new String(writeBuf);

                    break;
                case MESSAGE_READ: {
                    time1 = System.nanoTime();
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    int recog_time;
                    long t;
                    String color_result;
                    readMessage = new String(readBuf, 0, msg.arg1);
                    readchars = readMessage.getBytes();
                    if (readchars[0] == '1' || readchars[0] == '2' || readchars[0] == '3' || readchars[0] == '4' || readchars[0] == '5' || readchars[0] == '6') {
                        //camera.takePicture(null, null, new MyPictureCallback());

                        camera.autoFocus(new Camera.AutoFocusCallback() {
                            @Override
                            public void onAutoFocus(boolean b, Camera camera) {
                                camera.takePicture(null, null, new MyPictureCallBack());
                            }
                        });
                        //sendMessages(readMessage);

                    } else if (readchars[0] == '7') {

                        colorRecognition.Get_RubikTotalColor(0, 0);
                        Log.e("color1", colorRecognition.color_mask);
                        color_result = rubikrobot.Test_Solution(colorRecognition.color_mask);
                        Log.e("1", color_result );
                        recog_time = 1;//颜色识别的次数
                        t = System.nanoTime();
                        while ((color_result.charAt(0) == 'E') && ((System.nanoTime() - t) < 5000 * 1.0e6))//5S
                        {
                            int x_bias = ((int) (Math.random() * 70)) - 35;//-30到30的随机数
                            int y_bias = ((int) (Math.random() * 70)) - 35;
                            colorRecognition.Get_ColorMask(x_bias, y_bias);
                            color_result = rubikrobot.Test_Solution(colorRecognition.color_mask);
                            recog_time++;
                        }

                        if (color_result.charAt(0) != 'E') {
                            String send_string=rubikrobot.Get_Solution(colorRecognition.color_mask);
                            Log.e("3", send_string );
                            sendMessages(send_string);
                            //Toast.makeText(MainActivity.this ,"颜色识别结果："+colorRecognition.look_color()+"\r\n"+"颜色识别次数："+String.valueOf(recog_time)+"\r\n"+"解算公式："+send_string,Toast.LENGTH_LONG).show();
                            Toast.makeText(MainActivity.this ,"识别成功！\r\n"+"颜色识别次数："+String.valueOf(recog_time)+"\r\n"+"解算公式："+send_string+"\r\n",Toast.LENGTH_LONG).show();

                        } else {
                            //Toast.makeText(MainActivity.this ,"\r\n"+result+"\r\n"+ColorRecognition.rubiktotalcolor,Toast.LENGTH_LONG).show();
                            Log.e("color2", colorRecognition.color_mask);
                            String send_str=rubikrobot.Get_Solution(colorRecognition.color_mask);
                            Log.e("4", send_str );
                            sendMessages(send_str);
                            Toast.makeText(MainActivity.this, "经过" + String.valueOf(recog_time) + "次颜色识别，用时5S\r\n" + colorRecognition.look_color(), Toast.LENGTH_LONG).show();
                        }

                    }
                    //Toast.makeText(MainActivity.this, "蓝牙接收到字符串" + readMessage, Toast.LENGTH_SHORT).show();
                    //sendMessages(search.solution(colorRecognition.color_mask, 21, 100, 0, 0));
                }
                break;
                case MESSAGE_DEVICE_NAME:
                    // save the connected device's name
                    mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                    Toast.makeText(getApplicationContext(), "连接到 "
                            + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                    break;
                case MESSAGE_TOAST:
                    Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                            Toast.LENGTH_SHORT).show();
                    break;
            }
        }
    };

    public void Change_Readchars() {
        switch (readchars[0]) {
            case '1': {
                photoID = 1;
            }
            break;
            case '2': {
                photoID = 2;
            }
            break;

            case '3': {
                photoID = 3;
            }
            break;

            case '4': {
                photoID = 4;
            }
            break;

            case '5': {
                photoID = 5;
            }
            break;

            case '6': {
                photoID = 6;
            }
            break;
            default: {

            }
            break;

        }

    }


    private void setupChat() {
        Log.d(TAG, "setupChat()");

        //初始化BluetoothChatService来执行蓝牙连接
        mChatService = new BluetoothChatService(this, mHandler);

        // 初始化输出流
        mOutStringBuffer = new StringBuffer("");
    }

    private void sendMessages(String message) {
        // 检查连接
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // 检查发送内容是否为空
        if (message.length() > 0) {
            // 获取消息字节，并BluetoothChatService进行写出
            byte[] send = message.getBytes();
            mChatService.write(send);

            // 发送完毕清空输出流
            mOutStringBuffer.setLength(0);

        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            // 否则，设置会话
        } else {
            if (mChatService == null) setupChat();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mChatService != null) {
            // 状态为STATE_NONE, 没有开始，启动ChatService
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
                // Start the Bluetooth chat services
                mChatService.start();
            }
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mChatService != null) mChatService.stop();
    }
}