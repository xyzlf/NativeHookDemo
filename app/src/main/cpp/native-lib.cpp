#include <jni.h>
#include <string>
#include "log.h"
#include "xhook/xhook.h"


// hook __android_log_print
static int my_system_log_print(int prio, const char *tag, const char *fmt, ...) {
    va_list ap;
    char buf[1024];
    int r;

    snprintf(buf, sizeof(buf), "[%s] %s", (NULL == tag ? "" : tag), (NULL == fmt ? "" : fmt));

    va_start(ap, fmt);
    r = __android_log_vprint(prio, "NdkLogger", buf, ap);
    va_end(ap);

    LOG("my_system_log_print --------- %d %s %s", prio, tag, fmt);
    return r;
}


static void (*suspend_all_origin)(char const *, bool);

// hook
static void suspend_all_proxy(char const *cause, bool long_suspend = false) {
    int i = 0;
    while (cause[i++] != '\0') {
        LOG("1111  %d", cause[i]);
    }
    LOG("suspend_all _ZN3art16ScopedSuspendAllC1EPKcb cause. %s %d\n", cause, long_suspend);

    suspend_all_origin(cause, long_suspend);
}




extern "C" JNIEXPORT jstring JNICALL
Java_com_xyzlf_demo_nativehook_NativeHook_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    char *name = "Hello, the log is from jni.";
//    LOG("jni logger: %s\n", name);
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xyzlf_demo_nativehook_NativeHook_start(JNIEnv *env, jobject thiz) {
    // implement start()

//    xhook_register("^/system/.*\\.so$", "__android_log_print", (void *) my_system_log_print, NULL);
//    xhook_register("^/vendor/.*\\.so$", "__android_log_print", (void *) my_system_log_print, NULL);


    // ScopedSuspendAll::ScopedSuspendAll(const char* cause, bool long_suspend)
    if (0 != xhook_register(".*\\.so$", "_ZN3art16ScopedSuspendAllC1EPKcb",
                            (void *) suspend_all_proxy, (void **) suspend_all_origin)) {
        LOG("hooooook _ZN3art16ScopedSuspendAllC1EPKcb (ScopedSuspendAll::ScopedSuspendAll) error\n");
    } else {
        LOG("hooooook _ZN3art16ScopedSuspendAllC1EPKcb (ScopedSuspendAll::ScopedSuspendAll) success\n");
    }


    // void ThreadList::SuspendAll(const char* cause, bool long_suspend)
//    if (0 != xhook_register(".*\\.so$", "_ZN3art10ThreadList10SuspendAllEPKcb",
//                            (void *) suspend_all_proxy, (void **) suspend_all_origin)) {
//        LOG("hooooook _ZN3art10ThreadList10SuspendAllEPKcb (ThreadList::SuspendAll) errror\n");
//    } else {
//        LOG("hooooook _ZN3art10ThreadList10SuspendAllEPKcb (ThreadList::SuspendAll) success\n");
//    }

//    xhook_enable_debug(1);
    xhook_refresh(0);
}