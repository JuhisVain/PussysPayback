#include "doggy.h"

int doggy_logic(dog_cons* doggy, world* world);

void run_dogs(world* world)
{
    for (dog_cons* doggy = world->dog_list; doggy; doggy = doggy->rest) {
	doggy_logic(doggy, world);
    }
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

int doggy_logic(dog_cons* doggy, world* world)
{
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
	    doggy->crd.x = target_x;
	    doggy->crd.y = target_y;
	    crd_type tt = tile_type(target_x,target_y,world);
	    if (tt == CCAT) {
		printf("Cat cought!\n");
	    } else if (tt == CFOOD) {
		*at(target_x, target_y, world) = FLOOR; // as in original game
	    }
	    return 1;
	}
    }
    return 0;
}

int force_doggy_move(dog_cons* doggy, world* world)
{
    int x = doggy->crd.x;
    int y = doggy->crd.y;
    // call doggy_logic here
}
