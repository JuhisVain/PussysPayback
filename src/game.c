#include "game.h"

game* new_game()
{
    game* new_game = malloc(sizeof(game));
    new_game->score = 0;
    new_game->level = 1;
    new_game->time = 0;
    new_game->init_time = time(NULL);
    new_game->world = create_world(23, 23);
    return new_game;
}

void construct_level(game* game)
{
    unsigned char width = game->world->width;
    unsigned char height = game->world->height;
    switch (game->level) {
    case 1:
	for (int x = 4; x < width - 4; ++x) {
	    for (int y = 4; y < height - 4; ++y) {
		*at(x, y, game->world) = MOVEWALL;
	    }
	}
	*at(11,11,game->world) = FLOOR;
	break;
    default:
	break;
    }
}

void update_time(game* game)
{
    time_t new_time = time(NULL) - game->init_time;
    if (new_time > game->time) {
	game->time = new_time;
	run_dogs(game->world);
	if (game->time%30 == 0) {
	    make_doggy(3, 3, game->world); // TODO
	}
    }
}
