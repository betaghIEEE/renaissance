############################################################
#
# Makefile for renaissance
#
# renaissance version 3.0
# Created: 09/22/00
# previously called palette, enterprise, chisel
#
# renaissance is a set of basic elements for research in
# image processing,  computer vision and computer graphics
#
# It provides primitives for
# + graphical objects
#   - CPoint
#   - CDirection
#   - CFace
#   - CMesh
# + image objects
#   - CGrayImages
#   - CColorImages
# + numerical objects
#   - CMatrix
#   - CVector
#
# In addition, CGrid and CVolume provide basic templates
# for 2D and 3D operations
#
# Requirements:
#   Some LAPACK library
#
# Revisions:
#   04/01/02 Removed the image processing, vision and graphics
#            libraries ... not essential to renaissance
#
#   01/31/05 Dropping ImageMagick
#
############################################################

#include $(HOME)/research/LocalInfo
RENAISSANCE_INCLUDE = $(HOME)/include/renaissance

SRCS = \
	r_valarray.cpp \
	point2I.cpp \
	point2D.cpp \
	point3D.cpp \
	point3DH.cpp \
	vector2D.cpp \
	vector3D.cpp \
	matrix.cpp \
	grayImage.cpp \
	colorImage.cpp \
	utility.cpp \
	mesh.cpp \
	camera.cpp \
	face.cpp \
	edge.cpp

XHDRS = \
	octree.hpp \
	digits.hpp \
	config.hpp \
	slice_iter.hpp \
	grid.hpp \
	volume.hpp \
#	point.hpp 


HDRS = ${SRCS:.cpp=.hpp}
OBJS = ${SRCS:.cpp=.o}

INCDIR = $(LOCAL_INCLUDE) -I/sw/include
LIBDIR = -L/sw/lib
LIBS = 

CC = g++
CFLAGS = -O4 -g

renaissance: $(OBJS)	
#	$(ARCHIVE) libRenaissance.a $(OBJS) 
#	CC -ar -o libRenaissance.a $(OBJS) 
	ar -r libRenaissance.a $(OBJS)
	ranlib libRenaissance.a


.SUFFIXES : .o .cpp

.cpp.o : 
	$(CC) $(CFLAGS) -c $< $(INCDIR)


install : 
	-mkdir -p $(HOME)/lib
	-mkdir -p $(HOME)/include
	-mkdir -p $(RENAISSANCE_INCLUDE)
	-cp libRenaissance.a $(HOME)/lib/.
	-cp renaissance.hpp $(HOME)/include/.
	-cp $(HDRS) $(XHDRS) $(RENAISSANCE_INCLUDE)/.
	-cp config.hpp $(RENAISSANCE_INCLUDE)/.

clean :
	-rm -f  *~ *.o *.a core
	-du

backup:
	rm -rf backup
	mkdir backup
	cp *.cpp *.c *.h Makefile README backup


doc:
	doxygen docConfig
	(cd $(HOME)/docs/renaissance/latex; make; cp refman.pdf $(HOME)/docs/renaissance/html)
	rsync -Cautv --delete --rsh=ssh $(HOME)/docs/renaissance/html/* root@drill.cs.ttu.edu:/srv/www/htdocs/locked/docs/renaissance/

