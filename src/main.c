#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "BearLibTerminal.h"
#include "game.h"
#include "world.h"
#include "cat.h"
#include "doggy.h"
#include "render_queue.h"

void render_event_queue(world* world);
void render_world(world* world);
void render_gui(game* game);
void flush_input_queue();

int main()
{
    terminal_open();
    terminal_set("window: title='Pussy''s Payback', size=46x25, cellsize=10x20;");
    
    terminal_set("0x1000: tileset.png, size=20x20, align=top-left, spacing=2x1");

    srand(time(NULL));

    game* game = new_game();
    construct_level(game);
    make_doggy(3, 3, game->world);
    render_world(game->world);
    terminal_refresh();
    int tk_state;
    while (1) {
	if (terminal_has_input()) {
	    tk_state = terminal_read();
	    flush_input_queue();
	    switch (tk_state) {
	    case TK_CLOSE:
		goto game_end;
		break;
	    case TK_UP:
		cat_move_up(game->world, game);
		break;
	    case TK_DOWN:
		cat_move_down(game->world, game);
		break;
	    case TK_LEFT:
		cat_move_left(game->world, game);
		break;
	    case TK_RIGHT:
		cat_move_right(game->world, game);
		break;
	    default:
		break;
	    }
	}
	
	render_gui(game);
	if (render_queue) {
	    render_event_queue(game->world);
	    terminal_refresh();
	}

	update_time(game);
    }

  game_end:
    terminal_close();
}

void flush_input_queue()
{
    while (terminal_has_input()) {
	terminal_read();
    }
}

void render_gui(game* game)
{
    terminal_printf(1, 0, "%4u", game->time);
    terminal_printf(5, 0, "/%-4u", game->next_doggy);
    terminal_printf(1, 1, "SCORE: %d", game->score);
}

void render_event_queue(world* world)
{
    if (!render_queue) return;
    
    int ox = render_queue->origin_x;
    int oy = render_queue->origin_y;
    int xd = render_queue->delta_x;
    int yd = render_queue->delta_y;
    pop_render_event();
    int xinc = sign(xd);
    int yinc = sign(yd);
    int x = ox;
    int y = oy;
    do {
	switch(tile_type(x, y, world)) {
	case CMOVEWALL:
	    terminal_composition(TK_ON);
	    terminal_put(2*x,2+y,0x1005);
	    terminal_put(2*x,2+y,0x1004);
	    break;
	case CFLOOR:
	    terminal_put(2*x,2+y,0x1005);
	    break;
	case CHARDWALL:
	    terminal_put(2*x,2+y,0x1003);
	    break;
	case CDOGGY:
	    terminal_composition(TK_ON);
	    terminal_put(2*x,2+y,0x1005);
	    terminal_put(x * 2, y + 2, 0x1001);
	    break;
	case CFOOD:
	    terminal_composition(TK_ON);
	    terminal_put(2*x,2+y,0x1005);
	    terminal_put(2*x,2+y,0x1002);
	    break;
	case CCAT:
	    terminal_composition(TK_ON);
	    terminal_put(2*x,2+y,0x1005);
	    terminal_put(x * 2, y + 2, 0x1000);
	    break;
	}
	x += xinc;
	y += yinc;
	terminal_composition(TK_OFF);
    } while (!(x == xd+ox+xinc && y == yd+oy+yinc));
    
    render_event_queue(world);
}

void render_world(world* world)
{
    for (int x = 0; x < world->width; ++x) {
	for (int y = 0; y < world->height; ++y) {
	    switch (*at(x,y,world)) {
	    case MOVEWALL:
		terminal_composition(TK_ON);
		terminal_put(2*x,2+y,0x1005);
		terminal_put(2*x,2+y,0x1004);
		break;
	    case FLOOR:
		terminal_put(2*x,2+y,0x1005);
		break;
	    case FOOD:
		terminal_composition(TK_ON);
		terminal_put(2*x,2+y,0x1005);
	        terminal_put(2*x,2+y,0x1002);
	        break;
	    case HARDWALL:
	        terminal_put(2*x,2+y,0x1003);
		break;
	    }
	}
    }

    terminal_composition(TK_ON); // Draw movers on top of floor
    terminal_put(world->cat.x * 2, world->cat.y + 2, 0x1000);
    
    for (dog_cons* doggy = world->dog_list; doggy; doggy = doggy->rest) {
	terminal_put(doggy->crd.x * 2, doggy->crd.y + 2, 0x1001);
    }

    terminal_composition(TK_OFF);
    
}
