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
void printResultSetPtr(void*);

/* Properties functions */

Properties* createProperties(int);
void setStringProperty(char*, int, Properties*);
void setIntegerProperty(int, int, Properties*);
char* getStringProperty(int, Properties*);
int getIntegerProperty(int, Properties*);
int destroyProperties(Properties** proptr);

/* GraphNode functions */

GraphNode* createGraphNode(int, Properties*);
GraphNode* setPersonProperties(int id, char* name, char* surname, int age);
void printPersonProperties(GraphNode* n);
int getNumberOfGraphNodeEdges(GraphNode* gnode);
int destroyGraphNode(GraphNode** gnodeptr);

/* Edge Set functions */

int insertInEdgeSet(GraphNode* ptrN, Edge* edgeptr);

/* Edge functions */

Edge* createEdge(int, int , Properties*);
int getEdgeStartID(Edge* e);
int getEdgeEndID(Edge* e);
Properties* getEdgeProperties(Edge* e);
Edge* getFirstEdge(EdgeSet*);
Edge* getLastEdge(EdgeSet*);
Edge* getNextEdge(EdgeSet*,Edge* edge);
Edge* getPreviousEdge(EdgeSet*,Edge* edge);
Edge* setEdgeProperties(int startNodeID, int endNodeID, char* type, int weight);
void printEdgeProperties(Edge* n);
int destroyEdge(Edge**);

/*Functions for implementation of part 1*/

Graph* createGraph(int, int);
int insertNode(const GraphNode* const, Graph*);
int insertEdge(int startID, const Edge* const, Graph*);
GraphNode* lookUpNode(int, const Graph*);
int reachNode1(int, int, const Graph*  const);
ResultSet* reachNodeN(int , const Graph*  const);
int next(ResultSet*, Pair*);
int destroyGraph(Graph*);


/*Functions for implementation of part 2 */
long getNumberOfGraphNodes(Graph* ptrG);
long getNumberOfGraphEdges(Graph* ptrG);
int maxCC(Graph* g);
int numberofCCs(Graph* g);
double averagePathLength(Graph* g);
double density(Graph* g);

#endif /* GRAPHLIB_H_ */
