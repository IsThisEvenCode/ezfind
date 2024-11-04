//
// Created by kumpf on 02.11.2024.
//

#include "linked_list.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*** defines ***/
#define NO_LIST_PROVIDED 1
#define NODE_MEM_FAIL 2
#define RETURN_ARRAY_MEM_FAIL 3

/*** functions ***/

int llist_init(struct LinkedList* list, size_t data_size) {
    if (list == NULL) {
        errno = NO_LIST_PROVIDED;
        return -1;
    }
    list->start = NULL;
    list->head = NULL;
    list->data_size = data_size;
    list->count = 0;
    return 0;
}

int llist_append_node(struct LinkedList* list, const void* data) {
    if (list == NULL) {
        errno = NO_LIST_PROVIDED;
        return -1;
    }

    struct Node* new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        errno = NODE_MEM_FAIL;
        return -1;
    }
    new_node->data = malloc(list->data_size);
    if (new_node->data == NULL) {
        free(new_node);
        errno = NODE_MEM_FAIL;
        return -1;
    }

    memcpy(new_node->data, data, list->data_size);
    new_node->next = NULL;

    if (list->start == NULL) {
        // List is empty; initialize start and head
        list->start = new_node;
        list->head = new_node;
    } else {
        // List has one or more nodes; append to the end
        list->head->next = new_node;
        list->head = new_node;
    }
    list->count++;

    return 0;
}

void* llist_return_arr(struct LinkedList* list) {
    void* arr = malloc(list->count * list->data_size);
    if (arr == NULL) {
        errno = RETURN_ARRAY_MEM_FAIL;
        return NULL;
    }

    // Traverse linked list and copy values to array
    struct Node* curr = list->start;
    int i = 0;
    while (curr != NULL) {
        memcpy((char*)arr + i * list->data_size, curr->data, list->data_size);
        curr = curr->next;
        i++;
    }

    return arr;
}

/*** error handling ***/
const char* llist_strerror(int error_code) {
    switch (error_code) {
        case 1:
            return "No list provided";
        case 2:
            return "Error while allocation memory for node";
        case 3:
            return "Error while allocating array for returning values";
        default:
            return strerror(error_code);
    }

}