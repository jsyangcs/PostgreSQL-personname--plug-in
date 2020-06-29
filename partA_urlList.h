//
//  urlList.h
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef partA_urlList_h
#define partA_urlList_h

#define URLLEN 10

struct node {
    struct node *next;
    char        url[URLLEN];
    double      pr;
    int         num;
    int         degree;
} ;

typedef struct node * list;

// * Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.
list partA_create_node(char * url, list next);

// * create a new list node containing value
// and insert it at start of list
list partA_insert(list head, char * url);

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
// void print_list(list head);
// void print_list_items(list head);

// * generate contents of list
void partA_generate_file(list head, FILE *fp);

// return count of nodes in list
// int length(list head);

// * check if a url exists in list
bool partA_in_list(list head, char *url);

// * return pointer to first node with specified data value
// return NULL if no such node
list partA_find_node(list head, char * url);

// return pointer to first node with specified data value
// return NULL if no such node
list partA_find_node_by_num(list head, int num);


// Delete a Node from a list: Recursive
// list deleteR(list head, list node);

// Insert a Node into an Ordered list: recursive
// list insertR(list head, list node);

// free the allocated memory space
void partA_free_list(list head);

// * compare the order of two nodes
int partA_compare(list left, list right);

// * oder the list accoding to PageRank values
list partA_order(list head);

#endif /* partA_urlList_h */
