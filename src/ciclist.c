#include <stdio.h>
#include <stdbool.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

ciclist_ptr create()
{
    ciclist_ptr created = (ciclist_ptr)malloc(sizeof(ciclist));
    created->id = running_ciclists++;
    created->running = true;
    created->speed = LOW_SPEED;
    created->laps = 0;
    return created;
}

void destroy(ciclist_ptr c)
{
    running_ciclists--;
}

void update(ciclist_ptr c)
{
}