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
    bool running;
    double speed;
    int x_pos, y_pos;
    int laps;
    pthread_t thread;
    pthread_mutex_t mArrive;
} ciclist;

typedef ciclist *ciclist_ptr;

void create(int, int);
void destroy(ciclist_ptr);
void move_to(ciclist_ptr, int, int);
void *run(void *ciclist);
bool avanca_metro(ciclist_ptr c);
bool espaco_lado(int x, int y);
int espaco_frente(int x, int y);
void atualiza_velocidade(ciclist_ptr c);

#endif