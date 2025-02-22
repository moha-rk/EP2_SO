#include "ciclist.h"
#include <pthread.h>
#include <stdbool.h>

#define MAX_LENGTH 12345
#define MAX_WIDTH 10

#ifndef RACE_H
#define RACE_H

int **pista; 
pthread_mutex_t **pistaMutex;

ciclist_ptr *ciclistas; //vetor para guardar ciclistas por ordem de id
pthread_mutex_t *mArrive;

int **placar; //Este vetor deve ter 2*n ciclistas por n ciclistas
pthread_mutex_t mutexPlacar;

int lapAtual, ultimaLap;
pthread_mutex_t mutexUL;

int time_interval, current_time;
int velodromo_length, velodromo_width;
int ciclists_number, running_ciclists;
int *ranking_final;
pthread_mutex_t nCiclistMutex;
int *arrive, *cont;

void start_race(); //funcao para criar os ciclistas e definir configs iniciais
void update_race(bool debug, FILE *out);
void show_pista(bool debug);
void atualiza_placar();
void FisherYates(int *v, int n);
bool atualiza_volta();
void verifica_perdedores();
void acelera_ultimas_voltas();
int acelerado;
bool fim_de_volta();
void output_volta(FILE *output);
void para_ciclistas();
void destroy_race();

int *indices;
void FisherYates(int *v, int n);

#endif