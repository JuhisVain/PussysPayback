#include <stdlib.h>
#include "world.h"

tile* at(unsigned char x, unsigned char y, world* world)
{
    return &world->map[x + (y * world->width)];
}

world* create_world(unsigned char width, unsigned char height)
{
    world* new_world = malloc(sizeof(world));
    new_world->width = width;
    new_world->height = height;

    tile* world_map = malloc(width * height * sizeof(tile*));
    new_world->map = world_map;

    new_world->cat = (point){.x=1, .y=1};
    new_world->dog_list = NULL;

    for (int x = 0; x < width; ++x) {
	    tile* toptile = at(x,0,new_world);
	    *toptile = HARDWALL;
	     tile* bottile = at(x,height-1,new_world);
	    *bottile = HARDWALL;
	}

    for (int y = 0; y < height; ++y) {
	    tile* lefttile = at(0,y,new_world);
	    *lefttile = HARDWALL;
	    tile* righttile = at(width-1,y,new_world);
	    *righttile = HARDWALL;
	}

    for (int x = 1; x < width-1; ++x) {
	for (int y = 1; y < height-1; ++y) {
	    tile* tile = at(x,y,new_world);
	    *tile = FLOOR;
	}
    }
    return new_world;
}
