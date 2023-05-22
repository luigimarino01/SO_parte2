#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void printID(const char* s){
	pid_t pid=getpid();
	pthread_t tid=pthread_self();
	
	printf("%s con PID %d e TID %lu\n", s, pid, (unsigned long)tid);
}
void * start_routine(){
	printID("Sono il thread");
}

int main(){
	pthread_t tid;
	if(pthread_create(&tid, NULL, start_routine, NULL)!=0){
		printf("errore creaizone thread\n");
		exit(-1);
	}
	printID("Sono il thread principale");
	sleep(1);//per evitare che il thread principale (main) termini la sua esecuzione prima che il thread creato possa andare in esecuzione
	exit(0);
}
