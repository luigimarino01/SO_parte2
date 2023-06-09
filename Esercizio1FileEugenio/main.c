#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t* sem1;
sem_t* sem2;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;
struct timespec attesa;


struct {
    int n;
    int fd;
}shared;

void* scrivi(void* args);
void* leggi(void* args);
void* stampa(void* args);


int main(int argc, char* argv[]){
attesa.tv_sec = 0;
attesa.tv_nsec = 500000000L;
    sem_unlink("/sem1");
    sem_unlink("/sem2");
    if(argc!=2){
        printf("Errore durante il passaggio dei parametri.\n");
        return 1;
    }
    shared.n = atoi(argv[1]);
    if(shared.n<10 || shared.n>15){
        printf("N deve essere compresa tra 10 e 15.\n");
        return 1;
    }

    
    sem1 = sem_open("/sem1",O_CREAT,0777,1);
    sem2 = sem_open("/sem2",O_CREAT,0777,0);

    shared.fd = open("numeri",O_RDWR | O_CREAT | O_TRUNC, 0777);

    pthread_t scrittore,lettore,stampatore;

    if(pthread_create(&scrittore,NULL,&scrivi,NULL)!=0)
        return 1;

    if(pthread_create(&lettore,NULL,&leggi,NULL)!=0)
        return 1;
    
    if(pthread_create(&stampatore,NULL,&stampa,NULL)!=0)
        return 1;


    if(pthread_join(scrittore,NULL)!=0)
        return 1;

    if(pthread_join(lettore,NULL)!=0)
        return 1;
    
    if(pthread_join(stampatore,NULL)!=0)
        return 1;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    sem_unlink("/sem1");
    sem_unlink("/sem2");
    

    return 0;
}


void* scrivi(void* args){
        int i;
    int f;
    int a;
    int b;
    b = 1;
    a= 0;

    for(i = 0; i<= shared.n; i++){
        sem_wait(sem1);
        f = b+a;
        a = b;
        b = f;
        lseek(shared.fd,0,SEEK_SET);
        write(shared.fd,&b,sizeof(int));
        printf("Ho scritto %d\n", b);
        nanosleep(&attesa,NULL);
        sem_post(sem2);
    }
    pthread_exit(NULL);
    

}


void* leggi(void* args){
    int i;
    int buff;
    while(1){
        sem_wait(sem2);
        lseek(shared.fd,0,SEEK_SET);
        read(shared.fd,&buff,sizeof(int));
        printf("Ho letto %d\n",buff);
        
        pthread_mutex_lock(&mutex);
        count++;
        if(count == shared.n){
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            return;
        }
        
        pthread_mutex_unlock(&mutex);
        sem_post(sem1);

    }
}


void* stampa(void* args){
    pthread_mutex_lock(&mutex);
    while(count<shared.n){
        pthread_cond_wait(&cond,&mutex);
        }
        pthread_mutex_unlock(&mutex);
        pthread_t tid = pthread_self();
        printf("Operazioni concluse, arrivederci dal thread %u\n",(unsigned)tid);
       // return;
        pthread_exit(NULL);
        
   
    
}

