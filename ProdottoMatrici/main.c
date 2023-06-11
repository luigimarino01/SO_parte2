/*
Scrivi un programma che prende in input due matrici quadrate di dimensione NxN e le moltiplica utilizzando i thread.
Il programma deve dividere il lavoro tra i thread in modo che ciascun thread calcoli la moltiplicazione di una porzione 
delle righe della prima matrice per le colonne della seconda matrice.
Alla fine, il programma dovrebbe stampare la matrice risultante.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
int **matriceRisultante;


typedef struct{
    int **matriceUno;
    int **matriceDue;
    int i;
    int n;
    int dispari;
    int numeroThread;

}strutturaThread;


void* moltiplicazione(void* arg) {
    strutturaThread* data = (strutturaThread*)arg;
    int sum = 0;
    int numeroThread = data->numeroThread;

    if (data->i != numeroThread - 1) {
        for (int j = data->i * 2; j < (data->i + 1) * 2; j++) {
            for (int k = 0; k < data->n; k++) {
                for (int l = 0; l < data->n; l++) {
                    sum += (data->matriceUno[j][l] * data->matriceDue[l][k]);
                }
                matriceRisultante[j][k] = sum;
                sum = 0;
            }
        }
    } else {
        for (int j = data->i * 2; j < data->n; j++) {
            for (int k = 0; k < data->n; k++) {
                for (int l = 0; l < data->n; l++) {
                    sum += (data->matriceUno[j][l] * data->matriceDue[l][k]);
                }
                matriceRisultante[j][k] = sum;
                sum = 0;
            }
        }
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc != 2)
    {
        printf("Errore parametri.\n");
        return 1;
    }
    int nDispari = 0;
    int** matriceUno;
    int ** matriceDue;
    int n = 4;
    n = atoi(argv[1]);

    if ( (n%2) !=0)
    {
        nDispari = 1;
        
    }
    

    matriceUno = (int**)malloc(sizeof(int*)*n);
    for (size_t i = 0; i < n; i++)
    {
        matriceUno[i] = (int*)malloc(sizeof(int)*n);
    }
    
    matriceDue = (int**)malloc(sizeof(int*)*n);
    for (size_t i = 0; i < n; i++)
    {
        matriceDue[i] = (int*)malloc(sizeof(int)*n);
    }
    
    matriceRisultante = (int**)malloc(sizeof(int*)*n);
    for (size_t i = 0; i < n; i++)
    {
        matriceRisultante[i] = (int*)malloc(sizeof(int)*n);
    }

    //RIEMPIMENTO MATRICE
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            matriceUno[i][j] = (rand() % 5) + 1;
            matriceDue[i][j] = (rand() % 5) + 1;
        }     
    }
    printf("PRIMA MATRICE:\n");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            printf("%d\t", matriceUno[i][j]);
        }
        printf("\n");
        
    }

     printf("SECONDA MATRICE:\n");
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            printf("%d\t", matriceDue[i][j]);
        }
        printf("\n");
        
    }
    
    int numeroThread = 0;
    int lavoroDaFare = 0;
    int lavoroRimanente;

    numeroThread = n/2;
    pthread_t threads[numeroThread];
    strutturaThread strutturaThread[numeroThread];


    for (size_t i = 0; i < numeroThread; i++)
    {

        if (nDispari == 1 && i == numeroThread-1)
        {
            strutturaThread[i].dispari = 1;
        }
        else{
            strutturaThread[i].dispari = 0;
        }
        
        strutturaThread[i].i = i;
        strutturaThread[i].n = n;
        strutturaThread[i].matriceUno = matriceUno;
        strutturaThread[i].matriceDue = matriceDue;
        strutturaThread[i].numeroThread = numeroThread;
        pthread_create(&threads[i], NULL, &moltiplicazione,(void*)&strutturaThread[i]);
    }

    
    for (size_t i = 0; i < numeroThread; i++)
    {
        pthread_join(threads[i],NULL);
    }

    printf("Matrice risultante:\n");
    for (size_t i = 0; i < n; i++)
    {
            for (size_t j = 0; j < n; j++)
            {
                printf("%d\t", matriceRisultante[i][j]);
            }
            printf("\n");
            
    }


    for (size_t i = 0; i < n; i++)
    {
        free(matriceUno[i]);
        free(matriceDue[i]);
        free(matriceRisultante[i]);
    }
    free(matriceUno);
    free(matriceDue);
    free(matriceRisultante);
    
    
    
    


}