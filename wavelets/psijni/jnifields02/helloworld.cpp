#include <jni.h>
#include <iostream.h>
#include "HelloWorld.h"

JNIEXPORT void JNICALL Java_HelloWorld_print
  (JNIEnv *env, jobject obj)
  {
  	cout << "Hello World!\n";
	cout << "Stop smoking pot \n";
  }
  
