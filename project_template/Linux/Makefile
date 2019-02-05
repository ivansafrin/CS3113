
SDL_HEADER_DIR=/usr/include/SDL2

CFLAGS=-I$(SDL_HEADER_DIR)
LDFLAGS=-lc -lm -pthread -lSDL2 -lGLEW -lGL

default:
	g++ main.cpp $(CFLAGS) $(LDFLAGS) -o mygame

clean:
	rm mygame
