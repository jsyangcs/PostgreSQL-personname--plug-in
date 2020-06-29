//
//  list.c
//  pagerank
//
//  Created by JasonYang on 24/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

// Linked list implementation ... COMP9024 19T3
#include "partA_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int         v;
    struct Node *next;
} Node;

Node *partA_makeNode(int n) {
    Node *new = malloc(sizeof(Node));
    assert(new != NULL);
    new->v = n;
    new->next = NULL;
    return new;
}

List partA_insertLL(List L, int n) {
    if (partA_inLL(L, n))
        return L;

    // add new node at the beginning
    Node *new = partA_makeNode(n);
    new->next = L;
    return new;
}

List partA_deleteLL(List L, int n) {
    if (L == NULL)
        return L;
    if (L->v == n)
        return L->next;

    L->next = partA_deleteLL(L->next, n);
    return L;

}

bool partA_inLL(List L, int n) {
    if (L == NULL)
        return false;
    if (L->v == n)
       return true;

    return partA_inLL(L->next, n);
}

void partA_showLL(List L) {
    if (L == NULL)
        putchar('\n');
    else {
        printf("%d ", L->v);
        partA_showLL(L->next);
    }
}

void partA_freeLL(List L) {
    if (L != NULL) {
        partA_freeLL(L->next);
        free(L);
    }
}

int partA_lengthLL(List head) {
    if (head == NULL) {
        return 0;
    }
    return 1 + partA_lengthLL(head->next);
}
