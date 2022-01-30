#include "cat.h"

void cat_move_up(world* world, game* game)
{
    move(0, -1, world, game);
}

void cat_move_down(world* world, game* game)
{
    move(0, 1, world, game);
}

void cat_move_left(world* world, game* game)
{
    move(-1, 0, world, game);
}

void cat_move_right(world* world, game* game)
{
    move(1, 0, world, game);
}

int move(int x_delta, int y_delta, world* world, game* game)
{
    y_delta = sign(y_delta);
    x_delta = sign(x_delta);
    point* cat = &world->cat;
    switch (*at(cat->x + x_delta, cat->y + y_delta, world)) {
    case FLOOR:
	cat->x += x_delta;
	cat->y += y_delta;
	goto move_success;
	break;
    case HARDWALL:
	goto move_fail;
	break;
    case FOOD:
	cat->x += x_delta;
	cat->y += y_delta;
	game->score += 1; // maybe set global
	goto move_success;
	break;
    case MOVEWALL:
	if (push(cat->x, cat->y, x_delta, y_delta, world)) {
	    cat->x += x_delta;
	    cat->y += y_delta;
	    *at(cat->x, cat->y, world) = FLOOR;
	    goto move_success;
	} else {
	    goto move_fail;
	}
    }

  move_success:
    return 1;
  move_fail:
    return 0;
}

int push(int origin_x, int origin_y, int x_delta, int y_delta, world* world)
{
    switch(*at(origin_x + x_delta, origin_y + y_delta, world)) {
    case MOVEWALL:
	if (push(origin_x + x_delta, origin_y + y_delta, x_delta, y_delta, world)) {
	    goto push_success;
	} else {
	    goto push_fail;
	}
    case HARDWALL:
	goto push_fail;
    default:
	*at(origin_x + x_delta, origin_y + y_delta, world) = MOVEWALL;
	goto push_success;
    }

  push_success:
    return 1;
  push_fail:
    return 0;
}
