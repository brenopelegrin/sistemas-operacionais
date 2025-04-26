#include"queuelkdlist.h"
#include<stdlib.h>
#include<stdio.h>

LkdList* lkdlist_createList(int *flag){
    LkdList* list = (LkdList*) malloc(sizeof(LkdList));
    if(list == NULL){
        *flag = LKDLIST_ERROR_CANT_ALLOCATE_LIST;
        return NULL;
    }
    
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void lkdlist_addItem(LkdList* list, void* value, int* flag){
    Node* newNode = (Node*) malloc(sizeof(Node));

    if(newNode == NULL){
        *flag = LKDLIST_ERROR_CANT_ALLOCATE_NODE;
        return;
    }

    newNode->data = value;
    newNode->next = NULL;
    
    if(list->size == 0){
        list->first = newNode;
        list->last = newNode;
    } else{
        list->last->next = newNode;
        list->last = newNode;
    }

    list->size++;
    *flag = LKDLIST_SUCCESS;
    return;
}

void lkdlist_addItemLeft(LkdList* list, void* value, int* flag){
    Node* newNode = (Node*) malloc(sizeof(Node));

    if(newNode == NULL){
        *flag = LKDLIST_ERROR_CANT_ALLOCATE_NODE;
        return;
    }

    newNode->data = value;
    newNode->next = NULL;
    
    if(list->size == 0){
        list->first = newNode;
        list->last = newNode;
    } else{
        newNode->next = list->first;
        list->first = newNode;
    }

    list->size++;
    *flag = LKDLIST_SUCCESS;
    return;
}

Node* lkdlist_getNode(LkdList* list, int idx, int *flag){
    // idx must be in [0, size]
    // if size == 0, throw error

    // this if also comprehends the case where list->size == 0,
    // because when list->size == 0; idx = 0 comprehends idx = list->size,
    // which throws out of bounds error.
    if(idx < 0 || idx >= list->size){
        *flag = LKDLIST_ERROR_INDEX_OUT_OF_BOUNDS;
        return NULL;
    } else{
        Node* currNode = list->first;
        for(int i=0; i<idx; i++){
            //currNode = nextNode
            currNode = currNode->next;
        }
        return currNode;
    }
}

void* lkdlist_getData(LkdList* list, int idx, int *flag){
    Node* currNode = lkdlist_getNode(list, idx, flag);
    if(currNode == NULL){
        return NULL;
    }
    return &currNode->data;
}

void lkdlist_removeItem(LkdList* list, int idx, int* flag){
    // idx must be in [0, size]
    // if size == 0, throw error

    // this if also comprehends the case where list->size == 0,
    // because when list->size == 0; idx = 0 comprehends idx = list->size,
    // which throws out of bounds error.
    if(idx < 0 || idx >= list->size){
        *flag = LKDLIST_ERROR_INDEX_OUT_OF_BOUNDS;
        return;
    }
    if(idx > 0){
        Node* prevNode = lkdlist_getNode(list, idx-1, flag);
        if(prevNode == NULL){
            return;
        }
        
        Node* toBeFreed = prevNode->next;
        prevNode->next = toBeFreed->next;
        free(toBeFreed);
        list->size--;
        *flag = LKDLIST_SUCCESS;
        return;
    } else{
        // idx == 0
        Node* currNode = lkdlist_getNode(list, idx, flag);
        if(currNode == NULL){
            return;
        }
        list->first = currNode->next;
        free(currNode);
        list->size--;
        *flag = LKDLIST_SUCCESS;
        return;
    }
}

void lkdlist_printList(LkdList* list, int* flag){
    for(int i=0; i<list->size; i++){
        Node* currNode = lkdlist_getNode(list, i, flag);
        printf("%p\n", currNode->data);
    }
}

void lkdlist_deleteList(LkdList* list){
    Node* currNode = list->first;
    for(int i=0; i<list->size; i++){
        if(currNode->next != NULL){
            Node* prevNode = currNode;
            currNode = currNode->next;
            free(prevNode);
        }
    }
    free(currNode);
    free(list);
    return;
}

LkdList* lkdlist_createReversedList(LkdList* old, int* flag){
    LkdList* newList = lkdlist_createList(flag);
    if(newList == NULL){
        return NULL;
    }
    Node* currNode = old->first;
    for(int i=0; i<old->size; i++){
        lkdlist_addItemLeft(newList, currNode->data, flag);
        currNode = currNode->next;
    }
    return newList;
}

int lkdlist_isOnList(LkdList* list, void* value){
    Node* currNode = list->first;
    for(int i=0; i<list->size; i++){
        if(currNode->data == value){
            return 1;
        }
        if(currNode->next == NULL){
            break;
        }
        currNode = currNode->next;
    }
    return 0;
}

int lkdlist_getListSize(LkdList* list){
    return list->size;
}

int lkdlist_isEmpty(LkdList* list){
    if(list->size == 0){
        return 1;
    } else{
        return 0;
    }
}

void lkdlist_setData(LkdList* list, int idx, void* value, int *flag){
    Node* currNode = lkdlist_getNode(list, idx, flag);
    if(currNode == NULL){
        return;
    }
    currNode->data = value;
    return;
}

// Below we have the Queue implementation

Queue* queue_create(int *flag){
    Queue *Q=(Queue*)malloc(sizeof(Queue));
    if(Q==NULL){
        *flag=MEMORY_ALLOCATION_ERROR;
        return NULL;
    }
    Q->created = 1;
    Q->l = (LkdList*) lkdlist_createList(flag);
    return Q;
}


void queue_insert(Queue *Q, void* ele, int *flag){
    if(Q->created!=1){
        *flag=CREATION_ERROR;
        return;
    }
    lkdlist_addItem(Q->l, ele, flag);
    return;
}

void* queue_pop(Queue *Q, int *flag){
    void* ele = lkdlist_getData(Q->l,0,flag);
    lkdlist_removeItem(Q->l,0,flag);
    return ele;
}


int queue_len(Queue *Q,int *flag){
    if(Q->created != 1){
        *flag=CREATION_ERROR;
        return -1;
    } else{
        *flag=PROCESS_SUCESS;
        return lkdlist_getListSize(Q->l);
    }
}


int queue_haveElement(Queue *Q, void* ele, int *flag){
    Node *p;

    if(Q->created!=1){
        *flag=CREATION_ERROR;
        return -1;
    }
    if(queue_len(Q,flag)==0){
        *flag=PROCESS_SUCESS;
        return 0;
    }
    p=lkdlist_getNode(Q->l,0,flag);
    if(*flag!=LKDLIST_SUCCESS)
    return -1;
    *flag=PROCESS_SUCESS;
    while (p!=NULL){
        if(p->data==ele){
            return 1;
        }
        p=p->next;
    }
    return 0;
}


void queue_invert(Queue *Q, int *flag){
    LkdList *L=lkdlist_createReversedList(Q->l,flag);
    lkdlist_deleteList(Q->l);
    Q->l=L;
    return;
}

void queue_printQueue(Queue* Q, int* flag){
    lkdlist_printList(Q->l, flag);
    return;
}

void* queue_getElement(Queue *Q, int n, int *flag){
    void* val = lkdlist_getData(Q->l, n, flag);
    return val;
}

void queue_cleanQueue(Queue *Q, int *flag){
    lkdlist_deleteList(Q->l);
    LkdList *L=lkdlist_createList(flag);
    Q->l=L;
    return;
}

void queue_deleteQueue(Queue *Q){
    lkdlist_deleteList(Q->l);
    free(Q);
    return;
}