PRODUCT = jnifields02
USER = $(shell whoami)

OBJROOT = ./builds
SRCROOT = .
HEADERS = ./

FRAMEWORKS = /System/Library/Frameworks/JavaVM.framework/Headers
CPP_FLAGS = -g -I$

libinstancemethodcall.jnilib: InstanceMethodCall.o
	g++ -dynamiclib -o libinstancemethodcall.jnilib InstanceMethodCall.o -framework JavaVM

InstanceMethodCall.class: InstanceMethodCall.java
	javac InstanceMethodCall.java
InstanceMethodCall.h: InstanceMethodCall.class
	javah -jni InstanceMethodCall
InstanceMethodCall.o: InstanceMethodCall.h InstanceMethodCall.cpp
	g++ -c -I $(FRAMEWORKS) InstanceMethodCall.cpp
