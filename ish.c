/*by:Clayton Provan
* https://linux.die.net/man/3/execvp
* sources: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/

#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <signal.h>

void executeWait(char **arg);
void executeBackground(char **arg);
int GCD(char** args);
void argCount(char **args);


const char *mypath[] = { "./",
"/usr/bin/", "/bin/", NULL
};
extern char **getln();

int main(){

  /*used for parsing input */
  char hostName[100];
  int userID;
  char **args;
  char *argument1 = "NULL";

  userID = getuid();
  gethostname(hostName,100);
  /*contineus to get arguments as long as exit is not input*/
  while (1)
  {
  /* Wait for input */
    printf ("%s:%d:prompt> ",hostName,userID);
    args = getln();

    if( args[0] != NULL){
      argument1 = args[0];
    }
  //  printf("arG:%s, strcmp:%d:\n",argument1,strcmp strcmp(argument1,"exit"));
    if(strcmp(argument1,"exit") == 0){
      exit(1);
    }

    if(strcmp(argument1,"args")==0){
      argCount(args);
    }else if(strcmp(argument1,"gcd")==0){
      GCD(args);
    }else{

      executeWait(args);
    }


  /* If necessary locate executable using mypath array */

  /* Launch executable */
  // if (fork () == 0) {
  //
  // } else {
  //
  // } // end of if

  }

  return 0;
}

void executeWait(char **arg){
  pid_t  pid;
  int    status;

  if ((pid = fork()) < 0) {
       printf("A: forking child process failed\n");
       exit(1);
  }  /* cildprocess should have process id of zero.*/
  else if (pid == 0) {
       /*cool thing to do is run the shell inside the shell */
       if (execvp(*arg, arg) < 0) {
            printf("ALERT: exec failed\n");
            exit(1);
       }
  }
  else {
       //loops through until child process is done, waiting for child execution
       while (wait(&status) != pid){}
    }

}

void executeBackground(char **arg){


  if ( fork() < 0) {
       printf("A: forking child process failed\n");
       exit(1);
  }  /* cildprocess should have process id of zero.*/
  else {
       /*cool thing to do is run the shell inside the shell */
       if (execvp(*arg, arg) < 0) {
            printf("ALERT: exec failed\n");
            exit(1);
       }
  }
  //dont have to wait for command to end before coninuing.

}

int GCD(char** args){


  return 1;
}


void argCount(char **args){
  int i,k;
  i=0;

  while(args[i] != NULL){
    i++;
  }

  printf("argc = %d, args = ",i-1);
  for(k=1; k<i-1;k++){
    printf("%s,",args[k]);
  }

  printf("%s\n",args[i-1] );
}
