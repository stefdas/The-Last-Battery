BUILD_DIR = ./bin/
SRC_DIR = ./src/
SDL_FLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lSDL2_mixer
INCLUDE_DIR = ./include/

run: main.o gameCore.o Object.o level.o
	gcc -o $(BUILD_DIR)run $(BUILD_DIR)main.o $(BUILD_DIR)gameCore.o $(BUILD_DIR)Object.o $(BUILD_DIR)level.o $(SDL_FLAGS)
main.o:
	gcc -c $(SRC_DIR)main.c -o $(BUILD_DIR)main.o -I$(INCLUDE_DIR)

gameCore.o:
	gcc -c $(SRC_DIR)gameCore.c -o $(BUILD_DIR)gameCore.o -I$(INCLUDE_DIR)

Object.o:
	gcc -c $(SRC_DIR)Object.c -o $(BUILD_DIR)Object.o -I$(INCLUDE_DIR)

level.o:
	gcc -c $(SRC_DIR)Level.c -o $(BUILD_DIR)level.o -I$(INCLUDE_DIR)