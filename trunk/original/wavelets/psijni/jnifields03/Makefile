PRODUCT = jnifields02
USER = $(shell whoami)

OBJROOT = ./builds
SRCROOT = .
HEADERS = ./

FRAMEWORKS = /System/Library/Frameworks/JavaVM.framework/Headers
CPP_FLAGS = -g -I$

libinstancefieldaccess.jnilib: InstanceFieldAccess.o
	g++ -dynamiclib -o libinstancefieldaccess.jnilib InstanceFieldAccess.o -framework JavaVM

InstanceFieldAccess.class: InstanceFieldAccess.java
	javac InstanceFieldAccess.java
InstanceFieldAccess.h: InstanceFieldAccess.class
	javah -jni InstanceFieldAccess
InstanceFieldAccess.o: InstanceFieldAccess.h InstanceFieldAccess.cpp
	g++ -c -I $(FRAMEWORKS) InstanceFieldAccess.cpp
