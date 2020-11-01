#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void show_pista();

static void show_help(char *programName);

int main(int argc, char **argv)
{
    if (argc != 3)
        show_help(argv[0]);
    velodromo_length = atoi(argv[1]);
    ciclists_number = atoi(argv[2]);

    FILE *output = fopen("relatorio.txt", "w");

    start_race(); //seta as condicoes iniciais e cria os ciclistas

    while (running_ciclists > 0)
    {
        show_pista();
        current_time += time_interval;
        update_race();
        usleep(1000*time_interval);
    }

    return 0;
}

void show_pista()
{
    for (int j = 0; j < velodromo_width; j++){
        for (int i = 0; i < velodromo_length; i++){
            fprintf(stderr, "%d  ", pista[i][j]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n\n");
}


static void show_help(char *programName)
{
    fprintf(stderr, "%s: Uso \n"
                    "prompt> %s tamanho-do-velodromo ciclistas\n"
                    ,
            programName, programName);
    exit (EXIT_FAILURE);
}