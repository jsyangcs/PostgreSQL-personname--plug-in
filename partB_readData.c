//
//  readData.c
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "partB_urlList.h"
#include "partB_BST.h"
#include "partB_readData.h"


char * partB_s_gets(char * st, int n, FILE *filename);

#define ADDRLEN 20
#define MAXSTRING 1000

// Create a set (list) of urls to process by reading data from file “collection.txt”
blist partB_GetCollection(void){
    char delimBlank[2] = " ";
    char *token;
    char line[MAXSTRING];
    blist head = NULL;
    FILE * collection = fopen("collection.txt", "r");
    if (collection == NULL) {
        fprintf(stderr, "fail to open collections.txt\n");
        exit(1);
    }
    while(partB_s_gets(line, MAXSTRING, collection) != NULL) {
        token = strtok(line, delimBlank);
        while(token != NULL ) {
            if (partB_in_list(head, token) == false) {
                head = partB_insert(head, token);
            }
            token = strtok(NULL, delimBlank);
        }
    }
    return head;
}
Tree partB_GetInvertedList(blist head) {
    char fileAddress[ADDRLEN];
    int len;
    FILE *urlFile;
    char delimBlank[2] = " ";
    char *tokenFst;
    char *tokenSnd;
    char *token;
    char line[MAXSTRING];
    char word[WORDLEN];
    
    // create new tree
    Tree t = partB_newTree();
    
    blist pt = head;
    
    while (pt != NULL){
        // open urlxx.txt iteratively
        strncpy(fileAddress, pt->url, ADDRLEN - 1);
        fileAddress[ADDRLEN - 1] = '\0';
        len = ADDRLEN - (int)strlen(fileAddress) - 1;
        strncat(fileAddress, ".txt", len);

        urlFile = fopen(fileAddress, "r");
        if (urlFile == NULL){
            fprintf(stderr, "fail to open %s\n", fileAddress);
            exit(1);
        }
        while(partB_s_gets(line, MAXSTRING, urlFile) != NULL) {
            // skip lines until first two tokens are "#start" and  "Section-1"
            tokenFst = strtok(line, delimBlank);
            if (tokenFst == NULL || strcmp(tokenFst, "#start") != 0){
                continue;
            }
            tokenSnd = strtok(NULL, delimBlank);
            if (tokenSnd == NULL || strcmp(tokenSnd, "Section-2") != 0){
                continue;
            }
            else{
                // start reading the following contents
                // and stop reading when first two tokens are "#end" and  "Section-1"
                while(partB_s_gets(line, MAXSTRING, urlFile) != NULL){
                    tokenFst = strtok(line, delimBlank);
                    if (tokenFst != NULL){
                        tokenSnd = strtok(NULL, delimBlank);
                        if (tokenSnd != NULL){
                            if (strcmp(tokenFst, "#end") == 0 && strcmp(tokenSnd, "Section-2") == 0){
                                break;
                            }
                            else{
                                // add tokenFst
                                partB_Normalise(tokenFst, word);
                                t = partB_TreeAddToken(t, word, pt->url);
                                // add tokenSnd
                                partB_Normalise(tokenSnd, word);
                                t = partB_TreeAddToken(t, word, pt->url);
                                // add token, until token == NULL
                                token = strtok(NULL, delimBlank);
                                while (token != NULL){
                                    partB_Normalise(token, word);
                                    t = partB_TreeAddToken(t, word, pt->url);
                                    token = strtok(NULL, delimBlank);
                                }
                            }
                        }
                        else{
                            // add tokenFst
                            partB_Normalise(tokenFst, word);
                            t = partB_TreeAddToken(t, word, pt->url);
                        }
                    }
                    else{
                        continue;
                    }
                }
                // have finished job with file, move on to next
                break;
            }
        }
        pt = pt->next;
    }
    return t;
}

char * partB_s_gets(char * st, int n, FILE *filename) {
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

// removing leading and trailing spaces,
// converting all characters to lowercase,
// remove the following punctuation marks, if they appear at the end of a word: '.' (dot), ',' (comma), ';' (semicolon), ? (question mark)
void partB_Normalise(char *word, char *new){
    for (int i = 0; i < strlen(word) - 1; i++){
        new[i] = tolower(word[i]);
    }
    char last = word[strlen(word) - 1];
    if (last == '.' || last == ',' || last == ';' || last == '?'){
        new[strlen(word) - 1] = '\0';
    }
    else{
        new[strlen(word) - 1] = tolower(word[strlen(word) - 1]);
        new[strlen(word)] = '\0';
    }
}
