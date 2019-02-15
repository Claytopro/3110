#include <fcntl.h> // open
#include <unistd.h> // read
#include <sys/types.h> // read
#include <sys/uio.h> // read
#include <stdio.h> // fopen, fread
#include <stdlib.h>
#include <sys/time.h> // gettimeofday



struct timeval start, end; // maintain starting and finishing wall time.

void startTimer( ) { // memorize the starting time
  gettimeofday( &start, NULL );}

void stopTimer( char *str ) { // checking the finishing time and computes the elapsed time
  gettimeofday( &end, NULL );
  printf("%s's elapsed time\t= %ld\n",str, ( end.tv_sec - start.tv_sec ) * 1000000 +
  (end.tv_usec - start.tv_usec));
}



int main( int argc, char *argv[] ) {
  int typeofcalls,file;
  int byteSize;
  char* filename;
  char* buffer;
  char in;
  FILE *fp;
// validate arguments
// implementation
// Parsing the arguments passed to your C program
// Including the number of bytes to read per read( ) or fread( ), and
// the type of i/o calls used
// implementation

  filename = argv[1];
  byteSize = atoi(argv[2]);
  typeofcalls = atoi(argv[3]);
  buffer = malloc(sizeof(char)*byteSize+1);
  if (typeofcalls == 1) {
    // Use unix I/O system calls to
  // implementation
  startTimer();
  file =open(filename,O_RDONLY,0);
  while(read(file,buffer,byteSize) != 0){}

  close(file);
  stopTimer(filename);

  } else if (typeofcalls == 0) {
    // Use standard I/O
    // implementation
    fp = fopen(filename,"r");
    if(byteSize == 1){
      startTimer();
      while((in = fgetc(fp)) != EOF){}
    }else{

      startTimer();
      while(!feof(fp)){
        fread(buffer,byteSize,1,fp);
      }
    }
    stopTimer(filename);
  }


  return 1;
}
