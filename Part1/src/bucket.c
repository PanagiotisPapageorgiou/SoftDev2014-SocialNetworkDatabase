#include <stdio.h>					
#include <stdlib.h>					
#include <assert.h>	
#include "bucket.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: bucket.c
 MAIN AUTHOR: GEORGE LAMPRAKOPOYLOS														

 Details: This file contains all the functions used to handle the buckets and bucketChains
 of the Linear Hashing HashTable. 

 Each cell of the HashTable is able to hold a BucketChain. A BucketChain is a List of pointers 
 to Buckets in memory. Each Bucket contains an array (GNArray) of GraphNode*. Arrays were chosen
 to hold the GraphNode* in order to use binary search and find the desired records faster.

================================================================================================*/

/////////////////////////////////////GENERAL BUCKET FUNCTIONS /////////////////////////////////////////////

void initBucket(Bucket *b, int c){ /* This function initialises the contents of a Bucket */

	int i=0;

	if(b != NULL){
		b->c = c;
		b->items = 0;
		b->GNArray = NULL;
		b->GNArray = malloc(sizeof(GraphNode*) * c); /* Create an array of GraphNode* */
		b->last = 0;
		b->middle = 0;

		if(b->GNArray == NULL){
			printf("initBucket: Could not allocate enough memory for GNArray!\n");
			exit(1);
		}

		for(i=0; i < b->c; i++)
			b->GNArray[i] = NULL;
	}
	else{
		printf("initBucket: You need to allocate memory for Bucket first!\n");
		exit(1);
	}

}

void clearBucket(Bucket *b){ /* This function frees the contents of a Bucket */

	if(b != NULL){
		if(b->GNArray != NULL)
			free(b->GNArray);
		b->items = 0;
	}
	else{
		printf("emptyBucket: You need to allocate memory for Bucket first!\n");
		exit(1);
	}

}

int getItemsBucket(Bucket *b){ /* Return the number of items (GraphNode pointers) in this bucket */

	if(b != NULL){
		return b->items;
	}
	else{
		printf("getItemsBucket: Bucket Pointer is NULL!\n");
		exit(1);
	}

}

int isEmptyBucket(Bucket* b){ /* Checks if Bucket is empty */

	if(b != NULL){
		if(b->items == 0) return 1;
		else return 0;
	}
	else{
		printf("isEmptyBucket: Bucket Pointer is NULL!\n");
		exit(1);
	}

}

int isFullBucket(Bucket* b){ /* Checks if Bucket is full */

	if(b != NULL){
		if(b->items == b->c) return 1;
		else return 0;
	}
	else{
		printf("isFullBucket: Bucket Pointer is NULL!\n");
		exit(1);
	}

}

int insertWhere(Bucket *b, const struct GraphNode* gnode) { /* Determines where in the Bucket the GraphNode will fit */

	int i=0, where=0;

	if(b == NULL){
		printf("insertWhere: Bucket is NULL!\n");
		exit(1);
	}

	if(gnode == NULL){
		printf("insertWhere: GraphNode is NULL!\n");
		exit(1);
	}

	if(b->GNArray[0] == NULL){
		printf("insertWhere: MoveLeft has not worked properly! CASE 1 - Items: %d - Last: %d\n",b->items,b->last);
		exit(1);
	}


	while (b->GNArray[i]->id < gnode->id) {		
		where ++;
		if (i+1  < b->items)
			i++;
		else break;
		if(b->GNArray[i] == NULL){
			printf("insertWhere: MoveLeft has not worked properly! CASE 2 - Items: %d - Last: %d\n",b->items,b->last);
			exit(1);
		}
	}
	return where;
}

