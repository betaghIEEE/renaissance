#include <iostream.h>
#include "InstanceMethodCall.h"


JNIEXPORT void JNICALL 
Java_InstanceMethodCall_nativeMethod(JNIEnv *env, jobject obj) 
{
	jint ten = 10;
	int teni = 10;
    jclass cls = env->GetObjectClass( obj); 
    jmethodID mid = 
        env->GetMethodID( cls, "callback", "()V"); 
	jmethodID sid = env->GetMethodID(cls, "smokegun", "()V"); 
	jmethodID nid = env->GetMethodID(cls, "smokeint", "(I)I");
    if (mid == NULL) {
        return; /* method not found */ 
    } 
    printf("In C\n"); 
    	env->CallVoidMethod( obj, mid); 
	env->CallVoidMethod (obj, sid);
	teni = env->CallIntMethod (obj, nid, ten);
	//env->CallIntMethod (obj, nid, teni);
	//teni = ten;
	cout << " In C++ " << teni << "\n";
} 
