/*
Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread,
lancia n thread per cercare il valore minimo ed il valore massimo in una matrice nxn di interi.
La ricerca del minimo e del massimo deve essere effettuata simultaneamente su tutte le righe.
Individuati i valori cercati, si provvederà a stamparli a video.
La dimensione della matrice può essere fornita in input al programma in fase di esecuzione o da riga di comando
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
struct 
{
    int** matrice;
    int n;
}shared;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int massimoGlob = INT_MIN;
int minimoGlob = INT_MAX;

void* routine(void* args);

int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc!=2)
    {
        printf("Errore nel passaggio dei parametri.\n");
        return 1;
    }

    shared.n = atoi(argv[1]);
    if (shared.n<1)
    {
        printf("La n deve essere >0\n");
        return 1;
    }

    shared.matrice = (int**)malloc(sizeof(int*)*shared.n);
    for (size_t i = 0; i < shared.n; i++)
    {
        shared.matrice[i] = (int*)malloc(sizeof(int)*shared.n);
    }
    printf("MATRICE:\n");
    for (size_t i = 0; i < shared.n; i++)
    {
        for (size_t j = 0; j < shared.n; j++)
        {
            shared.matrice[i][j] = (rand()%256);
            printf("%d\t",shared.matrice[i][j]);
        }
        printf("\n");
        
    }

    pthread_t* tid = malloc(sizeof(pthread_t)*shared.n);
    int* t_id = malloc(sizeof(int)*shared.n);
    for (size_t i = 0; i < shared.n; i++)
    {
        t_id[i] = i;
        if (pthread_create(&tid[i],NULL,&routine,(void*)&t_id[i])!=0)
        {
            printf("Errore durante la creazione dei thread.\n");
            return 1;
        }
        
    }

    for (size_t i = 0; i < shared.n; i++)
    {
        if (pthread_join(tid[i],NULL)!=0)
        {
            printf("Errore durante i join dei thread.\n");
            return 1;
        }
        
    }


    printf("Il massimo globale e' %d.\nIl minimo globale e' %d.\n", massimoGlob,minimoGlob);

    free(shared.matrice);
    free(t_id);
    free(tid);
    pthread_mutex_destroy(&mutex);
    
    
    
}



    void* routine(void* args){
        int i = *(int*)args;
        int tmp = -1;

       
        for (size_t j = 0; j < shared.n; j++)
        {
            tmp = shared.matrice[i][j];

            pthread_mutex_lock(&mutex);
            if (tmp>massimoGlob)
            {
                printf("Thread: %d, massimo temporaneo trovato nella riga %d, valore: %d\n", i,i,tmp);
                massimoGlob = tmp;
            }
            else if (tmp<minimoGlob)
            {
                printf("Thread: %d, minimo temporaneo trovato nella riga %d, valore: %d\n", i,i,tmp);
                minimoGlob = tmp;
            }
            pthread_mutex_unlock(&mutex);
        }
        pthread_exit(NULL);
        

    }