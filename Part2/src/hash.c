#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hash.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: hash.c
 MAIN AUTHOR: LHDA-KYRIAKH ZACHAROPOULOU														

 Details: This file contains all functions required to use the HashTable struct. The HashTable
 struct includes everything required to create a Linear Hashing HashTable. 

 UPDATE: We have created a HashTable struct seperately from the Graph struct in order to allow 
 for use of Linear Hashing in Graph Traversal for the Set of BFS Search (keeps explored nodes). 
 We did this to speedup the lookup of nodes in the set.
================================================================================================*/

//TODO: IMPROVE HASHING SPEED

//////////////////////////////////////////LINEAR HASHING FUNCTIONS ///////////////////////////////////

int hash(int key, int i, int m) { /* Hash Function */
	
	/*m is always the initial size of the hashtable*/
	int size = pow(2,i)* m;
	return key%size;

}

int initHashTable(HashTable* hashtable,int m,int c){ /* Initialises a HashTable */
													 /* Every HashTable struct contains a hashmap which is an array of Bucket Chains */

	int i=0;

	if(hashtable != NULL){
		hashtable->bucket_capacity = c;
		hashtable->originaLm = m;
		hashtable->size = m;
		hashtable->i = 0;
		hashtable->p = 0;
		hashtable->num_records = 0;

		hashtable->hashmap = malloc( (hashtable->originaLm) * sizeof(BucketChain*) ); /* Allocate the array of BucketChain* */
		if(hashtable->hashmap == NULL){
			printf("initHashTable: Error! Could not allocate enough memory for Hash Table! - Exiting!\n");
			return EXIT_VALUE;
		}

		for(i=0; i < hashtable->originaLm; i++)
			hashtable->hashmap[i] = (BucketChain*) NULL;

	}
	else{
		printf("initHashTable: HashTable is not allocated! - Exiting!\n");
		return EXIT_VALUE;
	}

	//printf("===================INITIALISED HASHTABLE ====================\n");
	//printf("HashTable original Size: %d - Bucket Capacity: %d\n\n",hashtable->size,hashtable->bucket_capacity);

	return 0;

}

