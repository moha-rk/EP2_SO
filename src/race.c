
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"
#include "../libs/simulation.h"

void clean_velodromo()
{
    for (int i = 0; i < MAX_LENGTH; i++)
        for (int j = 0; j < MAX_WIDTH; j++)
            pista[i]->linha[j] = NULL;
}

void start_race()
{
    current_time = 0;    //tempo atual, em milissegundos
    time_interval = 60;  //milissegundos
    velodromo_width = 5; //maximo de 5 ciclistas lado a lado no inicio
    running_ciclists = 0;

    clean_velodromo();

    int x_pos, y_pos; //posicao de largada, que serao sorteadas
    while (running_ciclists <= ciclists_number)
    {
        x_pos = rand() % (int)ceil(ciclists_number / velodromo_width);
        y_pos = rand() % velodromo_width;

        if (pista[x_pos]->linha[y_pos] == NULL)
            pista[x_pos]->linha[y_pos] = create();
    }

    velodromo_width = 10; //maximo de 10 ciclistas lado a lado depois do inicio
}

void update_race()
{
    ciclist_ptr c;
    int vel, dist;

    for (int i = velodromo_length - 1; i >= 0; i--)
    {
        for (int j = velodromo_width - 1; j >= 0; j--)
        {
            if (pista[i]->linha[j] == NULL)
                continue;

            c = pista[i]->linha[j];

            vel = rand() % 3; // 3 tipos de velocidade possiveis
            if (vel == 0)
                c->speed = LOW_SPEED;
            else if (vel == 1)
                c->speed = AVG_SPEED;
            else
                c->speed = HIGH_SPEED;

            dist = (c->speed) * time_interval;

            while (dist--)
            {
                if (pista[i + dist]->linha[j] == NULL)
                {
                    //mover ciclistaaaaaaaaaaaaa
                }
            }
        }
    }
}