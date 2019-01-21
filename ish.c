/*
*
*/

#include "stdio.h"

const char *mypath[] = { "./",
"/usr/bin/", "/bin/", NULL
};


int main(){

  /*used for parsing input */
  char *argument = NULL;

  /*contineus to get arguments as long as exit is not input*/
  while (strcmp(argument,"exit") != 0)
  {
  /* Wait for input */
  printf ("prompt> ");
  fgets (argument,s ,stdin );

  /* Parse input */
  while (( ... = strsep (...)) != NULL)
  {
  ...
  }

  /* If necessary locate executable using mypath array */

  /* Launch executable */
  if (fork () == 0) {

  } else {

  } // end of if

  }

  return 0;
}
