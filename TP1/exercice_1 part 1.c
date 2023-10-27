#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <error.h>
#include <sys/types.h> 

void *thread_function(void *var) { //le code a executer par un thread
	printf("Je suis le thread de tid :%p, mon pid est :%d\n", pthread_self() ,getpid()); 
	sleep(6);
	printf("Au revoir du thread de tid :%p\n", pthread_self());
	pthread_exit((void*)42);
	// exit(42); // la fonction n'a pas retouné la valeur 42 au thread (main) - tue de processus -
}

int main(){
	
	int i;
	pthread_t tid;
	pthread_create(&tid, NULL, thread_function, NULL); // si le main se termine, les threads se disparaissent
	
	printf("Thread principal de (pid, tid) :(%d, %p), creation d'un thread de tid %p \n",getpid(),pthread_self(),tid);
	
	pthread_join(tid, (void **)&i); // Attendre la fin d'un autre thread
	printf("Thread principal, la valeure de retour du thread %p est: %d\n",tid,i);
	
	return 0; 
}
