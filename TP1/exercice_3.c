#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

void * fonction(void * var) { 
	printf("Je suis le thread de tid: %p \n",pthread_self()); 
	sleep(5); 
	printf("Au revoir");
}


int main() {
	
	pthread_attr_t attr;
	pthread_attr_init(&attr); //initialisation par les valeurs par défaut //afficher les valeurs par défaut des attributs
	
	//l’attribut scope d’ordonnancement 
	int scope; 
	pthread_attr_getscope (&attr, &scope);
	
	printf("Attribut scope par défaut est:%s",(scope==PTHREAD_SCOPE_SYSTEM? "Scope Systeme": (scope==PTHREAD_SCOPE_PROCESS ? "Scope processus":"OTHER")));
	
	//1: modifier la valeur de scope pour le rendre scope processus
	//priorite par défaut du processus 
	int policy; 
	struct sched_param sched; 
	pthread_getschedparam(pthread_self(), &policy, &sched); // -solved-
	printf ("politique : %s, priorite :%d\n", (policy == SCHED_OTHER ? "SCHED_OTHER" : (policy == SCHED_FIFO ? "SCHED_FIFO" : (policy == SCHED_RR ? "SCHED_RR" : "inconnu"))), sched.sched_priority);
	
	//2: modifier la priorité du thread a créer
	//afficher la valeur par défaut de de l’attribut detachstate 
	int detache;
	pthread_attr_getdetachstate (&attr, &detache);
	if(detache==PTHREAD_CREATE_DETACHED){
		printf("Detache thread attribut par défaut est Detache\n");
	} else if(detache==PTHREAD_CREATE_JOINABLE ){
		printf("Detache thread attribut par défaut est Joignable\n");
	}
	
	// 3: modifier le detach state pour créer un thread injoignable ou detache avec la fonction //pthread_attr_setdetachstate.
	//creer un nouveau thread avec les attributs crees et modifies en haut 
	
	pthread_t tid;
	pthread_create(&tid,&attr,fonction,NULL);
	pthread_attr_destroy(&attr); 
	printf("Thread principal, creation d’un thread de tid %p \n",tid);
	
    return 0;
}














// 22) BENMEZIANE ABDELMALEK SIGNATURE