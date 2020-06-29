//
//  readData.h
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef readDataB_h
#define readDataB_h

#include "partB_urlList.h"
#include "partB_BST.h"
#define WORDLEN 30
// Create a set (list) of urls to process by reading data from file “collection.txt”
blist partB_GetCollection(void);

Tree partB_GetInvertedList(blist head);

void partB_Normalise(char *word, char *new);
#endif /* readDataB_h */
