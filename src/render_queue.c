#include "render_queue.h"

render_event* render_queue = NULL;

void push_render_event(int origin_x, int origin_y, int delta_x, int delta_y)
{
    render_event* event = malloc(sizeof(render_event));
    event->origin_x = origin_x;
    event->origin_y = origin_y;
    event->delta_x = delta_x;
    event->delta_y = delta_y;
    event->rest = render_queue;
    render_queue = event;
}

void pop_render_event()
{
    render_event* rest = render_queue->rest;
    free(render_queue);
    render_queue = rest;
}
