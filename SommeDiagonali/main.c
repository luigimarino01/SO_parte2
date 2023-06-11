/*Si  realizzi  un  programma  C  e  Posix  in  ambiente  Linux che,  impiegando  la  libreria  Pthread,  
crei  una matrice  di  interi  di  dimensioni  n  x  n  con  n  numero  dispari  fornito  da  riga  di  comando,
che  provveda  a sommare in modo concorrente con quattro thread, gli elementi delle due diagonali, della riga centrale e della 
colonna centrale della matrice e ne determini il massimo da assegnare ad un'opportuna variabile. */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
int **matrice;
int max = -1;
int nGlob = 0;
pthread_mutex_t *mutex = PTHREAD_MUTEX_INITIALIZER;

void* count(void* args){
int numero = *(int*)args;
int parziale = 0;
int centrale = nGlob/2;

if (numero == 0)
{

    for (size_t i = 0; i < nGlob; i++)
    {
        parziale += matrice[i][i];
    }
    
}
else if (numero == 1){
    int j = nGlob-1;
    for (size_t i = 0; i < nGlob; i++)
    {
        parziale+= matrice[i][j];
        j--;
    }
    
}
else if (numero == 2)
{
    for (size_t i = 0; i < nGlob; i++)
    {
        parziale+=matrice[centrale][i];
    }
    
}
else if (numero == 3)
{
   
    for (size_t i = 0; i < nGlob; i++)
    {
        parziale+=matrice[i][centrale];
   
    }
    
}
printf("STO RITORNANDO - SONO IL THREAD %d - SOMMA PARZIALE: %d\n", numero,parziale);
*(int*)args = parziale;
return args;





}


int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc!=2)
    {
        printf("Errore durante il passaggio dei parametri.\n");
        return 1;
    }
    int n = atoi(argv[1]);
    if (n%2 == 0)
    {
       printf("N deve essere un numero dispari.\n");
        return 1;
    }
    nGlob = n;

    matrice = (int**)malloc(sizeof(int*)*n);
    for (size_t i = 0; i < n; i++)
    {
        matrice[i] = (int*)malloc(sizeof(int)*n);
    }
    printf("MATRICE:\n");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            matrice[i][j] = (rand() % 5) + 1;
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");
    }
    

    
    
    pthread_t threads[3];
    for (size_t i = 0; i < 4; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&threads[i],NULL,&count,a) != 0)
        {
            printf("Errore durante la creazione dei threads.\n");
            return 0;
        }
    
    }
    
    for (size_t i = 0; i < 4; i++)
    {
        int *r;
        if (pthread_join(threads[i],(void**)&r)!=0){
            perror("Error joining");
        }
        pthread_mutex_lock(&mutex);
        if (*r>max)
        {
            max = *r;
            printf("Max temporaneo: %d\n",max);
           
        }
        pthread_mutex_unlock(&mutex);
        
        free(r);
    }
        printf("Il max globale alla fine e' : %d\n", max);
   

  
    

    pthread_mutex_destroy(mutex);
    for (size_t i = 0; i < nGlob; i++)
    {
        free(matrice[i]);
    }
    free(matrice);
    return 0;
    
    
    
    
}