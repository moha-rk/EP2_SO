#ifndef PROCESS_H
#define PROCESS_H
#include <pthread.h>
#include <stdbool.h>

typedef struct ciclist_struct
{
    int id;
    pthread_t thread;
    struct ciclist_struct* next;
} ciclist;

typedef ciclist* ciclist_ptr;

ciclist_ptr create();

#endif