//
//  graph.h
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef partA_graph_h
#define partA_graph_h


// Graph ADT interface ... COMP9024 19T3
#include <stdbool.h>
#include "partA_list.h"

typedef struct GraphRep {
   List *edges;  // array of lists
   int   nV;     // #vertices
   int   nE;     // #edges
} GraphRep;

typedef struct GraphRep *Graph;

// vertices are ints
typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

// * create new graph
Graph partA_newGraph(int);

// int  numOfVertices(Graph);

// * insert edge
void  partA_insertEdge(Graph, Edge);

// void  removeEdge(Graph, Edge);

// * test if an vertex points to another
bool  partA_adjacent(Graph, Vertex, Vertex);

// void  showGraph(Graph);

// * free the memory occupied by graph
void  partA_freeGraph(Graph);

#endif /* partA_graph_h */
