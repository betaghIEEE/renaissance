multiplytest:	testmult.o multiply.o chainlink.o hook.o matrixType.o xform2D.o myVector.o xform.o convolve.o haarGen.o plotResults.o
	g++ testmult.o multiply.o chainlink.o hook.o matrixType.o xform2D.o myVector.o xform.o convolve.o haarGen.o plotResults.o -o multiplytest

matrixType.o: matrixType.cpp matrixType.h
	g++ -c matrixType.cpp
multiply.o: multiply.cpp multiply.h xform2D.hpp
	g++ -c multiply.cpp
xform2D.o: xform2D.cpp xform2D.hpp
	g++ -c xform2D.cpp 
chainlink.o:  chainlink.cpp chainlink.h
	g++ -c chainlink.cpp
hook.o: hook.cpp hook.h chlink.h
	g++ -c chlink.h
testmult.o:  testmult.cpp  multiply.h
	g++ -c testmult.cpp
myVector.o:	myVector.cpp myVector.h
	g++ -c myVector.cpp
xform.o:	xform.cpp xform.hpp
	g++ -c xform.cpp
convolve.o: convolve.cpp convolve.hpp
	g++ -c convolve.cpp
haarGen.o:	haarGen.cpp haarGen.hpp
	g++ -c haarGen.cpp
plotResults.o: 	plotResults.cpp plotResults.hpp
	g++ -c plotResults.cpp