int insertIDBucket(BucketChain* chain_ptr, Bucket* b, const struct GraphNode* gnode){ /* Inserts a GraphNode* in the Array of the Bucket */
	int where=0, i=0;

	assert (chain_ptr != NULL);
	assert (b != NULL);
	assert (gnode != NULL);

	if (isEmptyBucket(b))	
		b->GNArray[0] = gnode;				// just insert at start
	else if (! isFullBucket(b)) {
		where = insertWhere(b, gnode);
		moveRightBucket(b, where);			// move the bigger values right
		b->GNArray[where] = gnode;			// insert the new one
		b->last ++;					
	}
	else return -1;							// this function is not responsible for creating a new bucket
	if (b->items < b->c) 
		b->items ++;
	b->middle = b->last/2;					// it is always rounded down (ex. 5/2 = 2)

	chain_ptr->num_records++; /* Increase the record counter of this chain */

	/*printf("insertIDBucket: Inserted ID: %d in GNARRAY position : %d\n",gnode->id,where);
	printf("insertIDBucket: Items: %d - Last: %d\n",b->items,b->last);
	printf("\ninsertIDBucket: Printing the GNArray: \n");

	for(i=0; i <= b->last; i++){
		if(b->GNArray[i] == NULL){
			printf("NULL-");
		}
		else printf("%d-",b->GNArray[i]->id);
	}
	printf("\n");*/

	return 0;
}


int searchIDBucket(Bucket* b,int id,GraphNode** ptrN){ /* Searches for a specific GraphNode in Bucket and returns it*/

	if(b == NULL){
		printf("searchIDBucket: Bucket is NULL!\n");
		exit(1);
	}

	int location = binary_search(b,id);
	if(location == -1){
		*ptrN = NULL;
		return -1;
	}
	else{ /* Record was found! */
		*ptrN = b->GNArray[location];
		return 1;
	}


} 

int printBucketContents(Bucket* b){ /* Prints all IDs of bucket */

	int i=0;
	
	if(b != NULL){
		if(b->GNArray != NULL){
			for(i=0; i< b->c; i++){
				if(b->GNArray[i] != NULL){
					printGraphNodePtr( (void*) b->GNArray[i]);
				}
				else{
					printf("NULL");
				}
				printf("-");
			}
		}
		else{
			printf("NULL");
		}
	}
	else{
		printf("printBucketContents: Bucket is NULL!\n");
		exit(1);
	}

}

int binary_search(Bucket *b, int value) {
	int i;

	assert (b != NULL);
	return binary_search_lvl2(b->GNArray, value, 0, b->items-1);
} 

int binary_search_lvl2(GraphNode **bucket, int value, int low, int high) { 
	int mid;

	if (high < low) return -1;
	mid = low + (high-low)/2;
	if (bucket[mid]->id > value)
		return binary_search_lvl2(bucket, value, low, mid-1);
	else if (bucket[mid]->id < value)
		return binary_search_lvl2(bucket, value, mid+1, high);
	else
		return mid;			
}

/////////////////////////////////////BUCKETCHAIN FUNCTIONS ////////////////////////////////////////////////

void createBucketChain(BucketChain** chain_ptr){ /* Allocates Memory for Bucket Chain struct */

	if(chain_ptr != NULL){
		*chain_ptr = NULL;
		*chain_ptr = malloc(sizeof(BucketChain));

		if(*chain_ptr == NULL){
			printf("createBucketChain: Failed to allocate memory for bucket chain!\n");
			exit(1);
		}

		(*chain_ptr)->num_records = 0;
		createList(&((*chain_ptr)->bucket_chain), "BucketPtr", &allocateBucketPtr, &destroyBucketPtr, &compareBucketPtr, &assignBucketPtr, &printBucketPtr); /* Creates a list of buckets */
	}
	else{
		printf("createBucketChain: HashTable is NULL!\n");
		exit(1);
	}

}

void destroyBucketChain(BucketChain** chain_ptr){ /* Destroy the chain of buckets */

	if(chain_ptr != NULL){
		if(*chain_ptr != NULL){
			destroyList(&((*chain_ptr)->bucket_chain));
			free(*chain_ptr);
		}
	}
	else{
		printf("destroyBucketChain: HashTable is NULL!\n");
		exit(1);		
	}

}

