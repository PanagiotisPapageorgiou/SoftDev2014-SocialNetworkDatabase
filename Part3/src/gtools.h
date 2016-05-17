#include "GraphLib.h"

/* PART 1 TOOLS */
int BFS_Step(ResultSet*,int*);
int destroyResultSet(ResultSet**);
int printResultSet(ResultSet);
List* Double_BFS_Graph(Graph* ptrG,int start,int goal,int* success,int* total_distance,int* number_of_paths,int mode,int* errorFlag);
int checkBFSNeighbours(Graph*,GraphNode*,HashTable*,List*,int,int,int* errorFlag);
GraphNode** shortestCommonInFrontier(HashTable*,HashTable*,int mode,int* number_of_paths,int* errorFlag);
int getParentOfNode(HashTable*,GraphNode*,int,int* errorFlag);
GraphNode* firstCommonInFrontier(HashTable*, HashTable*,int* errorFlag);
int getDistanceOfNode(HashTable*, GraphNode*,int,int* errorFlag);
int fillPathWithSet(List*,HashTable*,int,int,int);

/* PART 2 TOOLS */
int comparePaths(List pathA, List pathB);
int cutNonUniquePaths(List* pathSet,int number_of_paths);
int nodeIsInPath(int id,List path);
int findCoherentGraphs(Graph* g,int* max_size,List* listOfSubGraphs,int mode,int* errorFlag);
GraphNode* pickExcludedNode(List listOfResultSets, Graph* g,int* errorFlag);
int getNumberOfLikes(GraphNode* personLikes,GraphNode* personIsLiked,Graph* postGraph,Graph* personGraph,int forumID,int* errorFlag);
int getNumberOfReplies(GraphNode* personReplies,GraphNode* personIsReplied,Graph* postGraph,Graph* personGraph,Graph* commentGraph,int forumID,int* errorFlag);
int trustBFSExpansion(List* frontier,HashTable* explored,Graph* trustGraph,int goalID,double* goalTrustValue,int* errorFlag);
int checkTrustBFSNeighbours(Graph* trustGraph,GraphNode* currentNode,HashTable* explored,List* frontier,int distance,int* errorFlag);
int edgeIsInPath(int startNodeID,int endNodeID,List path);
