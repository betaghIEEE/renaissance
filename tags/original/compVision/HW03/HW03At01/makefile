############################################################
#Makefile for testImage#
########################
#
#
############################################################

all: hw03

############################################################

KCC = cc -LANG:std
GCC = gcc 
CC = CC

OPTIMIZER = -g
CPPFLAGS = -LANG:std 
CXXFLAGS = $(OPTIMIZER) -LANG:std 

############################################################

#LIBS
GENERAL_LIBS = -lm
MATH_LIBS = -lscs
MAGICK_LIBS = -lMagick++ -L/usr/freeware/lib32 -lMagick -llcms -ltiff -ljpeg -lpng -lXext -lXt -lSM -lICE -lX11 -lbz2 -lxml2 -lz -lpthread

LIBS = $(GENERAL_LIBS) $(MAGICK_LIBS)

LIBDIR = -L/usr/freeware/lib32
INCDIR = -I/usr/freeware/include -I/usr/freeware/include/libxml2

############################################################

hw03:	main.o imageType.o slopeClass.o projectionMatrix.o EdgeAndLines.o
	$(CC) $(CXXFLAGS) -o hw03 \
	main.o imageType.o slopeClass.o projectionMatrix.o EdgeAndLines.o $(LIBDIR) $(LIBS)

############################################################

main.o: main.cpp 
	$(CC) $(CPPFLAGS) -c main.cpp $(INCDIR)
imageType.o: imageType.cpp imageType.h  
	$(CC) $(CPPFLAGS) -c imageType.cpp $(INCDIR)
EdgeAndLines.o: EdgeAndLines.cpp EdgeAndLines.h slopeClass.h
	$(CC) $(CPPFLAGS) -c EdgeAndLines.cpp $(INCDIR)
slopeClass.o: slopeClass.cpp slopeClass.h
	$(CC) $(CPPFLAGS) -c slopeClass.cpp $(INCDIR)
projectionMatrix.o: projectionMatrix.cpp projectionMatrix.h  
	$(CC) $(CPPFLAGS) $(MATH_LIBS) -c projectionMatrix.cpp $(INCDIR)
        
############################################################

.SUFFIXES : .o .cpp

.cpp.o : 
	$(CC) $(CPPFLAGS) -c $< $(INCDIR)

clean :
	-rm -rf *~ *.o outfile* *.aux *.log *.dvi core
	-du

extraclean :
	-rm -rf *~ *.o outfile* *.aux *.log *.dvi core basic
	-du -sk * | sort -n
