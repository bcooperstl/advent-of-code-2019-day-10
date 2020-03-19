#ifndef __ASTEROID_MAP_H__
#define __ASTEROID_MAP_H__

#define EMPTY_LOC '.'
#define ASTEROID_LOC '#'
#define VISIBLE 'V'
#define BLOCKED 'B'
#define SOURCE 'S'

struct asteroid_map {
    int rows;
    int cols;
    char ** map;
};

typedef struct asteroid_map asteroid_map;

asteroid_map * copy_map(asteroid_map * orig); 
void delete_map(asteroid_map * map);
void add_row(asteroid_map * map, int cols, char * row);
void print_map(asteroid_map * map);
int count_visible(asteroid_map * map);

void mapBlockedBySource(asteroid_map * map, int srcX, int srcY, int astX, int astY);
void processMapWithSource(asteroid_map * map, int srcX, int srcY);

int is_asteroid(asteroid_map * map, int x, int y);
int is_visible(asteroid_map * map, int x, int y);
int is_blocked(asteroid_map * map, int x, int y);
int is_source(asteroid_map * map, int x, int y);

    

#endif
