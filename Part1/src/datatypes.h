#include "uberlist.h"

/* A Single property of a Graph Node*/
typedef struct Property{
	void* attribute;
	char* type;
} Property;

/* Graph Node properties */
typedef struct Properties{
	int num_properties;
	Property* arrayP;
} Properties;

/* Edge struct */
typedef struct Edge{
	int startNodeID;
	int endNodeID;
	Properties* properties;
} Edge;

/* Edge Set Struct */
typedef struct EdgeSet{ /* An EdgeSet contains all Edges of a single relation type */
	List edges;
	char* relation;
} EdgeSet;

/* Graph Node struct */
typedef struct GraphNode{
	int id;
	int parentIDA; /* Used only in search - Keeps the ID of the GraphNode that opened this GraphNode while looking for Neighbours */
	int distanceA; /* Used only in search - Keeps the distance of a GraphNode from a another starting GraphNode */
	int parentIDB;
	int distanceB;
	Properties* properties;
	int num_edges; 		  /* The total number of Edges this node carries */
	List EdgeSetList; /* Contains all Edge Sets */
} GraphNode;

/* Result Pair struct */
typedef struct Pair* PairPtr;
typedef struct Pair{
	int ID;
	int distance;
} Pair;

int compareProperties(Properties*, Properties*);

/*	Int functions for List */
int allocateInt(void**, void*);
int destroyInt(void**);
int compareInts(void*, void*);
int assignInt(void*, void*);
void printInt(void*);

/* Result functions for List */
int allocateResultPair(void**, void*);
int destroyResultPair(void**);
int compareResultPair(void*,void*);
int assignResultPair(void*,void*);
void printResultPair(void*);

/* GraphNodePtr functions for List */
int allocateGraphNodePtr(void**, void*);
int destroyGraphNodePtr(void**);
int compareGraphNodesPtr(void*,void*);
int assignGraphNodePtr(void*,void*);
void printGraphNodePtr(void*);

/* Edge functions for List */
int allocateEdgePtr(void**, void*);
int destroyEdgePtr(void**);
int compareEdgePtr(void*, void*);
int assignEdgePtr(void*,void*);
void printEdgePtr(void*);

/* EdgeSet functions for List */
int allocateEdgeSet(void**, void*);
int destroyEdgeSet(void**);
int compareEdgeSet(void*, void*);
int assignEdgeSet(void*,void*);
void printEdgeSet(void*);

/* Char* functions for List */
int allocateString(void**, void*);
int destroyString(void**);
int compareStrings(void*, void*);
int assignString(void*,void*);
void printString(void*);

