#ifndef CICLIST_H
#define CICLIST_H
#include <pthread.h>
#include <stdbool.h>

#define LOW_SPEED 30 //meters per milissecond == 30km/h
#define AVG_SPEED 60  // == 60km/h
#define HIGH_SPEED 90 // == 90km/h

typedef struct ciclist_struct
{
    int id;
    int rank;
    bool running;
    bool quebrou;
    int speed;
    int x_pos, y_pos;
    int laps;
    int time_running;
    bool finishedLap;
    pthread_t thread;
} ciclist;

typedef ciclist *ciclist_ptr;

void create(int, int);
void elimina(ciclist_ptr);
void move_to(ciclist_ptr, int, int);

void *run(void *ciclist);
void anda_para_esquerda(ciclist_ptr c);

bool avanca_metro(ciclist_ptr c);
bool espaco_lado(int x, int y);
int espaco_frente(int x, int y);

void atualiza_velocidade(ciclist_ptr c);
bool quebrou(ciclist_ptr c);

#endif