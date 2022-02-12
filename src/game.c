#include "game.h"

game* new_game()
{
    game* new_game = malloc(sizeof(game));
    new_game->score = 0;
    new_game->level = 1;
    new_game->lives = 3;
    new_game->doggy_interval = 15;
    new_game->doggy_wait = 5;
    new_game->time = 0;
    new_game->init_time = time(NULL);
    new_game->next_doggy = new_game->time + new_game->doggy_interval;
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
    make_doggy(x, y, world);
}

void update_time(game* game)
{
    time_t new_time = time(NULL) - game->init_time;
    if (new_time > game->time) {
	game->time = new_time;
	if (!run_dogs(game)) { // all dogs trapped?
	    for (dog_cons* doggy = game->world->dog_list; doggy;) {
		*at(doggy->crd.x, doggy->crd.y, game->world) = FOOD;
		push_render_event(doggy->crd.x, doggy->crd.y, 0, 0);
		dog_cons* next_doggy = doggy->rest;
		free(doggy);
		doggy = next_doggy;
	    }
	    game->world->dog_list = NULL;
	    game->next_doggy = game->time + game->doggy_wait;
	}
	if (game->time == game->next_doggy) {
	    generate_doggy(game->world);
	    game->next_doggy = game->time + game->doggy_interval;
	}
    }
}

void kill_cat(game* game)
{
    if (--game->lives >= 0) {
	// Generate location to respawn:
	int x,y;
	double dist = 100.0;
	while (1) {
	    x = 1+rand()%20;
	    y = 1+rand()%20;
	    if (tile_type(x,y,game->world)!=CFLOOR) continue;
	    // Find smallest distance
	    for (dog_cons* c = game->world->dog_list; c != NULL; c = c->rest) {
		double cdist = distance(x,y,c->crd.x,c->crd.y);
		if (cdist < dist) {
		    dist = cdist;
		}
	    }
	    if (dist >= 5.0) break;
	}

	push_render_event(game->world->cat.x, game->world->cat.y, 0, 0);
	push_render_event(x, y, 0, 0);
	game->world->cat.x = x;
	game->world->cat.y = y;
    } else {
	printf("Game over\n");
    }
}

int doggy_logic(dog_cons* doggy, game* game);

int run_dogs(game* game)
{
    world* world = game->world;
    if (!world->dog_list) return 1;
    
    int dogs_awake = 0;
    for (dog_cons* doggy = world->dog_list; doggy; doggy = doggy->rest) {
	if (doggy_logic(doggy, game))
	    dogs_awake = 1;
    }
    return dogs_awake;
}

int doggy_walkable(int x, int y, world* world)
{
    crd_type tt = tile_type(x,y,world);
    if (tt==CFLOOR||tt==CFOOD||tt==CCAT)
	return 1;
    return 0;
}

//Move tables for doggy logic, 18 elements each in pairs
int up_mt[] = {0,1, 1,1, -1,1, 1,0, -1,0, 1,-1, -1,-1, 0,-1, 0,0};
int upri_mt[] = {1,1, 1,0, 0,1, 1,-1, -1,1, 0,-1, -1,0, -1,-1, 0,0};
int uple_mt[] = {-1,1, 0,1, -1,0, 1,1, -1,-1, 1,0, 0,-1, 1,-1, 0,0};
int ri_mt[] = {1,0, 1,-1, 1,1, 0,-1, 0,1, -1,-1, -1,1, -1,0, 0,0};
int le_mt[] = {-1,0, -1,-1, -1,1, 0,-1, 0,1, 1,-1, 1,1, 1,0, 0,0};
int dori_mt[] = {1,-1, 0,-1, 1,0, -1,-1, 1,1, -1,0, 0,1, -1,1, 0,0};
int dole_mt[] = {-1,-1, -1,0, 0,-1, -1,1, 1,-1, 0,1, 1,0, 1,1, 0,0};
int do_mt[] = {0,-1, -1,-1, 1,-1, -1,0, 1,0, -1,1, 1,1, 0,1, 0,0};

int doggy_logic(dog_cons* doggy, game* game)
{
    world* world = game->world;
    
    int dog_x = doggy->crd.x;
    int dog_y = doggy->crd.y;

    int cat_x = world->cat.x;
    int cat_y = world->cat.y;

    int delta_x = sign(cat_x - dog_x);
    int delta_y = sign(cat_y - dog_y);

    int target_x, target_y;

    int* move_table;
    if (delta_x==0) {
	if (delta_y==-1) move_table = do_mt;
	else /*if (delta_y==1)*/ move_table = up_mt;
    } else if (delta_x==-1) {
	if (delta_y==-1) move_table = dole_mt;
	else if (delta_y==0) move_table = le_mt;
	else /*if (delta_y==1)*/ move_table = uple_mt;
    } else /*if (delta_x==1)*/ {
	if (delta_y==-1) move_table = dori_mt;
	else if (delta_y==0) move_table = ri_mt;
	else /*if (delta_y==1)*/ move_table = upri_mt;
    }

    /*
    if (move_table == &up_mt) printf("UP\n");
    if (move_table == &do_mt) printf("DO\n");
    if (move_table == &dori_mt) printf("DOWN RIGHT\n");
    if (move_table == &dole_mt) printf("DOWN LEFT\n");
    if (move_table == &upri_mt) printf("UP RIGHT\n");
    if (move_table == &uple_mt) printf("UP LEFT\n");
    if (move_table == &le_mt) printf("LEFT\n");
    if (move_table == &ri_mt) printf("RIGHT\n");
    */

    for (int i = 0; delta_x=move_table[i], delta_y=move_table[i+1], i<18; i+=2) {
	/*printf("%d -- %d\n", delta_x, delta_y);*/
	target_x = dog_x + delta_x;
	target_y = dog_y + delta_y;
    
	if (doggy_walkable(target_x, target_y, world)) {
	    push_render_event(dog_x, dog_y, delta_x, delta_y);
	    doggy->crd.x = target_x;
	    doggy->crd.y = target_y;
	    crd_type tt = tile_type(target_x,target_y,world);
	    if (tt == CCAT) {
		printf("Cat cought!\n");
		kill_cat(game);
	    } else if (tt == CFOOD) {
		*at(target_x, target_y, world) = FLOOR; // as in original game
	    }
	    return 1;
	}
    }
    return 0;
}

int force_doggy_move(dog_cons* doggy, game* game)
{
    return doggy_logic(doggy, game);
}
