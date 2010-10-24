#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stdint.h>

void hello(){

  printf("got into my hello func");

}

int create_child(int id){
  struct timespec start,end;
  pid_t pid=fork();

  if (pid<0){
    perror("FAULT!");
    exit(1);
  }else if( pid > 0 ){
    printf("process pid %i node %i val %d\n",getpid(),id,rand());
  }

  return pid;
}


int main(int argc, char *argv[]){

  printf("parametros argc=%s\n",argv[1]);
  int i;
  for(i=0;i<atoi(argv[1]);i++){
    int id = create_child(i);
    if(id>0) break;
  }

  return 0;
}  

