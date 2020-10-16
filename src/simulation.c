#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sched.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"
#include "../libs/simulation.h"

bool ciclists_running();

int main(int argc, char **argv){

    if(argc != PARAMETERS){
        return EXIT_FAILURE;
    }

    velodromo_length = atoi(argv[1]);
    ciclists_number = atoi(argv[2]);
    current_time = 0; //tempo atual, em milissegundos

    FILE *output = fopen("relatorio.txt", "w");

    start_race(); //seta os ciclistas nas suas posicoes iniciais

    do{
        current_time+=time_interval;
    }while(ciclists_running());

    return EXIT_SUCCESS;
}

bool ciclists_running(){
    //percorremos a pista, atualizando os ciclistas, da frente pra tras
    for(int i=velodromo_length-1;i>=0;i--){
        for(int j=VELODROMO_WIDTH-1;j>=0;j--){
            update(ciclistaaaaa);
        }
    }
    
}