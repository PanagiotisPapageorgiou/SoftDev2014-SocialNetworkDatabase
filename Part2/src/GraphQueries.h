/*
 * GraphQueries.h
 *
  */

#ifndef GRAPHQUERIES_H_
#define GRAPHQUERIES_H_

#include "gtools.h"
#include "GraphNodeTypes.h"
#include "EdgeTypes.h"

/**
 * Stalkers
 */
typedef struct Stalkers {
	int id;
	double centralityScore;
} Stalkers;

//help functions
Graph* loadGraph(int bucketsNumber, int bucketSize,int* errorFlag);
int getStalkerID(int position,Stalkers* stalk,int* errorFlag);
int getMatch(int pairNumber,Matches* match,int* errorFlag);

//metrics functions
int degreeDistribution(Graph* g,int* errorFlag);
int diameter(Graph* g,int* errorFlag);
double averagePathLength(Graph* g,int* errorFlag);
int numberOfCCs(Graph* g,int* errorFlag);
int maxCC(Graph* g,int* errorFlag);
double density(Graph* g,int* errorFlag);
double closenessCentrality(GraphNode* node,Graph* g,int* errorFlag);
double betweennessCentrality(GraphNode* node,Graph* g,int* errorFlag);

//queries functions
Matches* matchSuggestion(GraphNode* ptrN,int k,int h,int x,int limit,Graph* ptrG,int* errorFlag);
Graph* getTopStalkers(int k, int x, int centralityMode, Graph* g, Stalkers* st,int* errorFlag);
void findTrends(int k, Graph *g, Graph *tagg, char **womenTrends, char **menTrends);
Graph* buildTrustGraph(char* forum,Graph* personGraph,Graph* forumGraph,Graph* commentGraph,Graph* postGraph,List* trustEdgesList,List* personNodes,int* errorFlag);
double estimateTrust(GraphNode* personA,GraphNode* personB, Graph* trustGraph,int* errorFlag);

#endif /* GRAPHQUERIES_H_ */



