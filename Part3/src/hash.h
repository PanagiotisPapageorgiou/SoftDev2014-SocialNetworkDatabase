#include "bucket.h"
#include <math.h>

typedef struct HashTable{	// HashTable was separated as a struct to allow it being used seperately
	int bucket_capacity;	// c (given in createGraph)
	int originaLm;	// initial hash table size
	int size;	// actual size of hash table in memory
	int i,p;	// keep the i and p here too for others functions
	long num_records;	// count graphnodes in graph for use in functions
	BucketChain** hashmap;	// an array of pointers to chains of buckets
} HashTable;

typedef struct Graph{ // Graph struct
	HashTable hashtable; //Every Graph has a hashtable
	List bannedEdgeSetList; //A List of EdgeSets with banned edges (ie Edges we cannot step on - Useful in PART3)
	pthread_mutex_t graphMutex; //My personal mutex to make access/lookUps/write thread safe
	long num_edges;		// number of edges Graph contains
} Graph;

int hash(int, int, int);
int initHashTable(HashTable*,int,int);
int clearHashTable(HashTable*);
BucketChain* getHashTableElement(HashTable* hashtable,int position,int* errorFlag);
int insertNodeHashTable(GraphNode*,HashTable*);
int increaseHashTable(HashTable*,int,int);
int printHashTable(HashTable*);
int rehashRecords(HashTable*);
int handleSplit(HashTable*);
GraphNode* lookUpNodeHashTable(int nodeID,HashTable* hashtable,int* errorFlag);

/* GraphPtr functions for List */
int allocateGraphPtr(void**, void*);
int destroyGraphPtr(void**);
int compareGraphPtr(void*,void*);
int assignGraphPtr(void*,void*);
int printGraphPtr(void*);
