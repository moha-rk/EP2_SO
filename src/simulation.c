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
bool debug = false;

int main(int argc, char **argv)
{
    if (argc < 3)
        show_help(argv[0]);
    velodromo_length = atoi(argv[1]);
    ciclists_number = atoi(argv[2]);
    if (argc == 4 && strcmp(argv[3], "d") == 0) debug = true;

    FILE *output = fopen("relatorio.txt", "w");
    if (output == NULL)
    {
        fprintf(stderr, "O arquivo de output não pode ser aberto\n");
        exit (EXIT_FAILURE);
    }

    start_race(); //seta as condicoes iniciais e cria os ciclistas

    while (running_ciclists > 0)
    {
        update_race(debug, output);
        //usleep(100*time_interval);
        usleep(1);
    }

    print_placar();

    destroy_race();

    //fprintf(output, "a");

    fclose(output);
    return 0;
}


static void show_help(char *programName)
{
    fprintf(stderr, "%s: Uso \n"
                    "prompt> %s tamanho-do-velodromo ciclistas [d]\n"
                    "[d] = argumento opcional para habilitar debug\n"
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
