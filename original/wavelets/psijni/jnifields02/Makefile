PRODUCT = jnifields02
USER = $(shell whoami)

OBJROOT = ./builds
SRCROOT = .
HEADERS = ./

FRAMEWORKS = /System/Library/Frameworks/JavaVM.framework/Headers
CPP_FLAGS = -g -I$

HelloWorld.class: helloworld.java
	javac helloworld.java
HelloWorld.h: HelloWorld.class
	javah -jni HelloWorld
helloworld.o: HelloWorld.h helloworld.cpp
	g++ -c -I $(FRAMEWORKS) helloworld.cpp
libhelloworld.jnilib: helloworld.o
	g++ -dynamiclib -o libhelloworld.jnilib helloworld.o -framework JavaVM
