#include "Level.h"

#include "Object.h"

struct level 
{
    Object Scene[18];
};

Level generateLevel(SDL_Renderer* renderer)
{
    Level level = malloc(sizeof(struct level));
    Object background = create_object("background/","background.png",(struct Vec2) {0,0},(struct Obj_Dimensions) {1280,720,1280,720},renderer);
    Object player = create_object("droid/","idle.png",(struct Vec2) {50,300},(struct Obj_Dimensions) {128,128,1024,384} , renderer);
    Object station = create_object("station/","station.png",(struct Vec2) {1100,300},(struct Obj_Dimensions) {128,128,1024,384},renderer);
    Object package = create_object("package/","battery.png",(struct Vec2) {220,300},(struct Obj_Dimensions) {128,128,128,128},renderer);
    Object text_failure = create_object("text/","text_failure.png",(struct Vec2) {500,300},(struct Obj_Dimensions) {128,128,128,128},renderer);
	Object text_success = create_object("text/","text_success.png",(struct Vec2) {500,300},(struct Obj_Dimensions) {128,128,128,128},renderer);
    level->Scene[0] = background; level->Scene[1] = player;  level->Scene[2] = station; level->Scene[3] = package;
    level->Scene[4] = text_failure; level->Scene[5] = text_success;

    struct Vec2 Object_pos;

    int count = 6;
    for (int i=0; i<=5; i++)
	{
        for (int j=1; j<=2; j++)
        {
            int randomHeight = rand() % 501;
            while ( randomHeight > 220 && (randomHeight < 400) )
               randomHeight = rand() % 501;

            int randomDistance = 300 + rand() % 1000;
            
            Object_pos.x = randomDistance;
            Object_pos.y = randomHeight;

            level->Scene[count] = create_object("gear/","gear.png",Object_pos,(struct Obj_Dimensions) {128,128,1024,384},renderer);
            count++;
        }
	}

    return level;
}

void destroyLevel(Level level)
{
    for (int i=0; i<16; i++)
    {
        destroy_object(level->Scene[i]);
    }
    free(level);
}

Object* getLevelObjects(Level level)
{
    return level->Scene;
}