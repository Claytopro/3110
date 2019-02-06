/*by:Clayton Provan
* https://linux.die.net/man/3/execvp
* sources: http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html
*/

#include <stdlib.h>
#include "stdio.h"
#include <ctype.h>
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <signal.h>
#include <pwd.h>
#include <math.h>


void executeWait(char **arg, int fileBool);
void executeBackground(char **arg, int fileBool);
void GCD(char** args);
void argCount(char **args);
char * getUserName(uid_t uid);
int isHex(int c);
int power(int base,int exponent);
int gcdRECURSIVE(int x, int y);
void findDay(char**);

const char *mypath[] = { "./",
"/usr/bin/", "/bin/", NULL
};
extern char **getln();

int main(){

  /*used for parsing input */
  char hostName[100];
  char *userID;
  char **args;
  char *argument1 = "NULL";
  int i =0;
  int fileBool=0;


  //userID = malloc(sizeof(char)*100);
  //userID = getUser(getuid());
  userID = getUserName(getuid());

  gethostname(hostName,100);
  /*contineus to get arguments as long as exit is not input*/
  while (1)
  {
  /* Wait for input */
    printf ("[%s@%s]$",userID,hostName);
    args = getln();

    if( args[0] != NULL){
      argument1 = args[0];
    }

//    printf("arg 1:%s\narg2:%s\n",argument1,args[1] );

    if(strcmp(argument1,"exit") == 0){
      exit(1);
    }

    if(strcmp(argument1,"args")==0){
      argCount(args);
    }else if(strcmp(argument1,"gcd")==0){
      GCD(args);
    }else if(strcmp(argument1,"day")==0){
      findDay(args);
    }else{

      while(args[i] != NULL) {
        if(strcmp(args[i], ">")==0){
          fileBool =i;
        }
        i++;
      }
      //brings i back into range
      i--;

      if(strcmp(args[i], "&") == 0){
        args[i] = NULL;
        executeBackground(args,fileBool);
      }else{
        executeWait(args,fileBool);
      }

    }


  }

  return 0;
}

void executeWait(char **arg, int fileBool){
  pid_t  pid;
  int    status;
  FILE *fp;

  if ((pid = fork()) < 0) {
       printf("A: forking child process failed\n");
       exit(1);
  }  /* cildprocess should have process id of zero.*/
  else if (pid == 0) {
       /*cool thing to do is run the shell inside the shell */
       // > or < sign was found at fileBool location in arg
       if(fileBool != 0){
         fp = freopen(arg[fileBool+1], "w+", stdout);
         arg[fileBool] = NULL;

       }

       if (execvp(*arg, arg) < 0) {
            printf("ALERT: exec failed\n");
            exit(1);
       }
       fclose(fp);
  }
  else {
       //loops through until child process is done, waiting for child execution
       while (wait(&status) != pid){}
    }

}

void executeBackground(char **arg,int fileBool){
  pid_t  pid;

  if ((pid = fork()) < 0) {
       printf("A: forking child process failed\n");
       exit(1);
  }  /* cildprocess should have process id of zero.*/
  else if (pid == 0){
       /*cool thing to do is run the shell inside the shell */
       if (execvp(*arg, arg) < 0) {
            printf("ALERT: exec failed\n");
            exit(1);
       }
  }
  //dont have to wait for command to end before coninuing.

}

