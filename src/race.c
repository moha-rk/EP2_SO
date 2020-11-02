
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void start_race()
{
    int i, j;

    /*ALOCAÇÃO DE MEMÓRIA PARA TODAS AS LISTAS*/

    pista = (int **) malloc(velodromo_length*sizeof(int *));
    pistaMutex = (pthread_mutex_t **) malloc(velodromo_length*sizeof(pthread_mutex_t *));
    for (i = 0; i < velodromo_length; i++)
    {
        pista[i] = (int *) calloc(MAX_WIDTH, sizeof(int));
        pistaMutex[i] = (pthread_mutex_t *) malloc(MAX_WIDTH*sizeof(pthread_mutex_t));
    }

    ciclistas = (ciclist_ptr *) malloc((ciclists_number+1)*sizeof(ciclist_ptr));
    mArrive = (pthread_mutex_t *) malloc((ciclists_number+1)*sizeof(pthread_mutex_t));

    placar = (int **) malloc((2*ciclists_number + 1)*sizeof(int *));
    for (i = 0; i <= 2*ciclists_number; i++)
    {
        placar[i] = (int *) calloc(ciclists_number+1, sizeof(int));
        placar[i][0] = ciclists_number;
    }

    arrive = (int *) malloc((ciclists_number+1)*sizeof(int));
    cont = (int *) malloc((ciclists_number+1)*sizeof(int));


    /*INICIALIZAÇÃO DOS MUTEX*/

    for (i = 1; i <= ciclists_number; i++)
    {
        pthread_mutex_init(&mArrive[i], NULL);
    }

    pthread_mutex_init(&mutexPlacar, NULL);
    pthread_mutex_init(&mutexUL, NULL);
    pthread_mutex_init(&nCiclistMutex, NULL);
    for (i = 0; i < velodromo_length; i++)
    {
        for (j = 0; j < MAX_WIDTH; j++)
        {
            pthread_mutex_init(&pistaMutex[i][j], NULL);
        }
    }

    /*INICIALIZAÇÃO DAS VARIÁVEIS*/

    current_time = 0;    //tempo atual, em milissegundos
    time_interval = 60;  //milissegundos
    velodromo_width = 5; //maximo de 5 ciclistas lado a lado no inicio
    running_ciclists = 0;
    lapAtual = 1;
    ultimaLap = 2*(ciclists_number-1);
    acelerado = -1;

    int x_pos, y_pos;                                         //posicao de largada, que sera sorteada
    int max_x = (int)ceil(ciclists_number / velodromo_width) + 1; //maximo x de largada
    int max_y = velodromo_width;                              // maximo y de largada

    /*CRIAÇÃO E POSICIONAMENTO DOS CICLISTAS*/

    int linha_atual = 0;
    int n_ciclistas_linha = 0;
    while (running_ciclists < ciclists_number)
    {
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

    /*INICIALIZAÇÃO DA BARREIRA*/

    for (i = 1; i <= ciclists_number; i++)
    {
        cont[i] = 0;
        arrive[i] = 1;
    }
}

void update_race() //Update race deverá servir para coordenar o andamento dos ciclistas
{
    ciclist_ptr c;
    int vel, dist, i;

    for (i = 1; i <= ciclists_number; i++)
    {
        if (ciclistas[i] == NULL) continue;
        while (arrive[i] == 0) usleep(1);  //Quantia de sleep apenas para testes
    }
    //Aqui a execução está pausada, todos os ciclistas esperam pelo continue

    for (i = 1; i <= ciclists_number; i++)
        arrive[i] = 0; //Arrives serão zerados apenas quando a execução de todas as threads passar, pois seu valor é usado pelas outras threads


    atualiza_placar();
    verifica_perdedores();
    acelera_ultimas_voltas();
    para_ciclistas();

    show_pista();


    for (i = 1; i <= ciclists_number; i++) cont[i] = 1;
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


//Esta função confere ao fim de cada iteração se um ciclista acabou uma volta e então o coloca no placar daquela volta
void atualiza_placar()
{
    for (int i = 1; i <= ciclists_number; i++)
    {
        //Mudar aqui pois não está aleatorio, o ciclista de menor id chega primeiro
        if (ciclistas[i] == NULL) continue;
        if (!ciclistas[i]->finishedLap) continue;
        ciclistas[i]->finishedLap = false;

        int j = 1;

        while (placar[ciclistas[i]->laps][j] != 0) j++;
        
        placar[ciclistas[i]->laps][j] = ciclistas[i]->id;
    }


}

void verifica_perdedores()
{
    //Primeira posição dos vetores de placar corresponderá à quantia de ciclistas que competem naquela volta. 
    //Toda vez que um ciclista é eliminado, este número é subtraído por 1 nos vetores das voltas seguintes à
    //volta em que este ciclista estava. Caso tenha quebrado, o cilista não completou a volta e deve subtrair
    //da volta atual dele também.

    //A primeira posição de "placar[lapAtual]" guarda o número de ciclistas que estão participando dessa volta"
    int ultimo = placar[lapAtual][placar[lapAtual][0]];
    if (ultimo != 0) //Se verdadeiro, a lap acabou
    {
        if (lapAtual % 2 == 0) //Elimino o último
        {
            fprintf(stderr, "Ciclista %d eliminado\n", ciclistas[ultimo]->id);
            pthread_t tUltimo = ciclistas[ultimo]->thread;
            pthread_cancel(tUltimo); //Cancela a thread instantaneamente pois neste ponto ela está no usleep
            pthread_join(tUltimo, NULL);
            destroy(ciclistas[ultimo]);
            for (int i = lapAtual + 1; i <= 2*ciclists_number; i++) placar[i][0]--;
        }
        lapAtual++;
    }
}

void para_ciclistas()
{
    for (int i = 2*(ciclists_number-1); i >= ultimaLap; i--)
    {
        for (int j = 1; j <= ciclists_number; j++)
        {
            int idAtual = placar[i][j];
            //Se ninguem completou a ultima volta
            if (idAtual == 0) continue;
            //Se chegou aqui, alguém completou

            //Quando isso pode acontecer? Pq está acontecendo
            if (ciclistas[idAtual] == NULL || ciclistas[idAtual]->speed == 0) continue;

            //Devo parar o ciclista ou excluí-lo?  (vou excluí-lo para que não fique no caminho)
            ciclistas[idAtual]->speed = 0;
            running_ciclists--;
        }
    }
}

void acelera_ultimas_voltas()
{
    if (acelerado == 0) return;
    if (acelerado == 1)
    {
        if (placar[ultimaLap-2][2] != 0)
        {
            ciclistas[placar[ultimaLap-2][2]]->speed = HIGH_SPEED;
            acelerado = 0;
            time_interval = 20;
        }
        return;
    }
    
    if (placar[ultimaLap-2][1] != 0)
    {
        if (rand() % 100 < 10)
        {
            if (rand() % 100 < 50)
            {
                ciclistas[placar[ultimaLap-2][1]]->speed = HIGH_SPEED;
                acelerado = 0;
                time_interval = 20;
            }
            else acelerado = 1;
        }
        else acelerado = 0;
    }
}

void destroy_race()
{
    int i, j;

    for (i = 1; i <= ciclists_number; i ++)
    {
        if (ciclistas[i] != NULL)
        {
            pthread_t t = ciclistas[i]->thread;
            pthread_cancel(t); //Cancela a thread instantaneamente pois neste ponto ela está no usleep
            pthread_join(t, NULL);
            destroy(ciclistas[i]); //Destroi ciclistas remanescentes
        }
    }
    for (i = 0; i < velodromo_length; i++)
    {
        free(pista[i]);
        for (j = 0; j < MAX_WIDTH; j++)
            pthread_mutex_destroy(&pistaMutex[i][j]);
        free(pistaMutex[i]);
    }
    free(pistaMutex);
    free(pista);

    free(ciclistas);

    for (i = 1; i <= ciclists_number; i++)
    {
        pthread_mutex_destroy(&mArrive[i]);
    }

    free(mArrive);

    for (i = 0; i <= 2*ciclists_number; i++)
    {
        free(placar[i]);
    }
    free(placar);

    free(arrive);
    free(cont);

    pthread_mutex_destroy(&mutexPlacar);
    pthread_mutex_destroy(&mutexUL);
    pthread_mutex_destroy(&nCiclistMutex);
}
