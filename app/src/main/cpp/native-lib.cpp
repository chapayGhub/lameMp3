#include <jni.h>
#include <string>
#include <lame.h>
#include <lame_global_flags.h>
#include <android/log.h>

#define LOG_TAG  "C_TAG"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define NUM_SAMPLES 1000

lame_global_struct *global_struct;

/**
 * 初使化
 */
extern "C"
JNIEXPORT int JNICALL
Java_com_mp3_inc_lamemp3_MainActivity_initLame(JNIEnv *env, jobject instance, jint numChannels,
                                               jint sampleRate, jint brate, jint mode,
                                               jint quality) {
    global_struct = lame_init();
    lame_set_num_channels(global_struct, numChannels);
    lame_set_in_samplerate(global_struct, sampleRate);
    lame_set_brate(global_struct, brate);
    lame_set_mode(global_struct, (MPEG_mode) mode);
    lame_set_quality(global_struct, quality);
    int ret_code = lame_init_params(global_struct);
    return ret_code;
}

/**
 * wav文件转为mp3文件
 */
extern "C"
JNIEXPORT int JNICALL
Java_com_mp3_inc_lamemp3_MainActivity_encode3Mp3(JNIEnv *env, jobject instance) {

    // TODO
    FILE *file_in = fopen("/sdcard/birthday.wav", "rb");
    if (file_in == NULL) {
        lame_close(global_struct);
        return -1;
    }
    FILE *file_out = fopen("/sdcard/birthday.mp3", "wb");
    short input[NUM_SAMPLES];
    int out_size = NUM_SAMPLES * 1.25 + 7200;
    char output[out_size];
    int read_size;
    while (true) {
        read_size = fread(input, sizeof(short), NUM_SAMPLES, file_in);
        int code = lame_encode_buffer_interleaved(global_struct, input, read_size / 2,
                                                  (unsigned char *) output,
                                                  out_size);
        if (code < 0) {
            lame_close(global_struct);
            return 1;
        }
        fwrite(output, 1, code, file_out);
        if (read_size < NUM_SAMPLES) {
            int final_write = lame_encode_flush(global_struct, (unsigned char *) output,
                                                out_size);
            fwrite(output, 1, final_write, file_out);
            fclose(file_in);
            fclose(file_out);
            break;
        }
    }
    lame_close(global_struct);
    return 0;
}

//void showmessage(JNIEnv *env, jobject instance, std::string msg) {
//    jclass jclass1 = env->GetObjectClass(instance);
//    if (jclass1 == NULL) {
//        LOGD("jclass1 == NULL");
//    }
//    jmethodID jmethodID1 = env->GetMethodID(jclass1, "showMsg", "(Ljava/lang/String;)V");
//    if (jmethodID1 == NULL) {
//        LOGD("jmethodID1 == NULL");
//    }
//    jstring message = env->NewStringUTF(msg.c_str());
//    env->CallVoidMethod(instance, jmethodID1, message);
//}