void addNewBucket(BucketChain* chain_ptr,int bucket_capacity){ /* Adds a new overflow bucket */
															   /* NOTE: The Current pointer of the Bucket Chain will be pointing to the new Bucket after this operation! */

	if(chain_ptr != NULL){
		addLastList(&(chain_ptr->bucket_chain), (void*) (&bucket_capacity) ); /* Add a new Bucket at the end of the chain! */
	}
	else{
		printf("addNewBucket: Bucket Chain Pointer is NULL!\n");
		exit(1);
	}

}

void destroyFirstBucket(BucketChain* chain_ptr){ /* Destroys the first bucket of the bucket chain */

	if(chain_ptr == NULL){
		printf("destroyFirstBucket: Bucket Chain Pointer is NULL!\n");
		exit(1);
	}

	popList(&(chain_ptr->bucket_chain));

}

void destroyLastBucket(BucketChain* chain_ptr){ /* Destroys the last bucket of the bucket chain */

	if(chain_ptr == NULL){
		printf("removeLastList: Bucket Chain Pointer is NULL!\n");
		exit(1);
	}

	removeLastList(&(chain_ptr->bucket_chain));

}

void destroyCurrentBucket(BucketChain* chain_ptr){ /* Destroys the bucket that Current(uberlist's Current) points to */

	if(chain_ptr == NULL){
		printf("removeCurrentNode: Bucket Chain Pointer is NULL!\n");
		exit(1);
	}

	removeCurrentNode(&(chain_ptr->bucket_chain));

}

int isEmptyBucketChain(BucketChain* chain_ptr){ /* Checks if chain of bucket has any buckets */
	
	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.items > 0) return 0;
		else return 1;
	}
	else{
		printf("isEmptyBucketChain: Bucket Chain is NULL!\n");
		exit(1);
	}

}

