#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void show_pista();
void print_placar();

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
        //show_pista();
        current_time += time_interval;
        update_race();
        usleep(10*time_interval);
    }

    print_placar();


    fclose(output);
    return 0;
}


static void show_help(char *programName)
{
    fprintf(stderr, "%s: Uso \n"
                    "prompt> %s tamanho-do-velodromo ciclistas\n"
                    ,
            programName, programName);
    exit (EXIT_FAILURE);
}

void print_placar()
{
    for (int i = 1; i <= 2*ciclists_number; i++)
    {
        fprintf(stderr, "Volta %d:\n", i);
        for (int j = 0; j <= ciclists_number; j++)
        {
            fprintf(stderr, "%d ", placar[i][j]);
        }
        fprintf(stderr, "\n");
    }
}
