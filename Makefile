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


OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))
# Default: Build all tests
all: utilstest

 %.o : %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

utilstest: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LFLAGS) -o utilstest

clean:
	rm $(OBJ_FILES)
