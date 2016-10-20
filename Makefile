CXXFLAGS=-c -Wall -std=c++11 
LDFLAGS= -lpthread
CC = g++

SOURCEDIR = src

EXECUTABLE = Tracer
TEST = Tests

SOURCES=$(wildcard $(SOURCEDIR)/**/*.cpp $(SOURCEDIR)/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
#OBJECTS=$(patsubst $(SOURCEDIR)/%.cpp $(SOURCEDIR)/**/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

TEST_SRC=$(wildcard test/*.cpp)
TESTS_OBJ=$(TEST_SRC:.cpp=.o src/Math.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ 

.cpp.o:
	$(CC) $(CXXFLAGS) $< -o $@

test: $(TEST)

$(TEST): $(TESTS_OBJ)
	$(CC) $(LDFLAGS) $(TESTS_OBJ) -o $@

clean:
	rm Tracer Tests *.ppm src/*.o src/**/*.o test/*.o
