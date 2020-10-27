#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void show_pista();

int main(int argc, char **argv)
{
    if (argc != 3)
        return 0;
    velodromo_length = atoi(argv[1]);
    ciclists_number = atoi(argv[2]);

    FILE *output = fopen("relatorio.txt", "w");

    start_race(); //seta as condicoes iniciais e cria os ciclistas

    while (running_ciclists > 0)
    {
        current_time += time_interval;
        update_race();
        usleep(1000*time_interval);
        show_pista();
    }

    return 0;
}

void show_pista()
{
    for (int j = 0; j < velodromo_width-1; j++){
        for (int i = 0; i < velodromo_length; i++){
            printf("%d  ", pista[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}