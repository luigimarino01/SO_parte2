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

#define NUMERO_THREADS 5

int min = INT_MIN;

void* contaEntrate(void* args){
    char* stringa = (char*)args;
    DIR *dp;
    if  ((dp = opendir(stringa)) == -1)
        perror("Errore durante l'apertura della directory.");

    struct dirent *element;
    struct stat elementInfo;



}

int main(int argc, char* argv[]){


    if (argc!=6)
    {
        perror("Errore di parametri");
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
        char* a = malloc(sizeof(char));
        a = nomeDirectory[i];
        if (pthread_create(&threads[i],NULL,&contaEntrate,a)!=0)
        {
            perror("Errore durante la creazione dei thread.");
        }
        
    }

    for (size_t i = 0; i < NUMERO_THREADS; i++)
    {
        int *r;
        if (pthread_join(threads[i],(void**)&r)!=0)
        {
            perror("Errore durante il join dei thread.");
        }
        if (*r<min)
        {
            min = *r;
        }
        
        free(r);
    }
    
    
    
   
    


    
}
