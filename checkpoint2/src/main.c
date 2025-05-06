#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "lib-queuelkdlist/queuelkdlist.h"

#define BUFFER_SIZE 10
#define PRODUCER_WAIT_TIME_US 300000 // 300 milisegundos
#define CONSUMER_WAIT_TIME_US 150000 // 150 milisegundos

int* buffer[BUFFER_SIZE];
int flag;

// Mutexes
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Queue* Q_livre;
Queue* Q_ocupado;

void* producer(void* arg) {
  int local_generation_count = 0;
  while (1) {
    pthread_mutex_lock(&mutex); // entra na região crítica

    // Dorme enquanto a fila de livres está vazia (não há o que ser produzido)
    while (queue_len(Q_livre, &flag) == 0) {
      pthread_cond_wait(&cond_empty, &mutex);
    }

    // Início da região crítica
    int* available_ptr = queue_pop(Q_livre, &flag);
    *available_ptr = local_generation_count;

    local_generation_count = (local_generation_count + 1) % BUFFER_SIZE;

    queue_insert(Q_ocupado, available_ptr, &flag);
    printf("[debug] produtor: item produzido, endereço %p, valor %d.\n", (void*)available_ptr, *available_ptr);
    // Fim da região crítica
    
    pthread_cond_broadcast(&cond_full);  // sinaliza que há item disponível
    pthread_mutex_unlock(&mutex); // sai da região crítica
    usleep(PRODUCER_WAIT_TIME_US);
  }
  return NULL;
}

void* consumer(void* arg) {
  int* item;
  while (1) {
    pthread_mutex_lock(&mutex); // entra na região crítica

    // Dorme enquanto a fila de ocupados está vazia (não há o que ser lido)
    while (queue_len(Q_ocupado, &flag) == 0) {
      pthread_cond_wait(&cond_full, &mutex);
    }

    // Início da região crítica
    item = queue_pop(Q_ocupado, &flag);
    queue_insert(Q_livre, item, &flag);
    printf("[debug] consumidor: item consumido, endereço %p, valor %d.\n", item, *item);
    // Fim da região crítica
    
    pthread_cond_broadcast(&cond_empty);  // sinaliza que há espaço livre
    pthread_mutex_unlock(&mutex); // sai da região crítica
    usleep(CONSUMER_WAIT_TIME_US);
  }
  return NULL;
}


int main(int argc, char *argv[]){
  /* Cria a fila de endereços disponíveis */
  Q_livre = queue_create(&flag);
  if(Q_livre == NULL){
      printf("[erro] fila: não foi possível criar a fila de endereços livres.\n");
      return -1;
  }

  /* Cria a fila de endereços ocupados */
  Q_ocupado = queue_create(&flag);
  if(Q_ocupado == NULL){
      printf("[erro] fila: não foi possível criar a fila de endereços ocupados.\n");
      return -1;
  }

  /* Inicializa o buffer alocando pointeiros para inteiros */
  for(int i=0; i<BUFFER_SIZE; i++){
    int* ptr = (int*) malloc(sizeof(int));
    *ptr = -1;
    buffer[i] = ptr;

    /* Insere endereços livres na fila de livres */
    queue_insert(Q_livre, ptr, &flag);
    
    if (flag != 1){
      printf("[error] fila: falha ao adicionar item na fila.\n");
    }
    
    printf("[debug] alocado pointeiro int: endereço %p, valor %d.\n", ptr, *ptr);
  }
  printf("[debug] printando fila Q_livre:\n");
  queue_printQueue(Q_livre, &flag);

  /* Cria as threads do produtor e consumidor */
  printf("\n[info] criando threads para produtor e consumidor...\n");
  pthread_t consumer_thread, producer_thread;
  int iret1, iret2;
  
  iret1 = pthread_create(&producer_thread, NULL, producer, NULL);
  if(iret1){
      fprintf(stderr,"[erro] pthread_create(producer_thread): return code %d\n", iret1);
      exit(EXIT_FAILURE);
  }

  iret2 = pthread_create(&consumer_thread, NULL, consumer, NULL);
  if(iret2){
      fprintf(stderr,"[erro] pthread_create(consumer_thread): return code %d\n", iret2);
      exit(EXIT_FAILURE);
  }

  /* Aguarda as threads */
  pthread_join(consumer_thread, NULL);
  pthread_join(producer_thread, NULL);

  /* Destrói filas */
  queue_deleteQueue(Q_livre);
  queue_deleteQueue(Q_ocupado);

  /* Destrói os endereços alocados para o buffer */
  for(int i=0; i<BUFFER_SIZE; i++){
    free(buffer[i]);
  }

  exit(EXIT_SUCCESS);
}

