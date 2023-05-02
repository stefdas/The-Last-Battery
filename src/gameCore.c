#include <stdio.h>
#include <SDL2/SDL.h>

#include "gameCore.h"

static Mix_Music* music;

SDL_Window* create_window(const char* title,unsigned int width, unsigned int height)
{
	
	if (SDL_Init(SDL_INIT_VIDEO < 0) || SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Failed to initialize SDL2!\n");
		return NULL;
	}

	SDL_Window* window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED,width,height,0);

	if (!window)
	{
		printf("Failed to create window!\n");
		return NULL;
	}
	
	return window;
}

int update_window(SDL_Window* window)
{
	SDL_Surface* window_surface = SDL_GetWindowSurface(window);
	if (!window_surface)
	{
		printf("Failed to get the surface from the window\n");
		return 0;
	}
	SDL_UpdateWindowSurface(window);
	return 1;
}

void play_soundtrack()
{
	if (Mix_OpenAudio(44100,AUDIO_F32SYS,2,1024) != 0)
	{
		printf("Failed to init audio mixer!\n");
	}
	music = Mix_LoadMUS("../sound/soundtrack.wav");
	Mix_VolumeMusic(40);
	if (!music)
	{
		printf("Couldn't load music file!\n");
	}
	
	if (Mix_PlayMusic(music,-1) != 0)
	{
		printf("Failed to play music!\n");
	}
}

void close_audio(Uint8* wav_buffer)
{
	Mix_FreeMusic(music);
}