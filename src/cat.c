#include "cat.h"

void cat_move_up(world* world)
{
    --world->cat.y;
}

void cat_move_down(world* world)
{
    ++world->cat.y;
}

void cat_move_left(world* world)
{
    --world->cat.x;
}

void cat_move_right(world* world)
{
    ++world->cat.x;
}
