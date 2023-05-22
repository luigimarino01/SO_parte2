#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void* roll_dice() {
    srand(time(NULL));
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;

    return (void*) result; 

}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    int* res;
    
    if (pthread_create(&p1, NULL, &roll_dice, NULL) != 0) {
        return 1;
    }

    if (pthread_join(p1, (void**) &res) != 0) {
        return 3;
    }

    printf("%d\n", *res);
    free(res);
    
    
    return 0;
}