#include "GraphQueries.h" //Everything up to part2
#include "fileReader.h" //Needed to declare AllDataSetStructs
#include "jobQueue.h" //Needed to declare JobQueue
#define NUM_THREADS 10

typedef struct Forum{
	int forumID;
	long number_of_members;
	Graph* forumMembersGraph; //A graph that connects all members of a forum with KNOWS relations
	long num_KnowEdges; //The number of know edges between all members of the same forum (does not take into account banned edges)
} Forum;

////////////////////////////GLOBAL UTILITIES - DataStructs Accessible by all threads////////////////////////////////////////////////////

typedef struct Global_Utilities{
	JobQueue jobQueue; //The Scheduler Queue
	pthread_t* worker_threads; //An array of worker thread variables
	pthread_cond_t cond_nonempty; //Condition variable for not empty Scheduler queue
	pthread_mutex_t queue_mtx;	//Queue access mutex
	pthread_mutex_t malloc_mtx; //Malloc access mutex
	pthread_mutex_t free_mtx; //Free access mutex
	pthread_mutex_t active_workers_mtx; //Active Workers access mutex
	int thread_pool_size;	//Number of worker threads
	int active_workers;		//Number of active worker threads
	AllDataSetStructs allDataSetStructs; //All Graphs and Lists that can be created directly from the DataSet
	long jobCounter; //Counter used to assign different IDs to Jobs
	Forum* topNForums;	//The topNForums
	int N; //The number of top Forums
	pthread_mutex_t topNForums_mtx;
	long tasksLeft; //Shows how many tasks the main threads must still wait to be done
	pthread_mutex_t tasksLeft_mtx; //Protects tasksLeft variable
	pthread_cond_t cond_tasksDone; //Condition variable wakesup main thread each time a task is delivered
	pthread_cond_t cond_workerExits;
	int m,c; //m,c program parameters
	sigset_t signal_mask;		   //SIGSET mask used to catch SIGINT and SIGTERM and properly close the process while freeing memory
} Global_Utilities;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * signal_thread(void* arg);

int initSharedDataStructs(int m,int c);
int initSharedCondMutexes();
int destroySharedDataStructs();

/* Forum Preparation */
int createThreadPool(int N);
int insertTopNForums(int forumID, long members);
int calculateForumMembers(int forumID);
int fillForumMembersGraph(int forumIndex);
void* Consumer(void* argsptr);
Job* createForumPreparationJob(GraphNode* forumGNode,int* errorFlag);
Job* createForumFillJob(int forumIndex,int* errorFlag);
int addNewJobInQueue(Job* job);
int findTopNforums(int N);

/* CPM */
Community* cliquePercolationMethod(int k, Graph* g,int* numberOfCommunities,int* errorFlag);
Community* freeCPMResources(List* cliquesList,GraphNode** Gnode,Graph* hyperGraph,List* hyperGraphNodeList,List* hyperEdgesList,Edge** knowsEdge,List* communityList,int* errorFlag);
int checkKminus1Common(GraphNode* GNode1,GraphNode* GNode2,int k,int* errorFlag);

/* Girvan Newman */
double edgeBetweennessCentrality(Edge* e, Graph* g,List* listAllPaths,int* errorFlag);
int allEdgesBetweenessCentrality(Graph* g,Edge** maxEdgePtr,double* maxBetweenessValuePtr);
int countSameGraphKnowEdges(Graph* forumMembersGraph,int forumIndex,GraphNode* gnode,int* errorFlag);
double calculateModularity(Forum forum,int forumIndex,int* errorFlag);
Community* GirvanNewman(double modularity, Forum forum,int forumIndex,int* numberOfCommunities,int* errorFlag);

/* CommunityPtr functions for List */
int allocateCommunityPtr(void**,void*);
int destroyCommunityPtr(void**);
int printCommunityPtr(void*);
