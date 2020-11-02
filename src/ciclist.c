#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libs/race.h"
#include "../libs/ciclist.h"

void create(int x, int y)
{
    ciclist_ptr c = (ciclist_ptr)malloc(sizeof(ciclist));
    running_ciclists++;

    c->id = running_ciclists;

    c->running = true;
    c->speed = LOW_SPEED;
    c->laps = 0;
    c->finishedLap = false;
    c->x_pos = x;
    c->y_pos = y;
    if(pthread_create(&c->thread, NULL, run, c) != 0)
    {
        fprintf(stderr, "erro na criação do ciclista %d\n", c->id);
        exit (EXIT_FAILURE);
    }

    pista[x][y] = c->id;
    ciclistas[c->id] = c;
}

void destroy(ciclist_ptr c)
{
    pthread_mutex_lock(&pistaMutex[c->x_pos][c->y_pos]);
    pista[c->x_pos][c->y_pos] = 0;
    pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);

    pthread_mutex_lock(&nCiclistMutex);
    running_ciclists--;
    pthread_mutex_unlock(&nCiclistMutex);

    ciclistas[c->id] = NULL;
    free(c);
}

void move_to(ciclist_ptr c, int x, int y)
{
    pista[c->x_pos][c->y_pos] = 0;

    pista[x][y] = c->id;
    c->x_pos = x;
    c->y_pos = y;
}

void *run(void *ciclist)
{
    ciclist_ptr c = (ciclist_ptr) ciclist;
    bool running = true;
    int sixthMeter = 0;

    while (running) {
        while (cont[c->id] == 0) usleep(10); //Quantia de sleep apenas para testes
        cont[c->id] = 0;
        
        sixthMeter += c->speed*time_interval/600;
        if (sixthMeter >= 6){
            /*Codigo para passar para a próxima faixa*/
            if (avanca_metro(c))
            {
                sixthMeter = 0;
                if (c->x_pos == 0){
                    c->laps++;
                    if (c->laps > 0) 
                    {
                        if (quebrou(c))
                        {
                            pthread_mutex_lock(&mutexPlacar);
                            for (int i = c->laps; i <= 2*ciclists_number; i++) placar[i][0]--;
                            pthread_mutex_unlock(&mutexPlacar);
                            pthread_mutex_lock(&mutexUL);
                            ultimaLap -= 2; //A quebra de um ciclista faz com que a ultima volta aconteça mais cedo
                            pthread_mutex_unlock(&mutexUL);

                            pthread_mutex_lock(&mArrive[c->id]);
                            arrive[c->id] = 1;
                            pthread_mutex_unlock(&mArrive[c->id]);

                            destroy(c);

                            pthread_exit(NULL);
                        }
                        c->finishedLap = true;
                        atualiza_velocidade(c);
                    }
                }
            }
        }

        //Tenta mover o ciclista o mais a esquerda o possivel
        anda_para_esquerda(c);

        //Posso usar o próprio arrive para saber se determinado ciclista já andou. Só preciso proteger esse arive com um mutex
        pthread_mutex_lock(&mArrive[c->id]);
        arrive[c->id] = 1;
        pthread_mutex_unlock(&mArrive[c->id]);

    }
}

void anda_para_esquerda(ciclist_ptr c)
{
    int idAtual, y_ant;
    y_ant = c->y_pos;
    pthread_mutex_lock(&pistaMutex[c->x_pos][y_ant]);
    for (int i = 0; i < c->y_pos; i++)
    {
        if (pthread_mutex_trylock(&pistaMutex[c->x_pos][i]) == 0)
        {    
            idAtual = pista[c->x_pos][i];
            if (idAtual == 0)
            {
                move_to(c, c->x_pos, i);
                pthread_mutex_unlock(&pistaMutex[c->x_pos][y_ant]);
                pthread_mutex_unlock(&pistaMutex[c->x_pos][i]);
                return;
            }
            pthread_mutex_unlock(&pistaMutex[c->x_pos][i]);
        }
    }
    pthread_mutex_unlock(&pistaMutex[c->x_pos][y_ant]);
}

