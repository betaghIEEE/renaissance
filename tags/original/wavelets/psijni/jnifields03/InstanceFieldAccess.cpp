#include "InstanceFieldAccess.h"

JNIEXPORT void JNICALL 
Java_InstanceFieldAccess_accessField(JNIEnv *env, jobject obj) 
{
    jfieldID fid;   /* store the field ID */ 
    jstring jstr; 
    const char *str; 

    /* Get a reference to obj's class */ 
    jclass cls = env->GetObjectClass(obj); 

    printf("In C:\n"); 

    /* Look for the instance field s in cls */ 
    fid = env->GetFieldID( cls, "s", 
                             "Ljava/lang/String;"); 
    if (fid == NULL) {
        return; /* failed to find the field */ 
    } 



    /* Read the instance field s */ 
    jstr = env->GetObjectField(obj, fid); 
    str = env->GetStringUTFChars(jstr, NULL); 
    if (str == NULL) {
        return; /* out of memory */ 
    } 
    printf("  c.s = \"%s\"\n", str); 
    env->ReleaseStringUTFChars(jstr, str); 

    /* Create a new string and overwrite the instance field */ 
    jstr = env->NewStringUTF("123"); 
    if (jstr == NULL) {
        return; /* out of memory */ 
    } 
    env->SetObjectField(obj, fid, jstr); 
} 
