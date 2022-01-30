#ifndef CATH
#define CATH

#include "world.h"
#include "game.h"
#include "util.h"

void cat_move_up(world* world, game* game);
void cat_move_down(world* world, game* game);
void cat_move_left(world* world, game* game);
void cat_move_right(world* world, game* game);

int move(int x_delta, int y_delta, world* world, game* game);
int push(int origin_x, int origin_y, int x_delta, int y_delta, world* world);

#endif
