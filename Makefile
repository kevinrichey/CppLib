#
# Project files
#
EXE = MySDL
SRC = MySDL.cpp kwrsdl.cpp kwrgame.cpp

TESTEXE = test
TESTSRC = test.cpp TestCase.cpp testkwrlib.cpp

OBJS = $(addprefix obj/, $(SRC:.cpp=.o))
TESTOBJS = $(addprefix obj/, $(TESTSRC:.cpp=.o))

#
# Directories
#
OBJDIR = obj
EXEDIR = bin
DEBUGDIR = debug
DEPDIR = depends
TESTDIR = test

#
# Compiler
#
CXX = g++
INCLUDE_PATHS = -IC:\msys64\mingw64\include\SDL2
LIBRARY_PATHS = -LC:\msys64\mingw64\lib
# -Wl,-subsystem,windows gets rid of the console window
CXXFLAGS = -Wl,-subsystem,windows -g -DDEBUG
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#
# Build target rules
#

all: exe_target

run: all
	cd $(EXEDIR); ./$(EXE)

test: test_target
	cd $(EXEDIR); ./$(TESTEXE)

exe_target: $(OBJS)
	$(CXX) -o $(EXEDIR)/$(EXE) $^ $(LIBRARY_PATHS) $(CXXFLAGS) $(LINKER_FLAGS)

test_target: $(OBJS) $(TESTOBJS)
	$(CXX) -o $(EXEDIR)/$(TESTEXE) $^ $(LIBRARY_PATHS) $(CXXFLAGS) $(LINKER_FLAGS)

$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) $(DEPFLAGS) $(INCLUDE_PATHS) $(CXXFLAGS) -c -o $@ $<
	$(POSTCOMPILE)

clean:
	rm -f $(OBJS) $(TESTOBJS) $(EXEDIR)/$(EXE) $(EXEDIR)/$(TESTEXE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

