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
			printf("Error! Could not allocate enough memory for Hash Table!\n");
			exit(1);
		}

		for(i=0; i < hashtable->originaLm; i++)
			hashtable->hashmap[i] = NULL;

	}
	else{
		printf("initHashTable: HashTable is not allocated!\n");
	}

	//printf("===================INITIALISED HASHTABLE ====================\n");
	//printf("HashTable original Size: %d - Bucket Capacity: %d\n\n",hashtable->size,hashtable->bucket_capacity);

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
		exit(1);
	}

}

BucketChain* getHashTableElement(HashTable* hashtable,int position){ /* Returns a pointer to a specific chain of buckets in the HashTable */

	if(hashtable == NULL){
		printf("getHashTableElement: HashTable is NULL!\n");
		exit(1);
	}

	if(hashtable->hashmap[position] == NULL){
		printf("getHashTableElement: HashMap is NULL!\n");
		exit(1);
	}

	return hashtable->hashmap[position];

}


int insertNodeHashTable(const GraphNode* const ptrN, HashTable* hashtable){ /* Will insert a GraphNode* in a bucket of chains of the HashTable */

	int split = 0;
	int pos,j,k,l,t, ID;
	static int size_in_use;	//size of hashtable that is in use	

	BucketChain* bucketChainPtr;	

	if(hashtable == NULL){
		printf("insertNodeHashTable: HashTable has not been initiliased! Exiting!\n");
		exit(1);
	}

	if(ptrN == NULL){
		printf("insertNodeHashTable: GraphNode is NULL! Exiting!\n");
		exit(1);
	}

	if(hashtable->hashmap == NULL){
		printf("insertNodeHashTable: HashMap is NULL! Exiting!\n");
		exit(1);
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
		
		createBucketChain(&(hashtable->hashmap[pos])); /* Create a new chain of buckets in this position */

		addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity); /* Insert a new bucket in this chain */

	}

	if(hashtable->hashmap[pos]->bucket_chain.items == 0){
		
		addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity);

	}

	insertIDBucketChain(hashtable->hashmap[pos], ptrN, &split); /* Insert GraphNode* in this chain */

	//printHashTable(hashtable);

	if(split) { /* Overflow occured in insertion */

		handleSplit(hashtable); /* Handle the HashTable split event */
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
		printf("increaseHashTable: HashTable is NULL!\n");
		exit(1);
	}

	if(hashtable->hashmap == NULL){
		printf("increaseHashTable: HashMap is NULL!\n");
		exit(1);
	}

	//printf("Increasing hashtable...New size: %d\n",new_size);			
	hashtable->hashmap = realloc(hashtable->hashmap, (new_size)*sizeof(BucketChain*)); 
	
	if (hashtable->hashmap == NULL) {
		printf("increaseHashTable: HashMap could not be reallocated!\n");
		return 1; 
	}


	for(i=oldsize; i<new_size; i++)
		hashtable->hashmap[i] = NULL;

}

void printHashTable(HashTable* hashtable){ /* Print HashTable */

	int k=0;

	if(hashtable == NULL){
		printf("printHashTable: HashTable is NULL!\n");
		exit(1);
	}

	if(hashtable->hashmap == NULL){
		printf("printHashTable: HashMap is NULL!\n");
		exit(1);
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

}

int rehashRecords(HashTable* hashtable){ /* Distribute records after Split */

	int i,j,k,ID,num_of_buckets,num_items=0,split=0,pos=-1;
	Bucket* sourceCurrent, *targetCurrent;
	BucketChain *sourceChain,*targetChain;
	
	if(hashtable == NULL){
		printf("rehashRecords: HashTable is NULL!\n");
		exit(1);
	}

	if(hashtable->hashmap == NULL){
		printf("rehashRecords: HashMap is NULL!\n");
		exit(1);
	}

	sourceChain = hashtable->hashmap[hashtable->p];

	//printf("\n=======================REHASH RECORDS=============================\n");
	//printf("//Check! i: %d ,p: %d\n",hashtable->i,hashtable->p);

	goToHeadBucket(sourceChain); 							/* Get the first bucket of the source Chain of Buckets */
	sourceCurrent = getCurrentBucket(sourceChain); 			/* Source Current points to the First bucket of the chain now */

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
		
						createBucketChain(&(hashtable->hashmap[pos])); 	/* Create a new chain of buckets in this position */

						addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity); /* Insert a new bucket in this chain */

					}

					if(hashtable->hashmap[pos]->bucket_chain.items == 0){
		
						addNewBucket(hashtable->hashmap[pos],hashtable->bucket_capacity);

					}

					targetChain = hashtable->hashmap[pos];
		
					//printf("rehashRecords: Transferring to new BucketChain...\n");

					transferBucketChain(sourceCurrent,j,targetChain);

					sourceChain->num_records--;

					//printf("rehashRecords: Original BucketChain total records are now : %d\n",sourceChain->num_records);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					bucketChainConcat(sourceChain,&sourceCurrent,&j);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					//printHashTable(hashtable);

					if(sourceCurrent == NULL) break;
												
				}
				else{
					printf("rehashRecords: Invalid position in hashtable\n");
					exit(1);
				}
			}
		}
		sourceCurrent = getNextBucket(sourceChain,sourceCurrent); /* Move to the next bucket of the source Chain */
	}

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
			increaseHashTable(hashtable,oldsize,hashtable->size); /* Resize hashtable */

		}

		/* Rehash all records in this bucket chain*/	

		rehashRecords(hashtable); 
			
		(hashtable->p)++;
		//printf("\n handleSplit: p--> %d\n\n",hashtable->p);
			
		/*Check*/
		//printHashTable(hashtable);

	}
	//printf("\n***********************************************\n");

}

GraphNode* lookUpNodeHashTable(int nodeID, const struct HashTable* hashtable){ /* Uses the HashTable to locate a record */

	int pos,j=0;
	GraphNode* ptrN;

	if(hashtable != NULL){
		
		if(hashtable->hashmap != NULL){

			if((hashtable->bucket_capacity <= 0) || (hashtable->originaLm <= 0)){
				printf("lookUpNodeHashTable: Invalid bucket capacity or size!\n");
				exit(1);			
			}
			else{
				pos = hash(nodeID,hashtable->i,hashtable->originaLm); /* Find hashtable position */
				if (pos < hashtable->p) {
					pos = hash(nodeID,hashtable->i+1,hashtable->originaLm);
				}
		
				if(hashtable->hashmap[pos] == NULL){
					return NULL; /* Record was not found */	
				}

				if(searchIDBucketChain(hashtable->hashmap[pos],nodeID,&ptrN) == -1){ /* Look in this bucket chain */
					return NULL; /* Record was not found */
				}
				else{
					return ptrN; /* Record was found */
				}				
			}
		}
		else{
			printf("lookUpNodeHashTable: HashMap has not been initialised!\n");
			exit(1);
		}
	}
	else{
		printf("lookUpNodeHashTable: HashTable has not been initialised!\n");
		exit(1);
	}
		
}
