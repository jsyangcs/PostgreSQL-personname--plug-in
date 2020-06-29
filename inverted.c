//
//  main.c
//  inverted
//
//  Created by JasonYang on 26/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "partB_readData.h"
#include "partB_urlList.h"
#include "partB_BST.h"
int main(int argc, const char * argv[]) {
    blist l = partB_GetCollection();
    Tree t = partB_GetInvertedList(l);
    FILE *fp =fopen("invertedIndex.txt", "w") ;
    if (fp == NULL) {
        fprintf(stderr, "fail to open file invertedIndex.txt\n");
        exit(1);
    }
    partB_generateFile(t, fp);
    partB_free_list(l);
    partB_freeTree(t);
    return 0;
}
