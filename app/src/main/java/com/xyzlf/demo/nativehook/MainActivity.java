package com.xyzlf.demo.nativehook;

import android.os.Bundle;
import android.os.Debug;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    private int mCount = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        final TextView tv = findViewById(R.id.sample_text);
        tv.setText(NativeHook.getInstance().stringFromJNI());

        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (mCount == 0) {
                    Log.i("NativeHookLogger", "*** startMethodTracing");
                    Debug.startMethodTracing("NativeHookDemo.trace");

                    try {
                        Log.i("NativeHookLogger", "start dump hprof....");
                        String filePath = getExternalCacheDir() + "/NativeHookDemo.hprof";
                        Debug.dumpHprofData(filePath);
                        Log.i("NativeHookLogger", "end dump hprof....");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    Debug.stopMethodTracing();
                    Log.i("NativeHookLogger", "**** stopMethodTracing");
                }

                mCount++;

                if (mCount > 5) {
                    mCount = 0;
                }
            }
        });
    }
}
