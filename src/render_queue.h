#ifndef RENDER_QUEUEH
#define RENDER_QUEUEH

#include <stdlib.h>
#include "world.h"

typedef struct render_event
{
    int origin_x, origin_y;
    int delta_x, delta_y;
    struct render_event* rest;
} render_event;

void push_render_event(int origin_x, int origin_y, int delta_x, int delta_y);
void pop_render_event();
extern render_event* render_queue;

#endif
