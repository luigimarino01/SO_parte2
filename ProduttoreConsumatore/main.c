#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

void* produci(void* args);
void* consuma(void* args);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t vuoto,pieno;

struct 
{
    int n;
    int* buffer;
}shared;

int main(int argc, char* argv[]){
    if (argc!=2)
    {
        printf("Errore di parametri.\n");
        return 1;
    }

    shared.n = atoi(argv[1]);
    pthread_t produttore,consumatore;
    shared.buffer = (int*)malloc(sizeof(int)*shared.n);

    sem_init(&vuoto,0,shared.n);
    sem_init(&pieno,0,0);

    if (pthread_create(&produttore,NULL,&produci,NULL)!=0)
    {
       printf("Errore di creazione.\n");
        return 1;
    }
    if (pthread_create(&consumatore,NULL,&consuma,NULL)!=0)
    {
       printf("Errore di creazione.\n");
        return 1;
    }

    if (pthread_join(produttore,NULL)!=0)
    {
        printf("Errore di join.\n");
        return 1;
    }

     if (pthread_join(consumatore,NULL)!=0)
    {
        printf("Errore di join.\n");
        return 1;
    }
    


    sem_destroy(vuoto);
    sem_destroy(pieno);
    free(shared.buffer);
    return 0;
    
    
}


void* produci(void* args){
    int indice_produttore = 0;
    while (1)
    { 
        
        pthread_mutex_lock(&mutex);
        sem_wait(&vuoto);
        pthread_mutex_unlock(&mutex);
        
        shared.buffer[indice_produttore] = (rand()%10);
        printf("Ho prodotto %d in posizione %d\n",shared.buffer[indice_produttore], indice_produttore);
        indice_produttore++;
        if (indice_produttore==shared.n)
        {
            indice_produttore=0;
        }
        
        sem_post(&pieno);
        
    }
    
}

void* consuma(void* args){
    
    int indice_consumatore = 0;
    int mangiato = -1;
    while (1)
    { 
        
        pthread_mutex_lock(&mutex);
        sem_wait(&pieno);
        pthread_mutex_unlock(&mutex);
        mangiato = shared.buffer[indice_consumatore];
        shared.buffer[indice_consumatore] = -1;
        printf("Ho consumato %d in posizione %d\n",mangiato, indice_consumatore);
        indice_consumatore++;
        if (indice_consumatore==shared.n)
        {
            indice_consumatore=0;
        }
        
        sem_post(&vuoto);

    }
}