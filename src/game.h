#ifndef GAMEH
#define GAMEH

#include <stdlib.h>
#include "world.h"

typedef struct
{
    int score;
    int level;
    unsigned int time;
    world* world;
} game;

game* new_game();
void construct_level(game* game);

#endif
