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

void generate_doggy(world* world)
{
    int cx = world->cat.x;
    int cy = world->cat.y;

    int x,y;
    for (; x = 1+rand()%20, y = 1+rand()%20,
	     !((*at(x,y,world)==FLOOR) && (distance(x,y,cx,cy)>= 8.0)););
    printf("Making doggy at %d %d, distance to %d %d cat: %f\n", x, y, cx,cy, distance(x,y,cx,cy));
    make_doggy(x, y, world);
}

void update_time(game* game)
{
    time_t new_time = time(NULL) - game->init_time;
    if (new_time > game->time) {
	game->time = new_time;
	if (!run_dogs(game->world)) { // all dogs trapped?
	    for (dog_cons* doggy = game->world->dog_list; doggy;) {
		*at(doggy->crd.x, doggy->crd.y, game->world) = FOOD;
		push_render_event(doggy->crd.x, doggy->crd.y, 0, 0);
		dog_cons* next_doggy = doggy->rest;
		free(doggy);
		doggy = next_doggy;
	    }
	    game->world->dog_list = NULL;
	    generate_doggy(game->world);
	}
	if (game->time%15 == 0) {
	    generate_doggy(game->world);
	}
    }
}
