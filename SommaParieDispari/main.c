/*
    Si realizzi un programma in C e POSIX sotto linux che lanci n thread per calcolare la somma di una matrice n*n
    che contiene valori casuali compresi tra [0,255], allocata dinamicamente:
     
Il thread i-esimo con i pari, calcola la somma degli elementi di indice pari nella riga i-esima
Il thread i-esimo con i dispari, calcola la somma degli elementi di indice dispari nella riga i-esima
Alla fine calcolare il minimo tra tutti le somme parziali calcolate
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct {
    int n;
    int **matrice;
    int minimo;
}strutturaThread;



void* routine(void* args){
    int i = *(int*)args;
    int somma = 0;
    
    if (i%2 == 0)
    {
        //printf("Thread %d - pari.\n",i);
        for (size_t k = 0; k < strutturaThread.n; k+=2)
        {
           somma+= strutturaThread.matrice[i][k];
        }
    }
    else if (i%2 != 0)
    {
        //printf("Thread %d - dispari.\n",i);
        for (size_t k = 1; k < strutturaThread.n; k+=2)
        {
            somma+= strutturaThread.matrice[i][k];
        }
        
    }

    printf("Thread %d - somma parziale: %d\n", i, somma);
    pthread_mutex_lock(&mutex);
    if (somma<strutturaThread.minimo)
    {
        strutturaThread.minimo = somma;

    }
    pthread_mutex_unlock(&mutex);
    
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc!=2)
    {
        printf("Numero errato di parametri.\n");
        return 1;
    }

    strutturaThread.n = atoi(argv[1]);

    if (strutturaThread.n<1)
    {
        printf("N non valida.\n");
        return 1;
    }
    strutturaThread.minimo = INT_MAX;
    
    pthread_t* tid = malloc(sizeof(pthread_t)*strutturaThread.n);
    int* t_id = malloc(sizeof(int)*strutturaThread.n);
    strutturaThread.matrice = (int**)malloc(sizeof(int*)*strutturaThread.n);
    for (size_t i = 0; i < strutturaThread.n; i++)
    {
        strutturaThread.matrice[i] = (int*)malloc(sizeof(int)*strutturaThread.n);
    }

    printf("MATRICE: \n");
    for (size_t i = 0; i < strutturaThread.n; i++)
    {
        for (size_t j = 0; j < strutturaThread.n; j++)
    {
        strutturaThread.matrice[i][j] = (rand()%256);
        printf("%d\t", strutturaThread.matrice[i][j]);
    }
    printf("\n");
    }
    
    
    

    for (size_t i = 0; i < strutturaThread.n; i++)
    {
        t_id[i] = i;
        if (pthread_create(&tid[i], NULL, routine, (void*)&t_id[i])!= 0)
        {
            printf("Errore durante la creazione dei thread.\n");
            return 1;
        }
        
    }
    

    for (size_t i = 0; i < strutturaThread.n; i++)
    {
    
        if (pthread_join(tid[i],NULL)!=0)
        {
            printf("Errore durante la creazione dei thread.\n");
        }
        
    }


    

    printf("Minimo delle somme: %d\n", strutturaThread.minimo);


    free(strutturaThread.matrice);
    free(tid);
    free(t_id);
    pthread_mutex_destroy(&mutex);
    
}