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
	long num_edges;		// number of edges Graph contains
	char* relation;	    // Relation type this Graph represents
} Graph;

int hash(int, int, int);
int initHashTable(HashTable*,int,int);
int clearHashTable(HashTable*);
int insertNodeHashTable(const GraphNode* const,HashTable*);
int increaseHashTable(HashTable*,int,int);
void printHashTable(HashTable*);
int rehashRecords(HashTable*);
int handleSplit(HashTable*);
GraphNode* lookUpNodeHashTable(int, const struct HashTable*);
