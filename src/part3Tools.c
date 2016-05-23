#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part3Tools.h"

extern Global_Utilities global_utilities;

/////////////////////////////////////////////EXIT HANDLER////////////////////////////////////////

void * signal_thread(void* arg){ //An extra thread that will help us terminate the program whenever we want while also freeing memory correctly!

	int sig_caught;
	int rc;

	rc = sigwait(&(global_utilities.signal_mask), &sig_caught); //Catch SIGINT or SIGTERM

	if(rc != 0)
		printf("Exit Handler: - Sigwait failed: %s\n",strerror(errno));

	switch(sig_caught){
		case SIGINT:
			printf("Exit Handler: - Caught signal SIGINT\n");
			destroySharedDataStructs();
			break;
		case SIGTERM:
			printf("Exit Handler: - Caught signal SIGTERM\n");
			destroySharedDataStructs();		
			break;
		default:
			printf("\nExit Handler: - Unexpected signal %d\n",sig_caught);
			break;
	}

	printf("\nExit Handler: Bye-bye!\n");

	if(pthread_detach(pthread_self()) != 0){
		printf("\nExit Handler: Failed to detach myself!\n");
	}

	exit(1);

}

////////////////////////////////////////////GLOBAL DATA STRUCTS FUNCTIONS//////////////////////////////////////////////////////

int initSharedDataStructs(int m,int c){ //Initialise all shared resources for threads

	setNULLDataSetStructs(&(global_utilities.allDataSetStructs)); /*Set to NULL all pointers inside the struct */

	printf("Main Thread: Importing DataSet...\n");
	if(loadDataSet("./DataSetPart3/",DATASET_DEBUG,&(global_utilities.allDataSetStructs),m,c) == EXIT_VALUE){ /* Reads everything in DataSet folder - DATASET_DEBUG flag means it will ask for permission before reading */
		printf("Main Thread: loadDataSet failed! - Exiting!\n");
		printf("Destroying DataSet resources...\n");	
		destroyDataSetStructs(&(global_utilities.allDataSetStructs)); /* Free all DataSet related resources */
															   		  //TODO: Still alot of stuff to free
		return EXIT_VALUE;
	}
	
	initSharedCondMutexes(); //Initialise condition variables and mutexes
	printf("Main Thread: Initialised Mutexes and Condition Variables...\n");

	global_utilities.worker_threads = NULL;
	global_utilities.topNForums = NULL;
	global_utilities.jobCounter=0;
	global_utilities.m = m;
	global_utilities.c = c;

	if(initJobQueue(0,-1,&(global_utilities.jobQueue)) == EXIT_VALUE){ //Initialise scheduler queue
		printf("Main Thread: Failed to Initialise Scheduler Queue!\n");
		destroySharedDataStructs();
	}
	printf("Main Thread: Initialised Scheduler Queue!\n");

	if(createThreadPool(NUM_THREADS) == EXIT_VALUE){ //Create and initialise threads for scheduler
		printf("Main Thread: createThreadPool failed! Exiting!\n");
		destroySharedDataStructs();
	}

	return 0;

}

int createThreadPool(int N){ //Create the worker threads

	int i=0;

	if(N <= 0){
		printf("createThreadPool: Given number of threads must be positive!\n");
		return EXIT_VALUE;
	}

	global_utilities.thread_pool_size = N;

	//printf("Main Thread: Now we will allocate %d pointers for worker threads!\n",(global_utilities.thread_pool_size));
	global_utilities.worker_threads = NULL;
	global_utilities.worker_threads = malloc(sizeof(pthread_t)*(global_utilities.thread_pool_size));

	if(global_utilities.worker_threads == NULL){
		printf("Main Thread: Failed to allocate memory for worker threads!\n");
		return EXIT_VALUE;
	}

	
	for(i=0; i< global_utilities.thread_pool_size; i++){
		pthread_create(&(global_utilities.worker_threads[i]), 0, Consumer, NULL); //GO MY WORKER THREADS!
	}

	global_utilities.active_workers = global_utilities.thread_pool_size;

	printf("Main Thread: %d worker Threads are now ready to work!\n",NUM_THREADS);

	return 0;
}


int initSharedCondMutexes(){ //Initialise shared mutexes and condition variables

	int i=0;

	pthread_mutex_init(&(global_utilities.queue_mtx), NULL); //Queue access Mutex
	pthread_cond_init(&(global_utilities.cond_nonempty), NULL); //Condition queue == non empty
	//pthread_cond_init(&(global_utilities.cond_nonfull), NULL); //Condition queue == non full
	pthread_mutex_init(&(global_utilities.malloc_mtx), NULL); //Malloc access Mutex
	pthread_mutex_init(&(global_utilities.free_mtx), NULL); //Free access Mutex
	pthread_mutex_init(&(global_utilities.topNForums_mtx), NULL); //topNForums access Mutex
	pthread_mutex_init(&(global_utilities.tasksLeft_mtx), NULL); //tasksLeft mutex protector
	pthread_mutex_init(&(global_utilities.active_workers_mtx), NULL); //Active Workers mutex protector
	pthread_cond_init(&(global_utilities.cond_tasksDone), NULL); //tasksDone condition variable
	pthread_cond_init(&(global_utilities.cond_workerExits), NULL); //A worker Exited condition variable

	for(i=0; i < 25; i++){ //DataSet Graphs and Lists access mutexes
		pthread_mutex_init(&(global_utilities.allDataSetStructs.dataSetMutexes[i]), NULL);
	}

	printf("Main Thread: Initialised all mutexes!\n");

	return 0;

}

int destroySharedDataStructs(){ //Attemps to free all memory allocation of the program

	int flag=0,i=0;

///////////////FREEING DATASET RESOURCES/////////////
	for(i=0; i < 25; i++){
		pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[i]));
	}
	printf("destroySharedDataStructs: Destroying all DataSet Graphs/Lists!\n");
	destroyDataSetStructs(&(global_utilities.allDataSetStructs));
	for(i=0; i < 25; i++){
		pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[i]));
	}

///////////////FREEING SCHEDULER QUEUE///////////////////
	pthread_mutex_lock(&(global_utilities.queue_mtx));
	clearJobQueue(&(global_utilities.jobQueue)); //Destroy Scheduler Queue
	pthread_mutex_unlock(&(global_utilities.queue_mtx));

//////////////FREEING THREAD RESOURCES///////////////////
	if(global_utilities.worker_threads != NULL){ //Terminate all worker threads
		printf("destroySharedDataStructs: - Waiting for all worker threads to terminate...\n");
		while(global_utilities.active_workers > 0){ //Basically waits until all workers detach themselves. A worker detaches when he realises the Main queue has
													//been destroyed. The broadcast is required in order to wake them up if they are waiting for the Main queue to fill
													//When they wake up they will see that the Main Queue does not exist and then they will detach
			pthread_cond_broadcast(&(global_utilities.cond_nonempty)); //Signal this condition variable to wake any sleeping worker thread
			pthread_cond_wait(&(global_utilities.cond_workerExits), &(global_utilities.active_workers_mtx));
		}
			
		pthread_mutex_lock(&(global_utilities.free_mtx));
		free(global_utilities.worker_threads);
		global_utilities.worker_threads = NULL;
		pthread_mutex_unlock(&(global_utilities.free_mtx));			

	}

	pthread_mutex_lock(&(global_utilities.topNForums_mtx));
	if(global_utilities.topNForums != NULL){
		printf("destroySharedDataStructs: Destroying topNForums struct...\n");
		for(i=0; i < global_utilities.N; i++){
			if(global_utilities.topNForums[i].forumMembersGraph != NULL){
				destroyGraph(global_utilities.topNForums[i].forumMembersGraph); //Free all forumMembersGraphs
			}
		}
	}
	pthread_mutex_unlock(&(global_utilities.topNForums_mtx));

	printf("destroySharedDataStructs: - Destroying condition variables...\n");
	flag=0;
	while((pthread_cond_destroy(&(global_utilities.cond_nonempty))) == EBUSY){
		if(flag == 0){
			printf("destroySharedDataStructs: cond_nonempty is locked and can't be destroyed yet!\n");
			flag = 1;
		}
		continue;
	}

	flag=0;
	while((pthread_cond_destroy(&(global_utilities.cond_tasksDone))) == EBUSY){
		if(flag == 0){
			printf("destroySharedDataStructs: cond_tasksDone is locked and can't be destroyed yet!\n");
			flag = 1;
		}
		continue;
	}

	flag=0;
	while((pthread_cond_destroy(&(global_utilities.cond_workerExits))) == EBUSY){
		if(flag == 0){
			printf("destroySharedDataStructs: cond_workerExits is locked and can't be destroyed yet!\n");
			flag = 1;
		}
		continue;
	}

	printf("destroySharedDataStructs: Destroying worker mutexes...\n");

	for(i=0; i < 25; i++){
		flag=0;
		while((pthread_mutex_destroy(&(global_utilities.allDataSetStructs.dataSetMutexes[i]))) == EBUSY) { 
			if(flag == 0){
				printf("destroySharedDataStructs: dataSetMutex: %d is locked and can't be destroyed yet!\n",i);
				flag = 1;
			} 
			continue;
		}		
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.queue_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: Scheduler Queue Mutex is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.active_workers_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: active_workers_mtx is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.topNForums_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: topNForums_mtx is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.tasksLeft_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: tasksLeft_mtx is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.malloc_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: Malloc Mutex is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	flag=0;
	while((pthread_mutex_destroy(&(global_utilities.free_mtx))) == EBUSY) { 
		if(flag == 0){
			printf("destroySharedDataStructs: Free Mutex is locked and can't be destroyed yet!\n");
			flag = 1;
		} 
		continue;
	}

	return 0;

}

////////////////////////////////////////////THREAD FUNCTIONS ///////////////////////////////////////////////////////////////////////

int insertTopNForums(int forumID, long members){ //After topNForums array lock calculate the N most populated forums

	int i=0,j=0;

	if(global_utilities.N == 1){
		if(global_utilities.topNForums[0].forumID == -1){
			global_utilities.topNForums[0].forumID = forumID;
			global_utilities.topNForums[0].number_of_members = members;
			return 0;
		}
		if(global_utilities.topNForums[0].number_of_members < members){
			global_utilities.topNForums[0].forumID = forumID;
			global_utilities.topNForums[0].number_of_members = members;

			return 0;
		}
	}
	else{
		for(i=0; i < global_utilities.N; i++){
			if(global_utilities.topNForums[i].forumID == -1){
				global_utilities.topNForums[i].forumID = forumID;
				global_utilities.topNForums[i].number_of_members = members;
				return 0;
			}
			else{
				if(global_utilities.topNForums[i].number_of_members < members){
					if(i == global_utilities.N-1){
						global_utilities.topNForums[i].forumID = forumID;
						global_utilities.topNForums[i].number_of_members = members;
					}
					else{
						for(j = global_utilities.N-2; j >= i; j--){ //Thankfully no memory is allocated for graphs here yet TODO:
							global_utilities.topNForums[j+1].forumMembersGraph = global_utilities.topNForums[j].forumMembersGraph; 
							global_utilities.topNForums[j+1].forumID = global_utilities.topNForums[j].forumID; 
							global_utilities.topNForums[j+1].number_of_members = global_utilities.topNForums[j].number_of_members;
						}
						global_utilities.topNForums[i].forumID = forumID;
						global_utilities.topNForums[i].number_of_members = members;
					}
					return 0;		
				}
			}
		}

		return 0;
	}

}

int printTopNForums(){

	int i=0;

	printf("printTopNForums:|%lu| - Printing topNForums for debugging!\n",pthread_self());

	for(i=0; i < global_utilities.N; i++){
		printf("ForumID: %d - Members: %ld\n",global_utilities.topNForums[i].forumID,global_utilities.topNForums[i].number_of_members);
	}
	printf("\n");

	return 0;

}

int calculateForumMembers(int forumID){ //Given a forumID a thread will calculate the number of members this forum has

	if(forumID < 0){
		printf("calculateForumMembers:|%lu| - Sorry I cannot work with a negative forumID...\n",pthread_self());
		return EXIT_VALUE;
	}

	long members=0;
	int errorFlag=0;

	if(global_utilities.allDataSetStructs.forumHasMemberPersonList->items <= 0){
		printf("calculateForumMembers:|%lu| - Sorry I cannot work with a negative/zero items list...\n",pthread_self());
		return EXIT_VALUE;
	}

	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));
	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST])); //This might seem uneccessary but we want to block access to all forum GNodes

	GraphNode* forumGNode = lookUpNode(forumID, global_utilities.allDataSetStructs.forumGraph, &errorFlag);

	if(errorFlag == EXIT_VALUE){
		printf("calculateForumMembers:|%lu| - Error in lookUpNode\n",pthread_self());
		return EXIT_VALUE;
	}
	if(forumGNode == NULL){
		printf("calculateForumMembers:|%lu| - Failed to locate Forum GraphNode!\n",pthread_self());
		return EXIT_VALUE;
	}

	EdgeSet* membersEdgeSet = findEdgeSet(forumGNode, "HasMember",&errorFlag);

	if(errorFlag == EXIT_VALUE){
		printf("calculateForumMembers:|%lu| - Error in findEdgeSet\n",pthread_self());
		return EXIT_VALUE;
	}
	if(membersEdgeSet == NULL){
		printf("calculateForumMembers:|%lu| - Failed to locate HasMember EdgeSet!\n",pthread_self());
		return EXIT_VALUE;
	}
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST]));

	members = membersEdgeSet->edges.items; //The number of members this forum has is already saved ;)

	//printf("calculateForumMembers:|%lu| - Calculated %ld members for forumID: %d!\n",pthread_self(),members,forumID);

	pthread_mutex_lock(&(global_utilities.topNForums_mtx));
	insertTopNForums(forumID, members); //Insert in topNForums array this pair of ForumID, Members
	pthread_mutex_unlock(&(global_utilities.topNForums_mtx));

	return 0;

}

