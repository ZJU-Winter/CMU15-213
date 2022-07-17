/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new() {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (!q)
        return;
    list_ele_t *next;
    list_ele_t *ptr = q->head;
    while (ptr) {
        next = ptr->next;
        free(ptr->value);
        free(ptr);
        ptr = next;
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s) {
    if (!q)
        return false;
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    newh->next = NULL;
    if (!newh->value)
        return false;
    
    strcpy(newh->value, s);
    if (q->size == 0) {
        q->tail = newh;        
    }
    newh->next = q->head;
    q->head = newh;
    q->size += 1;
    return true;
}

/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q)
        return false;
    list_ele_t *newtail = malloc(sizeof(list_ele_t));
    if (!newtail)
        return false;
    
    newtail->value = malloc((strlen(s) + 1) * sizeof(char));
    newtail->next = NULL;
    if (!newtail->value)
        return false;
    strcpy(newtail->value, s);

    if (q->size == 0) {
        q->head = newtail;
        q->tail = newtail;
    } else {
        q->tail->next = newtail;
        q->tail = newtail;
    }    
    q->size += 1;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize) {
    if (!q || !q->head)
        return false;
    list_ele_t* head = q->head;
    char* value = head->value;
    int i = 0;
    for (i = 0; *value != '\0' && i < bufsize - 1; i += 1, value += 1) {
        *(sp + i) = *value;
    }
    *(sp + i) = '\0';

    q->head = q->head->next;
    q->size -= 1;
    free(head->value);
    free(head);
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if (!q || !q->head)
        return;
    list_ele_t *next = q->head;
    list_ele_t *head = NULL;
    list_ele_t *reminder = NULL;
    while (next) {
        reminder = next->next;
        next->next = head;
        head = next;
        next = reminder;
    }
    q->tail = q->head;
    q->head = head;
}
