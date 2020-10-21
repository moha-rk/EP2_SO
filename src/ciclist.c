#include <stdio.h>
#include <stdbool.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void create(int x, int y)
{
    ciclist_ptr c = (ciclist_ptr)malloc(sizeof(ciclist));
    running_ciclists++;

    c->id = running_ciclists;

    c->running = true;
    c->speed = LOW_SPEED;
    c->laps = 0;
    c->x_pos = x;
    c->y_pos = y;

    pista[x][y] = c->id;
    ciclistas[c->id] = c;
}

void destroy(ciclist_ptr c)
{
    pista[c->x_pos][c->y_pos] = 0;
    running_ciclists--;
}

void move_to(ciclist_ptr c, int x, int y)
{
    pista[c->x_pos][c->y_pos] = 0;

    pista[x][y] = c->id;
    c->x_pos = x;
    c->y_pos = y;
}