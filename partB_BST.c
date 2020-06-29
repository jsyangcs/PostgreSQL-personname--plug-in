//
//  BST.c
//  inverted
//
//  Created by JasonYang on 26/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "partB_BST.h"
#include "partB_urlList.h"

#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

#define data(tree)  ((tree)->data)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

typedef struct bNode {
    Tree left, right;
    char  data[100];
    blist  urls;
} bNode;

// make a new node containing data
Tree partB_newNode(Item it) {
    Tree new = malloc(sizeof(bNode));
    assert(new != NULL);
    strcpy(new->data, it);  // replace,   data(new) = it;
    new->urls = NULL;
    left(new) = right(new) = NULL;
    return new;
}

// create a new empty Tree
Tree partB_newTree() {
    return NULL;
}

// free memory associated with Tree
void partB_freeTree(Tree t) {
    if (t != NULL) {
        partB_freeTree(left(t));
        partB_freeTree(right(t));
        free(t);
    }
}

// display Tree sideways
void partB_showTreeR(Tree t, int depth) {
    if (t != NULL) {
         partB_showTreeR(right(t), depth+1);
    int i;
    for (i = 0; i < depth; i++)
        putchar('\t');            // TAB character
    printf("%s\n", data(t));
    partB_showTreeR(left(t), depth+1);
    }
}

void partB_showTree(Tree t) {
    partB_showTreeR(t, 0);
}

void partB_myPrint(Tree t) {
    if (t == NULL){
        return;
    }
    partB_myPrint(t->left);
    printf("%s ", t->data);
    partB_print_list(t->urls);
    putchar('\n');
    partB_myPrint(t->right);
}
void partB_generateFile(Tree t, FILE *file){
    if (t == NULL){
        return;
    }
    partB_generateFile(t->left, file);
    fprintf(file, "%s ", t->data);
    partB_print_list_in_file(t->urls, file);
    fprintf(file, "\n");
    partB_generateFile(t->right, file);
}

// check whether a key is in a Tree and return a pointer to it if exists
Tree partB_TreeSearch(Tree t, Item it) {
    if (t == NULL)
        return t;
    else if (strcmp(it, data(t)) < 0)   //replace, else if (it < data(t))
        return partB_TreeSearch(left(t), it);
    else if (strcmp(it, data(t)) > 0)  //replace, else if (it > data(t))
        return partB_TreeSearch(right(t), it);
    else                                 // it == data(t)
        return t;
}

// insert a new item into a Tree
Tree partB_TreeInsert(Tree t, Item it) {
    if (t == NULL)
        t = partB_newNode(it);
    else if (strcmp(it, data(t)) < 0)   //replace, else if (it < data(t))
        left(t) = partB_TreeInsert(left(t), it);
    else if (strcmp(it, data(t)) > 0)  //replace, else if (it > data(t))
        right(t) = partB_TreeInsert(right(t), it);
    return t;
}

// insert a word and accompanied url into the tree
Tree partB_TreeAddToken(Tree t, Item it, Item url) {
    Tree pt;
    // what if t == NULL
    if ((pt = partB_TreeSearch(t, it)) == NULL){
        t = partB_TreeInsert(t, it);
        pt = partB_TreeSearch(t, it);
        pt->urls = partB_insert(pt->urls, url);
    }
    else{
        // something wrong
        if (partB_in_list(pt->urls, url) == false){
            pt->urls = partB_insert_ordered(pt->urls, partB_create_node(url, NULL));
        }
    }
    return t;
}
