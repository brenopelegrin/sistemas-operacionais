#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


#define TEST_STRING "Testing string to write blah blah blah\n"
#define MAX_INPUT_SIZE 256
#define FILE_NAME "test_file.txt"
#define N_ITERATIONS 100
#define FILE_OPEN_SUCCESS "File opened successfully!\n"
#define FILE_CLOSE_SUCCESS "File closed successfully!\n"
#define WRITE_SUCESS "Written succesfully!\n"


/**
 * Function: main
 * --------------
 * Calls the open(), close(), write(), read() syscalls for demonstration
 *
 * Returns:
 *   0 - executed with success.
 *  -1 - executed with error.
 */
int main(int argc, char* argv[]){
  const int STDIN = 0;

  // Opening/creating a file
  // O_CREAT: creates file if it doesn't exist
  // O_WRONLY: write only
  // O_TRUNC: erases content if the file already exists
  int fd = open(FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC);
  
  // Error handling
  if (fd == -1){
    exit(-1);
  }else{
    write(STDOUT_FILENO, FILE_OPEN_SUCCESS, sizeof(FILE_OPEN_SUCCESS));
  }

  int nBytesWritten;
  int totalBytes = 0;

  // Writes a string TEST_STRING N_ITERATIONS times in the opened file
  for(int i = 0; i<N_ITERATIONS; i++){
    nBytesWritten = write(fd, TEST_STRING, sizeof(TEST_STRING));
    if (nBytesWritten == -1) exit(-1);  
    totalBytes += nBytesWritten;
  }

  write(STDOUT_FILENO, WRITE_SUCESS, sizeof(WRITE_SUCESS));


  // Closing the file
  int fstatus = close(fd);

  // Error handling
  if(fstatus == -1){
    exit(-1);
  }else{
    write(STDOUT_FILENO, FILE_CLOSE_SUCCESS, sizeof(FILE_CLOSE_SUCCESS));
  }



  // Opening file for 2nd time
  int fd2 = open(FILE_NAME, O_RDONLY);
  
  // Error handling
  if (fd2 == -1){
    exit(-1);
  }else{
    write(STDOUT_FILENO, FILE_OPEN_SUCCESS, sizeof(FILE_OPEN_SUCCESS)); 
  }

  char buffer[MAX_INPUT_SIZE];


  // Reading from the file previously created
  int nBytesRead;

  write(STDOUT_FILENO, "Reading from file:\n", sizeof("Reading from file:\n"));
  nBytesRead = read(fd2, buffer, sizeof(buffer));
  write(STDOUT_FILENO, buffer, nBytesRead);
  write(STDOUT_FILENO, "\n", sizeof("\n"));
  
  // Closing the file
  int fstatus2 = close(fd2);

  // Error handling
  if (fstatus2 == -1){
    exit(-1);
  }else{
    write(STDOUT_FILENO, FILE_CLOSE_SUCCESS, sizeof(FILE_CLOSE_SUCCESS));
  } 

  return 0;

}