int fillForumMembersGraph(int forumIndex){ //Given a specific index in the topNForums we will create a person Graph with forum Members that know each other

	if(forumIndex < 0){
		printf("fillForumMembersGraph:|%lu| - Sorry I cannot work with a negative forumID...\n",pthread_self());
		return EXIT_VALUE;
	}

	if(global_utilities.allDataSetStructs.forumHasMemberPersonList->items <= 0){
		printf("fillForumMembersGraph:|%lu| - Sorry I cannot work with a negative/zero items list...\n",pthread_self());
		return EXIT_VALUE;
	}

	if((global_utilities.topNForums[forumIndex].number_of_members < 0) || (global_utilities.topNForums[forumIndex].forumID < 0)){
		printf("fillForumMembersGraph:|%lu| - Sorry this forum has negative members and/or forumID...\n",pthread_self());
		return EXIT_VALUE;
	}

	if(global_utilities.topNForums[forumIndex].number_of_members == 0){
		printf("fillForumMembersGraph:|%lu| - Sorry this forum has zero members?!...\n",pthread_self());
		return EXIT_VALUE;
	}
	
	if(global_utilities.topNForums[forumIndex].forumMembersGraph != NULL){
		printf("fillForumMembersGraph:|%lu| - Sorry this forum has a non NULL Graph?!...\n",pthread_self());
		return EXIT_VALUE;
	}

	printf("fillForumMembersGraph:|%lu| - My assigned forumIndex is: %d...\n",pthread_self(),forumIndex);
	pthread_mutex_lock(&(global_utilities.malloc_mtx));
	int errorFlag = 0;
	global_utilities.topNForums[forumIndex].forumMembersGraph = createGraph(global_utilities.m, global_utilities.c,&errorFlag);
	if(errorFlag == EXIT_VALUE){
		printf("fillForumMembersGraph:|%lu| - Failed to allocate memory for forumMembersGraph!...\n",pthread_self());
		pthread_mutex_unlock(&(global_utilities.malloc_mtx));
		return EXIT_VALUE;
	}
	pthread_mutex_unlock(&(global_utilities.malloc_mtx));
	

	int forumID = global_utilities.topNForums[forumIndex].forumID;

	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));
	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST])); //This might seem uneccessary but we want to block access to all forum GNodes
	GraphNode* forumGNode = lookUpNode(forumID, global_utilities.allDataSetStructs.forumGraph, &errorFlag);

	if(errorFlag == EXIT_VALUE){
		printf("fillForumMembersGraph:|%lu| - Error in lookUpNode\n",pthread_self());
		return EXIT_VALUE;
	}
	if(forumGNode == NULL){
		printf("fillForumMembersGraph:|%lu| - Failed to locate Forum GraphNode!\n",pthread_self());
		return EXIT_VALUE;
	}

	EdgeSet* membersEdgeSet = findEdgeSet(forumGNode, "HasMember",&errorFlag);

	if(errorFlag == EXIT_VALUE){
		printf("fillForumMembersGraph:|%lu| - Error in findEdgeSet\n",pthread_self());
		return EXIT_VALUE;
	}
	if(membersEdgeSet == NULL){
		printf("fillForumMembersGraph:|%lu| - Failed to locate HasMember EdgeSet!\n",pthread_self());
		return EXIT_VALUE;
	}
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST]));

	Node* myCurrent = NULL;
	Edge* tempEdgePtr = NULL;
	GraphNode* personGNode = NULL;
	GraphNode* otherPersonGNode = NULL;
	EdgeSet* knowEdgeSet = NULL;
	Edge* edge = NULL;
	int personID = 0;
	char* type=NULL;
	int otherPersonID=0;
	myCurrent = membersEdgeSet->edges.Head;
	global_utilities.topNForums[forumIndex].num_KnowEdges = 0;
	while(myCurrent != NULL){
		tempEdgePtr = (Edge*) myCurrent->data;
		type = getStringProperty(0, tempEdgePtr->properties,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("fillForumMembersGraph:|%lu| - Failed to access Edge type!...\n",pthread_self());
		}

		personID = tempEdgePtr->endNodeID;
		pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_GRAPH])); //Lock mutex here because lookUpNode messes with Graph navigation pointers
		personGNode = lookUpNode(personID,global_utilities.allDataSetStructs.personGraph,&errorFlag); //Locate personGraphNode in personGraph and insert it into membersGraph
		pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_GRAPH]));
		if(errorFlag == EXIT_VALUE){
			printf("fillForumMembersGraph:|%lu| - lookUpNode failed!\n",pthread_self());
			return EXIT_VALUE;
		}
		if(personGNode == NULL){
			printf("fillForumMembersGraph:|%lu| - lookUpNode failed to locate personID: %d!\n",pthread_self(),personID);
			return EXIT_VALUE;
		}
		if(insertNode(personGNode, global_utilities.topNForums[forumIndex].forumMembersGraph) == EXIT_VALUE){
			printf("fillForumMembersGraph:|%lu| - insertNode failed!\n",pthread_self());
			return EXIT_VALUE;					
		}

		personGNode = NULL;

		myCurrent = myCurrent->next;
	}

	if(global_utilities.topNForums[forumIndex].forumMembersGraph->hashtable.num_records > 0){ //If person Nodes exist in this forum
		int i=0,j=0,k=0;
		Bucket* bucket=NULL;

		//printf("fillForumMembersGraph:|%lu| - Now beginning to locate number of Edges!\n",pthread_self());
		for(i=0; i < global_utilities.topNForums[forumIndex].forumMembersGraph->hashtable.size; i++){
			if(global_utilities.topNForums[forumIndex].forumMembersGraph->hashtable.hashmap[i] != NULL){
				bucket = getHeadBucket(global_utilities.topNForums[forumIndex].forumMembersGraph->hashtable.hashmap[i],&errorFlag); //Get head bucket TODO: THREAD SAFE
				if(errorFlag == EXIT_VALUE){
					printf("fillForumMembersGraph: getHeadBucket failed!\n");
					return EXIT_VALUE;
				}
				while(bucket != NULL){
					for(j=0; j < bucket->items; j++){ //For every person we will count how many other forum members he knows
						personGNode = bucket->GNArray[j];
						if(personGNode != NULL){
							//printf("fillForumMembersGraph:|%lu| - Currently examining edges of GraphNode: %d!\n",pthread_self(),personGNode->id);
							knowEdgeSet = NULL;
							edge = NULL;
							knowEdgeSet = findEdgeSet(personGNode,"knows",&errorFlag);
							if(errorFlag == EXIT_VALUE){
								printf("fillForumMembersGraph:|%lu| - findEdgeSet failed!\n",pthread_self());
								return EXIT_VALUE;
							}

							pthread_mutex_lock(&(global_utilities.malloc_mtx));
							if(personGNode->topNForumNumEdges == NULL){
								personGNode->topNForumNumEdges = malloc(sizeof(int) * (global_utilities.N));
								for(k=0; k < global_utilities.N; k++){
									personGNode->topNForumNumEdges[k] = 0; //This will keep track of how many edges this graphnode has in this the forumIndex forum
								}
							}
							pthread_mutex_unlock(&(global_utilities.malloc_mtx));

							if(knowEdgeSet != NULL){
								myCurrent = NULL;
								myCurrent = knowEdgeSet->edges.Head;
								//printf("fillForumMembersGraph:|%lu| - GraphNode: %d - Knows %d people!\n",pthread_self(),personGNode->id,knowEdgeSet->edges.items);
								while(myCurrent != NULL){
									otherPersonGNode = NULL;
									edge = NULL;
									edge = (Edge*) myCurrent->data;
									if(edge == NULL){
										printf("fillForumMembersGraph:|%lu| - edge is NULL!\n",pthread_self());
										return EXIT_VALUE;
									}
									otherPersonID = edge->endNodeID;
									//printf("fillForumMembersGraph:|%lu| - GraphNode: %d - Knows a person with ID: %d!\n",pthread_self(),personGNode->id,otherPersonID);

									otherPersonGNode = lookUpNode(otherPersonID, global_utilities.topNForums[forumIndex].forumMembersGraph, &errorFlag); //Check this person is a forum Members
									if(errorFlag == EXIT_VALUE){
										printf("fillForumMembersGraph:|%lu| - lookUpNode failed!\n",pthread_self());
										return EXIT_VALUE;									
									}
									if(otherPersonGNode != NULL){ //This other person is a member too
										global_utilities.topNForums[forumIndex].num_KnowEdges++;
										personGNode->topNForumNumEdges[forumIndex]++;
										//printf("fillForumMembersGraph:|%lu| - Person with %d is also a member!\n",pthread_self(),otherPersonID);
									}

									myCurrent = myCurrent->next;
								}
							}
						}
					}
					bucket = getNextBucket(global_utilities.topNForums[forumIndex].forumMembersGraph->hashtable.hashmap[i],bucket,&errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("fillForumMembersGraph:|%lu| - getNextBucket failed!\n",pthread_self());
						return EXIT_VALUE;						
					}
				}
			}
		}
		//printf("fillForumMembersGraph:|%lu| - Located: %ld!\n",pthread_self(),global_utilities.topNForums[forumIndex].num_KnowEdges);
	}

	return 0;
}

void* Consumer(void* argsptr){ //Worker Thread main function,attempts to take an item from the Main Queue. Blocks when Main Queue is empty.
							   //Detaches itself when Main Queue has been destroyed.
							   //TODO: EXIT CASES ARE NOT RIGHT!
	Job* tempJob=NULL;
	Job* currentJob=NULL;
	int errorFlag=0;

	printf("WORKER THREAD:|%lu| - Hello! I was just created!\n",pthread_self());

	do{
		pthread_mutex_lock(&(global_utilities.queue_mtx)); //Locking the queue mutex
		//printf("WORKER THREAD:|%lu| - Accessing the Queue...\n",pthread_self());
		while((global_utilities.jobQueue.queueList.items == 0) && (global_utilities.jobQueue.working == 1)){ //While the Scheduler Queue is existing and no items are loaded on it
			//printf("WORKER THREAD:|%lu| - Queue is empty, waiting...\n",pthread_self());
			if(global_utilities.jobQueue.working == 1)
				pthread_cond_wait(&(global_utilities.cond_nonempty),&(global_utilities.queue_mtx)); //Wait until signaled that an item has been loaded
			else break;
		}
		if(global_utilities.jobQueue.working == 0){ //Time to detach this worker thread,Main Queue is destroyed
			printf("WORKER THREAD:|%lu| - Guess I won't be needed anymore!\n",pthread_self());
			pthread_mutex_unlock(&(global_utilities.queue_mtx));
			break;
		}
		//printf("WORKER THREAD:|%lu| - Let's get to work!!!\n",pthread_self());

		///EXTRACT JOB///

		tempJob = NULL;
		currentJob = NULL;
		tempJob = frontJobQueue(&(global_utilities.jobQueue),&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("WORKER THREAD:|%lu| - frontJobQueue failed!!\n",pthread_self());
			pthread_mutex_unlock(&(global_utilities.queue_mtx));
			break;
		}

		pthread_mutex_lock(&(global_utilities.malloc_mtx)); //Allocate my temporary Job variable
		currentJob = malloc(sizeof(Job));
		pthread_mutex_unlock(&(global_utilities.malloc_mtx));

		if(currentJob == NULL){
			printf("WORKER THREAD:|%lu| - Failed to allocate memory for job!\n",pthread_self());
			pthread_mutex_unlock(&(global_utilities.queue_mtx)); //Unlocking queue mutex
			break;
		}

		currentJob->jobID = tempJob->jobID;
		currentJob->priority = tempJob->priority;
		currentJob->type = tempJob->type;
		currentJob->status = tempJob->status;
		pthread_mutex_lock(&(global_utilities.malloc_mtx));
		createList(&(currentJob->argumentList),"ArgumentList",&allocateArgumentPtr,&destroyArgumentPtr,NULL,NULL,&printArgumentPtr);
		appendLists(&(currentJob->argumentList),&(tempJob->argumentList));
		pthread_mutex_unlock(&(global_utilities.malloc_mtx));
	
		pthread_mutex_lock(&(global_utilities.free_mtx)); //Remove Job from Queue
		popJobQueue(&(global_utilities.jobQueue));
		pthread_mutex_unlock(&(global_utilities.free_mtx));
		
		pthread_mutex_unlock(&(global_utilities.queue_mtx)); //Unlock Scheduler queue mutex

		if(currentJob->type == FORUM_PREPARATION){ //Thread needs to calculate the number of members the forum has
			//printf("WORKER THREAD:|%lu| - Received task FORUM PREPARATION!\n",pthread_self());
			Argument* argumentPtr = (Argument*) currentJob->argumentList.Head->data;
			int* IDPtr = (int*) argumentPtr->value;
			if(calculateForumMembers(*IDPtr) == EXIT_VALUE){ //Calculate the forum Members of this forum
				printf("WORKER THREAD:|%lu| - FORUM PREPARATION Task failed - Exiting!\n",pthread_self());
				pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
				global_utilities.tasksLeft--;
				pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
				pthread_cond_signal(&(global_utilities.cond_tasksDone)); //Notify main thread that my job is done		
				pthread_mutex_lock(&(global_utilities.free_mtx));
				destroyJobPtr((void**) &(currentJob)); //Free temporary Job
				pthread_mutex_unlock(&(global_utilities.free_mtx));
				break;		
			}
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
			global_utilities.tasksLeft--;
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
			pthread_cond_signal(&(global_utilities.cond_tasksDone)); //Notify main thread that my job is done
			pthread_mutex_lock(&(global_utilities.free_mtx));
			destroyJobPtr((void**) &(currentJob)); //Free temporary Job
			pthread_mutex_unlock(&(global_utilities.free_mtx));
		}
		else if(currentJob->type == FORUM_FILL){ //Thread needs to fill Graph with forum members
			//printf("WORKER THREAD:|%lu| - Received task FORUM FILL!\n",pthread_self());
			Argument* argumentPtr = (Argument*) currentJob->argumentList.Head->data;
			int* forumIndexPtr = (int*) argumentPtr->value;
			if(fillForumMembersGraph(*forumIndexPtr) == EXIT_VALUE){
				printf("WORKER THREAD:|%lu| - FORUM FILL Task failed - Exiting!\n",pthread_self());
				pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
				global_utilities.tasksLeft--;
				pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
				pthread_cond_signal(&(global_utilities.cond_tasksDone)); //Notify main thread that my job is done
				pthread_mutex_lock(&(global_utilities.free_mtx));
				destroyJobPtr((void**) &(currentJob)); //Free temporary Job
				pthread_mutex_unlock(&(global_utilities.free_mtx));
				break;		
			}
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
			global_utilities.tasksLeft--;
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
			pthread_cond_signal(&(global_utilities.cond_tasksDone));
			pthread_mutex_lock(&(global_utilities.free_mtx));
			destroyJobPtr((void**) &(currentJob)); //Free temporary Job
			pthread_mutex_unlock(&(global_utilities.free_mtx));
		}
		else{
			printf("WORKER THREAD:|%lu| - Unrecognised type of task! Exiting by default!\n",pthread_self());
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
			global_utilities.tasksLeft--;
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
			pthread_cond_signal(&(global_utilities.cond_tasksDone));
			pthread_mutex_lock(&(global_utilities.free_mtx));
			destroyJobPtr((void**) &(currentJob)); //Free temporary Job
			pthread_mutex_unlock(&(global_utilities.free_mtx));
			break;
		}
	}while(1);

	printf("WORKER THREAD:|%lu| - BYE-BYE...\n",pthread_self());
	pthread_mutex_lock(&(global_utilities.active_workers_mtx));
	(global_utilities.active_workers)--;
	pthread_mutex_unlock(&(global_utilities.active_workers_mtx));
	pthread_cond_signal(&(global_utilities.cond_workerExits));
	pthread_detach(pthread_self());
}

