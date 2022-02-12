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
    int ox = cat->x;
    int oy = cat->y;
    int push_total = 0; // The amount of movement
    switch (tile_type(cat->x + x_delta, cat->y + y_delta, world)) {
    case CFLOOR:
	push_total = 1;
	cat->x += x_delta;
	cat->y += y_delta;
	goto move_success;
	break;
    case CHARDWALL:
	goto move_fail;
	break;
    case CFOOD:
	push_total = 1;
	cat->x += x_delta;
	cat->y += y_delta;
	game->score += 1;
	*at(cat->x, cat->y, world)=FLOOR;
	goto move_success;
	break;
    case CMOVEWALL:
	// Don't actually need to render all but EH!
	push_total = push(cat->x, cat->y, x_delta, y_delta, game);
	if (push_total) {
	    cat->x += x_delta;
	    cat->y += y_delta;
	    *at(cat->x, cat->y, world) = FLOOR;
	    goto move_success;
	} else {
	    goto move_fail;
	}
    case CDOGGY:
	kill_cat(game);
	goto move_fail;
    case CCAT:
	printf("Cat collides with cat error\n");
    }

  move_success:
    push_render_event(ox, oy, x_delta*push_total, y_delta*push_total);
    return 1;
  move_fail:
    return 0;
}

int push(int origin_x, int origin_y, int x_delta, int y_delta, game* game)
{
    int total_push = 0;
    world* world = game->world;
    switch(tile_type(origin_x + x_delta, origin_y + y_delta, world)) {
    case CMOVEWALL:
	total_push = push(origin_x + x_delta, origin_y + y_delta, x_delta, y_delta, game);
	if (total_push) {
	    return total_push + 1;
	} else {
	    goto push_fail;
	}
    case CHARDWALL:
	goto push_fail;
    case CFLOOR:
	*at(origin_x + x_delta, origin_y + y_delta, world) = MOVEWALL;
	goto push_success;
    case CDOGGY:
	if (force_doggy_move(is_doggy(origin_x+x_delta, origin_y+y_delta, world), game)) {
	    *at(origin_x + x_delta, origin_y + y_delta, world) = MOVEWALL;
	    goto push_success;
	} else {
	    goto push_fail;
	}
    case CFOOD:
	*at(origin_x + x_delta, origin_y + y_delta, world) = MOVEWALL;
	goto push_success;
    case CCAT:
	printf("Cats pushing cats ERROR!\n");
	goto push_fail;
    }

  push_success:
    return 1;
  push_fail:
    return 0;
}
