#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "lib-queuelkdlist/queuelkdlist.h"

#define BUFFER_SIZE 10
#define PRODUCER_WAIT_TIME_US 200000 // 20 milisegundos
#define CONSUMER_WAIT_TIME_US 100000 // 10 milisegundos

int* buffer[BUFFER_SIZE];
int flag;

// Semáforos e mutexes
sem_t sem_empty; // conta quantos espaços livres existem
sem_t sem_full;  // conta quantos espaços ocupados existem
sem_t sem_mutex; // exclusão mútua para seções críticas

Queue* Q_available;
Queue* Q_occupied;

/**
 * @brief Função da thread produtora.
 *
 * Pega um endereço da fila de endereços livres (int*), produz um item (do tipo int),
 * e armazena-o no endereço livre, e então move o endereço da fila de endereços
 * livres para a fila de endereços ocupados.
 *
 * Sincronização:
 * - Espera por um slot livre usando `sem_empty`.
 * - Entra na seção crítica protegida por `sem_mutex`.
 * - Sinaliza a disponibilidade de um novo item usando `sem_full`.
 *
 * @param arg Não utilizado. Existe apenas para satisfazer a assinatura de função do pthread.
 * @return Sempre retorna NULL.
 */
void* producer(void* arg) {
  int local_generation_count = 0; // contador usado para gerar valores diferentes para cada item

  while (1) {
    sem_wait(&sem_empty); // espera por espaço livre
    sem_wait(&sem_mutex); // entra na seção crítica

    // Início da seção crítica
    int* available_ptr = queue_pop(Q_available, &flag); // tira o endereço da fila de livres
    
    // Início da produção
    /* 
      Aqui estamos apenas produzindo um valor inteiro baseado em um contador, mas poderíamos
      produzir algo útil para algum cálculo, por exemplo.
    */
    *available_ptr = local_generation_count;
    // Fim da produção

    local_generation_count++; // incrementa contador 

    queue_insert(Q_occupied, available_ptr, &flag); // coloca o endereço da fila de ocupados
    printf("[debug] produtor: item produzido, endereço %p, valor %d.\n", (void*)available_ptr, *available_ptr);
    // Fim da seção crítica
    
    sem_post(&sem_mutex); // sai da seção crítica
    sem_post(&sem_full);  // sinaliza que há item disponível

    usleep(PRODUCER_WAIT_TIME_US);
  }
  return NULL;
}

/**
 * @brief Função da thread consumidora.
 *
 * Consome itens (do tipo int) na fila de endereços ocupados (int*), e move 
 * o item da fila de endereços ocupados de volta para a fila de endereços livres.
 *
 * Sincronização:
 * - Espera por um item disponível usando `sem_full`.
 * - Entra na seção crítica protegida por `sem_mutex`.
 * - Sinaliza a disponibilidade de um endereço livre usando `sem_empty`.
 *
 * @param arg Não utilizado. Existe apenas para satisfazer a assinatura de função do pthread.
 * @return Sempre retorna NULL.
 */
void* consumer(void* arg) {
  int* item_ptr;
  int item_value;
  while (1) {
    sem_wait(&sem_full);  // espera por item disponível
    sem_wait(&sem_mutex); // entra na seção crítica

    // Início da seção crítica
    item_ptr = queue_pop(Q_occupied, &flag); // tira o endereço da fila de ocupados

    // Realiza operação com o item consumido
    /* 
      Aqui não fazemos nada com o item consumido (item_value), mas poderíamos utilizá-lo
      para algum cálculo, por exemplo.
    */
    item_value = *item_ptr;
    // Finaliza operação com o item consumido

    queue_insert(Q_available, item_ptr, &flag); // coloca o endereço na fila de livres
    printf("[debug] consumidor: item consumido, endereço %p, valor %d.\n", item_ptr, *item_ptr);
    // Fim da seção crítica
    
    sem_post(&sem_mutex); // sai da seção crítica
    sem_post(&sem_empty); // sinaliza que há espaço livre

    usleep(CONSUMER_WAIT_TIME_US);
  }
  return NULL;
}

