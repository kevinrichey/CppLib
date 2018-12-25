
# Config

ACTIVE_TARGET = testsdl
#ACTIVE_TARGET = test
#ACTIVE_TARGET = hello

# Files

HELLO_SOURCE = kwrlib.cpp
KWR_SOURCE = kwrsdl.cpp kwrgame.cpp kwrprng.cpp kwrlib.cpp
TEST_SOURCE = TestCase.cpp testkwrlib.cpp TestKwrGame.cpp

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

# Targets

all: tags run

run: $(ACTIVE_TARGET)
	./$(ACTIVE_TARGET).exe

debug: $(ACTIVE_TARGET)
	gdb ./$(ACTIVE_TARGET).exe -q 

break: $(ACTIVE_TARGET)
	gdb ./$(ACTIVE_TARGET).exe -q -ex "b $(FILE):$(LINE)" -ex run

tags: $(KWR_SOURCE) $(TEST_SOURCE) test.cpp test.h
	ctags -R .
	cscope -R -b


test: test.cpp test.h

testsdl: testsdl.cpp kwrgame.cpp kwrsdl.cpp

game: MySDL
	./MySDL.exe

noise: NoiseTest
	./NoiseTest.exe

spline: DrawSpline
	./DrawSpline.exe

rng: rngtest
	./rngtest.exe


hello: $(HELLO_SOURCE:.cpp=.o) 

MySDL: $(KWR_SOURCE:.cpp=.o)

TestKwr: $(TEST_SOURCE:.cpp=.o) $(KWR_SOURCE:.cpp=.o)

NoiseTest: $(KWR_SOURCE:.cpp=.o)

SplineTest: $(KWR_SOURCE:.cpp=.o)

rngtest: $(KWR_SOURCE:.cpp=.o)

DrawSpline: $(KWR_SOURCE:.cpp=.o)

clean:
	rm -f *.exe *.o *.d

.PHONY: clean all run 

# Include the .d dependency files

include $(wildcard $(KWR_SOURCE:.cpp=.d))
include $(wildcard $(TEST_SOURCE:.cpp=.d))
