#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void escreve_placar(FILE *output);

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

    escreve_placar(output);

    destroy_race();

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

void escreve_placar(FILE *output)
{
    int posAtual = 1, idAtual;
    fprintf(output, "\nRanqueamento Final:\n");
    for (int i = 1; i <= ciclists_number; i++)
    {
        if (ranking_final[i] == 0) continue;
        idAtual = ranking_final[i];
        fprintf(output, "%d: Ciclista %d - %.2f segundos\n", posAtual, idAtual, (float)ciclistas[idAtual]->time_running/1000);

        posAtual++;
    }
    fprintf(output, "\n");

    for (int i = 1; i <= ciclists_number; i++)
    {
        if (ciclistas[i]->quebrou) fprintf(output, "Ciclista %d quebrou na volta %d - %.2f segundos\n", i, ciclistas[i]->laps, (float)ciclistas[i]->time_running/1000);
    }
   
}
