#include "uberlist.h"

#define EMPTY -500
#define EXIT_VALUE -899
#define INT 0
#define STRING 1
#define FLOAT 2
#define DOUBLE 3
#define LONG 4

/* A Single property of a Graph Node*/
typedef struct Property{
	void* attribute;
	int type;
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
	double trustValue;
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

/* Matches struct */
typedef struct Matches{
	int ID;
	double similarity_score;
} Matches;

int compareProperties(Properties*, Properties*);

/* Result functions for Matches */
int allocateMatches(void**, void*);
int destroyMatches(void**);
int compareMatches(void*,void*);
int assignMatches(void*,void*);
int printMatches(void*);

int compareProperties(Properties*, Properties*);

/*	Int functions for List */
int allocateInt(void**, void*);
int destroyInt(void**);
int compareInts(void*, void*);
int assignInt(void*, void*);
int printInt(void*);

/* Result functions for List */
int allocateResultPair(void**, void*);
int destroyResultPair(void**);
int compareResultPair(void*,void*);
int assignResultPair(void*,void*);
int printResultPair(void*);

/* GraphNodePtr functions for List */
int allocateGraphNodePtr(void**, void*);
int destroyGraphNodePtr(void**);
int compareGraphNodesPtr(void*,void*);
int assignGraphNodePtr(void*,void*);
int printGraphNodePtr(void*);

/* Edge functions for List */
int allocateEdgePtr(void**, void*);
int destroyEdgePtr(void**);
int compareEdgePtr(void*, void*);
int assignEdgePtr(void*,void*);
int printEdgePtr(void*);

/* EdgeSet functions for List */
int allocateEdgeSet(void**, void*);
int destroyEdgeSet(void**);
int compareEdgeSet(void*, void*);
int assignEdgeSet(void*,void*);
int printEdgeSet(void*);

/* Char* functions for List */
int allocateString(void**, void*);
int destroyString(void**);
int compareStrings(void*, void*);
int assignString(void*,void*);
int printString(void*);

