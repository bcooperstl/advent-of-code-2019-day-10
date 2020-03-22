#ifndef __ASTEROID_MAP_H__
#define __ASTEROID_MAP_H__

#define EMPTY_LOC '.'
#define ASTEROID_LOC '#'
#define VISIBLE 'V'
#define BLOCKED 'B'
#define SOURCE 'S'
#define VAPORIZED 'X'

#define DIRECTION_N 1
#define DIRECTION_NE 2
#define DIRECTION_E 3
#define DIRECTION_SE 4
#define DIRECTION_S 5
#define DIRECTION_SW 6
#define DIRECTION_W 7
#define DIRECTION_NW 8

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
int count_asteroids(asteroid_map * map);

void reduceDeltas(int * deltaX, int * deltaY);

void mapBlockedBySource(asteroid_map * map, int srcX, int srcY, int astX, int astY);
void processMapWithSource(asteroid_map * map, int srcX, int srcY);
void resetToAsteroids(asteroid_map * map, int srcX, int srcY);

int is_asteroid(asteroid_map * map, int x, int y);
int is_visible(asteroid_map * map, int x, int y);
int is_blocked(asteroid_map * map, int x, int y);
int is_source(asteroid_map * map, int x, int y);

int gcd(int a, int b);

int getLocation(int srcX, int srcY, int x, int y);
int compare(int srcX, int srcY, int x1, int y1, int x2, int y2);

void find_next_visible(asteroid_map * map, int srcX, int srcY, int * nextX, int * nextY);

#endif
