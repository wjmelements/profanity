CC=g++
CFLAGS=-c -std=c++11 -Wall -mmmx -O2
CDEFINES=
SOURCES=Dispatcher.cpp Mode.cpp precomp.cpp profanity.cpp SpeedSample.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=profanity.x64

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-framework OpenCL
else
	LDFLAGS=-s -lOpenCL
endif

all: $(SOURCES) $(EXECUTABLE)
inverse: chi.o rhopi.o
chitest: chi.o
rhopitest: rhopi.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CDEFINES) $< -o $@

clean:
	rm -rf *.o

