#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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

  int pip[2];
  int readstr;
  int pidpai=0;
  pipe(pip);
  int mypid=getpid();
  printf("PID PAI:%i\n",mypid);
  write(pip[1],mypid,7);
  close(pip[1]);
  int pid=fork();
  if(pid>0){
    printf("OK\n");
    read(pip[0],readstr,7);
    printf("valores lidos=%i\n",readstr);
  }

  return 0;
}  

