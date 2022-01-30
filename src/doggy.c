#include "doggy.h"

void doggy_logic(dog_cons* doggy, world* world);

void run_dogs(world* world)
{
    for (dog_cons* doggy = world->dog_list; doggy; doggy = doggy->rest) {
	doggy_logic(doggy, world);
    }
}

void doggy_logic(dog_cons* doggy, world* world)
{
    int dog_x = doggy->crd.x;
    int dog_y = doggy->crd.y;

    int cat_x = world->cat.x;
    int cat_y = world->cat.y;

    int delta_x = sign(cat_x - dog_x);
    int delta_y = sign(cat_y - dog_y);

    int target_x, target_y;
    //reroute:
    target_x = dog_x + delta_x;
    target_y = dog_y + delta_y;
    
    switch (*at(target_x, target_y, world)) { // TODO: need a walkable predicate func
    case FLOOR:
	doggy->crd.x = target_x;
	doggy->crd.y = target_y;
	return;
	break;
    default: // TODO: can doggies walk on food?
	//BORING
	//goto reroute;
	break;
    }
}
