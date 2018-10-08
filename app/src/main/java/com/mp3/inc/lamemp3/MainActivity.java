package com.mp3.inc.lamemp3;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("lame-lib");
    }

    private static final String TAG = "MainActivity";
    private Handler handler;
    private ProgressDialog progressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressDialog = new ProgressDialog(this);
        progressDialog.setCancelable(false);
        int retCode = initLame(2, 44100, 128, 1, 2);
        if (retCode >= 0) {
            Log.i(TAG, "初使化成功");
        } else {
            Log.i(TAG, "初使化失败");
        }

        handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                String strMsg = null;
                switch (msg.what) {
                    case -1:
                        strMsg = "wav文件不存在,请确保存在/sdcard/birthday.wav文件";
                        break;
                    case 0:
                        strMsg = "成功转换成/sdcard/birthday.mp3文件";
                        break;
                    case 1:
                        strMsg = "转换出错";
                        break;
                }
                if (progressDialog != null && progressDialog.isShowing()) {
                    progressDialog.dismiss();
                }
                Toast.makeText(MainActivity.this, strMsg, Toast.LENGTH_LONG).show();
            }
        };
    }

    /**
     * wav文件转换成mp3文件
     *
     * @param view
     */
    public void wav2Mp3(View view) {
        progressDialog.setMessage("正在转换，请稍候...");
        progressDialog.show();
        new Thread(new Runnable() {
            @Override
            public void run() {
                int retCode = encode3Mp3();
                handler.sendEmptyMessage(retCode);
            }
        }).start();
    }

    public native int initLame(int numChannels, int sampleRate, int brate, int mode, int quality);

    public native int encode3Mp3();
}