bool avanca_metro(ciclist_ptr c)
{
    /*    
    Primeiro travo o meu mutex, depois o da frente (posso pensar em usar o trylock)*/
    int x_front = (c->x_pos+1)%velodromo_length;
    
    pthread_mutex_lock(&pistaMutex[c->x_pos][c->y_pos]);
    pthread_mutex_lock(&pistaMutex[x_front][c->y_pos]);
    int idAFrente = pista[x_front][c->y_pos];
    if (idAFrente == 0)
    {
        int xAnt = c->x_pos;
        move_to(c, x_front, c->y_pos);
        pthread_mutex_unlock(&pistaMutex[xAnt][c->y_pos]);
        pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);
        //fprintf(stderr, "id 0");

        return true;
    }
    else
    {
        //Caso ciclista quebre e ainda esteja na posição 0 nessa iteração (nao deveria)
        if (ciclistas[idAFrente] == NULL)
        {
            int xAnt = c->x_pos;
            move_to(c, x_front, c->y_pos);
            pthread_mutex_unlock(&pistaMutex[xAnt][c->y_pos]);
            pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);
            return true;
        }
        pthread_mutex_lock(&mArrive[idAFrente]);
        int andou = arrive[idAFrente];
        pthread_mutex_unlock(&mArrive[idAFrente]);

        if (andou == 0) 
        {
            pthread_mutex_unlock(&pistaMutex[x_front][c->y_pos]);
        
            //Espera ocupada
            while (andou == 0)
            {
                pthread_mutex_lock(&mArrive[idAFrente]);
                andou = arrive[idAFrente];
                pthread_mutex_unlock(&mArrive[idAFrente]);
                usleep(1);
            }
            pthread_mutex_lock(&pistaMutex[x_front][c->y_pos]);
        }
        idAFrente = pista[x_front][c->y_pos];
        if (idAFrente == 0)
        {
            int xAnt = c->x_pos;
            move_to(c, x_front, c->y_pos);
            pthread_mutex_unlock(&pistaMutex[xAnt][c->y_pos]);
            pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);
            return true;
        }
        
        //Ciclista na frente que já andou, libero o mutex
        pthread_mutex_unlock(&pistaMutex[x_front][c->y_pos]);

        int y_front;
        //Ultrapassagem vagabundinha
        if (espaco_lado(c->x_pos, c->y_pos) && (y_front = espaco_frente(c->x_pos, c->y_pos)) != -1)
        {
            //fprintf(stderr, "ultrapassou\n");
            int xAnt = c->x_pos;
            int yAnt = c->y_pos;
            move_to(c, x_front, y_front);
            pthread_mutex_unlock(&pistaMutex[xAnt][yAnt]);
            pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);
            return true;
        }
        pthread_mutex_unlock(&pistaMutex[c->x_pos][c->y_pos]);
    }
    return false;
    
}

bool espaco_lado(int x, int y)
{
    int idAtual;
    for (int i = y; i < velodromo_width; i++)
    {
        //pthread_mutex_lock(&pistaMutex[x][i]);
        if (pthread_mutex_trylock(&pistaMutex[x][i]) == 0)
        {    
            idAtual = pista[x][i];
            if (idAtual == 0)
            { 
                pthread_mutex_unlock(&pistaMutex[x][i]);
                return true;
            }
            pthread_mutex_unlock(&pistaMutex[x][i]);
        }
    }
    return false;
}

int espaco_frente(int x, int y)
{
    int x_front = (x+1)%velodromo_length;
    int idAtual;
    //Não sei se devo ocmeçar em y ou em 0
    for (int i = y; i < velodromo_width; i++)
    {
        //Como poderíamos ter deadlock aqui?
        //pthread_mutex_lock(&pistaMutex[x_front][i]);
        //Não sei se precisa ser trylock, aparentemente não da deadlock com lock normal
        if (pthread_mutex_trylock(&pistaMutex[x_front][i]) == 0)
        {
            idAtual = pista[x_front][i];
            if (idAtual == 0) return i;
            pthread_mutex_unlock(&pistaMutex[x_front][i]);
        }
    }
    return -1;
}

void atualiza_velocidade(ciclist_ptr c)
{
    if (c->speed == LOW_SPEED && rand()%100 < 80)
        c->speed = AVG_SPEED;

    else if (c->speed == AVG_SPEED && rand()%100 < 40)
        c->speed = LOW_SPEED;
}

bool quebrou(ciclist_ptr c)
{
    if (c->laps % 6 != 0) return false;

    pthread_mutex_lock(&nCiclistMutex);
    if (running_ciclists <= 5)
    {
        pthread_mutex_unlock(&nCiclistMutex);
        return false;
    }
    pthread_mutex_unlock(&nCiclistMutex);

    if (rand()%100 < 20)
    {
        fprintf(stderr, "O ciclista %d quebrou após %d voltas\n", c->id, c->laps);
        return true;
    }
    return false;

}
