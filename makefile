
CC          = g++
CFLAGS      = -s -c -O2 -Wall -pedantic -std=gnu++11
LDFLAGS     = -lsfml-system -lsfml-window -lsfml-graphics -lsqlite3pp -lsqlite3 -ltinyxml -ltinyxpath
SOURCES     = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp) $(wildcard src/*/*/*/*/*.cpp)
OBJECTS     = $(SOURCES:.cpp=.o)
EXECUTABLE  = atcradar.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o bin/$(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
clean:
	del /s *.o *.out
