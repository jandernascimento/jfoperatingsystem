#include <stdio.h>
#include <stdlib.h>


void hello(){

  printf("got into my hello func");

}

int create_child(){

  pid_t pid;

  if ((pid=fork())<0){
    perror("FAULT!");
    exit(1);
  }else{

    printf("this is running in a child process");

  }

  return pid;


}


int main(int argc, char *argv[]){

  printf("parametros argc=%s\n",argv);
  int i;
  for(i=0;i<=argv[1];i++){
    create_child();
  }

  return 0;
}  

