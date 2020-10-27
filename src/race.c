
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    int i;
    for (i = 0; i < MAX_LENGTH-1; i++)
    {
        cont[i] = 0;
        arrive[i] = 1;
    }

    int linha_atual = 0;
    int n_ciclistas_linha = 0;
    while (running_ciclists < ciclists_number)
    {

        /* Por enquanto fica comentado por falta de proteção da pista, esta causando erros
        x_pos = rand() % max_x;
        y_pos = rand() % max_y;

        while (pista[x_pos][y_pos] != 0)
        {
            x_pos = (x_pos + 1) % max_x;
            y_pos = (y_pos + 1) % max_y;
        }
        */
        x_pos = linha_atual;
        y_pos = n_ciclistas_linha;

        create(x_pos, y_pos);
        n_ciclistas_linha++;
        if (n_ciclistas_linha == 5) {
            n_ciclistas_linha = 0;
            linha_atual--;
            if (linha_atual < 0) linha_atual = velodromo_length-1;
        }
    }

    velodromo_width = 10; //maximo de 10 ciclistas lado a lado depois do inicio
}

void update_race() //Update race deverá servir para coordenar o andamento dos ciclistas
{
    ciclist_ptr c;
    int vel, dist, i;

        for (i = 0; i < running_ciclists; i++)
        {
            while (arrive[i] == 0) usleep(1);  //Quantia de sleep apenas para testes
            arrive[i] = 0;
        }
        for (i = 0; i < running_ciclists; i++) cont[i] = 1;

    /*

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
        
    }*/

    /*Thread Coordinator {
        while (true) {
            for [i = 1 to n] {
                while (arrive[i] == 0) skip;
                arrive[i] = 0;
            }
            for [i = 1 to n] continue[i] = 1;
        }
    }*/
}
