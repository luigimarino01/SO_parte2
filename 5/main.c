#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int i = 0;
int mails = 0;
pthread_mutex_t mutex;


void* routine(){
    for (size_t i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
    
    
}


int main(){
    pthread_t t[4];
    pthread_mutex_init(&mutex, NULL);

    for(i = 0; i<4; i++){
        pthread_create(t + i, NULL, &routine, NULL);
    }
    
    for(i = 0; i<4; i++){
        pthread_join(t[i], NULL);
    }

    printf("mails: %d\n", mails);

    pthread_mutex_destroy(&mutex);
    return 0;


}