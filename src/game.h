#ifndef GAMEH
#define GAMEH

#include <stdlib.h>
#include <time.h>
#include "world.h"
#include "doggy.h"
#include "render_queue.h"
#include "util.h"

typedef struct
{
    int score;
    int level;
    unsigned int init_time;
    unsigned int time;
    world* world;
} game;

game* new_game();
void construct_level(game* game);
void update_time(game* game);

#endif
