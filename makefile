CC          = g++
CFLAGS      = -s -c -O2 -Wall -Wextra -pedantic -std=gnu++11
LDFLAGS     = -lsfml-system -lsfml-window -lsfml-graphics -lsqlite3pp -lsqlite3 -lsqlua
SOURCES     = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*/*/*.cpp) 
OBJECTS     = $(SOURCES:.cpp=.o)
OBJ			= obj/
OBJ_FILES	= $(OBJ)*.o
EXECUTABLE  = atcradar.exe

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o bin/$(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $(OBJ)$(notdir $@)
clean:
	del /s *.o $(EXECUTABLE)