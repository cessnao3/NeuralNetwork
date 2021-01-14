CXX=g++
CXXFLAGS=-Wall -Werror -pedantic -Isrc/
CXXLIBS = $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_ttf-5 allegro_font-5)

SRCDIRS=car neural optim states tiles

SRC=$(foreach folder,$(SRCDIRS),$(wildcard src/$(folder)/*.cpp))
HDR=$(foreach folder,$(SRCDIRS),$(wildcard src/$(folder)/*.h))

MAIN=src/main.cpp
EXEC=neural.out

OBJS=$(patsubst %.cpp,%.o,$(SRC))

release: CXXFLAGS+=-O2
release: all

all: $(EXEC)

$(EXEC): $(OBJS) $(MAIN)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(MAIN) $(CXXLIBS)

%.o: %.cpp $(HDR)
	$(CXX) -o $@ $(CXXFLAGS) -c $< $(CXXLIBS)

clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean
