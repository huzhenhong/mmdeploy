/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class mmdeploy_PoseTracker */

#ifndef _Included_mmdeploy_PoseTracker
    #define _Included_mmdeploy_PoseTracker
    #ifdef __cplusplus
extern "C"
{
    #endif
    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    create
     * Signature: (JJJ)J
     */
    JNIEXPORT jlong JNICALL        Java_mmdeploy_PoseTracker_create(JNIEnv*, jobject, jlong, jlong, jlong);

    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    destroy
     * Signature: (J)V
     */
    JNIEXPORT void JNICALL         Java_mmdeploy_PoseTracker_destroy(JNIEnv*, jobject, jlong);

    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    createState
     * Signature: (JLmmdeploy/PoseTracker/Params;)J
     */
    JNIEXPORT jlong JNICALL        Java_mmdeploy_PoseTracker_createState(JNIEnv*, jobject, jlong, jobject);

    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    destroyState
     * Signature: (J)V
     */
    JNIEXPORT void JNICALL         Java_mmdeploy_PoseTracker_destroyState(JNIEnv*, jobject, jlong);

    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    setDefaultParams
     * Signature: ()Lmmdeploy/PoseTracker/Params;
     */
    JNIEXPORT jobject JNICALL      Java_mmdeploy_PoseTracker_setDefaultParams(JNIEnv*, jobject);

    /*
     * Class:     mmdeploy_PoseTracker
     * Method:    apply
     * Signature: (J[J[Lmmdeploy/Mat;[I[I)[Lmmdeploy/PoseTracker/Result;
     */
    JNIEXPORT jobjectArray JNICALL Java_mmdeploy_PoseTracker_apply(JNIEnv*, jobject, jlong, jlongArray, jobjectArray, jintArray, jintArray);

    #ifdef __cplusplus
}
    #endif
#endif
