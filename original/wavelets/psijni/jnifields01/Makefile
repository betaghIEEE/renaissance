PRODUCT = jnifields01
USER    = $(shell whoami)

#
# These variables are used for building separate to the source code,
# which makes building read-only examples possible
#
OBJROOT = /Builds/$(USER)/$(PRODUCT)/$(PRODUCT).obj
SYMROOT = /Builds/$(USER)/$(PRODUCT)/$(PRODUCT).sym
DSTROOT = /Builds/$(USER)/$(PRODUCT)/$(PRODUCT).dst
SRCROOT = .

#
# Set the DYLIBLOCATION variable to wherever you want the dylibs
# WARNING: dylib locations are written at build time and your program
# will not run without the dylib in the correct location
#
DYLIBLOCATION = $(SYMROOT)
JARFILE = $(SYMROOT)/jnifields01.jar
JNILIB  = $(SYMROOT)/libjnifields01.jnilib
DYLIB   = $(SYMROOT)/libjnifields01.dylib

all: directories $(JARFILE) $(JNILIB) $(DYLIB)
	@echo Finished building.

install: $(DSTROOT) all
	mkdir -p $(DSTROOT)/Library/Java/Extensions $(DSTROOT)/$(DYLIBLOCATION)
	cp $(JARFILE) $(DSTROOT)/Library/Java/Extensions
	cp $(JNILIB) $(DSTROOT)/Library/Java/Extensions
	cp $(DYLIB) $(DSTROOT)/$(DYLIBLOCATION)

clean:
	/bin/rm -rf $(SYMROOT) $(OBJROOT)

directories: $(OBJROOT) $(SYMROOT)


C_FLAGS      = -g -I$(OBJROOT) -I/System/Library/Frameworks/JavaVM.framework/Headers
DYLIB_FLAGS  = 
BUNDLE_FLAGS = -ljnifields01 -L$(SYMROOT)


$(OBJROOT) $(SYMROOT) $(DSTROOT):
	mkdir -p $@

$(JARFILE): $(OBJROOT)/JNIWrapper.class
	cd $(OBJROOT); \
	echo "Main-Class: JNIWrapper" > $(OBJROOT)/manifest; \
	jar cmf $(OBJROOT)/manifest $@ *.class

$(JNILIB): $(OBJROOT)/jnifields01jnilib.o $(DYLIB)
	cc -bundle $(BUNDLE_FLAGS) -o $@ $<

$(OBJROOT)/jnifields01jnilib.o : jnifields01jnilib.c $(OBJROOT)/JNIWrapper.h
	cc $(C_FLAGS) -c -o $@ $<

$(OBJROOT)/JNIWrapper.h: $(OBJROOT)/JNIWrapper.class
	javah -classpath $(OBJROOT) -d $(OBJROOT) JNIWrapper

$(OBJROOT)/JNIWrapper.class: JNIWrapper.java
	javac -classpath $(OBJROOT) -d $(OBJROOT) $<

$(DYLIB): $(OBJROOT)/jnifields01dylib.o 
	cc -dynamiclib $(DYLIB_FLAGS) -install_name $(DYLIBLOCATION)/libjnifields01.dylib -o $@ $<

$(OBJROOT)/jnifields01dylib.o: jnifields01dylib.c jnifields01dylib.h
	cc $(C_FLAGS) -c -o $@ $<

test: $(JARFILE)
	java -classpath $(JARFILE) -Djava.library.path=$(DYLIBLOCATION) JNIWrapper
