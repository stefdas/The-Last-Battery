#include <stdio.h>

//////////////////////////////
#include <SDL2/SDL_image.h>
#include "gameCore.h"
#include "Level.h"
//////////////////////////////

static int running = 1;
static SDL_Window* window;
static SDL_Renderer* renderer;

struct clock
{
    uint32_t last_tick_time;
    uint32_t delta;
};

typedef struct clock* Clock;

void* Update(Level,uint32_t delta);
void tick(Clock clock);
void Render(Level,uint32_t delta);
void CleanUp(Level);

int main()
{
	window = create_window("The Last Battery",WINDOW_WIDTH,WINDOW_HEIGHT);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

	Clock clock = malloc(sizeof(struct clock));

	srand(time(NULL));

	uint32_t start = 0;
	uint32_t end = SDL_GetTicks();
	uint32_t delta = 0;

	Level level = generateLevel(renderer);

	play_soundtrack();

	while (running)
	{
		delta = end - start;

		Level newLevel = Update(level,delta);

		if (newLevel != NULL)
			level = newLevel;

		Render(level,delta);

		start = end;
		end = SDL_GetTicks();
	}

	CleanUp(level);	

	return 0;
}


void* Update(Level level,uint32_t delta)
{
	system("clear");
	SDL_RenderClear(renderer);

	float velocity_up = 20.0f;
	float velocity_side = 10.0f;
	float gravity = 0.5f;
	static int playerDir;

	Object* Scene = getLevelObjects(level);

	Object player = Scene[1];
	struct Vec2 player_pos = get_position(player);

	Object station = Scene[2];
	struct Vec2 station_pos = get_position(station);

	Object package = Scene[3];
	struct Vec2 package_pos = get_position(package);

	if (collision(package,player,96))
	{
		switch (playerDir)
		{
			case 1:
			{
				package_pos.x += 5.0f;
				set_position(package,package_pos);

				player_pos.x -= velocity_side;
				set_position(player,player_pos);

				break;
			}
			case -1:
			{
			 	package_pos.x -= 5.0f;
			 	set_position(package,package_pos);

				player_pos.x += velocity_side;
				set_position(player,player_pos);

				break;
			}
			case 2:
			{
				package_pos.y -= 5.0f;
			 	set_position(package,package_pos);

				player_pos.y += velocity_up;
				set_position(player,player_pos);

				break;
			}
		}
	}

	if (collision(package,station,64))
	{
		draw(Scene[5],renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
		destroyLevel(level);
		level = generateLevel(renderer);
		return level;
	}

	if (package_pos.y >= 700)
	{
		draw(Scene[4],renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
		set_position(player,(struct Vec2) {50,300});
		set_position(package,(struct Vec2) {220,260});
		return NULL;
	}


	for (int i=6; i<18; i++)
	{
		if (collision(package,Scene[i],64))
		{
			draw(Scene[4],renderer);
			SDL_RenderPresent(renderer);
			SDL_Delay(1000);
			set_position(player,(struct Vec2) {50,300});
			set_position(package,(struct Vec2) {220,260});
			return NULL;
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{		
			running = 0;
			return NULL;
		}
		else
		{
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_UP])
			{
				playerDir = 2;
				change_animation(player,"fly.png");
				if (player_pos.y > 50)
					player_pos.y -= velocity_up;
				set_position(player,player_pos);
			}
			else if (currentKeyStates[SDL_SCANCODE_DOWN])
			{
				playerDir = 0;
				change_animation(player,"fly.png");
				if (player_pos.y < 600)
					player_pos.y += velocity_up;
				set_position(player,player_pos);
			}
			else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				playerDir = 1;
				change_animation(player,"fly.png");
				if (player_pos.x <= WINDOW_WIDTH - 200)
					player_pos.x += velocity_side;
				set_position(player,player_pos);
			}
			else if (currentKeyStates[SDL_SCANCODE_LEFT])
			{
				playerDir = -1;
				change_animation(player,"fly.png");
				if (player_pos.x >= 20)
					player_pos.x -= velocity_side;
				set_position(player,player_pos);
			}
			else
			{
				change_animation(player,"idle.png");
			}
			
		}
		
	}
	

	package_pos.y += gravity;
	set_position(package,package_pos);

	gravity *= 3;

	return NULL;
}

void Render(Level level,uint32_t delta)
{
	Object* Scene = getLevelObjects(level);

	for (int i=0; i<4; i++)
	{
		draw(Scene[i],renderer);
		next_frame(Scene[i]);
	}

	for (int i=6; i<=17; i++)
	{
		draw(Scene[i],renderer);
		next_frame(Scene[i]);
	}

	SDL_RenderPresent(renderer);
	
	if (delta < 42)
		SDL_Delay(42-delta);
}

void CleanUp(Level level)
{
	destroyLevel(level);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void tick(Clock clock)
{
        uint32_t tick_time = SDL_GetTicks();
        clock->delta = tick_time - clock->last_tick_time;
        clock->last_tick_time = tick_time;
}
