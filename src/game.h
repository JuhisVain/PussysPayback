#ifndef GAMEH
#define GAMEH

#include <stdlib.h>
#include <time.h>
#include "world.h"
#include "render_queue.h"
#include "util.h"

typedef struct
{
    int score;
    int level;
    int lives;
    unsigned int doggy_interval; // time between doggy spawnings when untrapped
    unsigned int doggy_wait; // time before spawn after trap
    unsigned int next_doggy; // scheduled next spawn
    unsigned int init_time;
    unsigned int time;
    world* world;
} game;

game* new_game();
void construct_level(game* game);
void update_time(game* game);
void kill_cat(game* game);

int run_dogs(game* game);
int force_doggy_move(dog_cons* doggy, game* game);


#endif
