#pragma once

#include <stdlib.h>

#include <assert.h>

#include <SDL2/SDL.h>

struct Vec2
{
	float x;
	float y;
};

struct Obj_Dimensions
{
	int spriteWidth;
	int spriteHeight;
	int imageWidth;
	int imageHeight;
};


typedef struct object* Object;

typedef struct animation* Animation;

Object create_object(char* name,char* animationStart,struct Vec2 pos,struct Obj_Dimensions dim,SDL_Renderer* renderer);

char* get_obj_name(Object object);

void destroy_object(Object object);

struct Vec2 get_position(Object object);

void set_position(Object object,struct Vec2 position);

void draw(Object object,SDL_Renderer* renderer);

void next_frame(Object object);

int isVisible(Object object);

char** find_spritesheets_in(char* directory);

int count_spritesheets_in(char* directory);

void change_animation(Object object,char* name);

int collision(Object A, Object B,int threshold);