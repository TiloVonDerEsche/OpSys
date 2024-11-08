#ifndef LINKED_LIST
#define LINKED_LIST

#include <stdlib.h>
#include <stdio.h>

// The data that is stored within a node of the doubly-linked list
struct Data {
    int id;
    int length;
};

// Struct for the nodes of the doubly-linked list
struct Node {
    struct Data data;
    struct Node * prev;
    struct Node * next;
};

// Define the structure of the doubly-linked list
struct DoublyLinkedList {
    struct Node * start;
    struct Node * end;
};

// Function to create a new node
// Return a pointer to the node
// Exit if allocation fails.
struct Node* createNode(struct Data data) {
  struct Node* result = calloc(1, sizeof(struct Node));
  result->data = data;

  return result;
}

// Function to initialize a doubly-linked list with NULL
void initializeDoublyList(struct DoublyLinkedList* list) {
  list->start = 0;
  list->end = 0;
}

// Function to print the doubly-linked list in forward direction
void printListForward(const struct DoublyLinkedList* list) {
  struct Node * node_i = list->start;
  while (node_i) {
    printf("ID: %d, Length: %d\n",node_i->data.id,node_i->data.length);

    node_i = node_i->next;
  }

}

// Function to print the doubly-linked list in reverse direction
void printListReverse(const struct DoublyLinkedList* list) {
  struct Node * node_i = list->end;
  while (node_i) {
    printf("ID: %d, Length: %d\n",node_i->data.id,node_i->data.length);

    node_i = node_i->prev;
  }
}

// Function to free the memory allocated for the doubly-linked list
void freeDoublyList(struct DoublyLinkedList* list) {
  struct Node * node_i = list->start;

  struct Node * next = 0;
  while (node_i) {
    next = node_i->next;
    free(node_i);
    node_i = next;
  }
}

// Function to insert a node at the end of the doubly-linked list
// A list cannot be full so this operation theoretically cannot fail.
void append(struct DoublyLinkedList* list, struct Data data) {
  struct Node *node = createNode(data);
  if(list->end) {
    list->end->next = node;
    node->prev = list->end;
    list->end = node;
  } else {
    list->start = node;
    list->end = node;
  }
}

// Function to insert a node at the head of the doubly-linked list
void appendLeft(struct DoublyLinkedList* list, struct Data data) {
  struct Node *node = createNode(data);
  if(list->start) {
    list->start->prev = node;
    node->next = list->start;
    list->start = node;
  } else {
    list->start = node;
    list->end = node;
  }
}

// Pops the current head of the linked list
// Stores the popped data in the variable pointed to by popped
// Return 0 if the popping worked, 1 if you tried to pop from an empty list (do not exit)
int pop(struct DoublyLinkedList* list, struct Data* val) {

  //if the end attr of list is NULL, then abort
  if (!list->end) {
    return 1;
  }

  //save the to be popped node
  struct Node * startNode = list->start;
  struct Node * nextNode = startNode->next;

  //remember the value that we pop
  *val = startNode->data;

  //move the start pointer
  list->start = startNode;

  //if only 1 node is present in the doubly linked list
  // if(!prevNode) {
  //   list->start = NULL;
  //   list->end = NULL;
  // }
  // else {
  //   list->end->next = NULL;
  // }


  free(endNode);
  return 0;
}





int popRight(struct DoublyLinkedList* list, struct Data* val) {

  //if the end attr of list is NULL, then abort
  if (!list->end) {
    return 1;
  }

  //save the to be popped node
  struct Node * endNode = list->end;
  struct Node * prevNode = endNode->prev;

  //remember the value that we pop
  *val = endNode->data;

  //move the end pointer
  list->end = prevNode;

  //if only 1 node is present in the doubly linked list
  if(!prevNode) {
    list->start = NULL;
    list->end = NULL;
  }
  else {
    list->end->next = NULL;
  }


  free(endNode);
  return 0;
}

#endif
