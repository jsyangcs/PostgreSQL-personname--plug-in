//
//  list.h
//  pagerank
//
//  Created by JasonYang on 24/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

// Linked list interface ... COMP9024 19T3
#ifndef partA_list_h
#define partA_list_h


#include <stdbool.h>
typedef struct Node *List;

List partA_insertLL(List L, int n);
List partA_deleteLL(List, int);
bool partA_inLL(List, int);
void partA_freeLL(List);
void partA_showLL(List);
int partA_lengthLL(List head);

#endif /* partA_list_h */
