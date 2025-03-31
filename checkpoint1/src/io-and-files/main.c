#include<stdio.h>
#include<stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open() flags
#include <unistd.h>
#include <errno.h> // errors


#define TEST_STRING "A string to be written to the files."
#define MAX_INPUT_SIZE 256
#define FILE_NAME "test_file.txt"
#define N_ITERATIONS 1000



int main(int argc, char* argv[]){
  
  const int STDIN = 0;
//   char fileNameBuffer[MAX_INPUT_SIZE];

//  printf("Hello world!\n");
//  printf("Give me a small file name, including the extension: \n");

//   int nRead = read(STDIN, fileNameBuffer, MAX_INPUT_SIZE);
//   if (nRead == -1){
//     // catch errors
//     exit(-1);  
//   }else{
//     printf("File name received: %s", fileNameBuffer);
//   }
  
//   printf("\n");
  // abrindo/criando um arquivo
  int fd = open(FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC);
  
  if (fd == -1){
    exit(-1);

  }else{
    printf("File %s opened successfully!\n", FILE_NAME);
  }

  // char buffer[MAX_INPUT_SIZE];

  // int nBytesRead = read(STDIN, buffer, MAX_INPUT_SIZE); 
  // if(nBytesRead == -1){
  //   exit(-1);
  // }else{  
  //   printf("String to be written: %s\n", buffer);
  // }
  int nBytesWritten;
  for(int i = 0; i<N_ITERATIONS; i++){
    nBytesWritten = write(fd, TEST_STRING, sizeof(TEST_STRING));
    if (nBytesWritten == -1) exit(-1);  
  }
  printf("Test string written %d times!\n", N_ITERATIONS);



  // // fsync 


  // // closing the file
  int fstatus = close(fd);

  if(fstatus == -1){
    // check errno
    exit(-1);
  }else{
    printf("File closed successfully!\n");
  }

  int fd2 = open(FILE_NAME, O_RDONLY);
  if (fd2 == -1){
    exit(-1);
  }else{
    printf("File opened successfully!\n");
  }

  char buffer[1080];
  int nBytesRead = read(fd2, buffer, MAX_INPUT_SIZE);
  if (nBytesRead == -1){
    exit(-1);
  }else{
    printf("File contents of %s: %s%s\n", FILE_NAME, buffer);
  }

  int fstatus2 = close(fd2);

  if (fstatus2 == -1){
    exit(-1);
  }else{
    printf("File closed successfully!\n");
  } 

  return 0;

}