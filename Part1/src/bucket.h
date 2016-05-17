#include "datatypes.h"

/* Bucket struct*/
typedef struct Bucket* BucketPtr;
typedef struct Bucket{
	GraphNode** GNArray;	// bucket contains an array of GraphNode*
	int c;					// bucket capacity
	int items;				// current number of items (Overflow when c == items)
	int last;				// index that shows the last element of array
	int middle;				// index that shows the middle element of array
} Bucket;

/* Bucket Chain */
typedef struct BucketChain* BucketChainPtr; /* A Chain of Buckets */
typedef struct BucketChain{
	List bucket_chain;
	int num_records;
} BucketChain;

/* Bucket General functions */

void initBucket(Bucket *b, int c);
void clearBucket(Bucket *b);
int getItemsBucket(Bucket *b);
int isEmptyBucket(Bucket* b);
int isFullBucket(Bucket* b);
int insertWhere(Bucket *b, const struct GraphNode* gnode);
int insertIDBucket(BucketChain* chain_ptr,Bucket* b, const struct GraphNode* gnode);
int deleteIDBucket(Bucket* bucket, int id);
int searchIDBucket(Bucket* b,int id,GraphNode** ptrN);
int printBucketContents(Bucket* b);
int binary_search(Bucket *b, int value);
int binary_search_lvl2(GraphNode **bucket, int value, int low, int high);

/* Bucket Chain functions */

void createBucketChain(BucketChain** chain_ptr);
void destroyBucketChain(BucketChain** chain_ptr);
void addNewBucket(BucketChain* chain_ptr,int bucket_capacity);
void destroyFirstBucket(BucketChain* chain_ptr);
void destroyLastBucket(BucketChain* chain_ptr);
void destroyCurrentBucket(BucketChain* chain_ptr);
int isEmptyBucketChain(BucketChain* chain_ptr);
int getItemsChainBucket(BucketChain* chain_ptr);
int transferBucketChain(Bucket* bucketA, int pos, BucketChain* targetChain);
Bucket* getHeadBucket(BucketChain* chain_ptr);
Bucket* getLastBucket(BucketChain* chain_ptr);
Bucket* getCurrentBucket(BucketChain* chain_ptr);
int goToHeadBucket(BucketChain* chain_ptr);
int goToLastBucket(BucketChain* chain_ptr);
int goToNextBucket(BucketChain* chain_ptr);
int goToPreviousBucket(BucketChain* chain_ptr);
int searchIDBucketChain(BucketChain* chain_ptr,int id,GraphNode** ptrN);
int insertIDBucketChain(BucketChain* chain_ptr, const struct GraphNode* gnode,int* split);
int concatenateBucketChain(BucketChain* chain_ptr);
void printBucketChain(BucketChain* chain_ptr);
void moveRightBucket(Bucket*, int);
int numBucket(BucketChain *chain_ptr, Bucket *b);

/* BucketPtr functions for List */
int allocateBucketPtr(void**, void*);
int destroyBucketPtr(void**);
int compareBucketPtr(void*,void*);
int assignBucketPtr(void*,void*);
void printBucketPtr(void*);
