//
//  list.h
//  testX
//
//  Created by JasonYang on 17/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef partC_list_h
#define partC_list_h

#include <stdio.h>

#define URLLEN 10
#define LINES 30

struct partC_node {
    struct partC_node *next;
    char        url[URLLEN];
    int         match;
    float       rank;
} ;

typedef struct partC_node * partC_list;

// Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.
partC_list partC_create_node(char * url, int match, float rank, partC_list next);

// return pointer to last node in list
// NULL is returned if list is empty
partC_list partC_last(partC_list head);

bool partC_in_list(partC_list head, char *url);

// create a new list node containing value
// and partC_insert it at start of list
partC_list partC_insert(partC_list head, char * url, int match, float rank);

// create a new list node containing value
// and append it to end of list
// return head not modified
partC_list partC_append(partC_list head, char * url, int match, float rank);

// return sum of list data fields: using recursive call
// int sum(list head);

// print contents of list in Python syntax
void partC_print_list(partC_list head);
void partC_print_list_items(partC_list head);


// return count of nodes in list
int partC_length(partC_list head);

// return pointer to first node with specified data value
// return NULL if no such node
partC_list partC_find_node(partC_list head, char * url);

// Delete a Node from a list: Recursive
// list deleteR(list head, list node);

// partC_insert a Node into an Ordered list: recursive
// list partC_insertR(list head, list node);

// free the allocated memory space
void partC_free_list(partC_list head);

// compare the order of two nodes
int partC_compare(partC_list left, partC_list right);

// sort the list(partC_insertion sort)
partC_list partC_sort_list(partC_list head);

#endif /* partC_list_h */
