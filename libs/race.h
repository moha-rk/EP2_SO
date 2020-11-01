#include "ciclist.h"
#include <pthread.h>

#define MAX_LENGTH 12345
#define MAX_WIDTH 10

#ifndef RACE_H
#define RACE_H

int pista[MAX_LENGTH][MAX_WIDTH];  //se necessario mudar para aloc dinamica
ciclist_ptr ciclistas[MAX_LENGTH]; //vetor para guardar ciclistas por ordem de id

int placar[MAX_LENGTH*2][MAX_LENGTH]; //Este vetor deve ter 2*n ciclistas por n ciclistas
int lapAtual;

int time_interval, current_time;
int velodromo_length, velodromo_width;
int ciclists_number, running_ciclists;
pthread_mutex_t nCiclistMutex;
int arrive[MAX_LENGTH], cont[MAX_LENGTH]; //se necessario mudar para aloc dinamica
pthread_mutex_t pistaMutex[MAX_LENGTH][MAX_WIDTH];

void start_race(); //funcao para criar os ciclistas e definir configs iniciais
void update_race();
void atualiza_placar();
void verifica_perdedores();

#endif