/*
 * GraphLib.h
 *
  */

#ifndef GRAPHLIB_H_
#define GRAPHLIB_H_

#include "hash.h"
#define PATH_CONSTRUCTION 1

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

#define CHECK(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }


/* ResultSet struct */
typedef struct ResultSet{
	int startID;
	Pair currentResult;
	List queue; /* Linked list for the frontier */
	HashTable set; /* HashTable for set/closed/explored GraphNodes* */
	Graph* ptrG;
} ResultSet;

/* ResultSetPtr functions for List */
int allocateResultSetPtr(void**, void*);
int destroyResultSetPtr(void**);
int compareResultSetPtr(void*,void*);
int assignResultSetPtr(void*,void*);
int printResultSetPtr(void*);

/* Properties functions */

Properties* createProperties(int number,int* errorFlag);
int setStringProperty(char*, int, Properties*);
int setIntegerProperty(int, int, Properties*);
int setDoubleProperty(double property, int index, Properties* p);
char* getStringProperty(int, Properties*,int* errorFlag);
int getIntegerProperty(int, Properties*,int* errorFlag);
double getDoubleProperty(int index, Properties* p,int* errorFlag);
int destroyProperties(Properties** proptr);

/* GraphNode functions */

GraphNode* createGraphNode(int, Properties*,int* errorFlag);
int getNumberOfGraphNodeEdges(GraphNode* gnode,int* errorFlag);
int destroyGraphNode(GraphNode** gnodeptr);

/* Edge functions */
Edge* createEdge(int, int , Properties*,int* errorFlag);
int getEdgeStartID(Edge* e,int* errorFlag);
int getEdgeEndID(Edge* e,int* errorFlag);
Properties* getEdgeProperties(Edge* e,int* errorFlag);
int destroyEdge(Edge**);

/* EdgeSet functions */
int insertInEdgeSet(GraphNode* ptrN, Edge* edgeptr);
EdgeSet* findEdgeSet(GraphNode* gnode, char* relation,int* errorFlag);
Edge* getFirstEdge(EdgeSet*,int* errorFlag);
Edge* getLastEdge(EdgeSet*,int* errorFlag);
Edge* getNextEdge(EdgeSet*,Edge* edge,int* errorFlag);
Edge* getPreviousEdge(EdgeSet*,Edge* edge,int* errorFlag);

/*Functions for implementation of part 1*/

Graph* createGraph(int, int, int* errorFlag);
int insertNode(GraphNode*, Graph*);
int insertEdge(int startID,Edge*, Graph*);
GraphNode* lookUpNode(int, Graph*,int *errorFlag);
int reachNode1(int, int, Graph*,int* errorFlag);
ResultSet* reachNodeN(int , Graph*, int* errorFlag);
int next(ResultSet*, Pair*,int* errorFlag);
int destroyGraph(Graph*);

/*Functions for implementation of part 2*/

int findAllCoherentGraphs(Graph* g,List* GraphPtrList,int* errorFlag);

#endif /* GRAPHLIB_H_ */
