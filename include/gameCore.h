#include <stdio.h>
#include <SDL2/SDL.h>
#include "Object.h"
#include "SDL2/SDL_mixer.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

SDL_Window* create_window(const char* title,unsigned int width, unsigned int height);

int update_window(SDL_Window* window);

void play_soundtrack();


