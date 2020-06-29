//
//  BST.h
//  inverted
//
//  Created by JasonYang on 26/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef partB_BST_h
#define partB_BST_h
/*
 Inverted Index ADT (partial) implementation, written by Ashesh Mahidadia Jan 2020.
 You may need to modify the following implementation and test it properly before using
 in your program.
*/


#include <stdbool.h>

typedef char *Item;      // item is just a key

typedef struct bNode *Tree;

Tree partB_newTree(void);        // create an empty Tree
void partB_freeTree(Tree);   // free memory associated with Tree
void partB_showTree(Tree);   // display a Tree (sideways)
void partB_myPrint(Tree t);
void partB_generateFile(Tree t, FILE *file);
Tree partB_TreeSearch(Tree, Item);   // check whether an item is in a Tree and return a pointer to it if exists
Tree partB_TreeInsert(Tree, Item);   // insert a new item into a Tree

Tree partB_TreeAddToken(Tree t, Item it, Item url);
#endif /* partB_BST_h */
