
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void start_race()
{
    current_time = 0;    //tempo atual, em milissegundos
    time_interval = 60;  //milissegundos
    velodromo_width = 5; //maximo de 5 ciclistas lado a lado no inicio
    running_ciclists = 0;

    int x_pos, y_pos;                                         //posicao de largada, que sera sorteada
    int max_x = (int)ceil(ciclists_number / velodromo_width) + 1; //maximo x de largada
    int max_y = velodromo_width;                              // maximo y de largada

    while (running_ciclists < ciclists_number)
    {
        x_pos = rand() % max_x;
        y_pos = rand() % max_y;

        while (pista[x_pos][y_pos] != 0)
        {
            x_pos = (x_pos + 1) % max_x;
            y_pos = (y_pos + 1) % max_y;
        }

        create(x_pos, y_pos);
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
            if (pista[i][j] == 0)
                continue;

            c = ciclistas[pista[i][j]];

            vel = rand() % 3; // 3 tipos de velocidade possiveis
            if (vel == 0)
                c->speed = LOW_SPEED;
            else if (vel == 1)
                c->speed = AVG_SPEED;
            else
                c->speed = HIGH_SPEED;

            dist = (c->speed) * time_interval; //checar problemas de arredondamento (int = double*int)
        }
    }
}
