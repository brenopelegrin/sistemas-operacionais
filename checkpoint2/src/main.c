#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "lib-queuelkdlist/queuelkdlist.h"

void *print_message_function( void *ptr ){
  char *message;
  message = (char *) ptr;
  printf("%s \n", message);
}

int main(int argc, char *argv[]){
  int flag;

  /* Criar uma fila */
  Queue* Q = queue_create(&flag);
  if(Q == NULL){
      printf("Erro: Não foi possível criar a fila.");
      return 0;
  }
  for (int i=0; i<10; i++){
    queue_insert(Q, i, &flag);
    if(flag != PROCESS_SUCESS){
      printf("Ocorreu um problema inesperado\n");
      exit(EXIT_FAILURE);
    }
  }

  printf("Printing a test queue with 10 elements:\n");

  queue_printQueue(Q, &flag);
  queue_deleteQueue(Q);

  printf("\nCreating threads to print hello world...\n");

  pthread_t thread1, thread2;
  const char *message1 = "Thread 1 - hello world";
  const char *message2 = "Thread 2 - hello world";
  int  iret1, iret2;
  
  /* Create independent threads each of which will execute function */
  iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
  if(iret1)
  {
      fprintf(stderr,"Error - pthread_create() return code: %d\n", iret1);
      exit(EXIT_FAILURE);
  }

  iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);
  if(iret2)
  {
      fprintf(stderr,"Error - pthread_create() return code: %d\n", iret2);
      exit(EXIT_FAILURE);
  }

  printf("pthread_create() for thread 1 returns: %d\n", iret1);
  printf("pthread_create() for thread 2 returns: %d\n", iret2);

  /* Wait till threads are complete before main continues. Unless we  */
  /* wait we run the risk of executing an exit which will terminate   */
  /* the process and all threads before the threads have completed.   */
  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);

  exit(EXIT_SUCCESS);
}

