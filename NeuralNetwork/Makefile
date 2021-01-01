CXX=g++
CXXFLAGS=-Wall -Werror -pedantic -Ihdr/
CXXLIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_ttf-5 allegro_font-5)

SRC=$(wildcard src/car/*.cpp) $(wildcard src/neural/*.cpp) $(wildcard src/optim/*.cpp) $(wildcard src/states/*.cpp) $(wildcard src/tiles/*.cpp)
HDRS=$(wildcard/hdr/car/*.h) $(wildcard hdr/neural/*.h) $(wildcard hdr/optim/*.h) $(wildcard hdr/states/*.h) $(wildcard hdr/tiles/*.h)

MAIN=src/main.cpp
EXEC=neural.out

OBJS=$(patsubst %.cpp,%.o,$(SRC))

release: CXXFLAGS+=-O2
release: all

all: $(EXEC)

$(EXEC): $(OBJS) $(MAIN)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(MAIN) $(CXXLIBS)

%.o: %.cpp $(HDRS)
	$(CXX) -o $@ $(CXXFLAGS) -c $< $(CXXLIBS)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
