#include<stdlib.h>
#include<stdio.h>
#include"lib-queuelkdlist/queuelkdlist.h"

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
      return -1;
    }
  }

  queue_printQueue(Q, &flag);
  queue_deleteQueue(Q);
}