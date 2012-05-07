convolve1D:	xform.o convolve.o plotResults.o haarGen.o myVector.o main.o
	g++ xform.o convolve.o plotResults.o haarGen.o myVector.o main.o -o convolve1D 
#convolve2D: xform.o xform2D.o convolve.o plotResults.o haarGen.o myVector.o imageio.o matrixType.o wave2D.o 
#	g++ xform.o xform2D.o convolve.o plotResults.o haarGen.o myVector.o imageio.o matrixType.o wave2D.o   -o convolve2D
xform2D.o:	xform.o xform2D.cpp xform2D.hpp
	g++ -c xform2D.cpp
xform.o:	xform.cpp convolve.o myVector.o plotResults.o haarGen.o xform.hpp
	g++ -c xform.cpp
convolve.o:	convolve.cpp convolve.hpp myVector.o
	g++ -c convolve.cpp
plotResults.o:	plotResults.cpp plotResults.hpp myVector.o
	g++ -c plotResults.cpp
haarGen.o:	haarGen.cpp haarGen.hpp myVector.o
	g++ -c haarGen.cpp
myVector.o:	myVector.cpp myVector.h
	g++ -c myVector.cpp
imageio.o:	imageio.hpp imageio.cpp
	g++ -c imageio.cpp
matrixType.o:	matrixType.h matrixType.cpp	
	g++ -c matrixType.cpp
main.o: 	main.cpp
	g++ -c main.cpp
wave2D.o:	wave2D.cpp
	g++ -c wave2D.cpp

	
