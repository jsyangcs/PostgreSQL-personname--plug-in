//
//  urlList.h
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef partB_urlList_h
#define partB_urlList_h

#include <stdbool.h>
#define URLLEN 10

struct bnode {
    struct bnode *next;
    char        url[URLLEN];
} ;

typedef struct bnode * blist;

// * Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.
blist partB_create_node(char * url, blist next);

// * create a new list node containing value
// and insert it at start of list
blist partB_insert(blist head, char * url);

// return pointer to last node in list
// NULL is returned if list is empty
// list last(list head);

// create a new list node containing value
// and append it to end of list
// return head not modified
// list append(list head, char * url);

// return sum of list data fields: using recursive call
// int sum(list head);

// print contents of list
void partB_print_list(blist head);

// * generate contents of list
void partB_print_list_in_file(blist head, FILE *fp);

// return count of nodes in list
// int length(list head);

// * check if a url exists in list
bool partB_in_list(blist head, char *url);

// * return pointer to first node with specified data value
// return NULL if no such node
blist partB_find_node(blist head, char * url);


// Delete a Node from a list: Recursive
// list deleteR(list head, list node);

// Insert a Node into an Ordered list: recursive
// list insertR(list head, list node);

// free the allocated memory space
void partB_free_list(blist head);

//Insert a Node into an Ordered List
blist partB_insert_ordered(blist head, blist node);

// * compare the order of two nodes
int partB_compare(blist left, blist right);

// * oder the list accoding to PageRank values
blist partB_order(blist head);

#endif /* partB_urlList_h */
