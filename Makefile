##########################################################################
##########################################################################

# Compiler settings
CC     = g++
CFLAGS = -g -arch x86_64 
CFLAGS +=  -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
CFLAGS += -I/opt/local/include -I.

# Linker settings
LFLAGS = -framework Cocoa -framework OpenGL

SRC_FILES = expsolver.cpp \
	thread.cpp \
	event.cpp \
	urlparser.cpp \
	tokenizer.cpp \
	curve.cpp \
	observer.cpp \
	PropertyContainer.cpp \
	logger.cpp \
	xmlparser.cpp \
	utilstest.cpp

# Default: Build all tests
all: demo

demo: $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) $(LFLAGS) -o utilstest
	# Debug/demo

clean:
	rm Debug/demo