///////////////////////////////////////MAIN THREAD PRODUCER FUNCTIONS//////////////////////////////////////////////////////

Job* createForumPreparationJob(GraphNode* forumGNode,int* errorFlag){ //Creates Scheduler Task that tells worker thread to count forum members

	Job* currentJob=NULL;
	currentJob=malloc(sizeof(Job)); //Allocate a Job!
	if(currentJob == NULL){
		printf("createForumPreparationJob: Could not allocate memory for currentJob!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}
	currentJob->jobID=global_utilities.jobCounter;
	(global_utilities.jobCounter)++;

	currentJob->type=FORUM_PREPARATION; //This flag is important in order for the worker threads to know what operation to do
	currentJob->status=BLOCKED;
	currentJob->priority=0;
			
	Argument* argumentPtr=NULL;
	argumentPtr=malloc(sizeof(Argument)); //Allocate memory for argument
	if(argumentPtr == NULL){
		printf("createForumPreparationJob: Could not allocate enough memory!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}
	argumentPtr->type=INT;
	argumentPtr->value = malloc(sizeof(int));
	if(argumentPtr->value == NULL){
		printf("createForumPreparationJob: Could not allocate enough memory!\n");
		*errorFlag = EXIT_VALUE;
		destroyArgumentPtr((void**) (&argumentPtr));
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}
	int* IDptr = (int*) argumentPtr->value;
	*IDptr = forumGNode->id; //Get forum ID

	createList(&(currentJob->argumentList),"argumentList",&allocateArgumentPtr,&destroyArgumentPtr,NULL,NULL,&printArgumentPtr);
	if(addLastList(&(currentJob->argumentList),argumentPtr) == EXIT_VALUE){ //Add argument to argument List of this job
		printf("createForumPreparationJob: addLastList failed!\n");
		*errorFlag = EXIT_VALUE;
		destroyArgumentPtr((void**) (&argumentPtr));
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}

	//printf("createForumPreparationJob: Created Job with JobID: %ld - Type: FORUM_PREPARATION - forumID: %d\n",currentJob->jobID,*IDptr);
	destroyArgumentPtr((void**) (&argumentPtr));

	return currentJob;

}

Job* createForumFillJob(int forumIndex,int* errorFlag){ //Create Scheduler Task that tells worker thread to fill forum with members

	Job* currentJob=NULL;
	currentJob=malloc(sizeof(Job)); //Allocate a Job!
	if(currentJob == NULL){
		printf("createForumFillJob: Could not allocate memory for currentJob!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}
	currentJob->jobID=global_utilities.jobCounter;
	(global_utilities.jobCounter)++;

	currentJob->type=FORUM_FILL; //This flag is important in order for the worker threads to know what operation to do
	currentJob->status=BLOCKED;
	currentJob->priority=0;
			
	Argument* argumentPtr=NULL;
	argumentPtr=malloc(sizeof(Argument)); //Allocate memory for argument
	if(argumentPtr == NULL){
		printf("createForumFillJob: Could not allocate enough memory!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		return ((Job*) NULL);
	}
	argumentPtr->type=INT;
	argumentPtr->value = malloc(sizeof(int));
	if(argumentPtr->value == NULL){
		printf("createForumPreparationJob: Could not allocate enough memory!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		destroyArgumentPtr((void**) (&argumentPtr));
		return ((Job*) NULL);
	}
	int* IDptr = (int*) argumentPtr->value;
	*IDptr = forumIndex; //Get forum ID

	createList(&(currentJob->argumentList),"argumentList",&allocateArgumentPtr,&destroyArgumentPtr,NULL,NULL,&printArgumentPtr);
	if(addLastList(&(currentJob->argumentList),argumentPtr) == EXIT_VALUE){ //Add argument to argument List of this job
		printf("createForumPreparationJob: addLastList failed!\n");
		*errorFlag = EXIT_VALUE;
		destroyJobPtr((void**) (&currentJob));
		destroyArgumentPtr((void**) (&argumentPtr));
		return ((Job*) NULL);
	}

	//printf("createForumPreparationJob: Created Job with JobID: %ld - Type: FORUM_FILL - forumID: %d\n",currentJob->jobID,*IDptr);
	destroyArgumentPtr((void**) (&argumentPtr));

	return currentJob;

}

int addNewJobInQueue(Job* job){ //Trasferring an item from the Temp Queue of its producer to the Main Queue of the program

	//printf("Main Thread:|%lu| - Adding Job to Queue...\n",pthread_self());
	pthread_mutex_lock(&(global_utilities.queue_mtx)); //Access the Main Queue

	pthread_mutex_lock(&(global_utilities.malloc_mtx));
	addJobQueue(&(global_utilities.jobQueue),job); //Place Job In scheduler Queue
	pthread_mutex_unlock(&(global_utilities.malloc_mtx));

	//printf("Main Thread:|%lu| - Added Job to Queue!\n",pthread_self());

	//printf("Main Thread:|%lu| - The Main Queue currently contains:|%d| items\n",pthread_self(),global_utilities.jobQueue.queueList.items);
	pthread_mutex_unlock(&(global_utilities.queue_mtx));

	//printf("Main Thread:|%lu| - Signaling worker threads to work!\n",pthread_self());
	pthread_cond_broadcast(&(global_utilities.cond_nonempty)); //Wake up worker threads

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int findTopNforums(int N){

	int answer=0;
	int i=0;
////////////////////////////MAKING SURE WE HAVE ALL REQUIRED DATA///////////////////////////
	//Extracting required data from AllDataStructs
	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));
	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST]));
	Graph* forumGraph = global_utilities.allDataSetStructs.forumGraph;
	if(forumGraph->hashtable.num_records == 0){
		printf("findTopNforums: Your forumGraph seems empty...Would you like to fill from dataset? Yes:1|No:0\n");
		answer=0;
		do{
			scanf("%d",&answer);
		}while((answer != 0) && (answer != 1));

		if(answer == 0){
			printf("findTopNforums: We can't work with an empty forumGraph! Exiting!\n");
			return EXIT_VALUE;
		}
		else{
			forumReader("./DataSetPart3/forum.csv",global_utilities.allDataSetStructs.forumList, global_utilities.allDataSetStructs.forumGraph, NULL);
			Graph* forumGraph = global_utilities.allDataSetStructs.forumGraph;
		}
	}
	List* forumList = global_utilities.allDataSetStructs.forumList;
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_LIST]));
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_GRAPH]));

	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_GRAPH]));
	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_LIST]));
	Graph* personGraph = global_utilities.allDataSetStructs.personGraph;
	if(personGraph->hashtable.num_records == 0){
		printf("findTopNforums: Your personGraph seems empty...Would you like to fill from dataset? Yes:1|No:0\n");
		answer=0;
		do{
			scanf("%d",&answer);
		}while((answer != 0) && (answer != 1));

		if(answer == 0){
			printf("findTopNforums: We can't work with an empty personGraph! Exiting!\n");
			return EXIT_VALUE;
		}
		else{
			personReader("./DataSetPart3/person.csv",global_utilities.allDataSetStructs.personList, global_utilities.allDataSetStructs.personGraph, NULL);
			Graph* personGraph = global_utilities.allDataSetStructs.personGraph;
		}
	}
	List* personList = global_utilities.allDataSetStructs.personList;
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_LIST]));
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_GRAPH]));

	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_HASMEMBER_LIST]));
	List* forumHasMemberPersonList = global_utilities.allDataSetStructs.forumHasMemberPersonList;
	if(forumHasMemberPersonList->items == 0){
		printf("findTopNforums: Your forumHasMemberPersonList seems empty...Would you like to fill from dataset? Yes:1|No:0\n");
		answer=0;
		do{
			scanf("%d",&answer);
		}while((answer != 0) && (answer != 1));

		if(answer == 0){
			printf("findTopNforums: We can't work with an empty forumHasMemberPersonList! Exiting!\n");
			return EXIT_VALUE;
		}
		else{
			forumHasMemberPersonReader("./DataSetPart3/forum_hasMember_person.csv",global_utilities.allDataSetStructs.forumHasMemberPersonList, global_utilities.allDataSetStructs.forumGraph, global_utilities.allDataSetStructs.personGraph);
			List* forumHasMemberPersonList = global_utilities.allDataSetStructs.forumHasMemberPersonList;
		}
	}
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[FORUM_HASMEMBER_LIST]));

	pthread_mutex_lock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_KNOWS_PERSON_LIST]));
	List* personKnowsPersonList = global_utilities.allDataSetStructs.personKnowsPersonList;
	if(personKnowsPersonList->items == 0){
		printf("findTopNforums: Your personKnowsPersonList seems empty...Would you like to fill from dataset? Yes:1|No:0\n");
		answer=0;
		do{
			scanf("%d",&answer);
		}while((answer != 0) && (answer != 1));

		if(answer == 0){
			printf("findTopNforums: We can't work with an empty personKnowsPersonList! Exiting!\n");
			return EXIT_VALUE;
		}
		else{
			personKnowsPersonReader("./DataSetPart3/person_knows_person.csv",global_utilities.allDataSetStructs.personKnowsPersonList, global_utilities.allDataSetStructs.personGraph, NULL);
			List* personKnowsPersonList = global_utilities.allDataSetStructs.personKnowsPersonList;
		}
	}
	pthread_mutex_unlock(&(global_utilities.allDataSetStructs.dataSetMutexes[PERSON_KNOWS_PERSON_LIST]));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(N > forumList->items){
		printf("findTopNforums: Can't find the top N forums with less than N forums available from DataSet!\n");
		return EXIT_VALUE;
	}

	global_utilities.N = N;

	if(forumList->items > 0){
		if(global_utilities.topNForums != NULL){
			printf("findTopNforums: topNForums pointer is not NULL! Attempt to free memory first? Yes:1|No:0\n");
			answer=0;
			do{
				scanf("%d",&answer);
			}while((answer != 0) && (answer != 1));
			
			if(answer == 1){
				pthread_mutex_lock(&(global_utilities.topNForums_mtx));
				pthread_mutex_lock(&(global_utilities.free_mtx));
				for(i=0; i < forumList->items; i++){
					destroyGraph(global_utilities.topNForums[i].forumMembersGraph);	
				}
				free(global_utilities.topNForums);
				pthread_mutex_unlock(&(global_utilities.free_mtx));
				pthread_mutex_unlock(&(global_utilities.topNForums_mtx));
			}
			else{
				printf("findTopNforums: WARNING! You are not freeing a big amount of memory! Exiting by default!\n");
				return EXIT_VALUE;
			}
		}
		pthread_mutex_lock(&(global_utilities.topNForums_mtx));
		pthread_mutex_lock(&(global_utilities.malloc_mtx));
		global_utilities.topNForums = malloc(sizeof(Forum)*N);
		if(global_utilities.topNForums == NULL){
			printf("findTopNforums: Failed to allocate memory for topNForums!\n");
			return EXIT_VALUE;
		}
		for(i=0; i < N; i++){
			global_utilities.topNForums[i].forumMembersGraph = NULL;
			global_utilities.topNForums[i].forumID = -1;
			global_utilities.topNForums[i].number_of_members = -1;
			global_utilities.topNForums[i].num_KnowEdges = 0;
		}
		pthread_mutex_unlock(&(global_utilities.topNForums_mtx));
		pthread_mutex_unlock(&(global_utilities.malloc_mtx));

		int* IDptr=NULL;
		GraphNode* currentGNode=NULL;

		int errorFlag = 0;
		Job* currentJob=NULL;

		if(forumList->items > 0){

			printf("findTopNforums: Beginning Forum Preparation!\n");
			///////ADD TASKS TO COUNT THE MEMBERS OF EACH OF FORUM////
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx)); //Set number of tasks to be done
			global_utilities.tasksLeft = forumList->items; 
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));

			Node* myCurrent = NULL;
			myCurrent = forumList->Head;
			for(i=0; i < forumList->items; i++){ //We will create a Job for every forum
				currentGNode = (GraphNode*) myCurrent->data;
				currentJob = createForumPreparationJob(currentGNode,&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("findTopNforums: createForumPreparationJob failed! Exiting!\n");
					return EXIT_VALUE;
				}
				addNewJobInQueue(currentJob); //Add Job to Scheduler Queue
				destroyJobPtr((void**) &currentJob); //Free my copy of Job, the queue has its own
				myCurrent = myCurrent->next;
			}

			//WAIT TILL TASKS ARE ONE//
			printf("findTopNforums: Waiting for ForumPreparation to finish...\n");
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx)); //Locking the queue mutex
			while((global_utilities.tasksLeft > 0)){ //While the Scheduler Queue is existing and no items are loaded on it
				pthread_cond_wait(&(global_utilities.cond_tasksDone),&(global_utilities.tasksLeft_mtx)); //Wait until signaled that a job has been done
			}
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
			
			//CHECK RESULTS//
			pthread_mutex_lock(&(global_utilities.queue_mtx));
			if(global_utilities.jobQueue.queueList.items > 0){
				printf("findtopNForums: Something has gone wrong! TasksLeft flag is 0 but tasks still exist in the queue!\n");
				pthread_mutex_unlock(&(global_utilities.queue_mtx));
				return EXIT_VALUE;
			}
			pthread_mutex_unlock(&(global_utilities.queue_mtx));

			printf("findtopNforums: We have found the topN forums! Now we need to form Graphs for each one of them!\n");

			printf("findtopNforums: Printing now details about what we found...\n");
			int topForumsLocated=0;
			int forumID = -1;
			long members = -1;
			for(i=0; i < global_utilities.N; i++){
				forumID = global_utilities.topNForums[i].forumID;
				members = global_utilities.topNForums[i].number_of_members;
				if(forumID != -1){
					topForumsLocated++; //TODO: WARNING DON'T COMMENT THIS
					printf("I: %d - ForumID: %d - Members: %ld\n",i,forumID,members);
				}
				else{
					printf("I: %d - ForumID: %d - Members: %ld\n",i,forumID,members);
				}
			}
			
			if(topForumsLocated == 0){
				printf("findtopNforums: Something went wrong with topNForums calculation...\n");
				return EXIT_VALUE;
			}

			printf("findTopNforums: Continue to forumFill? Yes:1|No:0\n");
			answer=0;
			do{
				scanf("%d",&answer);
			}while((answer != 0) && (answer != 1));

			if(answer == 0){
				return 0;
			}

			///////ADD TASKS TO FILL THE TOP-N FORUMS WITH MEMBERS////
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx));
			global_utilities.tasksLeft = topForumsLocated;
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));

			printf("findTopNforums: Beginning ForumFill!\n");
			for(i=0; i < topForumsLocated; i++){ //We will fill each of the topNForums
				forumID = global_utilities.topNForums[i].forumID;
				if(forumID == -1){
					printf("findTopNforums: Negative forumID! Exiting!\n"); //We shouldn't bump into a negative forumID even if one exists in the array since we have topForumsLocated
					return EXIT_VALUE;				
				}
				currentJob = createForumFillJob(i,&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("findTopNforums: createForumFillJob failed! Exiting!\n");
					return EXIT_VALUE;
				}
				addNewJobInQueue(currentJob); //Add Job to Scheduler Queue
				destroyJobPtr((void**) &currentJob); //Free my copy of Job, the queue has its own
			}

			//WAIT TILL TASKS ARE DONE//
			printf("findTopNforums: Waiting for ForumFill to finish...\n");
			pthread_mutex_lock(&(global_utilities.tasksLeft_mtx)); //Locking the queue mutex
			while((global_utilities.tasksLeft > 0)){ //While the Scheduler Queue is existing and no items are loaded on it
				pthread_cond_wait(&(global_utilities.cond_tasksDone),&(global_utilities.tasksLeft_mtx)); //Wait until signaled that an item has been loaded
			}
			pthread_mutex_unlock(&(global_utilities.tasksLeft_mtx));
			
			//CHECK RESULTS//
			pthread_mutex_lock(&(global_utilities.queue_mtx));
			if(global_utilities.jobQueue.queueList.items > 0){
				printf("findtopNForums: Something has gone wrong! TasksLeft flag is 0 but tasks still exist in the queue!\n");
				pthread_mutex_unlock(&(global_utilities.queue_mtx));
				return EXIT_VALUE;
			}
			pthread_mutex_unlock(&(global_utilities.queue_mtx));

			printf("findtopNforums: We have filled successfully the topN forums!\n");
			do{
				printf("findtopNforums: Type a number 0 to %d to print the desired forum OR type -1 to EXIT!\n",global_utilities.N);
				scanf("%d",&i);
				if(i == -1){
					break;
				}
				if((i < 0) || (i >= global_utilities.N)){
					printf("findtopNforums: Sorry invalid index!\n");
					continue;				
				}
				if(global_utilities.topNForums[i].forumID == -1){
					printf("findtopNforums: Sorry this forum is empty!\n");
					continue;
				}
				printf("findtopNforums: Printing Forum: %d\n",i);
				printHashTable(&(global_utilities.topNForums[i].forumMembersGraph->hashtable));
				printf("\n");
				printf("Forum has %ld edges!\n\n",global_utilities.topNForums[i].num_KnowEdges);
			} while(1);
		}
		else{
			printf("findTopNForums: No forums available!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("findTopNforums: Can't find the top N forums without any forums in forumList!\n");
		return EXIT_VALUE;
	}
	
	return 0;

}

