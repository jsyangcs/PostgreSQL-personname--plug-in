//
//  main.c
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "partA_readData.h"
#include "partA_urlList.h"
#include "partA_graph.h"
#include "partA_list.h"

list partA_calculatePageRank(Graph g, double d, double diffPR, int maxIteration);

// global variable
static list l;

int main(int argc, const char * argv[]) {
    
    // get list of urls from collections.txt
    l = partA_GetCollection();
    
    // create graph with urls in the previous list as vertices and edges from the other files
    Graph g = partA_GetGraph(l);
    
    // create variable for the output file
    FILE *fp = fopen("pagerankList.txt", "w");
    if (argc < 4) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }
    // calculate PageRank accoding to the graph and output a list
    list new = partA_calculatePageRank(g, atof(argv[1]), atof(argv[2]), atoi(argv[3]));
    
    // free the memory taken up by graph g
    partA_freeGraph(g);
    
    // order the list in descending order of PageRank
    new = partA_order(new);
    
    // generate the output file
    if (fp == NULL){
        fprintf(stderr, "fail to open file pagerankList.txt\n");
        exit(1);
    }
    partA_generate_file(new, fp);
    
    partA_free_list(new);
    
    return 0;
}
list partA_calculatePageRank(Graph g, double d, double diffPR, int maxIteration) {
    int nodesNum = g->nV;
    list pt = l;
    int iteration = 0;
    while (pt != NULL){
        pt->pr = (double)1 / nodesNum;
        pt->degree = partA_lengthLL(g->edges[pt->num]);
        pt = pt->next;
    }
    pt = l;
    list previousList = l;
    double diff = diffPR;
    while (iteration < maxIteration && diff >= diffPR){
        list new = NULL;
        diff = 0;
        while (pt != NULL){
            // add node to list new
            new = partA_insert(new, pt->url);
            new->num = pt->num;
            new->degree = pt->degree;
            double sum = 0;
            for (int i = 0; i < nodesNum; i++){
                if (partA_adjacent(g, i, pt->num)){
                    // pt has been updated，should use new pr and L
                    list nodeI = partA_find_node_by_num(previousList, i);
                    // sum+=pr(pj)/outDegree(pj), pj is incident on pt->num
                    sum += (nodeI->pr) / (nodeI->degree);
                }
            }
            sum = (1 - d) / nodesNum + d *sum;
            diff += fabs(sum - pt->pr);
            new->pr = sum;
            pt = pt->next;
        }
        // printf("Iteration: %d\nDiff: %f\n", iteration, diff);
        // after one iteration, free previous pt and assign new to pt
        partA_free_list(pt);
        previousList = new;
        pt = new;
        iteration++;
    }
    return pt;
}
