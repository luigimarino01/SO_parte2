#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int threadAlLavoro = 0;
int xTrovata = 0;
int yTrovata = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int** matrice;
    int elemento;
    int n;
    int i;
    int* trovato;
}strutturaThread;

void* trovaElemento(void* args){
    int myNumber = 0;
    strutturaThread* data = (strutturaThread*)args;


    pthread_mutex_lock(&mutex);
        myNumber = threadAlLavoro;
        threadAlLavoro++;
    pthread_mutex_unlock(&mutex);

    for (size_t i = 0; i < data->n; i++)
    {
        if (*(data->trovato) == 1)
        {
            break;
        }
        
        if (data->matrice[data->i][i] == data->elemento)
        {
            pthread_mutex_lock(&mutex);
            *(data->trovato) = 1;
            printf("Elemento %d trovato dal thread %d in posizione [%d][%d].\n", data->elemento,data->i,data->i,i);
            xTrovata = data->i;
            yTrovata = i;
            pthread_mutex_unlock(&mutex);
            break;
        }        
    }



    if (myNumber < data->n-1)
        {
            pthread_mutex_lock(&mutex);
            printf("sto aspettando\n");
            pthread_cond_wait(&cond,&mutex);
            printf("attesa finita\n");
            pthread_mutex_unlock(&mutex);
        
            if(*(data->trovato)==1)
            printf("Sono il thread %d, mi sono state notificate le coordinate: [%d][%d]\n",data->i,xTrovata,yTrovata);
            return NULL;

        
        } else
        {
            printf("sono l'ultimo - segnalo.\n");
            pthread_cond_broadcast(&cond);
            return NULL;
        }

    

    
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
    
    pthread_cond_init(&cond,NULL);
 
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
    

    //Creazione dei thread
    for (size_t i = 0; i < n; i++)
    {
        strutturaThread[i].matrice = matrice;
        strutturaThread[i].n = n;
        strutturaThread[i].elemento = elemento;
        strutturaThread[i].i = i;
        strutturaThread[i].trovato = &trovato;
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
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    


    return 0;
}