//find gcd of entered values
void GCD(char** args){
  char *num;

  int  first,second,numlength,hexCount;

  //if there are to many arugments
  if(args[3] != NULL || args[2] ==NULL || args[1]==NULL) {
    printf("INVALID NUMBER OF ARGUMENTS\n");
    return ;
  }

  num = args[1];
  hexCount =0;
  first =0;


  //num1 is hexadecimal
  if(num[0] == '0' && num[1] == 'x'){
    numlength = strlen(num);

    while(numlength >2){
      numlength--;
      if(isdigit(num[numlength]) > 0){
        first += (num[numlength] - 48) * power(16,hexCount);

        hexCount++;

      }else if(isHex(num[numlength])!=0){

        first += isHex(num[numlength])*power(16,hexCount);
        hexCount++;
      }else{
        printf("INVALID NUM");
        return;
      }

    }
    //number is decimal
  }else{
    numlength = strlen(num);
    first = atoi(num);
    while(numlength>0){
      numlength--;
      if(isdigit(num[numlength]) == 0){
        printf("INVALID ENTRY\n");
        return;
      }
    }
  }

  //do second number
  num = args[2];
  second =0;
  hexCount =0;

  if(num[0] == '0' && num[1] == 'x'){
    numlength = strlen(num);

    while(numlength >2){
      numlength--;
      if(isdigit(num[numlength]) > 0){
        second += (num[numlength] - 48) * power(16,hexCount);

        hexCount++;
      }else if(isHex(num[numlength])!=0){

        second += isHex(num[numlength])*power(16,hexCount);
        hexCount++;
      }else{
        printf("INVALID NUM");
        return;
      }

    }
    //number is decimal
  }else{
    numlength = strlen(num);
    second = atoi(num);
    while(numlength>0){
      numlength--;
      if(isdigit(num[numlength]) == 0){
        printf("INVALID ENTRY \n");
        return;
      }
    }
  }


  printf("GCD(%s,%s) = %d\n",args[1],args[2],gcdRECURSIVE(first,second));

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

//checks if it is a hexadecimal letter A-F, return if false
//returns value of letter if it is valid
int isHex(int c){
  // valid lower case hex
  if(c <103 && c>96){
    return c-87;
  }
  // valid uppercase hex
  if(c<71 && c > 64){
    return c-54;
  }

  return 0;
}

//gets username of user woohoo
char * getUserName(uid_t uid){

  struct passwd *password = getpwuid(uid);
  if (password != NULL)
  {
    return password->pw_name;
  }

  return "";
}

int power(int base,int exponent){
  int i =0;
  int inbase;
  if(exponent == 0) return 1;
  inbase =1;
  for(i=0; i<exponent; i++){
    inbase = inbase*base;
  }

  return inbase;
}

int gcdRECURSIVE(int x, int y){

  if(x==0){
    return x;
  }

  if(y ==0){
    return y;
  }

  if(x ==y){
    return x;
  }

  if(x>y){
    return(gcdRECURSIVE((x-y),y));
  }else{
    return gcdRECURSIVE(x,(y-x));
  }

}

void findDay(char** args){
  char year[4];
  char yearCen[4];
  int month,day,yearVal,yearCentury;
  int weekDay;
  float f;



  if(args[4]!=NULL){
    printf("ERROR:TOO MANY ENTRIES\n");
  }

  if(args[1] == NULL){
    printf("INVALID ENTRY\n");
    return;
  }else{
    day = atoi(args[1]);
  }


  if(args[3] == NULL|| strlen(args[3])>4){
    printf("INVALID ENTRY\n");
    return;
  }else{

    strncpy(yearCen, args[3], 2);
    yearCentury = atoi(yearCen);

    strncpy(year, args[3]+2, 2);
    yearVal = atoi(year);
  }

    if(args[2] == NULL){
      printf("INVALID ENTRY\n");
      return;
    }else{
      month = atoi(args[2]);
      if(month < 3){
        month = month +10;
        yearVal--;
      }else{
        month = month - 2;
      }
    }

  f = day + floor((13*month-1)/5) + yearVal + floor(yearVal/4) + floor(yearCentury/4) - (2*yearCentury);


  weekDay = ((int)f)%7;

  switch (weekDay) {
    case 1:
    printf("It was a Monday\n");
    break;
    case 2:
    printf("It was a Tuesday\n");
    break;
    case 3:
    printf("It was a Wednesday\n");
    break;
    case 4:
    printf("It was a Thurday\n");
    break;
    case 5:
    printf("It was a Friday\n");
    break;
    case 6:
    printf("It was a Saturday\n");
    break;
    case 0:
    printf("It was a Sunday\n");
    break;
    default:
    printf("something went wrong. I dont know what day it is\n");
  }

}
