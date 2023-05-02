#include <SDL2/SDL_image.h>

#include "Object.h"

#include <stdlib.h>
#include <time.h>

typedef struct level* Level;

Level generateLevel(SDL_Renderer*);

Object* getLevelObjects(Level level);

void destroyLevel(Level level);