#include <stdio.h>
#include <string.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"
#include "../libs/simulation.h"

int main(int argc, char **argv)
{

    if (argc != 2)
        return 0;

    velodromo_length = atoi(argv[1]);
    ciclists_number = atoi(argv[2]);

    FILE *output = fopen("relatorio.txt", "w");

    start_race(); //seta as condicoes iniciais

    while (running_ciclists > 0)
    {
        current_time += time_interval;
        update_race();
    }

    return 0;
}
