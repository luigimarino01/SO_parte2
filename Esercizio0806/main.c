/*
Sviluppare un programma sotto linux che generi n thread (n passato come input da terminale), generi una matrice n*n,
i cui valori all'interno sono generati casualmente da un intervallo [0,255].
Ogni thread è identificato da un valore intero 'i', ogni thread i-esimo calcola la media della riga i-esima della matrice,
quando tutte le medie sono state calcolate, viene notificato al thread n+1 che provvederà a calcolare la somma totale di tutte le medie.
Utilizzare i mutex e le variabili di condizione.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct {
    int n;
    int **matrice;
    int *medie;
}shared;

int arrivati = 0;
int inseriti = 0;

void* media(void* arg){
    int i = *(int*)arg;
    int ultimo = 0;
    int somma = 0;
    int media = 0;

        if (i == shared.n)
        {
            ultimo = 1;
        }

        
        
        if (ultimo == 0)
        {
           for (size_t j = 0; j < shared.n; j++)
           {
            somma += shared.matrice[i][j];
           }
           media = somma/shared.n;
           printf("Thread %d: media della riga %d = %d\n",i,i,media);
           shared.medie[i] = media;
           pthread_mutex_lock(&mutex);
           inseriti++;
           if (inseriti == shared.n)
           {
            pthread_cond_signal(&cond);
           }
           
           pthread_mutex_unlock(&mutex);
        }
        else if(ultimo == 1){
            int sommaMedie = 0;
            pthread_mutex_lock(&mutex);
                while (inseriti != shared.n)
                {
                
                pthread_cond_wait(&cond,&mutex);
                }
                pthread_mutex_unlock(&mutex);

                for (size_t j = 0; j < shared.n; j++)
                {
                    sommaMedie+=shared.medie[j];
                }
                
                printf("Il thread %d e' l'ultimo thread ed ha eseguito la somma delle medie: %d\n", i,sommaMedie);
        }

    
        pthread_exit(NULL);
}

int main(int argv, char* argc[]){
    srand(time(NULL));
    if (argv!=2)
    {
        printf("Parametri errati.\n");
        return 1;
    }

    shared.n = atoi(argc[1]);
    if (shared.n<1)
    {
        printf("N non valida.\n");
        return 1;
    }

    shared.medie = (int*)malloc(sizeof(int)*shared.n);
    shared.matrice = (int**)malloc(sizeof(int*)*shared.n);
    for (size_t i = 0; i < shared.n; i++)
    {
        shared.matrice[i] = (int*)malloc(sizeof(int)*shared.n);
    }

    for (size_t i = 0; i < shared.n; i++)
    {
        for (size_t j = 0; j < shared.n; j++)
        {
            shared.matrice[i][j] = (rand()%256);
          //  printf("%d\t", shared.matrice[i][j]);
        }
        //printf("\n");
        
    }

    pthread_t* tid = malloc(sizeof(pthread_t)*shared.n+1);
    int* t_id = malloc(sizeof(int)*shared.n);

    for (size_t i = 0; i < shared.n+1; i++)
    {
        t_id[i] = i;
        if (pthread_create(&tid[i],NULL,&media,(void*)&t_id[i])!=0)
        {
            printf("Errore durante la creazione dei thread.\n");
            return 1;
        }
    }

    for (size_t i = 0; i < shared.n+1; i++)
    {
        if (pthread_join(tid[i],NULL)!=0)
        {
            printf("Errore durante il join dei thread.\n");
            return 1;
        }
        
    }
    
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(shared.matrice);
    free(tid);
    free(t_id);

    
    

    
}