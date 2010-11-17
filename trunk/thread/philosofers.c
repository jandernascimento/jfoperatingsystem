#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#define nb_threads 2
#define THINK 1
#define EAT 2


pthread_mutex_t mut;

typedef struct philosopher_type {

	int id;
	char *name;
	int status;

} ph_t, *ph_p;

void *supporter(void *arg){
	int x=0;
	ph_p p=(ph_p) arg;

	pthread_mutex_lock( &mutex );

	for(;x<20;x++){
		printf("%s\n",p->name);
		sleep(1);	
	}


	pthread_mutex_unlock( &mutex );

	return 1;
}


int main(){

	ph_t *ph_list;
	ph_list=malloc(nb_threads*sizeof(ph_t));

	ph_list[0].id=0;
	ph_list[0].name="Jander";

	ph_list[1].id=1;
	ph_list[1].name="Doido";
	
	pthread_t *tids;
	tids=malloc(nb_threads*sizeof(pthread_t));
	pthread_create(&tids[0],NULL,supporter,&ph_list[0]);
	pthread_create(&tids[1],NULL,supporter,&ph_list[1]);
	//pthread_create(&tids[1],NULL,supporter,"Philosofer 2\n");
	int i;
	for(i=0;i<nb_threads;i++){
		pthread_join(tids[i],NULL);
	}

}
