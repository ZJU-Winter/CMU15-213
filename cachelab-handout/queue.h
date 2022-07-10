#ifndef QUEUE_HEAD
#define QUEUE_HEAD

#include "csim.h"

typedef struct node
{
    cacheLine* elem;
    struct node* next;
} node, *node_ptr;

typedef struct
{
    int size;
    node_ptr head;
    node_ptr tail;
} queue;

queue* create();
void insert_tail(queue* q);

#endif