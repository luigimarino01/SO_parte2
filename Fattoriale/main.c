/*
Si progetti un programma in ambiente Linux che, impiegando  la  libreria  pthread,  crei  un  thread  che  estrae N volte
(con N passato da riga di comando) un numero casuale compreso tra 0 e 10 e ne calcoli il fattoriale  ponendolo  in  una  variabile  condivisa
e  un  thread  che  stampi  a  video  il  fattoriale  calcolato  N  volte prelevandolo dalla variabile condivisa.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

struct{
    long long condivisa;
    int n;
}shared;
sem_t semCalcola, semPreleva;
void* calcola(void*);
void* preleva(void*);
long long fattoriale(long long);

int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc!=2)
    {
        printf("Errore durante il passaggio dei parametri.\n");
        return 1;
    }
    sem_init(&semCalcola,0,1);
    sem_init(&semPreleva,0,0);

    shared.n = atoi(argv[1]);
    if (shared.n<1)
    {
        printf("N deve essere >0.\n");
        return 1;
    }

    pthread_t* tid = malloc(sizeof(pthread_t)*2);
   
    if (pthread_create(&tid[0],NULL,&calcola,NULL)!=0)
    {
        printf("Errore durante la creazione del thread calcolatore.\n");
        return 1;
    }

    if (pthread_create(&tid[1],NULL,&preleva,NULL)!=0)
    {
        printf("Errore durante la creazione del thread prelevatore.\n");
        return 1;
    }


    if (pthread_join(tid[0],NULL)!=0)
    {
        printf("Errore durante il join del thread calcolatore.\n");
        return 1;
    }

    if (pthread_join(tid[1],NULL)!=0)
    {
        printf("Errore durante il join del thread prelevatore.\n");
        return 1;
    }

    sem_destroy(&semCalcola);
    sem_destroy(&semPreleva);
    free(tid);


}

void* calcola(void* args){
    for (size_t i = 0; i < shared.n; i++)
    {
        long long casuale = rand()%11;
        sem_wait(&semCalcola);
        printf("Numero casuale generato di cui calcolare il fattoriale: %ld\n",casuale);
        shared.condivisa = fattoriale(casuale);
        sem_post(&semPreleva);
        
    }
    pthread_exit(NULL);
    
}


void* preleva(void* args){
    for (size_t i = 0; i < shared.n; i++)
    {
        long long buff = 0;
        sem_wait(&semPreleva);
        buff = shared.condivisa;
        shared.condivisa = -1;
        printf("Valore prelevato: %ld\n",buff);
        sem_post(&semCalcola);

    }
    
}

long long fattoriale(long long n){
    if (n == 0 || n == 1)
    {
        return 1;
    }
    else{
        return n * fattoriale(n-1);
    }
    
}


