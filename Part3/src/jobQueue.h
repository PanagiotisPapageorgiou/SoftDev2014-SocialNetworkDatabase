typedef struct JobQueue{
	List queueList;
	int priorityQueueFlag;
	int limit;
	int working;
} JobQueue;

int initJobQueue(int priorityQueueFlag, int limit,JobQueue* jobQueue);
int clearJobQueue(JobQueue* jobQueue);
int addJobQueue(JobQueue* jobQueue,Job* job);
Job* frontJobQueue(JobQueue* jobQueue,int* errorFlag);
int popJobQueue(JobQueue* jobQueue);
int printJobQueue(JobQueue* jobQueue);
