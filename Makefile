##########################################################################
##########################################################################

# Compiler settings
CC     = g++
CFLAGS = -g -arch x86_64 -std=c++11
CFLAGS +=  -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk
CFLAGS += -I/opt/local/include -I.

# Linker settings
LFLAGS = -framework Cocoa -framework OpenGL

SOLVER_SRC_FILES = expsolver.cpp \
	tokenizer.cpp \
	solver.cpp

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
	process.cpp \
	utilstest.cpp


OBJ_FILES := $(patsubst %.cpp,%.o,$(SRC_FILES))
SOLVER_OBJ_FILES := $(patsubst %.cpp,%.o,$(SOLVER_SRC_FILES))
# Default: Build all tests
all: utilstest solve

 %.o : %.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

utilstest: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LFLAGS) -o utilstest

solve: $(SOLVER_OBJ_FILES)
	$(CC) $(CFLAGS) $(SOLVER_OBJ_FILES) $(LFLAGS) -o solve


clean:
	rm $(OBJ_FILES) $(SOLVER_OBJ_FILES)
