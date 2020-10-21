#ifndef CICLIST_H
#define CICLIST_H
#include <pthread.h>
#include <stdbool.h>

#define LOW_SPEED 1 / 120 //meters per milissecond == 30km/h
#define AVG_SPEED 1 / 60  // == 60km/h
#define HIGH_SPEED 1 / 40 // == 90km/h

typedef struct ciclist_struct
{
    int id;
    bool running;
    double speed;
    int x_pos, y_pos;
    int laps;
    pthread_t thread;
} ciclist;

typedef ciclist *ciclist_ptr;

void create(int, int);
void destroy(ciclist_ptr);
void move_to(ciclist_ptr, int, int);

#endif