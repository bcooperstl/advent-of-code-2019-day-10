#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "asteroid_map.h"

void delete_map(asteroid_map * map)
{
    for (int i=0; i<map->rows; i++)
    {
        free(map->map[i]);
    }
    if (map->map != NULL)
    {
        free(map->map);
    }
}

void add_row(asteroid_map * map, int cols, char * row)
{
    map->cols = cols;
    char ** new_map = malloc(sizeof(char *)*map->rows+1);
    for (int i=0; i<map->rows; i++)
    {
        new_map[i]=map->map[i];
    }
    new_map[map->rows]=row;
    char ** old_map = map->map;
    map->rows++;
    map->map=new_map;
    if (old_map!=NULL)
    {
        free(old_map);
    }
}

asteroid_map * copy_map(asteroid_map * src)
{
    asteroid_map * new_map = malloc(sizeof(asteroid_map));
    new_map->rows = src->rows;
    new_map->cols = src->cols;
    new_map->map = malloc(sizeof(char *)*src->rows);
    for (int i=0; i<src->rows; i++)
    {
        char * new_row = malloc(sizeof(char)*src->cols);
        memcpy(new_row, src->map[i], sizeof(char)*src->cols);
        new_map->map[i]=new_row;
    }
    return new_map;
}

void print_map(asteroid_map * src)
{
    printf("Map has %d rows and %d cols\n", src->rows, src->cols);
    for (int i=0; i<src->rows; i++)
    {
        for (int j=0; j<src->cols; j++)
        {
            printf("%c", src->map[i][j]);
        }
        printf("\n");
    }
}

int is_asteroid(asteroid_map * map, int x, int y) // x is column, y is row
{
    return map->map[y][x]==ASTEROID_LOC;
}

int is_visible(asteroid_map * map, int x, int y) // x is column, y is row
{
    return map->map[y][x]==VISIBLE;
}

int is_blocked(asteroid_map * map, int x, int y) // x is column, y is row
{
    return map->map[y][x]==BLOCKED;
}

int is_source(asteroid_map * map, int x, int y) // x is column, y is row
{
    return map->map[y][x]==SOURCE;
}

void mapBlockedBySource(asteroid_map * map, int srcX, int srcY, int astX, int astY) // x is for column, y is for row
{
    printf("Mapping blocked asteroids for source %d,%d asteroid %d,%d\n", srcX, srcY, astX, astY);
    
    int deltaX=astX-srcX;
    int deltaY=astY-srcY;
    
    printf("  DeltaX=%d, DeltaY=%d\n", deltaX, deltaY);
    
    int nextX=astX+deltaX;
    int nextY=astY+deltaY;
    
    printf("  NextX=%d, NextY=%d\n", nextX, nextY);
    
    while ((nextX >= 0 && nextX < map->cols) && (nextY >=0 && nextY < map->rows))
    {
        if (is_asteroid(map,nextX,nextY) || is_visible(map,nextX,nextY))
        {
            printf("    Marking as BLOCKED\n");
            map->map[nextY][nextX]=BLOCKED;
        }
        
        nextX+=deltaX;
        nextY+=deltaY;
        printf("  NextX=%d, NextY=%d\n", nextX, nextY);
    }
}

void processMapWithSource(asteroid_map * map, int srcX, int srcY) // x is column, y is row
{
    map->map[srcY][srcX] = SOURCE;
    for (int i=0; i<map->rows; i++)
    {
        for (int j=0; j<map->cols; j++)
        {
            if (is_asteroid(map, j, i))
            {
                map->map[i][j]=VISIBLE;
                mapBlockedBySource(map, srcX, srcY, j, i);
            }
        }
    }
    print_map(map);
}

int count_visible(asteroid_map * map)
{
    int count=0;
    for (int i=0; i<map->rows; i++)
    {
        for (int j=0; j<map->cols; j++)
        {
            if (is_visible(map, j, i))
                count++;
        }
    }
    return count;
}

/*

*.... V.... B.... B.... B....
.*... .*... .V... .V... .V...
..S.. ..S.. ..S.. ..S.. ..S..
...*. ...*. ...*. ...V. ...V.
....* ....* ....* ....B ....B

*/