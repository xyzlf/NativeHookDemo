package com.xyzlf.demo.nativehook;

public class NativeHook {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private NativeHook(){}

    public static NativeHook getInstance() {
        return SubHolder.INSTANCE;
    }

    private static class SubHolder {
        private static final NativeHook INSTANCE = new NativeHook();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();


    public native void start();

}
