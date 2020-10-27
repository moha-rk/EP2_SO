#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void create(int x, int y)
{
    ciclist_ptr c = (ciclist_ptr)malloc(sizeof(ciclist));
    running_ciclists++;

    c->id = running_ciclists;

    c->running = true;
    c->speed = LOW_SPEED;
    c->laps = 0;
    c->x_pos = x;
    c->y_pos = y;
    if(pthread_create(&c->thread, NULL, run, c) != 0)
    {
        fprintf(stderr, "erro na criação do ciclista %d\n", c->id);
        exit (EXIT_FAILURE);
    }

    pista[x][y] = c->id;
    ciclistas[c->id] = c;
}

void destroy(ciclist_ptr c)
{
    pista[c->x_pos][c->y_pos] = 0;
    running_ciclists--;
}

void move_to(ciclist_ptr c, int x, int y)
{
    pista[c->x_pos][c->y_pos] = 0;

    pista[x][y] = c->id;
    c->x_pos = x;
    c->y_pos = y;
}

void *run(void *ciclist)
{
    ciclist_ptr c = (ciclist_ptr) ciclist;
    bool running = true;
    int sixthMeter = 0;

    while (running) {
        while (cont[c->id-1] == 0) usleep(10); //Quantia de sleep apenas para testes
        cont[c->id-1] = 0;
        /*
        codigo da tarefa i;
        */
        sixthMeter += c->speed*time_interval/600;
        if (sixthMeter == 6){
            //fprintf(stderr, "metro completo");
            /*Codigo para passar para a próxima faixa*/
            if (avanca_metro(c))
            {
                sixthMeter = 0;
                if (c->x_pos == 0){
                    c->laps++;
                    atualiza_velocidade(c);
                }
            }
        }
        /*
        arrive[i] = 1;
        while (continue[i] == 0) skip;
        continue[i] = 0;
        */
       arrive[c->id-1] = 1;
    }


}

bool avanca_metro(ciclist_ptr c)
{
    //Adicionar conferencia de volta completa
    //Adicionar mutex para proteger posições da pista
    if (pista[(c->x_pos+1)%velodromo_length][c->y_pos] == 0)
    {
        pista[c->x_pos][c->y_pos] = 0;
        pista[(c->x_pos+1)%velodromo_length][c->y_pos] = c->id;
        c->x_pos = (c->x_pos+1)%velodromo_length;
        return true;
    }
    else
    {
        int y_front;
        //Ultrapassagem vagabundinha
        if (espaco_lado(c->x_pos) && (y_front = espaco_frente(c->x_pos)) != -1)
        {
            pista[c->x_pos][c->y_pos] = 0;
            pista[(c->x_pos+1)%velodromo_length][y_front] = c->id;
            c->x_pos = (c->x_pos+1)%velodromo_length;
            return true;
        }
    }
    return false;
    
}

bool espaco_lado(int x)
{
    for (int i = 0; i < velodromo_length; i++)
    {
        //Mutex para conferir se tem alguém do lado
        if (pista[x][i] == 0) return true;
    }
    return false;
}

int espaco_frente(int x)
{
    int x_front = (x+1)%velodromo_length;
    for (int i = 0; i < velodromo_length; i++)
    {
        //Mutex para conferir se tem alguém do lado
        if (pista[x_front][i] == 0) return i;
    }
    return -1;
}

void atualiza_velocidade(ciclist_ptr c)
{
    if (c->speed == LOW_SPEED && rand()%100 < 80)
        c->speed = AVG_SPEED;

    else if (c->speed == AVG_SPEED && rand()%100 < 40)
        c->speed = LOW_SPEED;
}
