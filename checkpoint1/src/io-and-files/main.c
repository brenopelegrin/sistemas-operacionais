#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open() flags
#include <unistd.h>
#include <errno.h> // errors

#define TEST_STRING "Testing string to write blah blah blah"
#define MAX_INPUT_SIZE 256
#define FILE_NAME "test_file.txt"
#define N_ITERATIONS 1000

/**
 * Function: main
 * --------------
 * Calls the open(), read(), write(), close() syscalls for demonstration
 *
 * Returns:
 *   0 - executed with success.
 *  -1 - executed with error.
 */
int main(int argc, char* argv[]){
  const int STDIN = 0;

  // Opening/creating a file
  int fd = open(FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC);
  
  if (fd == -1){
    exit(-1);

  }else{
    printf("File %s opened successfully!\n", FILE_NAME);
  }

  int nBytesWritten;
  for(int i = 0; i<N_ITERATIONS; i++){
    nBytesWritten = write(fd, TEST_STRING, sizeof(TEST_STRING));
    if (nBytesWritten == -1) exit(-1);  
  }
  printf("Test string written %d times!\n", N_ITERATIONS);

  // Closing the file
  int fstatus = close(fd);

  if(fstatus == -1){
    // check errno
    exit(-1);
  }else{
    printf("File closed successfully!\n");
  }

  // Opening file for 2nd time
  int fd2 = open(FILE_NAME, O_RDONLY);
  if (fd2 == -1){
    exit(-1);
  }else{
    printf("File opened successfully!\n");
  }

  char buffer[1080];
  // Reading the file
  int nBytesRead = read(fd2, buffer, MAX_INPUT_SIZE);
  if (nBytesRead == -1){
    exit(-1);
  }else{
    printf("File contents of %s: %s%s\n", FILE_NAME, buffer);
  }

  // Closing the file
  int fstatus2 = close(fd2);

  if (fstatus2 == -1){
    exit(-1);
  }else{
    printf("File closed successfully!\n");
  } 

  return 0;

}