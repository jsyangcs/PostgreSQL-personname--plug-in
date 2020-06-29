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
#include "partA_graph.h"
#include "partA_urlList.h"
#include "partA_readData.h"


char * partA_s_gets(char * st, int n, FILE *filename);

#define ADDRLEN 20
#define MAXSTRING 1000

// Create a set (list) of urls to process by reading data from file “collection.txt”
list partA_GetCollection(void){
    char delimBlank[2] = " ";
    char *token;
    char line[MAXSTRING];
    list head = NULL;
    FILE * collection = fopen("collection.txt", "r");
    if (collection == NULL) {
        fprintf(stderr, "fail to open file\n");
        exit(1);
    }
    while(partA_s_gets(line, MAXSTRING, collection) != NULL) {
        token = strtok(line, delimBlank);
        while(token != NULL ) {
            if (partA_in_list(head, token) == false) {
                head = partA_insert(head, token);
            }
            token = strtok(NULL, delimBlank);
        }
    }
    return head;
}
Graph partA_GetGraph(list head) {
    char fileAddress[ADDRLEN];
    int len;
    FILE *urlFile;
    char delimBlank[2] = " ";
    char *tokenFst;
    char *tokenSnd;
    char *token;
    char line[MAXSTRING];
    int nodesNum = head->num + 1;
    
    // create new graph
    Graph g = partA_newGraph(nodesNum);
    
    int outVertex;
    int inVertex;
    Edge e;
    
    list pt = head;
    
    while (pt != NULL){
        outVertex = pt->num;
        e.v = outVertex;
        
        // open urlxx.txt iteratively
        strncpy(fileAddress, pt->url, ADDRLEN - 1);
        fileAddress[ADDRLEN - 1] = '\0';
        len = ADDRLEN - (int)strlen(fileAddress) - 1;
        strncat(fileAddress, ".txt", len);
        
        urlFile = fopen(fileAddress, "r");
        if (urlFile == NULL){
            fprintf(stderr, "file %s read error\n", fileAddress);
            exit(1);
        }
        while(partA_s_gets(line, MAXSTRING, urlFile) != NULL) {
            // skip lines until first two tokens are "#start" and  "Section-1"
            tokenFst = strtok(line, delimBlank);
            if (tokenFst == NULL || strcmp(tokenFst, "#start") != 0){
                continue;
            }
            tokenSnd = strtok(NULL, delimBlank);
            if (tokenSnd == NULL || strcmp(tokenSnd, "Section-1") != 0){
                continue;
            }
            else{
                // start reading the following contents
                // and stop reading when first two tokens are "#end" and  "Section-1"
                while(partA_s_gets(line, MAXSTRING, urlFile) != NULL){
                    tokenFst = strtok(line, delimBlank);
                    if (tokenFst != NULL){
                        tokenSnd = strtok(NULL, delimBlank);
                        if (tokenSnd != NULL){
                            if (strcmp(tokenFst, "#end") == 0 && strcmp(tokenSnd, "Section-1") == 0){
                                break;
                            }
                            else{
                                // add tokenFst
                                inVertex = partA_find_node(head, tokenFst)->num;
                                e.w = inVertex;
                                partA_insertEdge(g, e);
                                // add tokenSnd
                                inVertex = partA_find_node(head, tokenSnd)->num;
                                e.w = inVertex;
                                partA_insertEdge(g, e);
                                // add token, until token == NULL
                                token = strtok(NULL, delimBlank);
                                while (token != NULL){
                                    inVertex = partA_find_node(head, token)->num;
                                    e.w = inVertex;
                                    partA_insertEdge(g, e);
                                    token = strtok(NULL, delimBlank);
                                }
                            }
                        }
                        else{
                            // add tokenFst
                            inVertex = partA_find_node(head, tokenFst)->num;
                            e.w = inVertex;
                            partA_insertEdge(g, e);
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
    return g;
}

char * partA_s_gets(char * st, int n, FILE *filename) {
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
