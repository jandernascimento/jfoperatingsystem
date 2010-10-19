
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_child(){

  pid_t pid;

  if ((pid=fork())<0){
    perror("FAULT!\n");
    exit(1);
  }else{

    printf("this is running in a child process\n");

  }

  return pid;


}


int main(int argc, char *argv[]){

  printf("parametros argc=%s\n",argv[0]);
  int i;
  int val=create_child();

  if(val==0){
  
    printf("Este é o processo pai!\n");

  }else if (val>0){

    char *arg[]={"/bin/ls",NULL,NULL};
    int execution=execve(arg[0],arg, NULL);

    if(execution<0){
      printf("Execution failed!\n");
    }

  }
  return 0;
}  

