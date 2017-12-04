
# Files

KWR_SOURCE = kwrsdl.cpp kwrgame.cpp kwrlib.cpp
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

all: test run

run: MySDL
	./MySDL.exe

test: TestKwr
	./TestKwr.exe

noise: NoiseTest
	./NoiseTest.exe

spline: DrawSpline
	./DrawSpline.exe

MySDL: $(KWR_SOURCE:.cpp=.o)

TestKwr: $(TEST_SOURCE:.cpp=.o) $(KWR_SOURCE:.cpp=.o)

NoiseTest: $(KWR_SOURCE:.cpp=.o)

SplineTest: $(KWR_SOURCE:.cpp=.o)

DrawSpline: $(KWR_SOURCE:.cpp=.o)

clean:
	rm -f *.exe *.o *.d

.PHONY: clean all run test

# Include the .d dependency files

include $(wildcard $(SRCS:.c=.d))

