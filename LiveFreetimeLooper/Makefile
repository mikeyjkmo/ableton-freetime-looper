CC=clang++
CFLAGS=-std=c++14 -stdlib=libc++ -Wall -D__MACOSX_CORE__ -c
LDFLAGS=-framework CoreMIDI -framework CoreAudio -framework CoreFoundation
LIBSOURCES=../LiveFreetimeLooper.FreetimeLooper/**/*.cpp

SOURCES=$(wildcard LiveFreetimeLooper.cpp) $(wildcard $(LIBSOURCES))
TESTSOURCES=$(wildcard ../LiveFreetimeLooper.Tests/LiveFreetimeLooper.Tests.cpp) \
            $(wildcard ../LiveFreetimeLooper.Tests/**/*.cpp) \
	    $(wildcard $(LIBSOURCES))

OBJECTS=$(SOURCES:.cpp=.o)
TESTOBJECTS=$(TESTSOURCES:.cpp=.o)

EXECUTABLE=LiveFreetimeLooper
TESTEXECUTABLE=LiveFreetimeLooperTests

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

tests: $(TESTSOURCES) $(TESTEXECUTABLE)

$(TESTEXECUTABLE): $(TESTOBJECTS)
	$(CC) $(LDFLAGS) $(TESTOBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ -I .

clean:
	rm -f $(EXECUTABLE) $(TESTEXECUTABLE) $(OBJECTS) $(TESTOBJECTS)
