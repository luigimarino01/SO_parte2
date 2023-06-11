/*Scrivere un programma che prende in input un intero n, il nome di
un file di testo ed un carattere x

• Il programma ha il compito di contare le occorrenze
        del carattere x nel file di testo

• Il programma esegue tale compito creando n thread, ognuno dei
quali esamina una porzione diversa del file di testo
• ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread
esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes
successivi, e così via*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
int occorrenzeGlobali = 0;
typedef struct{
    char *file;
    char carattere;
    long start;
    long end;
    int nOccorrenze;
} strutturaThread;


void* occorrenze(void* args){
    strutturaThread *dati = (strutturaThread*) args;
    FILE *file = fopen(dati->file,"r");
    if (file == NULL)
    {
    perror("Errore durante l'apertura del file");
    pthread_exit(NULL);
    }

    fseek(file,dati->start, SEEK_SET);
    int ch;
    while ((ch = fgetc(file)) != EOF && ftell(file)<= dati->end)
    {
        if (ch == dati->carattere)
        {
            dati->nOccorrenze++;
        }
        
    }
    fclose(file);
    pthread_exit(NULL);
    
    

}

int main(int argc, char *argv[]){
FILE *file;
long byteCount = 0;
int daLeggere;


int n = atoi(argv[1]); // Converte l'argomento intero da stringa ad intero
char *nome_file = argv[2];
char x = argv[3][0]; // Prende il primo carattere dell'argomento
strutturaThread strutturaThread[n];

pthread_t threads[n];



    file = fopen(nome_file, "rb");
    if (file == NULL)
    {
    printf("Errore durante l'apertura del file");
    return 1;
    }

fseek(file,0,SEEK_END);
byteCount = ftell(file);
fclose(file);
daLeggere = byteCount/n;



for (size_t i = 0; i < n; i++)
{
    strutturaThread[i].file = nome_file;
    strutturaThread[i].carattere = x;
    strutturaThread[i].start = daLeggere*i;
    strutturaThread[i].end = strutturaThread[i].start+daLeggere;

    if (i == n-1)
    {
        strutturaThread[i].end += (byteCount%n);
    }

    strutturaThread[i].nOccorrenze = 0;
    
   
   if (pthread_create(&threads[i], NULL, occorrenze, (void *)&strutturaThread[i])) {
            perror("Errore durante la creazione del thread");
            return 1;
        }

}


for (size_t i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Errore durante l'attesa del thread");
            return 1;
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        occorrenzeGlobali+= strutturaThread[i].nOccorrenze;
    }

    printf("Numero occorrenze totali: %d\n", occorrenzeGlobali);
    



}
