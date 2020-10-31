
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
    pthread_mutex_init(&nCiclistMutex, NULL);

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
        if (linha_atual != 0)
            ciclistas[running_ciclists]->laps = -1;
        n_ciclistas_linha++;
        if (n_ciclistas_linha == 5) {
            n_ciclistas_linha = 0;
            linha_atual--;
            if (linha_atual < 0) linha_atual = velodromo_length-1;
        }
    }

    velodromo_width = 10; //maximo de 10 ciclistas lado a lado depois do inicio

    for (i = 0; i < velodromo_length; i++)
    {
        for (int j = 0; j < velodromo_width; j++)
        {
            pthread_mutex_init(&pistaMutex[i][j], NULL);
        }
    }

    for (i = 0; i < MAX_LENGTH*2; i++)
    {
        for (int j = 0; j < MAX_LENGTH; j++)
        {
            placar[i][j] = 0;
        }
    }
}

void update_race() //Update race deverá servir para coordenar o andamento dos ciclistas
{
    ciclist_ptr c;
    int vel, dist, i;

    for (i = 0; i < ciclists_number; i++)
    {
        if (ciclistas[i+1] == NULL) continue;
        while (arrive[i] == 0) usleep(1);  //Quantia de sleep apenas para testes
        arrive[i] = 0;
    }
    //Aqui a execução está pausada, todos os ciclistas esperam pelo continue
    //O ideal seria contabilizar voltas aqui, e eliminar quem deve ser eliminado
    //Claro, também sortear a nova velocidade na ultima volta


    atualiza_placar();
    verifica_perdedores();

    for (i = 0; i < ciclists_number; i++) cont[i] = 1;
}

//Esta função confere ao fim de cada iteração se um ciclista acabou uma volta e então o coloca no placar daquela volta
void atualiza_placar()
{
    for (int i = 0; i <= ciclists_number; i++)
    {
        if (ciclistas[i] == NULL) continue;
        if (!ciclistas[i]->finishedLap) continue;
        ciclistas[i]->finishedLap = false;

        int j = 0;
        while (placar[ciclistas[i]->laps][j] != 0) j++;
        placar[ciclistas[i]->laps][j] = ciclistas[i]->id;
    }

}

void verifica_perdedores()
{

}
