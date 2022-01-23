typedef enum
{
    HARDWALL, MOVEWALL, FLOOR
} tile;

typedef struct
{
    unsigned char x, y;
} point;

typedef struct dog_cons
{
    point crd;
    struct dog_cons* rest;
} dog_cons;

typedef struct
{
    unsigned char width, height;
    tile* map;
    point cat;
    dog_cons* dog_list;
} world;

tile* at(unsigned char x, unsigned char y, world* world);
world* create_world(unsigned char width, unsigned char height);
