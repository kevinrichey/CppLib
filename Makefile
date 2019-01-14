
# Config

#ACTIVE_TARGET = testsdl
#ACTIVE_TARGET = test
ACTIVE_TARGET = hello

# Source Files

HELLO_SOURCE = hello.cpp kwrlib.cpp
TEST_SOURCE = test.cpp kwrlib.cpp
KWR_SOURCE = kwrsdl.cpp kwrgame.cpp kwrprng.cpp kwrlib.cpp

TEST_OBJ = $(TEST_SOURCE:.cpp=.o)
HELLO_OBJ = $(HELLO_SOURCE:.cpp=.o)

# C++ Compiler Options

INCLUDE_PATH = -IC:\msys64\mingw64\include\SDL2
GENERATE_DEPENDENCY_RULES = -MMD
DEBUGGING = -g -D DEBUG
CPPFLAGS = $(INCLUDE_PATH) $(GENERATE_DEPENDENCY_RULES)
CXXFLAGS = $(DEBUGGING) 

# Linker Options

LIBRARY_PATH = -LC:\msys64\mingw64\lib
WINDOWS_SUBSYS = -Wl,-subsystem,windows
LDFLAGS = $(LIBRARY_PATH) $(WINDOWS_SUBSYS)
LDLIBS = -lmingw32 -lSDL2main -lSDL2
LINK.o = $(LINK.cc)

# Targets

all: tags runtest run

run: $(ACTIVE_TARGET)
	./$(ACTIVE_TARGET).exe Kevin

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

test: $(TEST_OBJ) 


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

include $(wildcard $(HELLO_SOURCE:.cpp=.d))
include $(wildcard $(TEST_SOURCE:.cpp=.d))
