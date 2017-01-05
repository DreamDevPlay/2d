#include <jni.h>
#include "model.h"

Model model;

extern "C"
void
Java_com_example_user_a2d_MainActivity_modelNextStep(
        JNIEnv *env,
        jobject /* this */,
        jbyteArray data){
    jboolean isCopy;
    jbyte *d = env->GetByteArrayElements(data, &isCopy);
    model.nextStep((char*)d);
    env->ReleaseByteArrayElements(data, d, JNI_ABORT);
}
