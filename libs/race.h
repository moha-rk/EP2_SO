#include "ciclist.h"
#include <pthread.h>

#define MAX_LENGTH 12345
#define MAX_WIDTH 10

#ifndef RACE_H
#define RACE_H

int **pista;  //se necessario mudar para aloc dinamica
ciclist_ptr *ciclistas; //vetor para guardar ciclistas por ordem de id

int **placar; //Este vetor deve ter 2*n ciclistas por n ciclistas
pthread_mutex_t mutexPlacar;
int lapAtual;

int time_interval, current_time;
int velodromo_length, velodromo_width;
int ciclists_number, running_ciclists;
pthread_mutex_t nCiclistMutex;
int *arrive, *cont; //se necessario mudar para aloc dinamica
pthread_mutex_t **pistaMutex;

void start_race(); //funcao para criar os ciclistas e definir configs iniciais
void update_race();
void atualiza_placar();
void verifica_perdedores();

#endif