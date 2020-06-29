//
//  urlList.c
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "partA_urlList.h"

// integer id of url
static int intId = 0;

// Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.

list partA_create_node(char * url, list next) {
    list n = malloc(sizeof(struct node));
    if (n == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    strncpy(n->url, url, URLLEN - 1);
    (n->url)[URLLEN - 1] = '\0';
    n->next = next;
    n->num = intId++;
    return n;
}


// create a new list node containing value
// and insert it at start of list
list partA_insert(list head, char * url) {
   if (partA_in_list(head, url) == true)
      return head;
   // add new node at the beginning
   list new = partA_create_node(url, NULL);
   new->next = head;
   return new;
}

// return pointer to last node in list
// NULL is returned if list is empty

list partA_last(list head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    return partA_last(head->next);
}


// create a new list node containing value
// and append it to end of list

list partA_append(list head, char * url) {
    if (head == NULL) {
        return partA_create_node(url, NULL);
    }
    head->next = partA_append(head->next, url);
    return head;
}

// return sum of list data fields: using recursive call

// int sum(list head) {
//     if (head == NULL) {
//         return 0;
//     }
//     return head->data + sum(head->next);
// }


// print contents of list

// void print_list(list head) {
//     if (head != NULL) {
//         print_list_items(head);
//     }
// }

// void print_list_items(list head) {
//     printf("%d, %s, %d, %.7f\n", head->num, head->url, head->degree, head->pr);
//     if (head->next != NULL) {
//         print_list_items(head->next);
//     }
// }

void partA_generate_file(list head, FILE *fp) {
    if (head != NULL) {
        fprintf(fp, "%s, %d, %.7f\n", head->url, head->degree, head->pr);
        if (head->next != NULL) {
            partA_generate_file(head->next, fp);
        }
    }
}


// return count of nodes in list

int partA_length(list head) {
    if (head == NULL) {
        return 0;
    }
    return 1 + partA_length(head->next);
}

bool partA_in_list(list head, char *url) {
    if (head == NULL)
        return false;
    if (strcmp(head->url, url) == 0)
        return true;
    return partA_in_list(head->next, url);
}

// return pointer to first node with specified data value
// return NULL if no such node

list partA_find_node(list head, char * url) {
    if (head == NULL || strcmp(head->url, url) == 0) {
        return head;
    }
    return partA_find_node(head->next, url);
}

// return pointer to first node with specified data value
// return NULL if no such node
list partA_find_node_by_num(list head, int num){
    if (head == NULL || head->num == num) {
        return head;
    }
    return partA_find_node_by_num(head->next, num);
}


// Delete a Node from a list: Recursive
// list deleteR(list head, list node) {
//    if (head == NULL) {
//       fprintf(stderr, "warning: node not in list\n");
//
//    } else if (node == head) {
//       head = head->next;        // remove first item
//       free(node);
      
//    } else {
//       head->next = deleteR(head->next, node);
//    }
//    return head;
// }

// Insert a Node into an Ordered list: recursive
// list insertR(list head, list node) {
//    if (head == NULL || head->data >= node->data) {
//       node->next = head;
//       return node;
//    }
//    head->next = insertR(head->next, node);
//    return head;
// }

// free the allocated memory space
void partA_free_list(list head) {
   struct node  *p, *temp;

   p = head;
   while (p != NULL) {
      temp = p->next;
      free(p);
      p = temp;
   }
}

// compare the order of two nodes
int partA_compare(list left, list right){
    if (left->pr == right->pr){
        // need double check
        return (strcmp(left->url, right->url) < 0);
    }
    return ((left->pr - right->pr) > 0);
}

// sort the list(insertion sort)
list partA_order(list head){
    list first = NULL;
    list tail = NULL;
    list pt;
    list min = NULL;
    list minPrevious = NULL;
    // only output 30 top urls
    int cnt = 0;
    while (head != NULL){
        // find the present highest ranked url
        for (pt = head, min = head; pt->next != NULL; pt = pt->next){
            if (partA_compare(pt->next, min) > 0){
                minPrevious = pt;
                min = pt->next;
            }
        }
        // insert the url into the sorted list
        if (first == NULL){
            first = min;
            tail = min;
        }
        else{
            tail->next = min;
            tail = min;
        }
        // remove the url from the previoous list
        if (min == head){
            head = head->next;
        }
        else{
            minPrevious->next = min->next;
        }
        cnt++;
    }
    if (first != NULL){
        tail->next = NULL;
    }
    head = first;
    return head;
}
