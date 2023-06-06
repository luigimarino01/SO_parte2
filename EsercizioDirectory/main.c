/*Si realizzi un programma in C e Posix sotto Linux che, con  l'ausilio della
libreria Pthread, lancia 5 thread che leggono concorrentemente altrettante directory specificate dall'utente.   
I thread, inoltre, scrivono in una variabile il numero di  file della directory
che,  tra  le tre esaminate, contiene il  minor numero di entrate.*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <apue.h>
#include <limits.h>

#define NUMERO_THREADS 5

int min = INT_MIN;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* contaEntrate(void* args){
    int count = 0;
    char* stringa = (char*)args;
    DIR *dp;
    if  ((dp = opendir(stringa)) == NULL)
        perror("Errore durante l'apertura della directory.");
        pthread_exit(NULL);

    struct dirent *element;

    while((element = readdir(dp))!=NULL){
        count++;
    }


    printf("Lettura directory :%s - Numero elementi: %d\n", stringa,count);
    closedir(dp);
    pthread_mutex_lock(&mutex);
    if (count<min){
        min = count;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;

}
    

int main(int argc, char* argv[]){


    if (argc!=6)
    {
        perror("Errore di parametri");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[NUMERO_THREADS];
    char *nomeDirectory[NUMERO_THREADS];
    nomeDirectory[0] = argv[1];
    nomeDirectory[1] = argv[2];
    nomeDirectory[2] = argv[3];
    nomeDirectory[3] = argv[4];
    nomeDirectory[4] = argv[5];

    for (size_t i = 0; i < NUMERO_THREADS; i++)
    {
        if (pthread_create(&threads[i],NULL,&contaEntrate,(void*)nomeDirectory[i])!=0)
        {
            perror("Errore durante la creazione dei thread.");
            exit(EXIT_FAILURE);
        }
        
    }

    for (size_t i = 0; i < NUMERO_THREADS; i++)
    {
        
        if (pthread_join(threads[i],NULL)!=0)
        {
            perror("Errore durante il join dei thread.");
            exit(EXIT_FAILURE);
        }
    
    }

    printf("Il file con le minori entrate ha %d entrate.", min);
    
    pthread_mutex_destroy(&mutex);
    
   
    


    
}
