#include <jni.h>
#include <string>
#include "TestClass.h"

#include <android/log.h>
#include <android/bitmap.h>
#include <complex>

#define  LOG_TAG    "native-lib"
#define  LogI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LogE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" {

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} argb;

JNIEXPORT jstring JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jint JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_testClassDataJNI(
        JNIEnv *env,
        jobject /* this */) {

    TestClass a(23);
    return jint(a.getValue());
}

JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_convertToGrayJNI(JNIEnv *env, jobject instance,
                                                                  jobject bitmapcolor,
                                                                  jobject bitmapgray) {

    void *pixelscolor;
    void *pixelsgray;

    int ret;
    int y;
    int x;

    AndroidBitmapInfo infoColor;
    AndroidBitmapInfo infoGray;

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infoColor)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, bitmapgray, &infoGray)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    LogI("color image :: width is %d; height is %d; stride is %d; format is %d;flags is %d",
         infoColor.width,
         infoColor.height,
         infoColor.stride,
         infoColor.format,
         infoColor.flags);
    if (infoColor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LogE("Bitmap format is not RGBA_8888 !");
        return;
    }


    LogI("gray image :: width is %d; height is %d; stride is %d; format is %d;flags is %d",
         infoGray.width,
         infoGray.height,
         infoGray.stride,
         infoGray.format,
         infoGray.flags);
    if (infoGray.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LogE("Bitmap format is not RGBA_8888 !");
        return;
    }


    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapgray, &pixelsgray)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    // modify pixels with image processing algorithm

    uint8_t pixel;
    for (y = 0; y < infoColor.height; ++y) {
        argb *line = (argb *) pixelscolor;
        argb *grayline = (argb *) pixelsgray;

        for (x = 0; x < infoColor.width; ++x) {
            pixel = (uint8_t) (0.29 * (double) line[x].r + 0.59 * (double) line[x].g +
                               0.11 * (double) line[x].b);
            grayline[x].r = pixel;
            grayline[x].g = pixel;
            grayline[x].b = pixel;
            grayline[x].a = line[x].a;
        }

        pixelscolor = (char *) pixelscolor + infoColor.stride;
        pixelsgray = (char *) pixelsgray + infoGray.stride;
    }

    LogI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapgray);

}

JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_invertColorsJNI(JNIEnv *env,
                                                                 jobject instance,
                                                                 jobject in,
                                                                 jobject out) {

    void *pixelsin;
    void *pixelsout;

    int ret;
    int y;
    int x;

    AndroidBitmapInfo infoin;
    AndroidBitmapInfo infoout;

    if ((ret = AndroidBitmap_getInfo(env, in, &infoin)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, out, &infoout)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (infoin.format != ANDROID_BITMAP_FORMAT_RGBA_8888
        || infoout.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LogE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, in, &pixelsin)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, out, &pixelsout)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    for (y = 0; y < infoin.height; ++y) {
        int32_t *line = (int32_t *) pixelsin;
        int32_t *outline = (int32_t *) pixelsout;
        for (x = 0; x < infoin.width; ++x) {
            outline[x] = (0xFFFFFF - line[x]) | 0xFF000000;
        }

        pixelsin = (char *) pixelsin + infoin.stride;
        pixelsout = (char *) pixelsout + infoout.stride;
    }

    LogI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, in);
    AndroidBitmap_unlockPixels(env, out);


}

JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_convertToSerpiaJNI(JNIEnv *env,
                                                                    jobject instance,
                                                                    jobject in,
                                                                    jobject out) {

    void *pixelsin;
    void *pixelsout;

    int ret;
    int y;
    int x;

    AndroidBitmapInfo infoin;
    AndroidBitmapInfo infoout;

    if ((ret = AndroidBitmap_getInfo(env, in, &infoin)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, out, &infoout)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (infoin.format != ANDROID_BITMAP_FORMAT_RGBA_8888
        || infoout.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LogE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, in, &pixelsin)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, out, &pixelsout)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    double pixel;
    for (y = 0; y < infoin.height; ++y) {
        argb *line = (argb *) pixelsin;
        argb *outline = (argb *) pixelsout;
        for (x = 0; x < infoin.width; ++x) {
            pixel = 0.29 * (double) line[x].r + 0.59 * (double) line[x].g +
                    0.11 * (double) line[x].b;
            outline[x].r = 0xff & (uint8_t) pixel;
            outline[x].g = 0xff & (uint8_t) (pixel * 0.81);
            outline[x].b = 0xff & (uint8_t) (pixel * 0.61);
            outline[x].a = line[x].a;
        }

        pixelsin = (char *) pixelsin + infoin.stride;
        pixelsout = (char *) pixelsout + infoout.stride;
    }

    LogI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, in);
    AndroidBitmap_unlockPixels(env, out);


}

JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_convertTo8BitAlphaJNI(JNIEnv *env,
                                                                       jobject instance,
                                                                       jobject bitmapcolor,
                                                                       jobject bitmapgray) {

    void *pixelscolor;
    void *pixelsgray;

    int ret;
    int y;
    int x;

    AndroidBitmapInfo infoColor;
    AndroidBitmapInfo infoGray;

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infoColor)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_getInfo(env, bitmapgray, &infoGray)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (infoColor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LogE("Bitmap format is not RGBA_8888 !");
        return;
    }


    if (infoGray.format != ANDROID_BITMAP_FORMAT_A_8) {
        LogE("Bitmap format is not A_8 !");
        return;
    }


    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &pixelscolor)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapgray, &pixelsgray)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    // modify pixels with image processing algorithm

    for (y = 0; y < infoColor.height; ++y) {
        argb *line = (argb *) pixelscolor;
        uint8_t *grayline = (uint8_t *) pixelsgray;
        for (x = 0; x < infoColor.width; ++x) {
            grayline[x] = 0.21 * line[x].r + 0.72 * line[x].g + 0.07 * line[x].b;
        }

        pixelscolor = (char *) pixelscolor + infoColor.stride;
        pixelsgray = (char *) pixelsgray + infoGray.stride;
    }

    LogI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    AndroidBitmap_unlockPixels(env, bitmapgray);

}

JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_findEdges(JNIEnv *env, jobject instance,
                                                           jobject bitmapgray,
                                                           jobject bitmapedges) {
    AndroidBitmapInfo infogray;
    void *pixelsgray;
    AndroidBitmapInfo infoedges;
    void *pixelsedge;
    int ret;
    int y;
    int x;
    int sumX, sumY, sum;
    int i, j;
    int Gx[3][3];
    int Gy[3][3];
    uint8_t *graydata;
    uint8_t *edgedata;


    LogI("findEdges running");

    Gx[0][0] = -1;
    Gx[0][1] = 0;
    Gx[0][2] = 1;
    Gx[1][0] = -2;
    Gx[1][1] = 0;
    Gx[1][2] = 2;
    Gx[2][0] = -1;
    Gx[2][1] = 0;
    Gx[2][2] = 1;


    Gy[0][0] = 1;
    Gy[0][1] = 2;
    Gy[0][2] = 1;
    Gy[1][0] = 0;
    Gy[1][1] = 0;
    Gy[1][2] = 0;
    Gy[2][0] = -1;
    Gy[2][1] = -2;
    Gy[2][2] = -1;


    if ((ret = AndroidBitmap_getInfo(env, bitmapgray, &infogray)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }


    if ((ret = AndroidBitmap_getInfo(env, bitmapedges, &infoedges)) < 0) {
        LogE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }


    if (infogray.format != ANDROID_BITMAP_FORMAT_A_8) {
        LogE("Bitmap format is not A_8 !");
        return;
    }

    if (infoedges.format != ANDROID_BITMAP_FORMAT_A_8) {
        LogE("Bitmap format is not A_8 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmapgray, &pixelsgray)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmapedges, &pixelsedge)) < 0) {
        LogE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    graydata = (uint8_t *) pixelsgray;
    edgedata = (uint8_t *) pixelsedge;

    for (y = 0; y <= infogray.height - 1; y++) {
        for (x = 0; x < infogray.width - 1; x++) {
            sumX = 0;
            sumY = 0;
            // check boundaries
            if (y == 0 || y == infogray.height - 1) {
                sum = 0;
            } else if (x == 0 || x == infogray.width - 1) {
                sum = 0;
            } else {
                // calc X gradient
                for (i = -1; i <= 1; i++) {
                    for (j = -1; j <= 1; j++) {
                        sumX += (int) ((*(graydata + x + i + (y + j)
                                                             * infogray.stride)) *
                                       Gx[i + 1][j + 1]);
                    }
                }

                // calc Y gradient
                for (i = -1; i <= 1; i++) {
                    for (j = -1; j <= 1; j++) {
                        sumY += (int) ((*(graydata + x + i + (y + j)
                                                             * infogray.stride)) *
                                       Gy[i + 1][j + 1]);
                    }
                }

                sum = std::abs(sumX) + std::abs(sumY);

            }

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            //*(edgedata + x + y * infogray.width) = 255 - (uint8_t) sum;
            *(edgedata + x + y * infogray.width) =  (uint8_t) sum;


        }
    }

    AndroidBitmap_unlockPixels(env, bitmapgray);
    AndroidBitmap_unlockPixels(env, bitmapedges);
}


JNIEXPORT void JNICALL
Java_examples_my_com_lection_1p2_15_MainActivity_doitJNI(JNIEnv* env, jobject obj) {

    jthrowable exc;

    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "callback", "()V");

    if (mid == nullptr)
        return;

    env->CallVoidMethod(obj, mid);
    exc = env->ExceptionOccurred();
    if (exc != nullptr) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        jclass newExc = env->FindClass("java/lang/IllegalArgumentException");
        if (newExc == nullptr) {
            return;
        }
        env->ThrowNew(newExc, "Throw from C++ code");
    }

}



} // extern "C"