//
//  readData.h
//  pagerank
//
//  Created by JasonYang on 20/1/20.
//  Copyright © 2020 JasonYang. All rights reserved.
//

#ifndef readData_h
#define readData_h

#include "partA_graph.h"
#include "partA_urlList.h"

// Create a set (list) of urls to process by reading data from file “collection.txt”
list partA_GetCollection(void);

// Create graph
// for each url in the above list read <url>.txt file
// and update graph by adding a node and outgoing links
Graph partA_GetGraph(list head);

#endif /* readData_h */