/**
 * @brief Função principal para inicializar e executar o sistema do produtor-consumidor.
 *
 * Esta função realiza os seguintes passos:
 * - Cria e inicializa duas filas:
 *   - Q_available: fila de ponteiros para endereços livres.
 *   - Q_occupied: fila de ponteiros para endereços ocupados.
 * - Aloca e inicializa os itens do buffer, inserindo os endereços na Q_available.
 * - Inicializa três semáforos:
 *   - sem_empty: conta os itens livres no buffer (valor inicial = BUFFER_SIZE).
 *   - sem_full: conta os itens ocupados no buffer (valor inicial = 0).
 *   - sem_mutex: semáforo binário (mutex) para exclusão mútua em seções críticas.
 * - Cria duas threads:
 *   - Uma thread consumidora que consome itens continuamente.
 *   - Uma thread produtora que produz itens continuamente.
 * - Aguarda ambas as threads finalizarem (elas executam indefinidamente).
 * - Caso o programa termine (se as threads terminarem devido a algum erro), destrói semáforos, deleta as filas e libera a memória.
 *
 * @param argc Contador de argumentos (não utilizado).
 * @param argv Vetor de argumentos (não utilizado).
 * @return Retorna EXIT_SUCCESS na execução normal (é executado somente se as threads tiverem algum erro).
 */
int main(int argc, char *argv[]){
  // Cria a fila de endereços disponíveis
  Q_available = queue_create(&flag);
  if(Q_available == NULL){
      printf("[erro] fila: não foi possível criar a fila de endereços livres.\n");
      return -1;
  }

  // Cria a fila de endereços ocupados
  Q_occupied = queue_create(&flag);
  if(Q_occupied == NULL){
      printf("[erro] fila: não foi possível criar a fila de endereços ocupados.\n");
      return -1;
  }

  // Inicializa o buffer alocando pointeiros para inteiros
  for(int i=0; i<BUFFER_SIZE; i++){
    int* ptr = (int*) malloc(sizeof(int));
    *ptr = -1;
    buffer[i] = ptr;

    /* Insere endereços livres na fila de livres */
    queue_insert(Q_available, ptr, &flag);
    
    if (flag != 1){
      printf("[error] fila: falha ao adicionar item na fila.\n");
    }
    
    printf("[debug] alocado pointeiro int: endereço %p, valor %d.\n", ptr, *ptr);
  }
  printf("[debug] printando fila Q_available:\n");
  queue_printQueue(Q_available, &flag);

  // Inicializa semáforos e mutexes
  sem_init(&sem_empty, 0, BUFFER_SIZE); // inicialmente todos espaços estão vazios
  sem_init(&sem_full, 0, 0);            // nenhum item disponível no início
  sem_init(&sem_mutex, 0, 1);           // binário: controla acesso à seção crítica

 // Cria as threads do produtor e consumidor
  printf("\n[info] criando threads para produtor e consumidor...\n");
  pthread_t consumer_thread, producer_thread;
  int iret1, iret2;
  
  iret2 = pthread_create(&consumer_thread, NULL, consumer, NULL);
  if(iret2 != 0){
      fprintf(stderr,"[erro] pthread_create: erro ao criar thread `consumer_thread`, return code=%d\n", iret2);
      exit(EXIT_FAILURE);
  }

  iret1 = pthread_create(&producer_thread, NULL, producer, NULL);
  if(iret1 != 0){
      fprintf(stderr,"[erro] pthread_create: erro ao criar thread `producer_thread`, return code=%d\n", iret1);
      exit(EXIT_FAILURE);
  }

  // Aguarda as threads
  pthread_join(consumer_thread, NULL);
  pthread_join(producer_thread, NULL);

  // Destroí semáforos e mutexes
  sem_destroy(&sem_empty);
  sem_destroy(&sem_full);
  sem_destroy(&sem_mutex);

  // Destrói filas
  queue_deleteQueue(Q_available);
  queue_deleteQueue(Q_occupied);

  // Destrói os endereços alocados para o buffer
  for(int i=0; i<BUFFER_SIZE; i++){
    free(buffer[i]);
  }

  exit(EXIT_SUCCESS);
}

