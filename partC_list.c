//
//  list.c
//  testX
//
//  Created by JasonYang on 17/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "partC_list.h"



// Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.

partC_list partC_create_node(char * url, int match, float rank, partC_list next) {
    partC_list n = malloc(sizeof(struct partC_node));
    assert(n != NULL);
    strncpy(n->url, url, URLLEN);
    n->match =match;
    n->rank = rank;
    n->next = next;
    return n;
}

// return pointer to last node in list
// NULL is returned if list is empty

partC_list partC_last(partC_list head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    return partC_last(head->next);
}

bool partC_in_list(partC_list head, char *url) {
    if (head == NULL)
        return false;
    if (strcmp(head->url, url) == 0)
        return true;
    return partC_in_list(head->next, url);
}

// create a new list node containing value
// and partC_insert it at start of list
partC_list partC_insert(partC_list head, char * url, int match, float rank) {
   if (partC_in_list(head, url) == true)
      return head;
   // add new node at the beginning
   partC_list new = partC_create_node(url, match, rank, NULL);
   new->next = head;
   return new;
}


// create a new list node containing value
// and append it to end of list

partC_list partC_append(partC_list head, char * url, int match, float rank) {
    if (head == NULL) {
        return partC_create_node(url, match, rank, NULL);
    }
    head->next = partC_append(head->next, url, match, rank);
    return head;
}

// return sum of list data fields: using recursive call

// int sum(list head) {
//     if (head == NULL) {
//         return 0;
//     }
//     return head->data + sum(head->next);
// }


// print contents of list in Python syntax

void partC_print_list(partC_list head) {
    if (head != NULL) {
        partC_print_list_items(head);
    }
}

void partC_print_list_items(partC_list head) {
    printf("%s\n", head->url);
    if (head->next != NULL) {
        partC_print_list_items(head->next);
    }
}

// return count of nodes in list

int partC_length(partC_list head) {
    if (head == NULL) {
        return 0;
    }
    return 1 + partC_length(head->next);
}


// return pointer to first node with specified data value
// return NULL if no such node

partC_list partC_find_node(partC_list head, char * url) {
    if (head == NULL || strcmp(head->url, url) == 0) {
        return head;
    }
    return partC_find_node(head->next, url);
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

// partC_insert a Node into an Ordered list: recursive
// list partC_insertR(list head, list node) {
//    if (head == NULL || head->data >= node->data) {
//       node->next = head;
//       return node;
//    }
//    head->next = partC_insertR(head->next, node);
//    return head;
// }

// free the allocated memory space
void partC_free_list(partC_list head) {
    partC_list p;
    partC_list temp;
   p = head;
   while (p != NULL) {
      temp = p->next;
      free(p);
      p = temp;
   }
}

// compare the order of two nodes
int partC_compare(partC_list left, partC_list right){
    // match->rank->url
    if (left->match == right->match){
        if (left->rank == right->rank) {
            return (strcmp(left->url, right->url) < 0);
        }
        return ((left->rank - right->rank) > 0);
    }
    return ((left->match - right->match) > 0);
}

// sort the list(partC_insertion sort)
partC_list partC_sort_list(partC_list head){
    partC_list first = NULL;
    partC_list tail = NULL;
    partC_list pt;
    partC_list min = NULL;
    partC_list minPrevious = NULL;
    // only output 30 top urls
    int cnt = 0;
    while (head != NULL && cnt < LINES){
        // find the present highest ranked url
        for (pt = head, min = head; pt->next != NULL; pt = pt->next){
            if (partC_compare(pt->next, min) > 0){
                minPrevious = pt;
                min = pt->next;
            }
        }
        // partC_insert the url into the sorted list
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
