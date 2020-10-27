#include "ciclist.h"

#define MAX_LENGTH 12345
#define MAX_WIDTH 10

#ifndef RACE_H
#define RACE_H

int pista[MAX_LENGTH][MAX_WIDTH];  //se necessario mudar para aloc dinamica
ciclist_ptr ciclistas[MAX_LENGTH]; //vetor para guardar ciclistas por ordem de id

int time_interval, current_time;
int velodromo_length, velodromo_width;
int ciclists_number, running_ciclists;
int arrive[MAX_LENGTH], cont[MAX_LENGTH]; //se necessario mudar para aloc dinamica

void start_race(); //funcao para criar os ciclistas e definir configs iniciais
void update_race();

#endif