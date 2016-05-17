#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datatypes.h"
#include "jobQueue.h"

////////////////////////////////////////////JOB QUEUE////////////////////////////////////////////////

int initJobQueue(int priorityQueueFlag, int limit,JobQueue* jobQueue){
	
	if(jobQueue == NULL){
		printf("initJobQueue: Job Queue is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	createList(&(jobQueue->queueList), "JobQueue", &allocateJobPtr, &destroyJobPtr, NULL, NULL, &printJobPtr); //Create queue based on uberlist
	jobQueue->limit = limit;
	jobQueue->working = 1;
	jobQueue->priorityQueueFlag = 0;

}

int clearJobQueue(JobQueue* jobQueue){
	
	if(jobQueue == NULL){
		printf("clearJobQueue: Job Queue is NULL! Exiting!\n");
		return EXIT_VALUE;
	}
	jobQueue->working = 0;

	destroyList(&(jobQueue->queueList));

	return 0;
}

int addJobQueue(JobQueue* jobQueue,Job* job){

	if(jobQueue == NULL){
		printf("addJobQueue: Job Queue is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	if(jobQueue->priorityQueueFlag == 1){
		printf("addJobQueue: Sorry priority Queue mode is not ready!\n");
		return EXIT_VALUE;
	}
	else{
		addLastList(&(jobQueue->queueList),job);
	}

}

Job* frontJobQueue(JobQueue* jobQueue,int* errorFlag){

	Job* tempJob = NULL;

	if(jobQueue == NULL){
		printf("frontJobQueue: Job Queue is NULL! Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Job*) NULL);
	}
	else{
		if(jobQueue->queueList.items > 0){
			tempJob = (Job*) frontList(jobQueue->queueList);
		}
		else{
			return tempJob;
		}
	}

}

int popJobQueue(JobQueue* jobQueue){

	if(jobQueue == NULL){
		printf("popJobQueue: JobQueue is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	if(jobQueue->queueList.items > 0){
		popList(&(jobQueue->queueList));
	}

	return 0;

}

int printJobQueue(JobQueue* jobQueue){

	if(jobQueue == NULL){
		printf("printJobQueue: JobQueue is NULL! Exiting!\n");
		return EXIT_VALUE;
	}

	printList(jobQueue->queueList);

	return 0;


}
