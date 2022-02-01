#include <stdlib.h>
#include "world.h"

tile* at(unsigned char x, unsigned char y, world* world)
{
    return &world->map[x + (y * world->width)];
}

dog_cons* is_doggy(int x, int y, world* world)
{
    for (dog_cons* doggy = world->dog_list; doggy; doggy = doggy->rest) {
	if (doggy->crd.x==x&&doggy->crd.y==y) {
	    return doggy;
	}
    }
    return NULL;
}

world* create_world(unsigned char width, unsigned char height)
{
    world* new_world = malloc(sizeof(world));
    new_world->width = width;
    new_world->height = height;

    tile* world_map = malloc(width * height * sizeof(tile*));
    new_world->map = world_map;

    new_world->cat = (point){.x=width/2, .y=height/2};
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

int make_doggy(unsigned char x, unsigned char y, world* world)
{
    if (*at(x, y, world) == FLOOR) { // also check dogginness/cattiness/foodiness
	dog_cons* new_doggy = malloc(sizeof(dog_cons));
	new_doggy->rest = world->dog_list;
	new_doggy->crd.x = x;
	new_doggy->crd.y = y;
	world->dog_list = new_doggy;
	return 1;
    } else {
	return 0; //Fail
    }
}

int kill_doggy(unsigned char x, unsigned char y, dog_cons** dog_list)
{
    point crd = (*dog_list)->crd; // This should be called in response to dog trap
    if (crd.x == x && crd.y == y) {
	printf("Freeing %p\n", *dog_list);
	free(*dog_list); //?
	*dog_list = (*dog_list)->rest;
	return 1;
    } else if ((*dog_list)->rest) {
	return kill_doggy(x, y, &(*dog_list)->rest);
    } else {
	return 0;
    }
}
