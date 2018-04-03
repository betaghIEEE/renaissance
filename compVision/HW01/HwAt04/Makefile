############################################################
#Makefile for testImage#
########################
#
#
############################################################

all: basic

############################################################

CC = CC
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

basic:	basicTest.o image.o 
	$(CC) $(CXXFLAGS) -o basic \
	basicTest.o image.o $(LIBDIR) $(LIBS)

############################################################

basicTest.o: basicTest.cpp image.hpp 
	$(CC) $(CPPFLAGS) -c basicTest.cpp $(INCDIR)
image.o: image.cpp image.hpp  
	$(CC) $(CPPFLAGS) -c image.cpp $(INCDIR)

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
