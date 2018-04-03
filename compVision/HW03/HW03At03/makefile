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
CC = KCC

OPTIMIZER = -g
CPPFLAGS = $(OPTIMIZER) -LANG:std 
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

hw03:	main.o imageType.o calibrate1.o
	$(CC) $(CXXFLAGS) -o hw03 \
	main.o imageType.o calibrate1.o $(LIBDIR) $(LIBS)

############################################################

main.o: main.cpp 
	$(CC) $(CPPFLAGS) -c main.cpp $(INCDIR)
imageType.o: imageType.cpp imageType.h  
	$(CC) $(CPPFLAGS) -c imageType.cpp $(INCDIR)
calibrate1.o: calibrate1.cpp calibrate1.h numReader.o
	$(CC) $(CPPFLAGS) $(MATH_LIBS) -c numReader.o calibrate.cpp $(INCDIR)
numReader.o: numReader.cpp numReader.h
	$(CC) $(CPPFLAGS) $(MATH_LIBS) -c numReader.cpp $(INCDIR)

        
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
