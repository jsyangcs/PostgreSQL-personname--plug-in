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
#include "partB_urlList.h"


// Create a new struct node containing the specified data,
// and next fields, return a pointer to the new struct node.

blist partB_create_node(char * url, blist next) {
    blist n = malloc(sizeof(struct bnode));
    assert(n != NULL);
    strncpy(n->url, url, URLLEN - 1);
    (n->url)[URLLEN - 1] = '\0';
    n->next = next;
    return n;
}


// create a new list node containing value
// and insert it at start of list
blist partB_insert(blist head, char * url) {
   if (partB_in_list(head, url) == true)
      return head;
   // add new node at the beginning
   blist new = partB_create_node(url, NULL);
   new->next = head;
   return new;
}

// return pointer to last node in list
// NULL is returned if list is empty

blist partB_last(blist head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    return partB_last(head->next);
}


// create a new list node containing value
// and append it to end of list

blist partB_append(blist head, char * url) {
    if (head == NULL) {
        return partB_create_node(url, NULL);
    }
    head->next = partB_append(head->next, url);
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

void partB_print_list(blist head) {
    if (head !=NULL) {
        printf("%s", head->url);
        if (head->next != NULL) {
            printf(" ");
            partB_print_list(head->next);
        }
    }
}

void partB_print_list_in_file(blist head, FILE *fp) {
    if (head !=NULL) {
        fprintf(fp, "%s", head->url);
        if (head->next != NULL) {
            fprintf(fp, " ");
            partB_print_list_in_file(head->next, fp);
        }
    }
}


// return count of nodes in list

int partB_length(blist head) {
    if (head == NULL) {
        return 0;
    }
    return 1 + partB_length(head->next);
}

bool partB_in_list(blist head, char *url) {
    if (head == NULL)
        return false;
    if (strcmp(head->url, url) == 0)
        return true;
    return partB_in_list(head->next, url);
}

// return pointer to first node with specified data value
// return NULL if no such node

blist partB_find_node(blist head, char * url) {
    if (head == NULL || strcmp(head->url, url) == 0) {
        return head;
    }
    return partB_find_node(head->next, url);
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
void partB_free_list(blist head) {
   struct bnode  *p, *temp;

   p = head;
   while (p != NULL) {
      temp = p->next;
      free(p);
      p = temp;
   }
}

//Insert a Node into an Ordered List
blist partB_insert_ordered(blist head, blist node) {
   blist previous = NULL;
   blist n = head;
   // find correct position
   while (n != NULL && strcmp(node->url, n->url) > 0) {
       previous = n;
       n = n->next;
   }
   
   // link new node into list
   if (previous == NULL) {
       head = node;
       node->next = n;
   } else {
       previous->next = node;
       node->next = n;
   }
   
   return head;
}



// compare the order of two nodes
int partB_compare(blist left, blist right){
    return (strcmp(left->url, right->url) < 0);
}

// sort the list(insertion sort)
blist partB_order(blist head) {
    blist first = NULL;
    blist tail = NULL;
    blist pt;
    blist min = NULL;
    blist minPrevious = NULL;
    while (head != NULL){
        // find the present highest ranked url
        for (pt = head, min = head; pt->next != NULL; pt = pt->next){
            if (partB_compare(pt->next, min) > 0){
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
    }
    if (first != NULL){
        tail->next = NULL;
    }
    head = first;
    return head;
}
