#include <stdlib.h>
#include "BearLibTerminal.h"
#include "world.h"

void render_world(world* world);

int main()
{
    terminal_open();
    terminal_set("window: title='Pussy''s Payback', size=46x25, cellsize=10x20;");
    
    terminal_set("0x1000: tileset.png, size=20x20, align=top-left");

    world* world = create_world(23, 23);
    render_world(world);
    
    terminal_refresh();
    while (terminal_read() != TK_CLOSE);
    
    terminal_close();
}

void render_world(world* world)
{
    for (int x = 0; x < world->width; ++x) {
	for (int y = 0; y < world->height; ++y) {
	    printf("x:%d y:%d type:%d\n", x, y, *at(x,y,world));
	    switch (*at(x,y,world)) {
	    case HARDWALL:
		terminal_put(2*x,2+y,0x1003);
		break;
	    case FLOOR:
		terminal_put(2*x,2+y,0x1005);
		break;
	    case MOVEWALL:
		terminal_put(2*x,2+y,0x1004);
		break;
	    }
	}
    }
}