int getItemsChainBucket(BucketChain* chain_ptr){ /* Returns the number of buckets in this chain */

	if(chain_ptr != NULL){
		return chain_ptr->bucket_chain.items;
	}
	else{
		printf("getItemsChainBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

Bucket* getHeadBucket(BucketChain* chain_ptr){ /* Returns the first bucket of the chain */

	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.Head == NULL){
			printf("getHeadBucket: List is NULL!\n");
			exit(1);
		}
		return (Bucket*) chain_ptr->bucket_chain.Head->data;
	}
	else{
		printf("getHeadBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

Bucket* getLastBucket(BucketChain* chain_ptr){ /* Returns the last bucket of the chain */

	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.Last == NULL){
			printf("getLastBucket: List is NULL!\n");
			exit(1);
		}
		return (Bucket*) chain_ptr->bucket_chain.Last->data;
	}
	else{
		printf("getLastBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

Bucket* getCurrentBucket(BucketChain* chain_ptr){ /* Returns the current(uberlist's Current) bucket of the chain */

	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.Current != NULL)
			return (Bucket*) chain_ptr->bucket_chain.Current->data;
		else
			return NULL;
	}
	else{
		printf("getCurrentBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

Bucket* getNextBucket(BucketChain* chain_ptr, Bucket* initialBucket){ /* Given a bucket this function returns the next one in the BucketChain */

	Node* CurrentNode=NULL; /* We use this pointer in order not to messup the Current Pointer of the uberlist */
	Bucket* CurrentBucket=NULL;

	if(chain_ptr != NULL){
		if(initialBucket == NULL) /* Given Bucket is NULL */
			return NULL;
		
		CurrentNode = chain_ptr->bucket_chain.Head; /* Bucket List is NULL */
		if(CurrentNode == NULL) return NULL;

		CurrentBucket = (Bucket*) CurrentNode->data; /* Bucket is NULL */
		if(CurrentBucket == NULL) return NULL;

		while(CurrentBucket != initialBucket){ /* Run through the UberList until you locate the desired Bucket */
			CurrentNode = CurrentNode->next;
			if(CurrentNode == NULL) return NULL;
			CurrentBucket = (Bucket*) CurrentNode->data; /* Extract bucket from List */
		}
	
		CurrentNode = CurrentNode->next; /* Get the next bucket */
		if(CurrentNode == NULL) return NULL;
		return (Bucket*) CurrentNode->data;

	}
	else{
		printf("getNextBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

Bucket* getPreviousBucket(BucketChain* chain_ptr, Bucket* initialBucket){ /* Given a bucket this function returns the previous one in the BucketChain */

	Node* CurrentNode=NULL; /* We use this pointer in order not to messup the Current Pointer of the uberlist */
	Bucket* CurrentBucket=NULL;

	if(chain_ptr != NULL){
		if(initialBucket == NULL)
			return NULL;
		
		CurrentNode = chain_ptr->bucket_chain.Head;
		if(CurrentNode == NULL) return NULL;

		CurrentBucket = (Bucket*) CurrentNode->data;
		if(CurrentBucket == NULL) return NULL;

		while(CurrentBucket != initialBucket){
			CurrentNode = CurrentNode->previous;
			if(CurrentNode == NULL) return NULL;
			CurrentBucket = (Bucket*) CurrentNode->data;
		}
	
		CurrentNode = CurrentNode->previous;
		if(CurrentNode == NULL) return NULL;
		return (Bucket*) CurrentNode->data;

	}
	else{
		printf("getPreviousBucket: Bucket Chain is NULL!\n");
		exit(1);
	}

}

int goToHeadBucket(BucketChain* chain_ptr){ /* Moves Current to Head of the BucketChain */

	if(chain_ptr != NULL){
		chain_ptr->bucket_chain.Current = chain_ptr->bucket_chain.Head;
	}
	else{
		printf("goToHeadBucketn: Bucket Chain is NULL!\n");
		exit(1);
	}
	
}

int goToLastBucket(BucketChain* chain_ptr){ /* Moves Current to Last of the BucketChain */

	if(chain_ptr != NULL){
		chain_ptr->bucket_chain.Current = chain_ptr->bucket_chain.Last;
	}
	else{
		printf("goToLastBucket: Bucket Chain is NULL!\n");
		exit(1);
	}
	
}

int goToNextBucket(BucketChain* chain_ptr){ /* Moves Current(uberlist's Current) to Next Bucket in chain */

	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.items > 0)
			if(chain_ptr->bucket_chain.Current != NULL)
				chain_ptr->bucket_chain.Current = chain_ptr->bucket_chain.Current->next;
	}
	else{
		printf("goToNextBucket: Bucket Chain is NULL!\n");
		exit(1);
	}
}

int goToPreviousBucket(BucketChain* chain_ptr){ /* Moves Current(uberlist's Current) to Previous Bucket in chain */

	if(chain_ptr != NULL){
		if(chain_ptr->bucket_chain.items > 0)
			if(chain_ptr->bucket_chain.Current != NULL)
				chain_ptr->bucket_chain.Current = chain_ptr->bucket_chain.Current->previous;
	}
	else{
		printf("goToPreviousBucket: Bucket Chain is NULL!\n");
		exit(1);
	}
}

int searchIDBucketChain(BucketChain* chain_ptr,int id,GraphNode** ptrN){ /* Locates and returns a specific GraphNode in the chain of buckets */

	int found=0;
	Bucket* currentBucket;

	if(chain_ptr != NULL){

		if(!isEmptyBucketChain(chain_ptr)){ /*If buckets exist in this chain */

			goToHeadBucket(chain_ptr); /* Move Current Pointer of List to Head */
			currentBucket = getCurrentBucket(chain_ptr); /* Get the Current Bucket */

			while(currentBucket != NULL){ /* For all Buckets in the chain */

				found = searchIDBucket(currentBucket,id,ptrN); /* Search for ID in Bucket */
				if(found == 1) return 1; /* ID was found! */

				goToNextBucket(chain_ptr); /* Move Current to Next Bucket */
				currentBucket = getCurrentBucket(chain_ptr); /* Get the Next Bucket */

			}
			
			return -1; /* ID does not exist in Chain */
			
		}
		else
			return -1; /* No buckets in this chain */
	}
	else{ /* Chain is not initialised */
		printf("searchIDBucketChain: This chain of buckets is NULL!\n");
	}

}

int insertIDBucketChain(BucketChain* chain_ptr, const struct GraphNode* gnode,int* split) { /* Inserts ID in a bucket of the Bucket Chain */

	Bucket *b=NULL, *newb, *a;
	GraphNode *saved;
	int last_id, buckets_left, i=1, overflow=0, o_f, where;

	assert (gnode != NULL);
	assert (chain_ptr != NULL);

	goToHeadBucket(chain_ptr);
	b = getHeadBucket(chain_ptr);

	assert(b != NULL);

	if (isEmptyBucket(b)) {
		//printf("insertIDBucketChain: Simply inserting ID: %d in empty bucket!\n",gnode->id);
		insertIDBucket(chain_ptr, b, gnode);
		return 0;
	}		
	while ( isFullBucket(b) && (b->GNArray[b->last]->id < gnode->id) ) {	
		//printf("Checking if last = %d is less than %d\n", b->GNArray[b->last]->id, gnode->id);
		// loop request: The last element of each full bucket to be smaller than gnode's.
		// Upon reaching a non full bucket, exit. If during the loops we encounter a last
		// element of a full bucket that is greater than gnode's, we force the latter to
		// be inserted there, pushing all elements of the bucket chain one place to the right
		newb = getNextBucket(chain_ptr, b);			// attempt to go to the next bucket
		//if (getLastBucket(chain_ptr) == b) printf("CORRECTLY ADDED A BUCKET\n");
		if (newb == NULL) {	//printf("Added a new bucket!\n");						// reached last bucket, create a new one and
			addNewBucket(chain_ptr, b->c);			// afterwards, add gnode's element there
			b = getCurrentBucket(chain_ptr);
			*split = 1; /* Hash needs to know, overflow occured during insertion */
		}
		else b = newb;								// we go to the next bucket, new or not, whatsoever
	}		
	o_f = insertIDBucket(chain_ptr, b, gnode);		// b is now the bucket that gnode should be inserted
	//if (o_f == -1) printf("Attempt to insert to full bucket with last = %d\n", b->GNArray[b->last]->id);
	//else printf("Insertion to non-full bucket with last = %d\n", b->GNArray[b->last]->id);
	if (o_f == -1) {								// b is full, its last element will be moved to the right bucket
		saved = b->GNArray[b->last];				// save the last bucket element, for it 'll be moved to the next bucket
		b->GNArray[b->last] = NULL;
		if(b->last > 0)
			b->last--;
		if(b->items > 0)
			b->items--;
		where = insertWhere(b, gnode);				// determine where in bucket 'b', 'gnode' will be inserted
		moveRightBucket(b, where);					// move right all elements of bucket 'b' that are right from 'where' 
		b->GNArray[where] = gnode;
		b->last++;
		b->items++;

		/*printf("\ninsertIDBucketChain: Inserted record: %d in where: %d but Bucket has overflow so last record: %d will be carried over!\n",b->GNArray[where]->id,where,saved->id);
		printf("insertIDBucketChain: Items: %d - Last: %d\n",b->items,b->last);
		printf("insertIDBucketChain: Printing the GNArray: \n");

		for(i=0; i <= b->last; i++){
			printf("%d-",b->GNArray[i]->id);
		}
		printf("\n");*/

		//printf("Called recursive insert for  %d  \n", saved->id);
		insertIDBucketChain(chain_ptr, saved, split);
		//printf("End of recursive call\n");
	}
	//printf("~~~~~ %d ~~~~~\n", numBucket(chain_ptr, b));
}


void moveRightBucket(Bucket* b, int from) {			// moves the elements of bucket 'b' after 'from'
	int i;											// one position to the right
	
	//printf("ITEMS: %d | FROM: %d\n", b->items, from);
	if(b == NULL){
		printf("moveRightBucket: Bucket is NULL!\n");
		exit(1);
	}

	if(b->GNArray == NULL){
		printf("moveRightBucket: Bucket Array is NULL!\n");
		exit(1);
	}

	assert (from <= b->items);

	if(b->last == b->c-1){
		printf("moveRightBucket: This function must not be called with full bucket!\n");
		exit(1);
	}

	for (i = b->last ; i >= from ; i--) 			// not calling this one with a full bucket
		b->GNArray[i+1] = b->GNArray[i];

	/*printf("\n==========================MOVERIGHT GNARRAY=============================\n");
	printf("moveRightBucket: GNArray is: \n");
	for(i=0; i < b->c; i++){
		if(b->GNArray[i] == NULL){
			printf("NULL-");
		}
		else{
			printf("%d-",b->GNArray[i]->id);
		}
	}
	printf("\n");*/

}

int numBucket(BucketChain *chain_ptr, Bucket *b) {
	Bucket *curr_b;
	int num=1;
	
	curr_b = getHeadBucket(chain_ptr);
	while (curr_b != b) { 
		curr_b = getNextBucket(chain_ptr, curr_b);
		assert (curr_b != NULL); 
		num ++;
	}
	return num;
}

int transferBucketChain(Bucket* sourceCurrent,int j ,BucketChain* targetChain){ /* This function carries over a record from a specific bucket to a bucketChain
																				   leaving NULL in the old position - It does not fix the gaps*/

	int split = 0,i=0;
	GraphNode* temp;

	if(targetChain == NULL){
		printf("transferBucketChain: targetChain has not been allocated!\n");
		exit(1);
	}
	
	if(sourceCurrent == NULL){
		printf("transferBucketChain: sourceCurrent is NULL!\n");
		exit(1);
	}

	if(sourceCurrent->GNArray[j] == NULL){
		printf("transferBucketChain: Record is NULL!\n");
		exit(1);
	}

	insertIDBucketChain(targetChain, sourceCurrent->GNArray[j], &split);

	sourceCurrent->GNArray[j] = NULL;	 /* Transfer the Record in position j in this bucket, to the new bucket! */
	sourceCurrent->items--;
	if(j == sourceCurrent->last){
		if(j > 0)
			sourceCurrent->last = sourceCurrent->last - 1;	
	}

	/*printf("\n==============TransferBucketChain occured===============\n");
	printf("TransferBucketChain: Last is %d - NEEDS CONCATENATE\n",sourceCurrent->last);
	printf("TransferBucketChain: Items are %d\n",sourceCurrent->items);
	printf("TransferBucketChain: GNArray is: \n");
	for(i=0; i < sourceCurrent->c; i++){
		if(sourceCurrent->GNArray[i] == NULL){
			printf("NULL-");
		}
		else{
			printf("%d-",sourceCurrent->GNArray[i]->id);
		}
	}
	printf("\n");*/

}

int moveLeftBucket(Bucket* currentBucket){ /* TODO: Make second loop faster by making it start where its previous one ended */

	int i=0,k=0,first_not_NULL=0;

	currentBucket->last = 0;

	/*printf("BEFORE: =======================MOVELEFT DEBUG=====================\n");
	printf("MoveLeftBucket: Last is %d\n",currentBucket->last);
	printf("MoveLeftBucket: Items are %d\n",currentBucket->items);
	printf("GNArray is: \n");
	for(i=0; i < currentBucket->c; i++){
		if(currentBucket->GNArray[i] == NULL){
			printf("NULL-");
		}
		else{
			printf("%d-",currentBucket->GNArray[i]->id);
		}
	}
	printf("\n");*/

	for(i = 0; i < currentBucket->c; i++){
		if((currentBucket->GNArray[i] == NULL) && (i != currentBucket->c-1)){ /* Gap located and it is not in the end of the array */
			for(k=i+1; k < currentBucket->c; k++){ /* Look if there are any non NULL entries in the array to move one of them */
				if(currentBucket->GNArray[k] != NULL){
					currentBucket->GNArray[i] = currentBucket->GNArray[k]; /* Place the non-null entry in the gap */
					/*printf("=======================MOVELEFT DEBUG=====================\n");
					printf("Last has been set to %d and items are %d\n",i,currentBucket->items);*/
					currentBucket->GNArray[k] = NULL; /* Leave a gap in place of the entry */
					break;
				}
			}
		}
	}

	for(i = 0; i < currentBucket->c; i++){
		if(currentBucket->GNArray[i] != NULL){
			currentBucket->last = i;
		}
	}

	currentBucket->middle = currentBucket->last / 2;

	//printf("\n=======================MOVELEFT GNARRAY=====================\n");
	//printf("MoveLeftBucket: Last is %d\n",currentBucket->last);
	//printf("MoveLeftBucket: Items are %d\n",currentBucket->items);
	//printf("MoveLeftBucket: GNArray is: \n");
	//for(i=0; i < currentBucket->c; i++){
		//if(currentBucket->GNArray[i] == NULL){
			//printf("NULL-");
	//	}
		//else{
			//printf("%d-",currentBucket->GNArray[i]->id);
		//}
	//}
	//printf("\n");

}

int bucketChainConcat(BucketChain* sourceChain,Bucket** sourceBucket,int* j){ /* This function closes any gaps left behind by transfer in the BucketChain */

	int i=0,k=0,flag_hungry=0;
	Bucket* currentBucket=NULL,*nextBucket=NULL;
	Node* currentBackup=NULL;

		/*Three senarios play here:

		1. Bucket has no items left (Will be deleted)
		2. Bucket is full (We move to the next one)
		3. Bucket is not full (We feed using the ones after it). Feeding will stop when:
			a. Bucket is full (we move to the next one if this happened)
			b. Bucket has no next Buckets that can feed it */

	if(sourceChain == NULL){
		printf("bucketChainConcat: Chain is NULL!\n");
		exit(1);
	}

	//printf("\n-------------------------BUCKET CONCATENATION---------------------------\n");

	if(sourceChain->bucket_chain.items > 0){

		goToHeadBucket(sourceChain);
		currentBucket = getCurrentBucket(sourceChain); /* We get the Head of the Bucket Chain aka first Bucket */

		while(currentBucket != *sourceBucket){ /* Go to the current Bucket we are examining in rehash */
			goToNextBucket(sourceChain);
			currentBucket = getCurrentBucket(sourceChain);
		}

		//printf("bucketChainConcat: Located the wanted Bucket in Chain...\n");

		if(currentBucket->items == 0){ /* This Bucket is empty let's destroy it */
			*sourceBucket = getNextBucket(sourceChain,*sourceBucket);
			destroyCurrentBucket(sourceChain);
			currentBucket = getCurrentBucket(sourceChain);
			*j = -1;
			//printf("bucketChainConcat: Given Bucket was empty so it is destroyed\n\n");
			if(currentBucket == NULL) return -1;
		}
		else if(currentBucket->items == currentBucket->c){ /* This should never happen for the initial bucket since this function is called after transfer */
			printf("BucketChainConcat: A record was transferred from this but Bucket is full?!\n");
			exit(1);
		}
		else{
			moveLeftBucket(currentBucket); /* Gather together to the left all the records in this bucket */
			*j = *j - 1;
			//printf("bucketChainConcat: All records of the given bucket were carried to the left!\n");
		}

		//printf("bucketChainConcat: Carrying to the left the rest of the chain!\n");
 
		while(currentBucket != NULL){ /* Now carry to the left all of the chain because space was created! */

			flag_hungry = 0;

			if(currentBucket->items == 0){ /* This Bucket is empty let's destroy it */
				//printf("bucketChainConcat: This bucket is empty let's destroy it!\n");
				destroyCurrentBucket(sourceChain);
				currentBucket = getCurrentBucket(sourceChain);
				if(currentBucket == NULL) return -1; /* Reached end of the chain */
			}
			else if(currentBucket->items == currentBucket->c){ /* This Bucket is not hungry, move to the next one */
				//printf("bucketChainConcat: This bucket is full let's skip it!\n");
				goToNextBucket(sourceChain);
				currentBucket = getCurrentBucket(sourceChain);
				if(currentBucket == NULL) return -1; /* Reached end of the chain */
			}
			else{ /* Feeding time */

				flag_hungry = 1;
				//printf("bucketChainConcat: This bucket is hungry!\n");

				moveLeftBucket(currentBucket); /* Gather together to the left all the records in this bucket */

				//printf("bucketChainConcat: All of its records were moved to the left!\n");
				/* Now we have pulled together to the left of the array all the non null entries */

				nextBucket = getNextBucket(sourceChain, currentBucket); /* Get the next Bucket */
				if(nextBucket == NULL){ /* No next Bucket to feed the Current one */
					return -1;
				}
				else{
					do{
						while((currentBucket->items < currentBucket->c) && (nextBucket->items > 0)){ /* While hungry and next is able to feed */
							currentBucket->items++;
							currentBucket->last++;
							currentBucket->middle = currentBucket->last / 2;
				
							for(k=0; k < nextBucket->c; k++){ /* Locate a non NULL record from the nextBucket and carry it over to the Current */
								if(nextBucket->GNArray[k] != NULL){ /* TODO: Make this loop faster by making it start where its previous one ended */
									currentBucket->GNArray[currentBucket->last] = nextBucket->GNArray[k];
									nextBucket->GNArray[k] = NULL;
									nextBucket->items--;
									break;
								}
							}
			
							//printf("bucketChainConcat: Carried over record...Current Bucket has LAST: %d - Items: %d\nNeighbour bucket has items: %d\n",currentBucket->last,currentBucket->items,nextBucket->items);
						}
						if(currentBucket->items == currentBucket->c){
							//printf("This bucket is NOT hungry anymore...\n");
							flag_hungry = 0;
						}
						if(nextBucket->items == 0){
							//printf("bucketChainConcat: The nextBucket is empty! Move the next one!\n");
							goToNextBucket(sourceChain);
							destroyCurrentBucket(sourceChain);
							goToPreviousBucket(sourceChain);
							nextBucket = getNextBucket(sourceChain, currentBucket);
							if(nextBucket == NULL) return -1;
						}

					}while(flag_hungry == 1);

				}

			}
		}
	}

}

void printBucketChain(BucketChain* chain_ptr){ /* Prints contents of all buckets in chain */

	if(chain_ptr != NULL){

		printList(chain_ptr->bucket_chain);

		printf("\n\n");

	}
	else{
		printf("printBucketChain: Bucket Chain is NULL!\n");
		exit(1);
	}

}

///////////////////////////// BUCKET PTR FUNCTIONS FOR LIST (Needed to Create List of Bucket*) //////////////////////////////

int allocateBucketPtr(void** new_bucket, void* bucket_capacity){ /* Allocates a new bucket as data for List */ 

	Bucket** tempBucket = (Bucket**) new_bucket;
	int* c = (int*) bucket_capacity;

	*tempBucket = malloc(sizeof(Bucket));

	if(*tempBucket == NULL){
		printf("allocateBucketPtr: Failed to allocate memory for bucket!\n");
		exit(1);
	}
	
	initBucket(*tempBucket, *c);

}

int destroyBucketPtr(void** target){

	Bucket** tempA = (Bucket**) target;

	if(tempA != NULL){
		if(*tempA != NULL){
			clearBucket(*tempA);
			free(*tempA);
		}
	}

}

int compareBucketPtr(void* target, void* source){

	printf("compareBucketPtr: Not ready for use - No reason to compare pointers!\n");
	exit(1);

}

int assignBucketPtr(void* target,void* source){

	printf("assignBucketPtr: Not ready for use - No reason to assign pointers!\n");
	exit(1);

}

void printBucketPtr(void* target){

	Bucket* temp = (Bucket*) target;

	if(temp != NULL){
		//printf("printBucketPtr: Printing the contents of this bucket!\n");
		printBucketContents(temp);
	}
	else{
		printf("printBucketPtr: Bucket pointer is NULL!\n");
		exit(1);
	}

}
