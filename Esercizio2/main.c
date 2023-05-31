#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct{
    int** matrice;
    int elemento;
    int n;
    int i;
    int* trovato;
    pthread_mutex_t* mutex;

}strutturaThread;

void* trovaElemento(void* args){
    strutturaThread* data = (strutturaThread*)args;


    for (size_t i = 0; i < data->n; i++)
    {
        if (*(data->trovato) == 1)
    {
        return NULL;
    }
    
        if (data->matrice[data->i][i] == data->elemento)
        {
            pthread_mutex_lock(data->mutex);
            *(data->trovato) = 1;
            printf("Elemento %d trovato dal thread %d in posizione [%d][%d].\n", data->elemento,data->i,data->i,i);
            pthread_mutex_unlock(data->mutex);
            signal
            return NULL;
        }
        
    }

    
    printf("Thread %d ha concluso la ricerca nella sua riga senza trovare l'elemento.\n",data->i);
    sleep(1);
    return NULL;
    
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    if(argc != 3){
        printf("error occured in parameter passing.");
        return 1;
    }
    
    int **matrice;
    int n = atoi(argv[1]);
    int elemento = atoi(argv[2]);
    pthread_t threads[n];
    strutturaThread strutturaThread[n];
 
    //Alloco dinamicamente la matrice
    matrice = (int**)malloc(n* sizeof(int*));
    for (size_t i = 0; i < n; i++)
    {
        matrice[i] = (int*)malloc(n* sizeof(int));
    }

    //Assegno valori casuali da 1 a 100 alla matrice.
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            matrice[i][j] = (rand() % 100) + 1;
        }
        
    }

    //Stampa della matrice
    printf("MATRICE:\n");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");
        
    }

    int trovato = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    //Creazione dei thread
    for (size_t i = 0; i < n; i++)
    {
        strutturaThread[i].matrice = matrice;
        strutturaThread[i].n = n;
        strutturaThread[i].elemento = elemento;
        strutturaThread[i].i = i;
        strutturaThread[i].trovato = &trovato;
        strutturaThread[i].mutex = &mutex;
        pthread_create(&threads[i],NULL,&trovaElemento,(void*)&strutturaThread[i]);
    }

    for (size_t i = 0; i < n; i++)
    {
        pthread_join(threads[i],NULL);
    }





    for (size_t i = 0; i < n; i++)
    {
        free(matrice[i]);
    }
    free(matrice);
    


    return 0;
}