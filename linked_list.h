//
// Created by kumpf on 02.11.2024.
//

// linked_list.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>

// Error codes
#define NO_LIST_PROVIDED 1
#define NODE_MEM_FAIL 2
#define RETURN_ARRAY_MEM_FAIL 3

/*** Structures ***/

// Node structure for linked list
struct Node {
    void* data;
    struct Node* next;
};

// Linked list structure
struct LinkedList {
    struct Node* start;
    struct Node* head;
    size_t data_size;
    int count;
};

/*** Function Prototypes ***/

// Initialize the linked list with a specified data size for each node
int llist_init(struct LinkedList* list, size_t data_size);

// Append a node with data to the linked list
int llist_append_node(struct LinkedList* list, const void* data);

// Return all data in the linked list as an array (user must free it)
void* llist_return_arr(struct LinkedList* list);

// Get a human-readable error message for a given error code
const char* llist_strerror(int error_code);

#endif // LINKED_LIST_H

