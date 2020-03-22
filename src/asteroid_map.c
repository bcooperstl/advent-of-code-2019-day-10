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

int gcd(int a, int b)
{
    int temp;
    while (b != 0)
    {
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}

void reduceDeltas(int * deltaX, int * deltaY)
{
    int xSign=1;
    int xValue=*deltaX;
    int ySign=1;
    int yValue=*deltaY;
    
    if (*deltaX < 0)
    {
        xSign=-1;
        xValue=-1*xValue;
    }
    if (*deltaY < 0)
    {
        ySign=-1;
        yValue=-1*yValue;
    }
    
    // special cases
    if (xValue==0 && yValue==0)
    {
        // do nothing
    }
    else if (xValue==0)
    {
        *deltaY=ySign;
    }
    else if (yValue==0)
    {
        *deltaX=xSign;
    }
    else if (xValue==yValue)
    {
        *deltaX=xSign;
        *deltaY=ySign;
    }
    else
    {
        int gcdValue=gcd(xValue, yValue);
        *deltaX=xSign*(xValue/gcdValue);
        *deltaY=ySign*(yValue/gcdValue);
    }
}

void mapBlockedBySource(asteroid_map * map, int srcX, int srcY, int astX, int astY) // x is for column, y is for row
{
    //printf("Mapping blocked asteroids for source %d,%d asteroid %d,%d\n", srcX, srcY, astX, astY);
    
    int deltaX=astX-srcX;
    int deltaY=astY-srcY;
    
    //printf("  DeltaX=%d, DeltaY=%d\n", deltaX, deltaY);
    reduceDeltas(&deltaX, &deltaY);
    //printf("  After reduction, DeltaX=%d, DeltaY=%d\n", deltaX, deltaY);
    
    int nextX=astX+deltaX;
    int nextY=astY+deltaY;
    
    //printf("  NextX=%d, NextY=%d\n", nextX, nextY);
    
    while ((nextX >= 0 && nextX < map->cols) && (nextY >=0 && nextY < map->rows))
    {
        if (is_asteroid(map,nextX,nextY) || is_visible(map,nextX,nextY))
        {
            //printf("    Marking as BLOCKED\n");
            map->map[nextY][nextX]=BLOCKED;
        }
        
        nextX+=deltaX;
        nextY+=deltaY;
        //printf("  NextX=%d, NextY=%d\n", nextX, nextY);
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

void resetToAsteroids(asteroid_map * map, int srcX, int srcY) // x is column, y is row
{
    map->map[srcY][srcX] = SOURCE;
    for (int i=0; i<map->rows; i++)
    {
        for (int j=0; j<map->cols; j++)
        {
            if (is_blocked(map, j, i))
            {
                map->map[i][j]=ASTEROID_LOC;
            }
        }
    }
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

int count_asteroids(asteroid_map * map)
{
    int count=0;
    for (int i=0; i<map->rows; i++)
    {
        for (int j=0; j<map->cols; j++)
        {
            if (is_asteroid(map, j, i))
                count++;
        }
    }
    return count;
}

int getLocation(int srcX, int srcY, int x, int y)
{
    if (srcY==y) // on a horizaontal line
    {
        if(x>srcX) // point is right of source
            return DIRECTION_E;
        else // point is left of source
            return DIRECTION_W;
    }
    if (srcX==x) // on a vertical line
    {
        if (y>srcY) // point is below source
            return DIRECTION_S;
        else // point is above source
            return DIRECTION_N;
    }
    if (y<srcY)
    {
        // above source; either NW or NE
        if (x < srcX)
        {
            // to the left of source; NW
            return DIRECTION_NW;
        }
        else
        {
            // to the right of source; NE
            return DIRECTION_NE;
        }
    }
    else
    {
        // below source; either SW or SE
        if (x < srcX)
        {
            // to the left of source; SW
            return DIRECTION_SW;
        }
        else
        {
            // to the right of source; SE
            return DIRECTION_SE;
        }
    }
}

// returns -1 for x1,y1 should be reached first or 1 for x2,y2 should be reached first
int compare(int srcX, int srcY, int x1, int y1, int x2, int y2)
{
    int loc1 = getLocation(srcX, srcY, x1, y1);
    int loc2 = getLocation(srcX, srcY, x2, y2);
    
    if (loc1 < loc2)
    {
        return -1;
    }
    else if (loc2 < loc1)
    {
        return 1;
    }
    
    // flip the y's because the coordinate systen is backwards for y-values
    double slope1=((double)(srcY-y1))/((double)(x1-srcX));
    double slope2=((double)(srcY-y2))/((double)(x2-srcX));
    
    switch (loc1)
    {
        case DIRECTION_NE:
            // both slopes are positive. Larger slope (2 vs 1/2) is reached first
            return slope1>slope2 ? -1 : 1;
        case DIRECTION_SE:
            // both slopes are negative. Larger slope (-1/2 vs -2) is reached first
            return slope1>slope2 ? -1 : 1;
        case DIRECTION_SW:
            // both slopes are positive. Larger slope (2 vs 1/2) is reached first
            return slope1>slope2 ? -1 : 1;
        case DIRECTION_NW:
            // both slopes are negative. Larger slope (-1/2 vs -2) is reached first
            return slope1>slope2 ? -1 : 1;
    }
    fprintf(stderr, "***Messed something up...src %d,%d, two points %d,%d and %d,%d did not match a case\n", srcX, srcY, x1, y1, x2, y2);
    return 0;
}

void find_next_visible(asteroid_map * map, int srcX, int srcY, int * nextX, int * nextY)
{
    int first=1;
    
    for (int i=0; i<map->rows; i++)
    {
        for (int j=0; j<map->cols; j++)
        {
            if (is_visible(map, j, i))
            {
                if (first==1)
                {
                    *nextX=j;
                    *nextY=i;
                    first=0;
                }
                else
                {
                    if(compare(srcX, srcY, *nextX, *nextY, j, i) == 1)
                    {
                        // this asteroid will be found before the current best one
                        *nextX=j;
                        *nextY=i;
                    }
                }
            }
        }
    }    
}



/*

*.... V.... B.... B.... B....
.*... .*... .V... .V... .V...
..S.. ..S.. ..S.. ..S.. ..S..
...*. ...*. ...*. ...V. ...V.
....* ....* ....* ....B ....B

*/