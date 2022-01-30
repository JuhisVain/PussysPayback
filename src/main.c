#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "BearLibTerminal.h"
#include "game.h"
#include "world.h"
#include "cat.h"

void render_world(world* world);
void render_gui(game* game);
void flush_input_queue();

int main()
{
    terminal_open();
    terminal_set("window: title='Pussy''s Payback', size=46x25, cellsize=10x20;");
    
    terminal_set("0x1000: tileset.png, size=20x20, align=top-left, spacing=2x1");

    game* game = new_game();
    construct_level(game);
    render_world(game->world);
    terminal_refresh();
    while (1) {
        int tk_state;
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
	render_world(game->world);
	}
	
	render_gui(game);
	terminal_refresh();

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
    terminal_printf(1, 0, "%d", game->time);
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
