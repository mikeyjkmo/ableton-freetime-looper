CC=clang++
CFLAGS=-std=c++14 -stdlib=libc++ -Wall -D__MACOSX_CORE__ -c
LDFLAGS=-framework CoreMIDI -framework CoreAudio -framework CoreFoundation
SOURCES=$(wildcard *.cpp) $(wildcard **/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=LiveFreetimeLooper

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ -I .

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
