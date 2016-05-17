#define RUNNING 1
#define BLOCKED 0

#define EMPTY -500
#define EXIT_VALUE -899
#define INT 0
#define STRING 1
#define FLOAT 2
#define DOUBLE 3
#define LONG 4
#define LIST 5

#define PERSON_GRAPH 0
#define POST_GRAPH 1
#define FORUM_GRAPH 2
#define TAG_GRAPH 3
#define COMMENT_GRAPH 4
#define ORGANISATION_GRAPH 5
#define PLACE_GRAPH 6
#define PERSON_LIST 7
#define POST_LIST 8
#define FORUM_LIST 9
#define TAG_LIST 10
#define COMMENT_LIST 11
#define ORGANISATION_LIST 12
#define PLACE_LIST 13
#define PERSON_KNOWS_PERSON_LIST 14
#define PERSON_IS_LOCATED_LIST 15
#define PERSON_LIKES_POST_LIST 16
#define PERSON_STUDYAT_LIST 17
#define PERSON_WORKAT_LIST 18
#define PERSON_HASINTEREST_LIST 19
#define FORUM_HASMEMBER_LIST 20
#define FORUM_CONTAINER_LIST 21
#define COMMENT_CREATOR_LIST 22
#define COMMENT_REPLY_LIST 23
#define POST_HASCREATOR_LIST 24

#define FORUM_PREPARATION 1
#define FORUM_FILL 2

#include "defines.h"
#include "uberlist.h"
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

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
	double betweenessCentrality;
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
	int* topNForumNumEdges; //The number of Edges this GraphNode has in each of the topNForums it might participate
	double trustValue;
	double betweenessCentrality;
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

/* Match struct */
typedef struct Match{
	int ID;
	double similarity_score;
} Match;

typedef List Matches;

typedef struct Community{
        int numberOfMembers;
        int *members;
} Community;

/////////////////////////Used for job Scheduler///////////////////
/* Argument struct */
typedef struct Argument{ /* A function argument for a worker thread */
	void* value;
	int type;
} Argument;

/* Job struct for worker thread*/
typedef struct Job{ 
	long jobID; //The ID of this Job
	int  priority; //How important this task is
	int type; //The type of task determines what function the worker thread will use
	int status; //The current status of this job
	List argumentList; //The arguments this job comes with
} Job;

//////////////////////////////////////////////////////////////////

int compareProperties(Properties*, Properties*);

/* Result functions for Matches */
int allocateMatch(void**, void*);
int destroyMatch(void**);
int compareMatch(void*,void*);
int assignMatch(void*,void*);
int printMatch(void*);

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

/* Argument functions for List */
int allocateArgumentPtr(void**, void*);
int destroyArgumentPtr(void**);
int printArgumentPtr(void*);

/* Job functions for List */
int allocateJobPtr(void**, void*);
int destroyJobPtr(void**);
int printJobPtr(void*);

/* List functions for List */
int allocateListPtr(void**, void*);
int destroyListPtr(void**);
int printListPtr(void*);

/* Char* functions for List */
int allocateString(void**, void*);
int destroyString(void**);
int compareStrings(void*, void*);
int assignString(void*,void*);
int printString(void*);

