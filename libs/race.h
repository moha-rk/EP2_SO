#include "ciclist.h"

#define MAX_LENGTH 12345
#define MAX_WIDTH 10

#ifndef RACE_H
#define RACE_H

typedef struct trecho_struct
{
    ciclist_ptr linha[MAX_WIDTH];
} trecho;

typedef trecho *trecho_ptr;

trecho_ptr pista[MAX_LENGTH]; //se necessario mudar para aloc dinamica

int time_interval, current_time;
int velodromo_length, velodromo_width;
int ciclists_number, running_ciclists;

void start_race(); //funcao para criar os ciclistas e definir configs iniciais
void update_race();

#endif