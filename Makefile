CC := g++
CFLAGS := -Werror -Wpedantic -Wall -std=c++17
SOURCES := picture.cpp
HEADERS := picture.h
OBJECTS := $(SOURCES:.cpp=.o)
EXECUTABLE := picture
all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) *.o