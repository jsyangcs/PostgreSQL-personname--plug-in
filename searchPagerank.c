//
//  main.c
//  testX
//
//  Created by JasonYang on 17/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "partC_list.h"

#define MAXSTRING 1000
char * partC_s_gets(char * st, int n, FILE * filename);
int main(int argc, const char * argv[]) {
    char delimBlank[2] = " ";
    char delimComma[3] = ", ";
    char *token;
    char *tokenRank;
    char line[MAXSTRING] ;
    // int lnCount = 0;
    FILE *invertedIndex;
    FILE *pagerankList;
    partC_list head = NULL;
    partC_list pt;
    bool isKeyword;
    invertedIndex = fopen("invertedIndex.txt", "r");
    pagerankList = fopen("pagerankList.txt", "r");
    if (argc == 1){
        fprintf(stderr, "too few arguments\n");
        exit(1);
    }
    if (invertedIndex == NULL|| pagerankList == NULL) {
        fprintf(stderr, "fail to open file\n");
        exit(1);
    }
    // deal with "invertedIndex.txt"
    while(partC_s_gets(line, MAXSTRING, invertedIndex) != NULL && line[0] != '\0')  {
        // first token should be among the keywords
        isKeyword = false;
        token = strtok(line, delimBlank);
        for (int i = 1; i < argc; i++){
            if (strcmp(argv[i], token) == 0){
                isKeyword = true;
                break;
            }
        }
        if (isKeyword == false){
            continue;
        }
        // printf("LINE START\n");
        // get the next token
        token = strtok(NULL, delimBlank);
        while(token != NULL ) {
            // printf("hey!\n");
            if (partC_in_list(head, token) == false){
                head = partC_insert(head, token, 1, 0.0);
                // printf("FIND URL!\n");
            }
            else{
                pt = partC_find_node(head, token);
                pt->match++;
            }
            // printf("url: %s\n", token);
            token = strtok(NULL, delimBlank);
        }
    }
    
    // printf("-----------\n");
    // print_list(head);
    // printf("-----------\n");
    
    
    // deal with "pagerankList.txt"
    while(partC_s_gets(line, MAXSTRING, pagerankList) != NULL)  {
        token = strtok(line, delimComma);
        if ((token != NULL) && ((pt = partC_find_node(head, token)) != NULL)){
            strtok(NULL, delimComma);
            tokenRank = strtok(NULL, delimComma);
            // printf("url: %s, rank: %s\n", token, tokenRank);
            pt->rank = atof(tokenRank);
        }
    }
    

    // sort the list
    head = partC_sort_list(head);
    
    // print the first 30 elements of the sorted list
    partC_print_list(head);
    
    partC_free_list(head);
    
    return(0);
}
char * partC_s_gets(char * st, int n, FILE *filename) {
    char * ret_val;
    int i = 0;
    ret_val = fgets(st, n, filename);
    if (ret_val) {
        while (st[i] != '\n' && st[i] != '\0'){
            i++;
        }
        if (st[i] == '\n'){
            st[i] = '\0';
        }
        else {
            while (getchar() != '\n'){
                continue;
            }
        }
    }
    return ret_val;
}
