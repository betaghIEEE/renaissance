############################################################
#Makefile for testImage#
########################
#
#
############################################################

all: hw03

############################################################

KCC = cc
GCC = gcc 
CC = $(KCC)

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

hw03:	main.o matrixType.o calType.o correlate.o
	$(CC) $(CXXFLAGS) -o hw03 \
	main.o matrixType.o calType.o correlate.o $(LIBDIR) $(LIBS)

############################################################

main.o: main.cpp 
	$(CC) $(CPPFLAGS) -c main.cpp 
matrixType.o: matrixType.cpp matrixType.h  
	$(CC) $(CPPFLAGS) -c matrixType.cpp 
svd.o: svd.cpp svd.h 
        $(CC) $(CPPFLAGS) $(MATH_LIBS) -c svd.cpp
calType.o: calType.cpp calType.h matrixType.o svd.o
	$(CC) $(CPPFLAGS)  -c svd.o calType.cpp 
correlate.o: correlate.cpp correlate.h calType.o
        $(CC) $(CPPFLAGS) -c calType.o correlate.cpp 

        
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
