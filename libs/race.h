#ifndef RACE_H
#define RACE_H

#include <pthread.h>
#include <stdbool.h>

#define VELODROMO_WIDTH 10 //maximo de 10 ciclistas lado a lado
#define MAX_VELODROMO_LENGTH 1e5

typedef struct trecho_struct
{
    int linha[10];
} trecho;

typedef trecho *trecho_ptr;

int velodromo_length;
int ciclists_number;
//ou criar pista estaticamente
trecho_ptr pista[MAX_VELODROMO_LENGTH];
//ou alocar pista dinamicamente
void create_pista();

//funcao para criar os ciclistas e definir suas posicoes iniciais
void start_race();

//funcao para contar o numero de ciclistas correndo
int count_ciclists();


#endif  