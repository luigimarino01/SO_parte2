#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

int indiceGenerale = 0;

struct{
    int** M;
    int* V;
    int* ElementiInseriti;
    int n;
    pthread_mutex_t mutex;
    pthread_cond_t sblocca;
    sem_t* sem;
}struttura;

void* routine(void* args){
    int t_id = *(int*)args;



    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    int val;

    if(argc != 2){
        printf("errore nel passaggio dei parametri.\n");
        return 1;
    }

    struttura.n = atoi(argv[1]);

    struttura.sem = sem_open("sem", O_CREAT, (S_IRUSR | S_IWUSR), 10);
    sem_post(struttura.sem);
    sem_getvalue(struttura.sem, &val);
    printf("value dopo post = %d\n", val);

    sem_wait(struttura.sem);
    sem_getvalue(struttura.sem, &val);
    printf("value dopo wait = %d\n", val);

    sem_close(struttura.sem);
    sem_unlink("/sem");

    return 0;
}