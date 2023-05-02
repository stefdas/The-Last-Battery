#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Object.h"

#include "gameCore.h"

#include <string.h>

#include <dirent.h>

char* path = "../sprites/";


struct animation
{
	char* name;
	SDL_Texture* spriteSheet;
	SDL_Rect* attributes;
	short int textureWidth;
	short int textureHeight;
};

struct object
{
	char name[12];
	struct Vec2 pos;
	Animation* anim;
	Animation active_anim;
	int anim_count;
	SDL_Rect* screen;
};

	
Object create_object(char* name,char* animationStart,struct Vec2 pos,struct Obj_Dimensions dim,SDL_Renderer* renderer)
{
	Object new = malloc(sizeof(struct object));
	
	strcpy(new->name,name);

	char folder_path[40];
	strcpy(folder_path,path);			// Folder path of each asset. Any asset has a folder which contains spritesheets with animations.
	strcat(folder_path,new->name);

	new->anim_count = count_spritesheets_in(folder_path);

	new->anim = malloc(new->anim_count * sizeof(struct animation));

	new->active_anim = NULL;

	char** spritesheets = find_spritesheets_in(folder_path);

	for (int i=0; i<(new->anim_count); i++)
	{
		char current_spritesheet_path[50];
		strcpy(current_spritesheet_path,folder_path);
		strcat(current_spritesheet_path,spritesheets[i]);
		//printf("%s\n",current_spritesheet_path);

		new->anim[i] = malloc(sizeof(struct animation));
		new->anim[i]->name = malloc(sizeof(char) * 30);
		strcpy(new->anim[i]->name,"../sprites/");
		strcat(new->anim[i]->name,new->name);
		strcat(new->anim[i]->name,"/");
		strcat(new->anim[i]->name,spritesheets[i]);
		free(spritesheets[i]);

		SDL_Surface* surface = IMG_Load(new->anim[i]->name);
		new->anim[i]->spriteSheet = SDL_CreateTextureFromSurface(renderer,surface);
		SDL_FreeSurface(surface);

		new->anim[i]->attributes =  malloc(sizeof(struct SDL_Rect));
		new->anim[i]->attributes->w = dim.spriteWidth;
		new->anim[i]->attributes->h = dim.spriteHeight;
		new->anim[i]->attributes->x = 0;
		new->anim[i]->attributes->y = 0;

		new->anim[i]->textureWidth = dim.imageWidth;
		new->anim[i]->textureHeight = dim.imageHeight;

		if (strstr(new->anim[i]->name,animationStart) != NULL)
		{
			new->active_anim = new->anim[i];
		}
	}

	free(spritesheets);	
	
	new->screen = malloc(sizeof(struct SDL_Rect));
	
	new->screen->x = (int) pos.x;	new->screen->y = (int) pos.y;
	new->screen->w = dim.spriteWidth;	new->screen->h = dim.spriteHeight;

	new->pos.x = pos.x;
	new->pos.y = pos.y;

	return new;
}

void destroy_object(Object object)
{
	assert(object != NULL);
	//printf("Inside destroy for %s\n",object->name);
	int count = object->anim_count;
	for (int i=0; i<count; i++)
	{
		free(object->anim[i]->name);
		SDL_DestroyTexture(object->anim[i]->spriteSheet);
		free(object->anim[i]->attributes);
		free(object->anim[i]);
	}
	free(object->screen);
	free(object);
}

struct Vec2 get_position(Object object)
{
	assert(object != NULL);
	struct Vec2 vec = {object->pos.x,object->pos.y};
	return vec;
}

void set_position(Object object,struct Vec2 position)
{
	object->pos.x = position.x;
	object->pos.y = position.y;
}

void next_frame(Object object)
{	
	Animation active = object->active_anim;

	if (active->attributes->x == (active->textureWidth - active->attributes->w) && active->attributes->y == (active->textureHeight - active->attributes->h) )
	{
		active->attributes->x = 0;
		active->attributes->y = 0;
		return;
	}

	if (active->attributes->x == (active->textureWidth - active->attributes->w) )
	{
		active->attributes->x = 0;
		active->attributes->y += active->attributes->h;
		return;
	}

	active->attributes->x += active->attributes->w;

}

void change_animation(Object object,char* name)
{
	for (int i=0; i<object->anim_count; i++)
	{
		if (strstr(object->anim[i]->name,name) != NULL)
		{
			object->active_anim = object->anim[i];
		}
	}
}

void draw(Object object,SDL_Renderer* renderer)
{
	object->screen->x = (int) object->pos.x;
	object->screen->y = (int) object->pos.y;
	SDL_RenderCopy(renderer,object->active_anim->spriteSheet,object->active_anim->attributes,object->screen);
}

char** find_spritesheets_in(char* directory)
{
	struct dirent* de;
	DIR* dir = opendir(directory);
	if (dir == NULL)
	{
		printf("Unable to open directory\n");
		return NULL;
	}

	char** spritesheets = malloc(4 * sizeof(char*));

	int index = 0;
	while ((de = readdir(dir)) != NULL)
	{
		if (strcmp(de->d_name,".") == 0 || strcmp(de->d_name,"..") == 0)
			continue;
		spritesheets[index] = malloc(10 * sizeof(char));
		strcpy(spritesheets[index],de->d_name);
		index++;
	}

	closedir(dir);

	return  spritesheets;

}

int count_spritesheets_in(char* directory)
{
	struct dirent* de;
	DIR* dir = opendir(directory);
	if (dir == NULL)
	{
		printf("Unable to open directory\n");
		return -1;
	}

	int num_of_files = 0;
	while ((de = readdir(dir)) != NULL)
	{
		num_of_files++;
	}

	closedir(dir);

	return num_of_files - 2;
}


int collision(Object A, Object B,int threshold)
{
	struct Vec2 originA = { A->screen->x + 64 , A->screen->y + 64};
	struct Vec2 originB = { B->screen->x + 64 , B->screen->y + 64};

	int offsetX = originA.x - originB.x;
	(offsetX > 0) ? offsetX : (offsetX = -offsetX);

	int offsetY = originA.y - originB.y;
	(offsetY > 0) ? offsetY : (offsetY = -offsetY);
	
	return offsetX < threshold && offsetY < threshold;
}


char* get_obj_name(Object object)
{
	return object->name;
}