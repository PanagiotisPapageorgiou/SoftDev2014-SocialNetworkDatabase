#include "GraphLib.h"

/* PART 1 TOOLS */
int BFS_Step(ResultSet*);
int destroyResultSet(ResultSet**);
int printResultSet(ResultSet);
List* Double_BFS_Graph(Graph* ptrG,int start,int goal,int* success,int* total_distance,int* number_of_paths,int mode);
int checkBFSNeighbours(Graph*,GraphNode*,HashTable*,List*,int,int);
GraphNode** shortestCommonInFrontier(HashTable*,HashTable*,int mode,int* number_of_paths);
int getParentOfNode(HashTable*,GraphNode*,int);
GraphNode* firstCommonInFrontier(HashTable*, HashTable*);
int getDistanceOfNode(HashTable*, GraphNode*,int);
void fillPathWithSet(List*,HashTable*,int,int,int);
int comparePaths(List pathA, List pathB);
int cutNonUniquePaths(List** pathSet,int number_of_paths);
int nodeIsInPath(int id,List path);

/* PART 2 TOOLS */
int findCoherentGraphs(Graph* g,int *max_size);
GraphNode* pickExcludedNode(List listOfResultSets, Graph* g);
