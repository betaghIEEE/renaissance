PRODUCT = jnixform
USER = $(shell whoami)

OBJROOT = ./builds
SRCROOT = .
HEADERS = ./

FRAMEWORKS = /System/Library/Frameworks/JavaVM.framework/Headers
CPP_FLAGS = -g -I$

#libinstancemethodcall.jnilib: InstanceMethodCall.o
	#g++ -dynamiclib -o libinstancemethodcall.jnilib InstanceMethodCall.o -framework JavaVM

libpgmio.jnilib: pgmio.o
	g++ -dynamiclib -o libpgmio.jnilib pgmio.o -framework JavaVM
    
pgmio.o: pgmio.h pgmio.cpp
	g++ -c -I $(FRAMEWORKS) pgmio.cpp
    
pgmio.class: pgmio.java
	javac pgmio.java
    
pgmio.h:    pgmio.class
	javah -jni pgmio

InstanceMethodCall.class: InstanceMethodCall.java
	javac InstanceMethodCall.java
InstanceMethodCall.h: InstanceMethodCall.class
	javah -jni InstanceMethodCall
InstanceMethodCall.o: InstanceMethodCall.h InstanceMethodCall.cpp
	g++ -c -I $(FRAMEWORKS) InstanceMethodCall.cpp


convolve2D: xform.o xform2D.o convolve.o haarGen.o myVector.o imageio.o matrixType.o wave2D.o quadtree.o
	g++ xform.o xform2D.o convolve.o haarGen.o myVector.o imageio.o matrixType.o wave2D.o -o convolve2D

multiply2D: xform.o xform2D.o convolve.o haarGen.o myVector.o imageio.o matrixType.o multiplyex.o quadtree.o
	g++ xform.o xform2D.o convolve.o haarGen.o myVector.o imageio.o matrixType.o multiplyex.o -o multiply2D

multiply2Dex: xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o matrixType.o testmult.o quadtree.o queuearray.o qtreearray.o stackarray.o xformqtree.o
	g++ xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o queuearray.o qtreearray.o matrixType.o stackarray.o testmult.o xformqtree.o -o multiply2Dex

multiply2Darg: xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o matrixType.o testmultarg.o quadtree.o queuearray.o qtreearray.o stackarray.o xformqtree.o
	g++ xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o queuearray.o qtreearray.o matrixType.o stackarray.o testmultarg.o xformqtree.o -o multiply2Darg

multiply2Dpsin: xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o matrixType.o testpsimultarg.o quadtree.o queuearray.o qtreearray.o stackarray.o xformpsin.o
	g++ xform.o xform2D.o quadnode.o convolve.o haarGen.o myVector.o imageio.o queuearray.o qtreearray.o matrixType.o stackarray.o testpsimultarg.o xformpsin.o -o multiply2Dpsin


qtWX2D: xformqtree.o queuearray.o haarGen.o myVector.o imageio.o matrixType.o testqt.o qtreearray.o stackarray.o quadnode.o
	g++ xformqtree.o queuearray.o testqt.o haarGen.o myVector.o imageio.o matrixType.o qtreearray.o stackarray.o quadnode.o -o qtWX2D

xformqtree.o:	xformqtree.cpp xformqtree.hpp
	g++ -c -O2 xformqtree.cpp 
        
xformpsin.o:	xformpsin.cpp xformpsin.hpp
	g++ -c -O2 xformpsin.cpp 

quadnode.o:	quadnode.cpp quadnode.hpp
	g++ -c -O2 quadnode.cpp

queuearray.o: queuearray.cpp queuearray.hpp
	g++ -c -O2 queuearray.cpp

haarGen.o:  haarGen.cpp haarGen.hpp
	g++ -c -O2 haarGen.cpp
        
testmult.o: testmult.cpp
	g++ -c -O2 testmult.cpp

testmultarg.o: testmultarg.cpp
	g++ -c  testmultarg.cpp

testpsimultarg.o: testpsimultarg.cpp
	g++ -c  testpsimultarg.cpp

stackarray.o: stackarray.cpp stackarray.hpp
	g++ -c -O2 stackarray.cpp

imageio.o: imageio.cpp imageio.hpp
	g++ -c -O2 imageio.cpp

matrixType.o: matrixType.cpp matrixType.h
	g++ -c -O2 matrixType.cpp

myVector.o:	myVector.cpp myVector.h
	g++ -c -O2 myVector.cpp

testqt.o:	testqt.cpp
	g++ -c -O2 testqt.cpp

qtreearray.o:	qtreearray.cpp qtreearray.hpp
	g++ -c -O2 qtreearray.cpp

xform2D.o:	xform.o xform2D.cpp xform2D.hpp
	g++ -c -O2 xform2D.cpp
xform.o:	xform.cpp convolve.o myVector.o  haarGen.o xform.hpp
	g++ -c -O2 xform.cpp
