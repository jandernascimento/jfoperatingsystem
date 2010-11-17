#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#define NB_SONG 3

void *supporter(void *arg){

char *str=(char*)arg;
int i;
int pid;
pthread_t tid;
pid=getpid();
tid=pthread_self();
srand((int)tid);

for(i=0;i<NB_SONG;i++){
printf ("Processus %d thread %x : %s",pid,(unsigned int)tid,str);
usleep(rand()/RAND_MAX*10000000.);
}
return (void *)tid;

}

int main (int argc,char **argv){
printf("ok");
int team1;
int team2;
int i;
int nb_threads=0;
pthread_t *tids;

if(argc!=3){
fprintf(stderr,"usage: %s team1 team2\n",argv[0]);
exit(-1);
}

team1=atoi(argv[1]);
team2=atoi(argv[2]);

nb_threads = team1 + team2;

tids=malloc(nb_threads*sizeof(pthread_t));



for(i=0;i<team1;i++){

pthread_create(&tids[i],NULL,supporter,"Allon\n");

}

for(;i<nb_threads;i++){

pthread_create(&tids[i],NULL,supporter,"Sweep\n");

}


for(i=0;i<nb_threads;i++){

pthread_join(tids[i],NULL);

}

free(tids);

return EXIT_SUCCESS;

}