int clearHashTable(HashTable* hashtable){ /* Clear the contents of a HashTable struct */

	int i=0;

	if(hashtable != NULL){
		if(hashtable->hashmap != NULL){
			for (i=0; i < hashtable->size; i++) {
				if(hashtable->hashmap[i] != NULL){ /* A chain of buckets exists here */
					destroyBucketChain(&(hashtable->hashmap[i])); /* Destroy this chain of buckets */
				}
			}
			free(hashtable->hashmap);
			hashtable->i=0;
			hashtable->p=0;
			hashtable->num_records = 0;
			hashtable->size = 0;
			//printf("=====================DESTROYED HASHTABLE ======================!\n");
		}
	}
	else{
		printf("destroyHashTable: HashTable is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;

}

BucketChain* getHashTableElement(HashTable* hashtable,int position,int* errorFlag){ /* Returns a pointer to a specific chain of buckets in the HashTable */

	*errorFlag = 0;

	if(hashtable == NULL){
		printf("getHashTableElement: HashTable is NULL!\n");
		*errorFlag = 1;
		return ((BucketChain*) (NULL));
	}

	if(hashtable->hashmap[position] == NULL){
		printf("getHashTableElement: HashMap is NULL!\n");
		*errorFlag = 1;
		return ((BucketChain*) (NULL));
	}

	return hashtable->hashmap[position];

}


int insertNodeHashTable(GraphNode* ptrN, HashTable* hashtable){ /* Will insert a GraphNode* in a bucket of chains of the HashTable */

	int split = 0;
	int pos,j,k,l,t, ID;
	static int size_in_use;	//size of hashtable that is in use	

	BucketChain* bucketChainPtr;	

	if(hashtable == NULL){
		printf("insertNodeHashTable: HashTable has not been initiliased! Exiting!\n");
		return EXIT_VALUE;
	}

	if(ptrN == NULL){
		printf("insertNodeHashTable: GraphNode is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	if(hashtable->hashmap == NULL){
		printf("insertNodeHashTable: HashMap is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	//printf("\n========================= INSERTING NODE ===========================\n");
	//printf("insertNodeHashTable: Inserting node with ID: %d...\n",ptrN->id);
	
	/*check size of hashtable*/
	size_in_use = hashtable->p + pow(2, hashtable->i) * (hashtable->originaLm); /* Store the size of the HashTable */
	//printf("insertNodeHashTable: //Check! Size of hashtable in use: %d\n",size_in_use);
	//printf("insertNodeHashTable: //Check! Actual size of hashtable in memory: %d\n",hashtable->size);
	////////////////////////////////LOCATE BUCKET CHAIN//////////////////////////////////


	pos = hash(ptrN->id,hashtable->i,hashtable->originaLm); /* Always send the original m of the HashTable */
	if (pos < hashtable->p) {
		pos = hash(ptrN->id,hashtable->i+1,hashtable->originaLm);
	}
	//printf("insertNodeHashTable: Inserting at HashTable Position: %d (hashtable->i: %d, hashtable->p: %d )\n",pos,hashtable->i,hashtable->p);

	/////////////////////////////////GET BUCKET CHAIN/////////////////////////////
	

	if(hashtable->hashmap[pos] == NULL){ /* No bucketChain in this position */
		
		if(createBucketChain(&(hashtable->hashmap[pos])) == EXIT_VALUE){ /* Create a new chain of buckets in this position */
			printf("insertNodeHashTable: createBucketChain failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity) == EXIT_VALUE){ /* Insert a new bucket in this chain */
			printf("insertNodeHashTable: addNewBucket failed! - Exiting!\n");
			return EXIT_VALUE;
		}
	}

	if(hashtable->hashmap[pos]->bucket_chain.items == 0){
		if(addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity) == EXIT_VALUE){
			printf("insertNodeHashTable: addNewBucket failed! - Exiting!\n");
			return EXIT_VALUE;			
		}
	}

	if(insertIDBucketChain(hashtable->hashmap[pos], ptrN, &split) == EXIT_VALUE){ /* Insert GraphNode* in this chain */
		printf("insertNodeHashTable: insertIDBucketChain failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	//printHashTable(hashtable);

	if(split) { /* Overflow occured in insertion */
		if(handleSplit(hashtable) == EXIT_VALUE){ /* Handle the HashTable split event */
			printf("insertNodeHashTable: handleSplit failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		//printf("CHECK!!!insertNodeHashTable: i= %d\n",hashtable->i);
		//printHashTable(hashtable);
	}
	(hashtable->num_records)++; /* Increase the total records counter of this hashtable */
	//printf("insertNodeHashTable: HashTable now contains: %ld records\n\n",hashtable->num_records);

	return 0;

}

int increaseHashTable(HashTable* hashtable,int oldsize,int new_size){ /* Increase the size of the hashmap in the HashTable */

	int i=0;

	if(hashtable == NULL){
		printf("increaseHashTable: HashTable is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(hashtable->hashmap == NULL){
		printf("increaseHashTable: HashMap is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	//printf("Increasing hashtable...New size: %d\n",new_size);			
	hashtable->hashmap = realloc(hashtable->hashmap, (new_size)*sizeof(BucketChain*)); 
	
	if (hashtable->hashmap == NULL) {
		printf("increaseHashTable: HashMap could not be reallocated! - Exiting!\n");
		return EXIT_VALUE; 
	}


	for(i=oldsize; i<new_size; i++)
		hashtable->hashmap[i] = (BucketChain*) NULL;

	return 0;

}

int printHashTable(HashTable* hashtable){ /* Print HashTable */

	int k=0;

	if(hashtable == NULL){
		printf("printHashTable: HashTable is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(hashtable->hashmap == NULL){
		printf("printHashTable: HashMap is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	printf("\n===============================PRINTING HASHTABLE==================================\n");
	printf("This Hashtable contains:\nRecords: %ld\nSize: %d\nBucket Capacity: %d\n\n",hashtable->num_records,hashtable->size,hashtable->bucket_capacity);

	for(k=0 ; k < hashtable->size; k++) {

		printf ("hashtable[%d]---->",k);

		if(hashtable->hashmap[k] != NULL){

			printBucketChain(hashtable->hashmap[k]);

		}
		else {
			printf ("NULL");
		}
		printf("\n");
	}
	printf("\n======================================================================================\n");

	return 0;

}

int rehashRecords(HashTable* hashtable){ /* Distribute records after Split */

	int i,j,k,ID,num_of_buckets,num_items=0,split=0,pos=-1,errorFlag=0;
	Bucket* sourceCurrent = (Bucket*) NULL, *targetCurrent = (Bucket*) NULL;
	BucketChain *sourceChain = (BucketChain*) NULL,*targetChain = (BucketChain*) NULL;
	
	if(hashtable == NULL){
		printf("rehashRecords: HashTable is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(hashtable->hashmap == NULL){
		printf("rehashRecords: HashMap is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	sourceChain = hashtable->hashmap[hashtable->p];

	//printf("\n=======================REHASH RECORDS=============================\n");
	//printf("//Check! i: %d ,p: %d\n",hashtable->i,hashtable->p);

	if(sourceChain == NULL) return 0; //Can't rehash records if current chain is NULL TODO: Check if valid!
	goToHeadBucket(sourceChain,&errorFlag); 
	if(errorFlag == EXIT_VALUE){ 							/* Get the first bucket of the source Chain of Buckets */
		printf("rehashRecords: goToHeadBucket failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	errorFlag=0;
	sourceCurrent = getCurrentBucket(sourceChain,&errorFlag); 			/* Source Current points to the First bucket of the chain now */
	if(errorFlag == EXIT_VALUE){
		printf("rehashRecords: getCurrentBucket failed! - Exiting!\n");
		return EXIT_VALUE;			
	}

	while(sourceCurrent != NULL){ 	/* For every bucket in the source Chain of Buckets */

		for(j=0; j < sourceCurrent->items; j++){ 						/* For every records in this bucket */

			if(sourceCurrent->GNArray[j] != NULL){ 			/* Records exists in here */

				ID = sourceCurrent->GNArray[j]->id; 		/* Extract the ID of this record */
				pos = hash( ID, hashtable->i + 1, hashtable->originaLm); 		/* Determine its position in the changed HashTable */
				//printf("rehashRecords: New position for ID: %d will be: %d\n" ,ID, pos);
				if(pos == hashtable->p){ 					/* Record remains where it is */
					//printf("rehashRecords: Record remains in the same bucket...\n\n");	
					continue;
				}
				else if(pos == (hashtable->p) + (hashtable->size)/2) { 		/* We have to move the record to another chain */
		
					if(hashtable->hashmap[pos] == NULL){ 					/* No bucketChain in this position */
		
						if(createBucketChain(&(hashtable->hashmap[pos])) == EXIT_VALUE){ 	/* Create a new chain of buckets in this position */
							printf("rehashRecords: createBucketChain failed! - Exiting!\n");
							return EXIT_VALUE;		
						}
						if(addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity) == EXIT_VALUE){ /* Insert a new bucket in this chain */
							printf("rehashRecords: addNewBucket failed! - Exiting!\n");
							return EXIT_VALUE;	
						}
					}

					if(hashtable->hashmap[pos]->bucket_chain.items == 0){
		
						if(addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity) == EXIT_VALUE){
							printf("rehashRecords: addNewBucket failed! - Exiting!\n");
							return EXIT_VALUE;
						}

					}

					targetChain = hashtable->hashmap[pos];
		
					//printf("rehashRecords: Transferring to new BucketChain...\n");

					if(transferBucketChain(sourceCurrent,j,targetChain) == EXIT_VALUE){
						printf("rehashRecords: transferBucketChain failed! - Exiting!\n");
						return EXIT_VALUE;
					}

					sourceChain->num_records--;

					//printf("rehashRecords: Original BucketChain total records are now : %d\n",sourceChain->num_records);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if(bucketChainConcat(sourceChain,&sourceCurrent,&j) == EXIT_VALUE){
						printf("rehashRecords: bucketChainConcat failed! - Exiting!\n");
						return EXIT_VALUE;
					}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//printHashTable(hashtable);

					if(sourceCurrent == NULL) break;
												
				}
				else{
					printf("rehashRecords: Invalid position in hashtable\n");
					return EXIT_VALUE;
				}
			}
		}
		errorFlag = 0;
		sourceCurrent = getNextBucket(sourceChain,sourceCurrent,&errorFlag); /* Move to the next bucket of the source Chain */
		if(errorFlag == EXIT_VALUE){
			printf("rehashRecords: getNextBucket Failed! - Exiting!\n");
			return EXIT_VALUE;
		}
	}

	return 0;
}

int handleSplit(HashTable* hashtable){ /* Handles Bucket Split event */

	int j=0,ID,oldsize=0,stop = 0;
	BucketChain* bucketChainPtr;
	
	//printf("\n****************************** HANDLE SPLIT *******************************\n");
	

	//printf("handleSplit: p--> %d\n",hashtable->p);
	//printf("handleSplit: hashtable->size: %d\n",hashtable->size);
	
	if(hashtable->p == hashtable->size/2){ //actual_size(4)/2 =2 after 8/2
		
		//printf("handleSplit: End of split round %d!!\n\n",hashtable->i);
		(hashtable->i)++;
		hashtable->p = 0;
		//printf("CHECK!!!handleSplit: i= %d\n",hashtable->i);
		//printf("CHECK!!!handleSplit: p= %d\n",hashtable->p);
		return 0;

	}
	if(hashtable->p < (hashtable->size/2) ) {  
	
		/*if first split of split round*/
		if(hashtable->p == 0) {
		
			oldsize = hashtable->size;
			hashtable->size = 2 * (hashtable->size); //size to realloc is a double size array to avoid linear augmentation
			//printf("handleSplit:  First split of round %d.\n\n",hashtable->i);
			if(increaseHashTable(hashtable,oldsize,hashtable->size) == EXIT_VALUE){ /* Resize hashtable */
				return EXIT_VALUE;
			}

		}

		/* Rehash all records in this bucket chain*/	

		if(rehashRecords(hashtable) == EXIT_VALUE){
			return EXIT_VALUE;
		} 
			
		(hashtable->p)++;
		//printf("\n handleSplit: p--> %d\n\n",hashtable->p);
			
		/*Check*/
		//printHashTable(hashtable);

	}
	//printf("\n***********************************************\n");

	return 0;

}

GraphNode* lookUpNodeHashTable(int nodeID,HashTable* hashtable,int* errorFlag){ /* Uses the HashTable to locate a record */

	int pos,j=0;
	GraphNode* ptrN;

	*errorFlag = 0;

	if(hashtable != NULL){
		
		if(hashtable->hashmap != NULL){

			if((hashtable->bucket_capacity <= 0) || (hashtable->originaLm <= 0)){
				printf("lookUpNodeHashTable: Invalid bucket capacity or size!\n");
				*errorFlag = EXIT_VALUE;
				return ((GraphNode*) NULL);	
			}
			else{
				pos = hash(nodeID,hashtable->i,hashtable->originaLm); /* Find hashtable position */
				if (pos < hashtable->p) {
					pos = hash(nodeID,hashtable->i+1,hashtable->originaLm);
				}
		
				if(hashtable->hashmap[pos] == NULL){
					return ((GraphNode*) NULL); /* Record was not found */	
				}

				if(searchIDBucketChain(hashtable->hashmap[pos],nodeID,&ptrN) == -1){ /* Look in this bucket chain */
					return ((GraphNode*) NULL); /* Record was not found */
				}
				else{
					return ptrN; /* Record was found */
				}				
			}
		}
		else{
			printf("lookUpNodeHashTable: HashMap has not been initialised!\n");
			*errorFlag = EXIT_VALUE;
			return ((GraphNode*) NULL);
		}
	}
	else{
		printf("lookUpNodeHashTable: HashTable has not been initialised!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) NULL);
	}
		
}
////////////////////////////////////////////GRAPH Ptr///////////////////////////////////////////////////////////
/* GraphPtr functions for List */

int allocateGraphPtr(void** target, void* source){ /* Allocation takes place outside */

	Graph** tempA = (Graph**) (target);
	Graph* tempB = (Graph*) (source);
	
	(*tempA) = tempB;

	return 0;

}

int destroyGraphPtr(void** target){ /* De-allocation takes place outside */

	Graph** temp = (Graph**) (target);
	//printf("Deleting Graph with ID: %d from List of Graphs!\n", (*temp)->id);//TODO does subgraph has id
	*temp = (Graph*) NULL;

	return 0;

}


int compareGraphPtr(void* target, void* source){ /* NOTE: This only compares the IDs (might be insufficient TODO*/

	/*Graph* tempA = (Graph*) (target);
	Graph* tempB = (Graph*) (source);
	if((tempA == NULL) || (tempB == NULL)){
		printf("compareGraphPtr: Graph comparison cannot occur with NULL pointers!\n");
		printf("compareGraphPtr: Sending EXIT Value!\n");
		return EXIT_VALUE;
	}
	if((tempA->id == tempB->id)) return 0;
	else if((tempA->id < tempB->id)) return 1;
	else return -1;
	*/
	return 0;
}

int assignGraphPtr(void* target,void* source){ //TODO: NOT READY, REMEMBER TO ALSO ASSIGN LISTS

	Graph* tempA = (Graph*) (target);
	Graph* tempB = (Graph*) (source);

	printf("assignGraphPtr: WARNING assignGraphPtr is under construction\nEdge assignment is not ready yet!\nSending EXIT Value!\n");
	return EXIT_VALUE;

}

int printGraphPtr(void* target){

	Graph* tempA = (Graph*) target;

	//if(tempA != NULL)
		//printf("%d",tempA->id);
	if (tempA == NULL) {
		printf("\nGraph pointer is NULL, can't print!\n");
		printf("\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}
	
	return 0;

}
//////////////////////////////////////////////////////////////////////////////