int allPathsInGraph(List* listAllPaths,Graph* g,int* errorFlag){

	int i=0,ID1,ID2,num_buckets1=0,num_buckets2=0,num_items1=0,num_items2=0,num_paths=0,num_inc_paths=0,success,number_of_paths,total_distance,j,k,t,s;
	double betweenessTotal=0.0;
	Bucket* bucket1 = (Bucket*) NULL,* bucket2 = (Bucket*) NULL;
	List* pathSet = (List*) NULL;
	List* tempPath = (List*) NULL;
	Node* myCurrent = (Node*) NULL;
	List* myPath = (List*) NULL;

	if(g == NULL){
		printf("allPathsInGraph: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("allPathsInGraph: HashMap in Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(listAllPaths == NULL){
		printf("allPathsInGraph: listAllPaths is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	for(i=0; i < g->hashtable.size; i++){
		if(g->hashtable.hashmap[i] != NULL){ //For every BucketChain
			bucket1 = getHeadBucket(g->hashtable.hashmap[i], errorFlag); //Get head bucket
			if(*errorFlag == EXIT_VALUE){
				printf("allPathsInGraph: getHeadBucket failed!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
			while(bucket1 != NULL){ //For every Bucket in bucketchain
				for(j=0; j < bucket1->items; j++){ //For every item in bucket1
					if(bucket1->GNArray[j] != NULL){
						for(k = 0; k < g->hashtable.size; k++){	//Second Graph loop begins
							if(g->hashtable.hashmap[k] != NULL){
								bucket2 = getHeadBucket(g->hashtable.hashmap[k], errorFlag);
								if(*errorFlag == EXIT_VALUE){
									printf("allPathsInGraph: getHeadBucket failed!\n");
									*errorFlag = EXIT_VALUE;
									return EXIT_VALUE;
								}
								while(bucket2 != NULL){
									for(t=0; t < bucket2->items; t++){
										if(bucket2->GNArray[t] != NULL){	
											if(bucket1->GNArray[j]->id < bucket2->GNArray[t]->id){ // s < t

												pathSet = (List*) NULL;
												success = 0;
												total_distance = 0;
												number_of_paths = 0;

												pathSet = (Double_BFS_Graph(g,bucket1->GNArray[j]->id,bucket2->GNArray[t]->id,&success,&total_distance,&number_of_paths,PATH_CONSTRUCTION,errorFlag)); //All shortest different paths between s and t

												if(*errorFlag == EXIT_VALUE){
													printf("allPathsInGraph: DOUBLE_BFS Failed!\n");
													*errorFlag = EXIT_VALUE;
													return EXIT_VALUE;
												}

												if(pathSet != NULL){
													if(success == 1){
														if(pathSet->items > 0)
															addLastList(listAllPaths, pathSet);
													}
													//destroyList(pathSet);
													//free(pathSet);
												}

											
											}
										}
									}

									bucket2 = getNextBucket(g->hashtable.hashmap[k],bucket2,errorFlag);

									if(*errorFlag == EXIT_VALUE){
										printf("allPathsInGraph: getNextBucket Failed!\n");
										*errorFlag = EXIT_VALUE;
										return EXIT_VALUE;
									}

								}

							}

						}
					}

				}

				bucket1 = getNextBucket(g->hashtable.hashmap[i],bucket1,errorFlag);

				if(*errorFlag == EXIT_VALUE){
					printf("allPathsInGraph: getNextBucket Failed!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}

			}

		}

	}

	//return (betweenessTotal / ( (g->hashtable.num_records-2) * (g->hashtable.num_records-1) / 2)); (normalised)
	return betweenessTotal;

}

double edgeBetweennessCentrality(Edge* e, Graph* g,List* listAllPaths,int* errorFlag){ //TODO: SPEED UP WITH THREADS

	int i=0,ID1,ID2,num_buckets1=0,num_buckets2=0,num_items1=0,num_items2=0,num_paths=0,num_inc_paths=0,success,number_of_paths,total_distance,j,k,t,s;
	double betweenessTotal=0.0;
	Bucket* bucket1 = (Bucket*) NULL,* bucket2 = (Bucket*) NULL;
	List* pathSet = (List*) NULL;

	/*if(g == NULL){
		printf("edgeBetweennessCentrality: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("edgeBetweennessCentrality: HashMap in Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(e == NULL){
		printf("edgeBetweennessCentrality: Edge is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;		
	}*/

	int startNodeID = e->startNodeID;
	int endNodeID = e->endNodeID;

	/*if(lookUpNode(startNodeID,g,errorFlag) == NULL){
		printf("edgeBetweennessCentrality: StartNodeID GraphNode does not exist in Graph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(*errorFlag == EXIT_VALUE){
		printf("edgeBetweennessCentrality: Error at lookUpNode startNodeID!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(lookUpNode(endNodeID,g,errorFlag) == NULL){
		printf("edgeBetweennessCentrality: EndNodeID GraphNode does not exist in Graph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(*errorFlag == EXIT_VALUE){
		printf("edgeBetweennessCentrality: Error at lookUpNode endNodeID!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}*/

	Node* myCurrent = (Node*) NULL;
	Node* myCurrent2 = (Node*) NULL;
	List* myPath = (List*) NULL;
	List* myPathSet = (List*) NULL;

	myCurrent = listAllPaths->Head;
	while(myCurrent != NULL){
		myPathSet = (List*) myCurrent->data;
		if(myPathSet != NULL){
			num_inc_paths = 0;
			num_paths = 0;
			myCurrent2 = myPathSet->Head;
			while(myCurrent2 != NULL){
				myPath = (List*) myCurrent2->data;
				if(myPath != NULL){
					if(edgeIsInPath(startNodeID,endNodeID,*myPath) == 1){
						num_inc_paths++;
					}
					num_paths++;
				}
				myCurrent2 = myCurrent2->next;
			}
			//printf("edgeBetweennessCentrality: Number of Paths: %d - Number of Paths this Edge is In: %d\n",num_paths,num_inc_paths);
			if(num_inc_paths > 0)
				betweenessTotal = betweenessTotal + (double) (num_inc_paths / num_paths);
		}
		myCurrent = myCurrent->next;
	}

	//return (betweenessTotal / ( (g->hashtable.num_records-2) * (g->hashtable.num_records-1) / 2)); (normalised)
	e->betweenessCentrality = betweenessTotal;

	return betweenessTotal;
}

int allEdgesBetweenessCentrality(Graph* g,Edge** maxEdgePtr,double* maxBetweenessValuePtr){ //TODO: USE BANNED EDGE LISTS

	int i=0,j=0,errorFlag=0,startNodeID,endNodeID;
	Bucket* bucket=NULL;
	GraphNode* GNode=NULL;
	EdgeSet* knowsEdgeSet=NULL;
	Edge* edge=NULL;
	Node* myCurrent=NULL;
	double edgeBetweenessValue=0.0;
	double maxBetweenessValue=-1.0;
	Edge* maxBetweenessEdge=NULL;

	if(g == NULL){
		printf("allEdgesBetweenessCentrality: Error Graph is NULL!\n");
		return EXIT_VALUE;
	}

	if(g->hashtable.num_records == 0){
		printf("allEdgesBetweenessCentrality: Error HashTable has no records!\n");
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("allEdgesBetweenessCentrality: Error HashMap is NULL!\n");
		return EXIT_VALUE;
	}

	if(maxEdgePtr == NULL){
		printf("allEdgesBetweenessCentrality: Error maxEdgePtr is NULL!\n");
		return EXIT_VALUE;
	}

	if(maxBetweenessValuePtr == NULL){
		printf("allEdgesBetweenessCentrality: Error maxBetweenessValuePtr is NULL!\n");
		return EXIT_VALUE;
	}

	int bannedFlag=0;

	printf("allEdgesBetweenessCentrality: First we need to find all paths in graph!\n");

	List* listAllPaths = (List*) NULL;

	listAllPaths = malloc(sizeof(List));
	if(listAllPaths == NULL){
		printf("allEdgesBetweenessCentrality: failed to allocate memory for listAllPaths...\n");
		return EXIT_VALUE;	
	}
	createList(listAllPaths,"ListofLists",&allocateListPtr,&destroyListPtr,NULL,NULL,&printListPtr);

	allPathsInGraph(listAllPaths,g,&errorFlag);
	if(errorFlag == EXIT_VALUE){
		printf("allEdgesBetweenessCentrality: allPathsInGraph failed!\n");
		destroyList(listAllPaths);
		free(listAllPaths);
		return EXIT_VALUE;	
	}

	printf("allEdgesBetweenessCentrality: Alright! Now let's check betweenessCentrality for every edge..\n");

	for(i=0; i < g->hashtable.size; i++){
		if(g->hashtable.hashmap[i] != NULL){ //For every BucketChain
			bucket = getHeadBucket(g->hashtable.hashmap[i],&errorFlag); //Get head bucket TODO: THREAD SAFE
			if(errorFlag == EXIT_VALUE){
				printf("allEdgesBetweenessCentrality: getHeadBucket failed!\n");
				destroyList(listAllPaths);
				free(listAllPaths);
				return EXIT_VALUE;
			}
			while(bucket != NULL){ //For every Bucket in bucketchain
				for(j=0; j < bucket->items; j++){
					GNode = bucket->GNArray[j];
					if(GNode != NULL){
						knowsEdgeSet = findEdgeSet(GNode, "knows" ,&errorFlag); //Get all knows edges this node has
						if(errorFlag == EXIT_VALUE){
							printf("allEdgesBetweenessCentrality: getHeadBucket failed!\n");
							destroyList(listAllPaths);
							free(listAllPaths);
							return EXIT_VALUE;
						}
						if(knowsEdgeSet != NULL){
							myCurrent = NULL;
							myCurrent = knowsEdgeSet->edges.Head;
							while(myCurrent != NULL){
								edge = (Edge*) myCurrent->data;
								if(edge != NULL){
									startNodeID = edge->startNodeID;
									endNodeID = edge->endNodeID;
									if(startNodeID != GNode->id){
										printf("allEdgesBetweenessCentrality: StartNodeID different than ID of GraphNode!\n");
										destroyList(listAllPaths);
										free(listAllPaths);
										return EXIT_VALUE;
									}
									if(lookUpNode(endNodeID,g,&errorFlag) != NULL){ //NULL is not bad it might mean that this person knows someone from another graph!
										//If we are in here this edge exists in this graph
										bannedFlag = isBannedEdge(g,edge);
										if(bannedFlag == EXIT_VALUE){
											printf("allEdgesBetweenessCentrality: isBannedEdge failed!\n");
											destroyList(listAllPaths);
											free(listAllPaths);
											return EXIT_VALUE;
										}
										if(bannedFlag == 0){
											edgeBetweenessValue = edgeBetweennessCentrality(edge,g,listAllPaths,&errorFlag);
											edge->betweenessCentrality = edgeBetweenessValue;
											if(errorFlag == EXIT_VALUE){
												printf("allEdgesBetweenessCentrality: edgeBetweennessCentrality failed!\n");
												destroyList(listAllPaths);
												free(listAllPaths);
												return EXIT_VALUE;
											}

											if(edgeBetweenessValue > maxBetweenessValue){
												maxBetweenessEdge = edge;
											}
										}
									}

									if(errorFlag == EXIT_VALUE){ //This has to be checked though
										printf("allEdgesBetweenessCentrality: lookUpNode failed!\n");
										destroyList(listAllPaths);
										free(listAllPaths);
										return EXIT_VALUE;
									}
								}
								else{
									printf("allEdgesBetweenessCentrality: Error Edge is NULL!\n");
									destroyList(listAllPaths);
									free(listAllPaths);
									return EXIT_VALUE;
								}
								myCurrent = myCurrent->next;
							}
						}
					}
				}
				bucket = getNextBucket(g->hashtable.hashmap[i],bucket,&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("allEdgesBetweenessCentrality: getNextBucket failed!\n");
					destroyList(listAllPaths);
					free(listAllPaths);
					return EXIT_VALUE;
				}
			}
		}
	}

	*maxEdgePtr = maxBetweenessEdge;
	*maxBetweenessValuePtr = maxBetweenessValue;
	destroyList(listAllPaths);
	free(listAllPaths);

	return 0;

}

int countSameGraphKnowEdges(Graph* forumMembersGraph,int forumIndex,GraphNode* gnode,int* errorFlag){

	Node* myCurrent = NULL;
	Edge* currentEdge = NULL;
	int startNodeID = 0;
	int endNodeID = 0;

	int number_of_edges = 0;
	int located_edges = 0;
	GraphNode* otherPerson=NULL;

	number_of_edges = gnode->topNForumNumEdges[forumIndex];

	/*EdgeSet* knowEdgeSet = findEdgeSet(gnode,"knows",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("countSameGraphKnowEdges: findEdgeSet failed!\n");
		return EXIT_VALUE;
	}
	
	Node* myCurrent = NULL;

	if(knowEdgeSet == NULL){
		return 0;
	}

	Edge* currentEdge = NULL;
	int startNodeID = 0;
	int endNodeID = 0;

	int number_of_edges = 0;
	int located_edges = 0;
	GraphNode* otherPerson=NULL;

	myCurrent = knowEdgeSet->edges.Head;
	while(myCurrent != NULL){
		currentEdge = (Edge*) myCurrent->data;
		if(currentEdge == NULL){
			printf("countSameGraphKnowEdges: Edge is NULL!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		endNodeID = currentEdge->endNodeID;
		otherPerson = lookUpNode(endNodeID,forumMembersGraph,errorFlag);
		if(*errorFlag == EXIT_VALUE){
			printf("countSameGraphKnowEdges: lookUpNode failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		if(otherPerson != NULL){
			number_of_edges++;
			located_edges = 1;
		}
		myCurrent = myCurrent->next;
	}*/

	if(number_of_edges > 0){
		if(forumMembersGraph->bannedEdgeSetList.items > 0){
			EdgeSet* knowEdgeSet = findBanEdgeSet(forumMembersGraph,"knows",errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("countSameGraphKnowEdges: findBanEdgeSet failed!\n");
				return EXIT_VALUE;
			}
			if(knowEdgeSet != NULL){
				myCurrent = knowEdgeSet->edges.Head;
				while(myCurrent != NULL){
					currentEdge = (Edge*) myCurrent->data;
					if(currentEdge == NULL){
						printf("countSameGraphKnowEdges: Edge is NULL!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					}
					startNodeID = currentEdge->endNodeID;
					if(startNodeID == gnode->id){ //This is a banned edge don't count it
						number_of_edges--;
					}
					myCurrent = myCurrent->next;
				}
			}			
		}
		return number_of_edges;
	}

	return 0;
}

int checkNodesAreNeighbours(Graph* g,GraphNode* gnode1,GraphNode* gnode2,int* errorFlag){

	if((g == NULL) || (gnode1 == NULL) || (gnode2 == NULL)){
		printf("checkNodesAreNeighbours: NULL Pointers!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	GraphNode* resultGNode=NULL;

	resultGNode = lookUpNode(gnode1->id,g,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("checkNodesAreNeighbours: lookUpNodefailed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(resultGNode == NULL) return 0;

	resultGNode = lookUpNode(gnode2->id,g,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("checkNodesAreNeighbours: lookUpNodefailed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(resultGNode == NULL) return 0;

	EdgeSet* knowEdgeSet = findEdgeSet(gnode1,"knows",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("checkNodesAreNeighbours: findEdgeSet failed!\n");
		return EXIT_VALUE;
	}
	
	Node* myCurrent = NULL;

	if(knowEdgeSet == NULL){
		return 0;
	}

	Edge* currentEdge = NULL;

	int locatedNeighbour=0;

	int bannedFlag=0;

	myCurrent = knowEdgeSet->edges.Head;
	while(myCurrent != NULL){
		currentEdge = (Edge*) myCurrent->data;
		if(currentEdge == NULL){
			printf("checkNodesAreNeighbours: Edge is NULL!\n");
			return EXIT_VALUE;
		}
		if(currentEdge->endNodeID == gnode2->id){
			if((bannedFlag = isBannedEdge(g,currentEdge)) == EXIT_VALUE){
				printf("checkNodesAreNeighbours: isBannedEdge failed!\n");
				return EXIT_VALUE;
			}
			if(bannedFlag == 0){
				locatedNeighbour++;
				break;
			}
		}
		myCurrent = myCurrent->next;
	}

	if(locatedNeighbour == 0){ //GNode 1 does not have an edge that leads to gnode 2
		return 0;
	}

	knowEdgeSet = findEdgeSet(gnode2,"knows",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("checkNodesAreNeighbours: findEdgeSet failed!\n");
		return EXIT_VALUE;
	}
	

	if(knowEdgeSet == NULL){
		return 0;
	}

	bannedFlag=0;

	myCurrent = knowEdgeSet->edges.Head;
	while(myCurrent != NULL){
		currentEdge = (Edge*) myCurrent->data;
		if(currentEdge == NULL){
			printf("checkNodesAreNeighbours: Edge is NULL!\n");
			return EXIT_VALUE;
		}
		if(currentEdge->endNodeID == gnode1->id){
			if((bannedFlag = isBannedEdge(g,currentEdge)) == EXIT_VALUE){
				printf("checkNodesAreNeighbours: isBannedEdge failed!\n");
				return EXIT_VALUE;
			}
			if(bannedFlag == 0){
				locatedNeighbour++;
				break;
			}
		}
		myCurrent = myCurrent->next;
	}

	if(locatedNeighbour == 1){ //GNode 2 does not have an edge that leads to gnode 1
		return 0;
	}
	else{
		return 1; //Both Nodes know each other as neighbours!
	}

	return 0;

}

int check_same_community(Graph* g,GraphNode* gnode1,GraphNode* gnode2,int* errorFlag){ //Check if two graphnodes exist in the same community

	if((g == NULL) || (gnode1 == NULL) || (gnode2 == NULL)){
		printf("check_same_community: NULL Pointers!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	GraphNode* resultGNode=NULL;

	resultGNode = lookUpNode(gnode1->id,g,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("check_same_community: lookUpNodefailed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(resultGNode == NULL) return 0;

	resultGNode = lookUpNode(gnode2->id,g,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("check_same_community: lookUpNodefailed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(resultGNode == NULL) return 0;

	if(*errorFlag == EXIT_VALUE){
		printf("check_same_community: DOUBLE_BFS Failed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(gnode1->id == gnode2->id) return 1;

	if(reachNode1(gnode1->id,gnode2->id,g,errorFlag) == INFINITY_REACH_NODE){ //Double BFS will show use if a unidirected path exists
		return 0;
	}

	if(*errorFlag == EXIT_VALUE){
		printf("check_same_community: DOUBLE_BFS Failed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	return 1; //Path exists, they are in the same community

}

double calculateModularity(Forum forum,int forumIndex,int* errorFlag){

	int bannedEdges = 0;
	EdgeSet* knowEdgeSet= (EdgeSet*) NULL;
	int m=0;
	int A=0;
	int i=0,ID1,ID2,num_buckets1=0,num_buckets2=0,num_items1=0,num_items2=0,j,k,t,s;
	double betweenessTotal=0.0;
	Bucket* bucket1 = (Bucket*) NULL,* bucket2 = (Bucket*) NULL;

	knowEdgeSet = findBanEdgeSet(forum.forumMembersGraph,"knows",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("calculateModularity: findBanEdgeSet failed!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(knowEdgeSet == NULL){
		bannedEdges = 0;
	}
	else{
		bannedEdges = knowEdgeSet->edges.items;
	}
	m = forum.num_KnowEdges - bannedEdges; //Total number of edges this graph has;

	if(m <= 0){
		printf("calculateModularity: Error cannot calculate modularity (<= 0) m!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	int same_community=0;
	int num_edges1 = 0;
	int num_edges2 = 0;
	int neighboursFlag=0;

	double modularitySum=0.0;

	for(i=0; i < forum.forumMembersGraph->hashtable.size; i++){ //TODO: CUT DUPLICATES / DIPLOTUPA - PATH APO I SE J == PATH APO J SE I
		if(forum.forumMembersGraph->hashtable.hashmap[i] != NULL){ //For every BucketChain
			bucket1 = getHeadBucket(forum.forumMembersGraph->hashtable.hashmap[i], errorFlag); //Get head bucket
			if(*errorFlag == EXIT_VALUE){
				printf("calculateModularity: getHeadBucket failed!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
			while(bucket1 != NULL){ //For every Bucket in bucketchain
				for(j=0; j < bucket1->items; j++){ //For every item in bucket1
					if(bucket1->GNArray[j] != NULL){
						for(k = 0; k < forum.forumMembersGraph->hashtable.size; k++){	//Second Graph loop begins
							if(forum.forumMembersGraph->hashtable.hashmap[k] != NULL){
								bucket2 = getHeadBucket(forum.forumMembersGraph->hashtable.hashmap[k], errorFlag);
								if(*errorFlag == EXIT_VALUE){
									printf("calculateModularity: getHeadBucket failed!\n");
									*errorFlag = EXIT_VALUE;
									return EXIT_VALUE;
								}
								while(bucket2 != NULL){
									for(t=0; t < bucket2->items; t++){ //Modularity does not take into account graphnode1 != graphnode 2
										if(bucket2->GNArray[t] != NULL){
											same_community = 0;
											same_community = check_same_community(forum.forumMembersGraph,bucket1->GNArray[j],bucket2->GNArray[t],errorFlag);
											if(same_community == EXIT_VALUE){
												printf("calculateModularity: check_same_community failed!\n");
												*errorFlag = EXIT_VALUE;
												return EXIT_VALUE;
											}
											num_edges1 = countSameGraphKnowEdges(forum.forumMembersGraph,forumIndex,bucket1->GNArray[j],errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("calculateModularity: countSameGraphKnowEdges failed!\n");
												*errorFlag = EXIT_VALUE;
												return EXIT_VALUE;
											}									
											num_edges2 = countSameGraphKnowEdges(forum.forumMembersGraph,forumIndex,bucket2->GNArray[t],errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("calculateModularity: countSameGraphKnowEdges failed!\n");
												*errorFlag = EXIT_VALUE;
												return EXIT_VALUE;
											}
											neighboursFlag = checkNodesAreNeighbours(forum.forumMembersGraph,bucket1->GNArray[j],bucket2->GNArray[t],errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("calculateModularity: checkNodesAreNeighbours failed!\n");
												*errorFlag = EXIT_VALUE;
												return EXIT_VALUE;
											}

											modularitySum = modularitySum + (neighboursFlag - ((num_edges1 * num_edges2) / (2*m))) * (same_community);
										}		
									}

									bucket2 = getNextBucket(forum.forumMembersGraph->hashtable.hashmap[k],bucket2,errorFlag);

									if(*errorFlag == EXIT_VALUE){
										printf("calculateModularity: getNextBucket Failed!\n");
										*errorFlag = EXIT_VALUE;
										return EXIT_VALUE;
									}


								}

							}

						}
					}
				}

				bucket1 = getNextBucket(forum.forumMembersGraph->hashtable.hashmap[i],bucket1,errorFlag);

				if(*errorFlag == EXIT_VALUE){
					printf("edgeBetweennessCentrality: getNextBucket Failed!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}

			}

		}

	}

	return (modularitySum / (2*m));

}

Community* GirvanNewman(double modularity, Forum forum,int forumIndex,int* numberOfCommunities,int* errorFlag){ 
																		//TODO: IN ALL LOOPS THAT RUN THROUGH GRAPHNODES, WATCH OUT FOR NULL GRAPHNODES
	Graph* g = forum.forumMembersGraph;

	if(g == NULL){
		printf("GirvanNewman: Error Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	if(g->hashtable.num_records == 0){
		printf("GirvanNewman: Error HashTable has no records!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	if(g->hashtable.hashmap == NULL){
		printf("GirvanNewman: Error HashMap is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	//maxEdgePtr = NULL;
	//allEdgesBetweenessCentrality(g,&maxEdgePtr,double* maxBetweenessValuePtr);

	Edge* maxEdge = (Edge*) NULL;
	double maxEdgeValue = 0.0;
	double currentModularity = 0.0;
	double previousModularity = 0.0;

	printf("==============================GIRVAN NEWMAN=================================\n");
	int i=1;

	printf("\nGirvanNewMan: Calculating betweenessCentrality for all edges in Graph...\n");

	maxEdge = (Edge*) NULL;
	maxEdgeValue = 0.0;

	if(allEdgesBetweenessCentrality(g,&maxEdge,&maxEdgeValue) == EXIT_VALUE){
		printf("GirvanNewman: allEdgesBetweenessCentrality Calculation failed!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	do{
		printf("\nGirvanNewman: This is Girvan-Newman's %d iteration!\n",i);

		printf("\nGirvanNewMan: Banning edge with greatest betweeness Centrality...\n");

		if(maxEdge != NULL){
			printf("GirvanNewMan: Located edge with greatest betweeness Centrality...\n");
			printPersonKnowsPersonProperties(maxEdge);
			//printf("\n");
			printf("GirvanNewMan: Banning edge with greatest betweeness Centrality...\n");
			if(banGraphEdge(g,maxEdge) == EXIT_VALUE){
				//printf("GirvanNewman: banGraphEdge failed!\n");
				*errorFlag = EXIT_VALUE;
				return ((Community*) NULL);
			}
			//printBanSet(g);
			
		}
		else{
			break;
		}

		maxEdge = (Edge*) NULL;
		maxEdgeValue = 0.0;

		//printf("\nGirvanNewMan: Re-Calculating betweenessCentrality for all edges in Graph...\n");

		if(allEdgesBetweenessCentrality(g,&maxEdge,&maxEdgeValue) == EXIT_VALUE){
			//printf("GirvanNewman: allEdgesBetweenessCentrality Calculation failed!\n");
			*errorFlag = EXIT_VALUE;
			return ((Community*) NULL);
		}

		//printf("\nGirvanNewMan: Calculating calculateModularity...Previous was: %lf\n",previousModularity);

		previousModularity = currentModularity;
		currentModularity = calculateModularity(forum,forumIndex,errorFlag);
		if(*errorFlag == EXIT_VALUE){
			//printf("GirvanNewman: calculateModularity failed!\n");
			return ((Community*) NULL);
		}
		
		printf("\nGirvanNewMan: Current Modularity is: %lf\n",currentModularity);

		if(previousModularity == currentModularity){
			printf("\nGirvanNewman: Even though we did a new iteration modularity didn't change! Breaking!\n");
			break;
		}

		i++;

	}while(currentModularity < modularity);

	printf("GirvanNewman: Maximum modularity has been reached!\n");

	int max_size = 0; //Keeps the size of the biggest Community
	int num_cc = 0; //Keeps the number of communities that exist
	List communityList; //Will fill with communities available in Graph
	createList(&communityList,"CommunityPtr",&allocateCommunityPtr,&destroyCommunityPtr,NULL,NULL,&printCommunityPtr);
	num_cc = findCoherentGraphs(g,&max_size,&communityList,3,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("GirvanNewman: findCoherentGraphs failed!\n");
		destroyList(&communityList);
	}

	printf("GirvanNewman: We have discovered %d Communities in this Graph and the biggest one had: %d nodes!\n",num_cc,max_size);
	
	Community* Results = NULL;

	if(communityList.items == 0){
		destroyList(&communityList);
		return ((Community*) NULL);
	}
	else{
		int j=0;

		Results = malloc(sizeof(Community)*communityList.items);

		*numberOfCommunities = communityList.items;

		if(Results == NULL){
			printf("GirvanNewman: NULL Results pointer!\n");	
			destroyList(&communityList);
			return ((Community*) NULL);
		}

		for(j=0; j < communityList.items; j++){
			Results[j].members = (int*) NULL;
		}

		Community* tempComm = (Community*) NULL;
		Node* myCurrent = (Node*) NULL;
		myCurrent = communityList.Head; //Run through all the Communities
		while(myCurrent != NULL){
			tempComm = myCurrent->data;
			if(tempComm == NULL){
				printf("GirvanNewman: NULL Community pointer!\n");	
				for(j=0; j < communityList.items; j++){
					if(Results[j].members != NULL)
						free(Results[j].members);
				}
				free(Results);
				destroyList(&communityList);
				return ((Community*) NULL);
			}

			Results[i].numberOfMembers = tempComm->numberOfMembers; //Store community
			Results[i].members = malloc(sizeof(int)*Results[i].numberOfMembers);
			if(Results[i].members == NULL){
				printf("cliquePercolationMethod: NULL Community members pointer!\n");
				for(j=0; j < communityList.items; j++){
					if(Results[j].members != NULL)
						free(Results[j].members);
				}
				free(Results);				
			}

			for(j=0; j < Results[i].numberOfMembers; j++){
				Results[i].members[j] = tempComm->members[j];
			}
       
			myCurrent = myCurrent->next;
			i++;
		}
				
		destroyList(&communityList);

		return Results;
	}

}

List* clique_1(Graph* g,int* errorFlag){ //Basic recursive case - Returns all Level-1 Cliques in a List

	int i=0,j=0;

	List* graphNodeList = NULL;
	Bucket* bucket=NULL;

	//printf("clique_1: We will build a List of Level1 Cliques!\n");

	graphNodeList = malloc(sizeof(List));

	if(graphNodeList == NULL){
		printf("clique_1: Failed to allocate memory for List!\n");
		*errorFlag = EXIT_VALUE;
		return ((List*) (NULL));
	}

	////////////////FIRST WE GATHER ALL GRAPHNODES IN GRAPH///////////////

	createList(graphNodeList,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);

	if(g != NULL){
		if(g->hashtable.hashmap != NULL){
			for(i=0; i < g->hashtable.size; i++){
				if(g->hashtable.hashmap[i] != NULL){ //For every BucketChain
					bucket = getHeadBucket(g->hashtable.hashmap[i], errorFlag); //Get head bucket
					if(*errorFlag == EXIT_VALUE){
						printf("clique_1: getHeadBucket failed!\n");
						destroyList(graphNodeList);
						free(graphNodeList);
						*errorFlag = EXIT_VALUE;
						return ((List*) (NULL));
					}
					while(bucket != NULL){ //For every Bucket in bucketchain
						for(j=0; j < bucket->items; j++){ //For every item in bucket
							if(bucket->GNArray[j] != NULL){
								addLastList(graphNodeList, bucket->GNArray[j]); //Add to 1-clique
							}
						}
						bucket = getNextBucket(g->hashtable.hashmap[i],bucket,errorFlag);
						if(*errorFlag == EXIT_VALUE){
							printf("clique_1: getNextBucket failed!\n");
							*errorFlag = EXIT_VALUE;
							destroyList(graphNodeList);
							free(graphNodeList);
							return ((List*) (NULL));
						}
					}	
				}
			}
		}
		else{
			printf("clique_1: HashMap is NULL!\n");
			*errorFlag = EXIT_VALUE;
			destroyList(graphNodeList);
			free(graphNodeList);
			return ((List*) (NULL));
		}
	}
	else{
		printf("clique_1: Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		destroyList(graphNodeList);
		free(graphNodeList);
		return ((List*) (NULL));
	}

	////////////////AFTER WE GATHER ALL GRAPHNODES IN GRAPH///////////////

	//Each GraphNode will be placed in a seperate GraphNode List that represents a Level-1 Clique (aka a Clique with 1 Node)
	//After that all Level-1 Cliques will be placed in a List that returns as the result of the function

	List* level1CliquesList= ((List*) NULL);

	//printf("clique_1: We have gathered this List of GraphNodes:\n");
	//printList(*graphNodeList);
	//printf("\n");

	level1CliquesList = malloc(sizeof(List));
	if(level1CliquesList == NULL){
		printf("clique_1: Failed to allocate memory for level1 Cliques List!\n");
		*errorFlag = EXIT_VALUE;
		destroyList(graphNodeList);
		free(graphNodeList);
		return ((List*) (NULL));		
	}
	
	createList(level1CliquesList,"ListofList",&allocateListPtr,&destroyListPtr,NULL,NULL,&printListPtr);
	
	Node* myCurrent= (Node*) NULL;
	GraphNode* gnode= (GraphNode*) NULL;

	List* level1Clique = (List*) NULL;

	//printf("clique_1: Now we will fill the list of level1 Cliques!\n");

	myCurrent = graphNodeList->Head;
	while(myCurrent != NULL){ //Run through all GraphNodes
		gnode = (GraphNode*) myCurrent->data; //Get a GraphNode
		if(gnode == NULL){
			//printf("clique_1: Error! NULL GraphNode!\n");
			if(level1CliquesList != NULL){
				destroyList(level1CliquesList);
				free(level1CliquesList);
			}
			*errorFlag = EXIT_VALUE;
			destroyList(graphNodeList);
			free(graphNodeList);
			return ((List*) (NULL));
		}
		level1Clique = malloc(sizeof(List)); //Allocate a List to represent a level1 Clique	
		if(level1Clique == NULL){
			//printf("clique_1: Error! Failed to allocate memory for level1Clique!\n");
			if(level1CliquesList != NULL){
				destroyList(level1CliquesList);
				free(level1CliquesList);
			}
			*errorFlag = EXIT_VALUE;
			destroyList(graphNodeList);
			free(graphNodeList);
			return ((List*) (NULL));
		}
		//printf("Allocated memory for Level 1 Clique\n");
		createList(level1Clique,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);
		//printf("Created Level1 Clique \n");
		if(addLastList(level1Clique,gnode) == EXIT_VALUE){ //Add a GraphNode to it
			//printf("clique_1: addLastList failed!\n");
			if(level1Clique != NULL){
				destroyList(level1Clique);
				free(level1Clique);
			}
			if(level1CliquesList != NULL){
				destroyList(level1CliquesList);
				free(level1CliquesList);
			}
			*errorFlag = EXIT_VALUE;
			destroyList(graphNodeList);
			free(graphNodeList);
			return ((List*) (NULL));
		}
		//printf("Added GraphNode to level1Clique \n");
		//printList(*level1Clique);
		//printf("\n");

		addLastList(level1CliquesList,level1Clique); //Add level1 Clique to List of level1 Cliques
		//printf("List of level1 Cliques contains: \n");
		//printList(*level1CliquesList);
		//printf("\n");

		destroyList(level1Clique); //Free this copy of level1Clique since
		free(level1Clique);

		myCurrent = myCurrent->next; //Get the next GraphNode
	}

	destroyList(graphNodeList);
	free(graphNodeList);

	//printf("List of level1 Cliques finally contains: \n");
	//printList(*level1CliquesList);
	//printf("\n");

	return ((List*) level1CliquesList); //Return all level1Cliques in a List

} 

List* cliqueBuild(Graph* g,int k,int* errorFlag){
	List* previousLevelCliques = (List*) NULL;
	List* currentLevelCliques = (List*) NULL;
	Bucket* bucket=(Bucket*) NULL;
	int i=0,j=0;

	if(k > g->hashtable.num_records){
		printf("cliqueBuild: Can't build any K-Cliques when K > Number of GraphNodes!\n");
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	if(k == 1){
		previousLevelCliques = clique_1(g,errorFlag);
		if(*errorFlag == EXIT_VALUE){
			return ((List*) NULL);
		}
		return ((List*) previousLevelCliques);
	}
	else{
		previousLevelCliques = cliqueBuild(g,k-1,errorFlag); //A list of k-1 Cliques
		if(*errorFlag == EXIT_VALUE){
			printf("cliqueBuild: previous Level's cliqueBuild failed!\n");
			return ((List*) NULL);
		}
		currentLevelCliques = malloc(sizeof(List));
		if(currentLevelCliques == NULL){
			*errorFlag = EXIT_VALUE;
			if(previousLevelCliques != NULL){
				destroyList(previousLevelCliques);
				free(previousLevelCliques);
			}
			printf("cliqueBuild: currentLevelCliques is NULL!\n");
			return ((List*) NULL);
		}
		createList(currentLevelCliques,"ListofList",&allocateListPtr,&destroyListPtr,NULL,NULL,&printListPtr); //S will be a List of level-k cliques		

		//printf("Created List of Level %d Cliques!\n",k);		
						
		Node* myCurrent = (Node*) NULL;
		Node* myCurrent2 = (Node*) NULL;
		Node* myCurrent3 = (Node*) NULL;
		GraphNode* GNode = (GraphNode*) NULL;

		List* aClique = (List*) NULL;
		int neighboursFlag = 0;
		int duplicateCheck = 0;
		List* newClique = (List*) NULL;
		List* tempClique = (List*) NULL;

		myCurrent = previousLevelCliques->Head;

		//printf("previousLevelCliques list has %d cliques in it!\n",previousLevelCliques->items);

		//printList(*previousLevelCliques);

		//printf("\n");

		while(myCurrent != NULL){
			aClique = myCurrent->data; //Get a k-1 Clique
			//printf("Got a clique:\n");
			//printList(*aClique);
			//printf("\n");

			if(aClique == NULL){
				*errorFlag = EXIT_VALUE;
				if(previousLevelCliques != NULL){
					destroyList(previousLevelCliques);
					free(previousLevelCliques);
				}
				if(currentLevelCliques != NULL){
					destroyList(currentLevelCliques);
					free(currentLevelCliques);
				}
				printf("cliqueBuild: aClique is NULL!\n");
				return ((List*) NULL);
			}

			//printf("Now I will run through Graph!\n");
			if(g != NULL){ //Run through all GraphNodes
				if(g->hashtable.hashmap != NULL){
					for(i=0; i < g->hashtable.size; i++){
						if(g->hashtable.hashmap[i] != NULL){ //For every BucketChain
							bucket = getHeadBucket(g->hashtable.hashmap[i], errorFlag); //Get head bucket
							if(*errorFlag == EXIT_VALUE){
								printf("cliqueBuild: getHeadBucket failed!\n");
								if(previousLevelCliques != NULL){
									destroyList(previousLevelCliques);
									free(previousLevelCliques);
								}
								if(currentLevelCliques != NULL){
									destroyList(currentLevelCliques);
									free(currentLevelCliques);
								}
								*errorFlag = EXIT_VALUE;
								return ((List*) NULL);
							}
							while(bucket != NULL){ //For every Bucket in bucketchain
								for(j=0; j < bucket->items; j++){ //For every item in bucket
									if(bucket->GNArray[j] != NULL){
										//printf("Holding GraphNode: %d\n",bucket->GNArray[j]->id);
										myCurrent2 = NULL;
										GNode = NULL;
										myCurrent2 = aClique->Head;
										while(myCurrent2 != NULL){ //Run through all elements of old Clique
											GNode = (GraphNode*) myCurrent2->data; //Get GraphNode from old Clique
											if(GNode == NULL){
												printf("cliqueBuild: GNode is NULL!\n");
												*errorFlag = EXIT_VALUE;
												if(previousLevelCliques != NULL){
													destroyList(previousLevelCliques);
													free(previousLevelCliques);
												}
												if(currentLevelCliques != NULL){
													destroyList(currentLevelCliques);
													free(currentLevelCliques);
												}
												return ((List*) NULL);
											}

											//printf("Holding Clique element: %d\n",GNode->id);

											neighboursFlag=0;
											neighboursFlag = checkNodesAreNeighbours(g,GNode,bucket->GNArray[j],errorFlag); //Check current Element of k-1 Clique is neighbour with current Element of Graph
											if(*errorFlag == EXIT_VALUE){
												printf("cliqueBuild: checkNodesAreNeighbours failed!\n");
												*errorFlag = EXIT_VALUE;
												if(previousLevelCliques != NULL){
													destroyList(previousLevelCliques);
													free(previousLevelCliques);
												}
												if(currentLevelCliques != NULL){
													destroyList(currentLevelCliques);
													free(currentLevelCliques);
												}
												return ((List*) NULL);
											}

											if(neighboursFlag == 0){ //This means that the current element of the graph can't form a new clique with the old Clique members
												//printf("These 2 are not neighbours, sorry this Graph Element can't form a new clique with the old one!\n");
												break;
											}

											myCurrent2 = myCurrent2->next;

											if(myCurrent2 == NULL){ //If this was the last element of this k-1 clique
												//printf("New level2-clique can be created!\n");

												newClique = (List*) NULL;
												newClique = malloc(sizeof(List));
												if(newClique == NULL){
													printf("cliqueBuild: Failed to allocate memory for newClique!\n");
													*errorFlag = EXIT_VALUE;
													if(previousLevelCliques != NULL){
														destroyList(previousLevelCliques);
														free(previousLevelCliques);
													}
													if(currentLevelCliques != NULL){
														destroyList(currentLevelCliques);
														free(currentLevelCliques);
													}
													return ((List*) NULL);
												}
												createList(newClique,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,
												&assignGraphNodePtr,&printGraphNodePtr);

												appendLists(newClique, aClique); //Add all of old clique to new clique
										
												addLastList(newClique,bucket->GNArray[j]); //Now add the new element too
							
												//printf("The new level2-clique now contains: \n");
												//printList(*newClique);
												//printf("\n");
												if(currentLevelCliques->items > 0){
													myCurrent3=NULL;
													myCurrent3 = currentLevelCliques->Head;
													duplicateCheck = 0;
													tempClique = (List*) NULL;
													while(myCurrent3 != NULL){
														tempClique = (List*) myCurrent3->data;
															
														duplicateCheck = listsAreDuplicated(*tempClique,*newClique,1); //This will keep only one of 1,2,3 3,2,1 2,1,3 will be kept
														if(duplicateCheck == 1) break;
														myCurrent3 = myCurrent3->next;
													}

												}
												
												if(duplicateCheck == 0){
													addLastList(currentLevelCliques, newClique);
												}
					
												//printf("The list of level2-clique2 now contains: \n");
												//printList(*currentLevelCliques);
												//printf("\n");

												destroyList(newClique);

												free(newClique);
											}

										}


									}
								}
								bucket = getNextBucket(g->hashtable.hashmap[i],bucket,errorFlag);
								if(*errorFlag == EXIT_VALUE){
									printf("cliqueBuild: getNextBucket failed!\n");
									*errorFlag = EXIT_VALUE;
									if(previousLevelCliques != NULL){
										destroyList(previousLevelCliques);
										free(previousLevelCliques);
									}
									if(currentLevelCliques != NULL){
										destroyList(currentLevelCliques);
										free(currentLevelCliques);
									}
									return ((List*) NULL);
								}
							}	
						}
					}
				}
				else{
					printf("cliqueBuild: HashMap is NULL!\n");
					*errorFlag = EXIT_VALUE;
					if(previousLevelCliques != NULL){
						destroyList(previousLevelCliques);
						free(previousLevelCliques);
					}
					if(currentLevelCliques != NULL){
						destroyList(currentLevelCliques);
						free(currentLevelCliques);
					}
					return ((List*) NULL);
				}
			}
			else{
				printf("cliqueBuild: Graph is NULL!\n");
				*errorFlag = EXIT_VALUE;
				if(previousLevelCliques != NULL){
					destroyList(previousLevelCliques);
					free(previousLevelCliques);
				}
				if(currentLevelCliques != NULL){
					destroyList(currentLevelCliques);
					free(currentLevelCliques);
				}
				return ((List*) NULL);
			}
			myCurrent = myCurrent->next; //Move to the next k-1 clique
		}

		destroyList(previousLevelCliques);
		free(previousLevelCliques);

		return ((List*) currentLevelCliques);

	}

}

Community* cliquePercolationMethod(int k, Graph* g,int* numberOfCommunities,int* errorFlag) { //Receives a forumMembersGraph and returns all Communities in it


	Graph* hyperGraph=NULL;
	int m=4; //To create Clique Graph (size of hashtable) for now
	int c=3; //     >>      (Capacity of  hashtable's bucket) for now
	//Checks
	if (k < 2) {
		printf("cliquePercolationMethod: Unable to find cliques! (k must be greater than 1 but k given is %d \n",k);
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
    }

	if(g == NULL){
		printf("cliquePercolationMethod: Graph is empty!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	if(g->hashtable.num_records == 0){
		printf("cliquePercolationMethod: Graph is empty!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

	if(g->hashtable.hashmap == NULL){
		printf("cliquePercolationMethod: Graph HashMap is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return ((Community*) NULL);
	}

/////////////////////////k-Cliques creation//////////////////////////////
	
	List* cliquesList = (List*) NULL;

	cliquesList = cliqueBuild(g,k,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("cliquePercolationMethod: cliqueBuild failed!\n");
		return freeCPMResources(cliquesList,NULL,NULL,NULL,NULL,NULL,NULL,errorFlag);	
	}

	printf("cliquePercolationMethod: This forum has the following %d-level Cliques!:\n",k);

	if(cliquesList != NULL){
		if(cliquesList->items > 0)
			printList(*cliquesList);
		else{
			printf("cliquePercolationMethod: No %d-level Cliques could be created for this forum!\n",k);
			freeCPMResources(cliquesList,NULL,NULL,NULL,NULL,NULL,NULL,errorFlag);
			*errorFlag = -1; //Means no cliques available;
			return ((Community*) NULL);	
		}
	}
	else{
		printf("cliquePercolationMethod: No cliques Available?\n");
		return freeCPMResources(cliquesList,NULL,NULL,NULL,NULL,NULL,NULL,errorFlag);	
	}

////////////////////////////////////////////////////////////////////////

	printf("cliquePercolationMethod: Creating Hyper Graph...\n");

	hyperGraph = createGraph(m,c,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("cliquePercolationMethod: Failed to create hyperGraph!\n");
		return freeCPMResources(cliquesList,NULL,hyperGraph,NULL,NULL,NULL,NULL,errorFlag);	
	}

	List hyperGraphNodesList; //Keep a list with all hyperGraphNodes
	List hyperEdgesList; //Keep a list with all hyperEdges

	createList(&hyperGraphNodesList,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);
	createList(&hyperEdgesList,"EdgePtr",&allocateEdgePtr,&destroyEdgePtr,&compareEdgePtr,&assignEdgePtr,&printEdgePtr);

	printf("cliquePercolationMethod: Creating HyperNodes...\n");
	
	Node* myCurrent = (Node*) NULL;
	Node* myCurrent2 = (Node*) NULL;
	GraphNode* GNode = (GraphNode*) NULL; //A Hyper/Clique GraphNode
	GraphNode* tempGNode = (GraphNode*) NULL;
	List* aClique = (List*) NULL;
	Edge* knowsEdge = (Edge*) NULL; //HyperEdge == Know Edges between clique GraphNodes, this was made for compatability reasons with reachNodeN
	int commonCheckResult=0;

	myCurrent = cliquesList->Head;
	int i=1;
	while(myCurrent != NULL){ //Run through all k-level Cliques
		aClique = (List*) myCurrent->data; //Get a clique

		if(aClique == NULL){
			printf("cliquePercolationMethod: aClique is NULL!\n");
			return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,NULL,errorFlag);
		}

		GNode = setCliqueProperties(i,aClique,errorFlag); //Creates hyper GraphNode aka cliqueGraphNode
		//printf("cliquePercolationMethod: We just created hyperGraphNode:\n");
		//printCliqueProperties(GNode);
		//printf("\n");

		if(*errorFlag == EXIT_VALUE){
			printf("cliquePercolationMethod: setCliqueProperties failed!\n");
			return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,NULL,errorFlag);
		}

		if(insertNode(GNode, hyperGraph) == EXIT_VALUE){ //Inserts hyper GraphNode into hyperGraph
			printf("cliquePercolationMethod: insertNode failed!\n");
			return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,NULL,errorFlag);	
		}

		//printf("cliquePercolationMethod: Inserted hyperGraphNode successfully!\n");

		if(hyperGraph->hashtable.num_records > 1){ //If more than one record exists in hyperGraphNode, time to check if we can place edges
			//printf("cliquePercolationMethod: Let's check if we can insert edges between this new hyperGraphNode and the old ones!\n");

			myCurrent2 = ((Node*) NULL);
			myCurrent2 = hyperGraphNodesList.Head;
			while(myCurrent2 != NULL){ //Run through all the already placed hyperGraphNodes
				tempGNode = (GraphNode*) myCurrent2->data; //Get one of them
				commonCheckResult=0;
				commonCheckResult = checkKminus1Common(tempGNode,GNode,k,errorFlag); //Check if k-1 common Edges exist between this GraphNode and the new one

				if(*errorFlag == EXIT_VALUE){
					printf("cliquePercolationMethod: checkKminus1Common failed NULL!\n");
					return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,NULL,errorFlag);				
				}

				if(commonCheckResult == 1){
					knowsEdge = (Edge*) NULL;
					knowsEdge = setCliqueKnowsCliqueProperties(GNode->id,tempGNode->id,"knows",errorFlag); //Create hyperEdge
					//printf("cliquePercolationMethod: We just created hyperEdge:\n");
					//printCliqueKnowsCliqueProperties(knowsEdge);
					//printf("\n");

					if(*errorFlag == EXIT_VALUE){
						printf("cliquePercolationMethod: setCliqueKnowsCliqueProperties failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);					
					}

					if(insertEdge(GNode->id,knowsEdge,hyperGraph) == EXIT_VALUE){ //Insert Edge in hyperGraph
						printf("cliquePercolationMethod: insertEdge failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);		
					}

					if(addLastList(&hyperEdgesList,knowsEdge) == EXIT_VALUE){ //Insert Edge in hyperEdgeList to properly free later
						printf("cliquePercolationMethod: addLastList failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);
					}

					knowsEdge = (Edge*) NULL;
					knowsEdge = setCliqueKnowsCliqueProperties(tempGNode->id,GNode->id,"knows",errorFlag); //Create hyperEdge

					if(*errorFlag == EXIT_VALUE){
						printf("cliquePercolationMethod: setCliqueKnowsCliqueProperties failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);			
					}

					if(insertEdge(tempGNode->id,knowsEdge,hyperGraph) == EXIT_VALUE){ //Insert Edge in hyperGraph
						printf("cliquePercolationMethod: insertEdge failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);
					}

					if(addLastList(&hyperEdgesList,knowsEdge) == EXIT_VALUE){ //Insert Edge in hyperEdgeList to properly free later
						printf("cliquePercolationMethod: addLastList failed!\n");
						return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,&knowsEdge,NULL,errorFlag);
					}

				}

				myCurrent2 = myCurrent2->next;
			}

		}

		if(addLastList(&hyperGraphNodesList, GNode) == EXIT_VALUE){
			printf("cliquePercolationMethod: addLastList failed NULL!\n");
			return freeCPMResources(cliquesList,&GNode,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,NULL,errorFlag);
		}

		i++;
		myCurrent = myCurrent->next;
	}


	int max_size = 0; //Keeps the size of the biggest Community
	int num_cc = 0; //Keeps the number of communities that exist
	List communityList; //Will fill with communities available in Graph
	createList(&communityList,"CommunityPtr",&allocateCommunityPtr,&destroyCommunityPtr,NULL,NULL,&printCommunityPtr);
	num_cc = findCoherentGraphs(hyperGraph,&max_size,&communityList,2,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("cliquePercolationMethod: findCoherentGraphs failed!\n");
		return freeCPMResources(cliquesList,NULL,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,&communityList,errorFlag);
	}


	printf("cliquePercolationMethod: We have discovered %d Communities in this Graph and the biggest one had: %d nodes!\n",num_cc,max_size);
	
	Community* Results = NULL;

	if(communityList.items == 0){
		freeCPMResources(cliquesList,NULL,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,&communityList,errorFlag); //Free temporary resources created by CPM
		return ((Community*) NULL);
	}
	else{
		i=0;
		int j=0;

		Results = malloc(sizeof(Community)*communityList.items);

		*numberOfCommunities = communityList.items;

		if(Results == NULL){
			printf("cliquePercolationMethod: NULL Results pointer!\n");	
			freeCPMResources(cliquesList,NULL,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,&communityList,errorFlag);
		}

		for(j=0; j < communityList.items; j++){
			Results[j].members = (int*) NULL;
		}

		Community* tempComm = (Community*) NULL;
		myCurrent = (Node*) NULL;
		myCurrent = communityList.Head; //Run through all the Communities
		while(myCurrent != NULL){
			tempComm = myCurrent->data;
			if(tempComm == NULL){
				printf("cliquePercolationMethod: NULL Community pointer!\n");	
				for(j=0; j < communityList.items; j++){
					if(Results[j].members != NULL)
						free(Results[j].members);
				}
				free(Results);

				return freeCPMResources(cliquesList,NULL,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,&communityList,errorFlag);
			}

			Results[i].numberOfMembers = tempComm->numberOfMembers; //Store community
			Results[i].members = malloc(sizeof(int)*Results[i].numberOfMembers);
			if(Results[i].members == NULL){
				printf("cliquePercolationMethod: NULL Community members pointer!\n");
				for(j=0; j < communityList.items; j++){
					if(Results[j].members != NULL)
						free(Results[j].members);
				}
				free(Results);				
			}

			for(j=0; j < Results[i].numberOfMembers; j++){
				Results[i].members[j] = tempComm->members[j];
			}
       
			myCurrent = myCurrent->next;
			i++;
		}
				
		freeCPMResources(cliquesList,NULL,hyperGraph,&hyperGraphNodesList,&hyperEdgesList,NULL,&communityList,errorFlag);
		*errorFlag = 0;

		return Results;
	}

}

Community* freeCPMResources(List* cliquesList,GraphNode** GNode,Graph* hyperGraph,List* hyperGraphNodesList,List* hyperEdgesList,Edge** knowsEdge,List* communityList,int* errorFlag){

	Node* myCurrent = (Node*) NULL;

	if(cliquesList != NULL){
		destroyList(cliquesList);
		free(cliquesList);
	}

	if(GNode != NULL){
		if(*GNode != NULL){
			destroyGraphNode(GNode);
			*GNode = (GraphNode*) NULL;
		}
	}

	if(hyperGraph != NULL){
		destroyGraph(hyperGraph);
	}

	if(hyperGraphNodesList != NULL){
		myCurrent = hyperGraphNodesList->Head;
		while(myCurrent != NULL){
			if(myCurrent->data != NULL){
				destroyGraphNode((GraphNode**) &(myCurrent->data));
				myCurrent->data = NULL;
			}
			myCurrent = myCurrent->next;
		}
		destroyList(hyperGraphNodesList);
	}

	if(hyperEdgesList != NULL){
		myCurrent = hyperEdgesList->Head;
		while(myCurrent != NULL){
			if(myCurrent->data != NULL){
				destroyEdge((Edge**) &(myCurrent->data));
				myCurrent->data = NULL;
			}
			myCurrent = myCurrent->next;
		}
		destroyList(hyperEdgesList);
	}

	if(knowsEdge != NULL){
		if(*knowsEdge != NULL){
			destroyEdge(knowsEdge);
			*knowsEdge = NULL;
		}
	}

	if(communityList != NULL){
		Community* tempComm = (Community*) NULL;
		myCurrent = (Node*) NULL;
		myCurrent = communityList->Head;
		while(myCurrent != NULL){
			tempComm = myCurrent->data;
			if(tempComm != NULL){
				if(tempComm->members != NULL){
					free(tempComm->members);
				}
				free(tempComm);
			}
			myCurrent = myCurrent->next;
		}
		destroyList(communityList);
	}

	*errorFlag = EXIT_VALUE;

	return ((Community*) NULL);

}

int checkKminus1Common(GraphNode* GNode1,GraphNode* GNode2,int k,int* errorFlag){

	if((GNode1 == NULL) || (GNode2 == NULL)){
		printf("checkMinus1Common: Hyper GraphNode is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(k < 2){
		printf("checkMinus1Common: k is less than 2!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	List* clique1 = (List*) NULL;	
	List* clique2 = (List*) NULL;	

	clique1 = (List*) GNode1->properties->arrayP[0].attribute;
	clique2 = (List*) GNode2->properties->arrayP[0].attribute;

	Node* myCurrent=NULL;
	Node* myCurrent2=NULL;

	GraphNode* tempGNode1=NULL;
	GraphNode* tempGNode2=NULL;

	int numberOfCommons=0;

	//printf("checkMinus1Common: Clique1: ");
	//printList(*clique1);
	//printf("\n");

	//printf("checkMinus1Common: Clique2: ");
	//printList(*clique2);
	//printf("\n");

	//printf("checkMinus1Common: Testing if the two cliques have k-1 commons...");

	myCurrent = clique1->Head;
	while(myCurrent != NULL){
		tempGNode1 = (GraphNode*) myCurrent->data;
		if(tempGNode1 == NULL){
			printf("checkMinus1Common: tempGNode is NULL!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		myCurrent2 = (Node*) NULL;
		myCurrent2 = clique2->Head;
		while(myCurrent2 != NULL){
			tempGNode2 = (GraphNode*) myCurrent2->data;
			if(tempGNode2 == NULL){
				printf("checkMinus1Common: tempGNode2 is NULL!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
			if(tempGNode1 == tempGNode2){ //We only need to check if addresses are equal because the original GraphNodes should exist only once
				numberOfCommons++;
			}
			
			if((tempGNode1->id == tempGNode2->id) && (tempGNode1 != tempGNode2)){
				printf("checkMinus1Common: Different GraphNode addresses but same IDs? something has gone wrong!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;			
			}

			myCurrent2 = myCurrent2->next;
		}

		myCurrent = myCurrent->next;
	}

	if(numberOfCommons == k-1){
		//printf("checkMinus1Common: They do!\n");
		return 1;
	}
	else{
		//printf("checkMinus1Common: They don't!\n");
		return 0;
	}

}

/* CommunityPtr functions for List */
int allocateCommunityPtr(void** target,void* source){

	Community** targetC = (Community**) target;
	Community* sourceC = (Community*) source;

	if(targetC != NULL){
		if(sourceC != NULL){
			*targetC = sourceC;
		}
		else{
			printf("allocateCommunityPtr: SourceC is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("allocateCommunityPtr: TargetC is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;
}

int destroyCommunityPtr(void** target){

	Community** targetC = (Community**) target;

	if(targetC != NULL){
		*targetC = NULL;
	}
	else{
		printf("destroyCommunityPtr: TargetC is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;

}

int printCommunityPtr(void* target){

	Community* targetC = (Community*) target;

	if(targetC == NULL){
		printf("printCommunityPtr: TargetC is NULL!\n");
		return EXIT_VALUE;		
	}

	if(targetC->members == NULL){
		printf("printCommunityPtr: TargetC NULL members pointer!\n");
		return EXIT_VALUE;	
	}

	printf("=========COMMUNITY========\n");
	int i=0;
	for(i=0; i < targetC->numberOfMembers; i++){
		if(i != targetC->numberOfMembers)
			printf("|%d|->",targetC->members[i]);
		else
			printf("|%d|\n",targetC->members[i]);
	}

	printf("\nNumberOfMembers: %d\n",targetC->numberOfMembers);

	return 0;

}
