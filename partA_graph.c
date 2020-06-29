//
//  graph.c
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//


// Graph ADT
// Adjacency List Representation ... COMP9024 19T3
#include "partA_graph.h"
#include "partA_list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

Graph partA_newGraph(int nV) {
   assert(nV >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;
   g->nE = 0;
   // allocate memory for array of lists
   g->edges = malloc(nV * sizeof(List));
   assert(g->edges != NULL);
   for (i = 0; i < nV; i++)
      g->edges[i] = NULL;

   return g;
}

int partA_numOfVertices(Graph g) {
   return g->nV;
}

// check if vertex is valid in a graph
bool partA_validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void partA_insertEdge(Graph g, Edge e) {
   assert(g != NULL && partA_validV(g,e.v) && partA_validV(g,e.w));
   if (!partA_inLL(g->edges[e.v], e.w)) {   // edge e not in graph
      g->edges[e.v] = partA_insertLL(g->edges[e.v], e.w);
      g->nE++;
   }
}

void partA_removeEdge(Graph g, Edge e) {
   assert(g != NULL && partA_validV(g,e.v) && partA_validV(g,e.w));
   if (partA_inLL(g->edges[e.v], e.w)) {   // edge e in graph
      g->edges[e.v] = partA_deleteLL(g->edges[e.v], e.w);
      g->nE--;
   }
}

bool partA_adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && partA_validV(g,v));
   return partA_inLL(g->edges[v], w);
}

// void showGraph(Graph g) {
//     assert(g != NULL);
//     int i;
//
//     printf("Number of vertices: %d\n", g->nV);
//     printf("Number of edges: %d\n", g->nE);
//     for (i = 0; i < g->nV; i++) {
//        printf("%d -  ", i);
//        showLL(g->edges[i]);
//     }
// }

void partA_freeGraph(Graph g) {
   assert(g != NULL);
   int i;

   for (i = 0; i < g->nV; i++)
      partA_freeLL(g->edges[i]);

   free(g->edges);
   free(g);
}
