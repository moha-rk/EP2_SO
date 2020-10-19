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
    int laps;
    pthread_t thread;
} ciclist;

typedef ciclist *ciclist_ptr;

ciclist_ptr create();
void destroy(ciclist_ptr);
void update(ciclist_ptr);

#endif