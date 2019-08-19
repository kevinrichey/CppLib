
# Config

#ACTIVE_TARGET = testsdl
#ACTIVE_TARGET = test
#ACTIVE_TARGET = hello
#ACTIVE_TARGET = sidewinder
ACTIVE_TARGET = sidewinder

# Source Files

KWR_SOURCE = kwrsdl.cpp kwrerr.cpp kwrgame.cpp kwrlib.cpp kwrlegocolors.cpp kwrprng.cpp
HELLO_SOURCE = hello.cpp kwrlib.cpp kwrerr.cpp kwrsdl.cpp kwrgame.cpp kwrlegocolors.cpp
MAZE_SOURCE = $(KWR_SOURCE) 
DRAWTEXT_SRC = $(KWR_SOURCE) drawtext.cpp
TEST_SOURCE = test.cpp kwrlib.cpp kwrerr.cpp

TEST_OBJ = $(TEST_SOURCE:.cpp=.o)
HELLO_OBJ = $(HELLO_SOURCE:.cpp=.o)
MAZE_OBJ = $(MAZE_SOURCE:.cpp=.o)

# C++ Compiler Options

INCLUDE_PATH = -I/mingw64/include/SDL2
GENERATE_DEPENDENCY_RULES = -MMD
DEBUGGING = -g -D DEBUG
CPPFLAGS = $(INCLUDE_PATH) $(GENERATE_DEPENDENCY_RULES)
CXXFLAGS = $(DEBUGGING) 

# Linker Options

LIBRARY_PATH = -LC:/mingw64/lib
WINDOWS_SUBSYS = -Wl,-subsystem,windows
LDFLAGS = $(LIBRARY_PATH) $(WINDOWS_SUBSYS)
LDLIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
LINK.o = $(LINK.cc)

# Targets

all: tags runtest run

run: $(ACTIVE_TARGET)
	./$(ACTIVE_TARGET).exe

runtest: test
	./test.exe

debug: $(ACTIVE_TARGET)
	gdb ./$(ACTIVE_TARGET).exe -q 

break: $(ACTIVE_TARGET)
	gdb ./$(ACTIVE_TARGET).exe -q -ex "b $(FILE):$(LINE)" -ex run

tags: $(HELLO_SOURCE) 
	ctags -R .
	cscope -R -b


hello: $(HELLO_OBJ)

drawtext: $(DRAWTEXT_SRC)

test: $(TEST_OBJ) 

bintreemaze: $(MAZE_OBJ)

sidewinder: $(MAZE_OBJ)

testsdl: testsdl.cpp kwrgame.cpp kwrsdl.cpp

game: MySDL
	./MySDL.exe

noise: NoiseTest
	./NoiseTest.exe

spline: DrawSpline
	./DrawSpline.exe

rng: rngtest
	./rngtest.exe

MySDL: $(KWR_SOURCE:.cpp=.o)

#TestKwr: $(TEST_SOURCE:.cpp=.o) $(KWR_SOURCE:.cpp=.o)

NoiseTest: $(KWR_SOURCE:.cpp=.o)

SplineTest: $(KWR_SOURCE:.cpp=.o)

rngtest: $(KWR_SOURCE:.cpp=.o)

DrawSpline: $(KWR_SOURCE:.cpp=.o)

clean:
	rm -f *.exe *.o *.d

.PHONY: clean all run runtest

# Include the .d dependency files

include $(wildcard $(KWR_SOURCE:.cpp=.d))
include $(wildcard $(HELLO_SOURCE:.cpp=.d))
include $(wildcard $(TEST_SOURCE:.cpp=.d))
include $(wildcard $(MAZE_SOURCE:.cpp=.d))
