#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "lib-queuelkdlist/queuelkdlist.h"

// Supondo as seguintes variáveis globais já estejam definidas em outro lugar
extern int buffer_size;
extern void** buffer; // ponteiros genéricos
extern Queue* fila_livres;   // fila 1: endereços livres
extern Queue* fila_ocupados; // fila 2: endereços ocupados
extern pthread_mutex_t mutex_livres;
extern pthread_mutex_t mutex_ocupados;
extern sem_t sem_empty;
extern sem_t sem_full;

// Função do produtor
void* produtor(void* arg) {
  int flag;

  while (1) {
      // 1. Espera até que haja um espaço livre na fila
      sem_wait(&sem_empty);

      // 2. Entra na região crítica da fila de endereços livres
      pthread_mutex_lock(&mutex_livres);
      void* endereco_ptr = queue_remove(fila_livres, &flag);
      pthread_mutex_unlock(&mutex_livres);

      if (flag != PROCESS_SUCESS || endereco_ptr == NULL) {
          fprintf(stderr, "Erro ao remover endereço livre da fila.\n");
          continue;
      }

      int index = *((int*) endereco_ptr); // índice do buffer

      // 3. Simula produção: escreve algo no buffer
      //    (alocamos um inteiro com valor aleatório)
      int* novo_item = malloc(sizeof(int));
      *novo_item = rand() % 1000; // valor aleatório
      buffer[index] = novo_item;

      printf("[Produtor] Produziu valor %d no buffer[%d]\n", *novo_item, index);

      // 4. Entra na região crítica da fila de endereços ocupados
      pthread_mutex_lock(&mutex_ocupados);
      queue_insert(fila_ocupados, endereco_ptr, &flag);
      pthread_mutex_unlock(&mutex_ocupados);

      if (flag != PROCESS_SUCESS) {
          fprintf(stderr, "Erro ao inserir endereço ocupado na fila.\n");
          // Iremos gerenciar a memória alocada? Se sim, seria aqui
          continue;
      }

      // 5. Sinaliza que há um novo item disponível para consumo
      sem_post(&sem_full);
  }

  return NULL;
}

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
    void* ptr = malloc(sizeof(int));
    queue_insert(Q, ptr, &flag);
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

