#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h> 
#include "part3Tools.h"

//TODO: FREE MEMORY ACCORIDINGTO EXIT
//TODO: QUERY 4 IS MORE SIMPLE AND IS BASED ON PERSON KNOWS PERSON

Global_Utilities global_utilities;

int main(void) {

	int m=2,c=3,rc=0;
	int errorFlag=0;

	pthread_t sig_thr_id;

//////////////////////TERMINATOR THREAD INITIALISATION////////////////////////////////
	/* Creating thread to catch termination */
    /* This thread will be very useful for freeing memory properly and stopping the program when it is very time consuming to wait for it */
	sigemptyset(&(global_utilities.signal_mask));
	sigaddset(&(global_utilities.signal_mask), SIGINT);
	sigaddset(&(global_utilities.signal_mask), SIGTERM);
	rc = pthread_sigmask(SIG_BLOCK, &(global_utilities.signal_mask), NULL);

	if(rc != 0){
		perror("Main Thread: - Signal mask installation");
		exit(1);
	}

	rc = pthread_create(&sig_thr_id, NULL, signal_thread, NULL);
	if(rc != 0){
		perror("Main Thread: - pthread_create for signals");
		exit(1);
	}

	printf("Main Thread: Installed EXIT_HANDLER Successfully!\n");
////////////////////////////////////PART 3//////////////////////////////////
	if(initSharedDataStructs(m,c) == EXIT_VALUE){ //Initialises all shared resources
		printf("Main Thread: initSharedDataStructs failed! Exiting!\n");
		if(pthread_detach(sig_thr_id) != 0){ //Detach Exit Handler
			printf("Main Thread: Failed to Detach Exit Handler Thread!\n");
		}
		exit(1);
	}

	if(findTopNforums(10) == EXIT_VALUE){
		printf("Main Thread: findTopNforums failed! Exiting!\n");
		destroySharedDataStructs();
		if(pthread_detach(sig_thr_id) != 0){ //Detach Exit Handler
			printf("Main Thread: Failed to Detach Exit Handler Thread!\n");
		}
		exit(1);
	}

	int k=4;
	int i=0;

	printf("Main Thread: Now we will try to build %d-level cliques on these forums!\n",k);

	//Tests k-clique creation for all topNForums

	/*for(i=0; i < global_utilities.N; i++){
		List* cliquesList = (List*) NULL;
		cliquesList = cliqueBuild(global_utilities.topNForums[i].forumMembersGraph,k,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("Main Thread: cliqueBuild failed!\n");
			if(cliquesList != NULL){
				destroyList(cliquesList);
				free(cliquesList);
			}
			break;
		}

		printf("Main Thread: Forum - %d has the following %d-level Cliques!:\n",global_utilities.topNForums[i].forumID,k);
		if(cliquesList != NULL){
			if(cliquesList->items > 0)
				printList(*cliquesList);
			else
				printf("Main Thread: No %d-level Cliques could be created for this forum!\n",k);
			destroyList(cliquesList);
			free(cliquesList);
		}
		else{
			printf("Main Thread: No cliques Available?\n");
		}

	}*/

	Community* Results = (Community*) NULL;
	int j=0;
	int numberOfCommunities=0;
	printf("Main Thread: Time to test cliquePercolationMethod with k=%d!\n",k);
	do{
		printf("Main Thread: Type a number 0 to %d to pick the desired forum OR type -1 to EXIT!\n",global_utilities.N);
		scanf("%d",&i);
		if(i == -1){
			break;
		}
		if((i < 0) || (i >= global_utilities.N)){
			printf("Main Thread: Sorry invalid index!\n");
			continue;				
		}
		if(global_utilities.topNForums[i].forumID == -1){
			printf("Main Thread: Sorry this forum is empty!\n");
			continue;
		}

		Results = cliquePercolationMethod(k, global_utilities.topNForums[i].forumMembersGraph, &numberOfCommunities,&errorFlag);
		if(errorFlag == -1){
			continue;
		}
		if(errorFlag == EXIT_VALUE){
			printf("Main Thread: cliquePercolationMethod failed!\n");
			destroySharedDataStructs();
			if(pthread_detach(sig_thr_id) != 0){ //Detach Exit Handler
				printf("Main Thread: Failed to Detach Exit Handler Thread!\n");
			}
			if(Results != NULL){
				for(i=0; i < numberOfCommunities; i++){
					if(Results[i].members != NULL){
						free(Results[i].members);
					}
				}
				free(Results);
			}
			return 0;
		}

		for(i=0; i < numberOfCommunities; i++){
			printf("==========COMMUNITY: %d===========\n",i);
			if(Results[i].members == NULL){
				printf("Main Thread: This community has NULL members pointer!\n");
			}
			else{
				for(j=0; j < Results[i].numberOfMembers; j++){
					if(Results[i].numberOfMembers-1 == j){
						printf("|%d|\n",Results[i].members[j]);
					}
					else{
						printf("|%d|->",Results[i].members[j]);
					}
				}
				free(Results[i].members);
				printf("\n");
			}
		}
		free(Results);

	} while(1);

	printf("Main Thread: Time to test Girvan-Newman!\n");
	do{
		printf("Main Thread: Type a number 0 to %d to pick the desired forum OR type -1 to EXIT!\n",global_utilities.N);
		scanf("%d",&i);
		if(i == -1){
			break;
		}
		if((i < 0) || (i >= global_utilities.N)){
			printf("Main Thread: Sorry invalid index!\n");
			continue;				
		}
		if(global_utilities.topNForums[i].forumID == -1){
			printf("Main Thread: Sorry this forum is empty!\n");
			continue;
		}

		Results = GirvanNewman(DBL_MAX, global_utilities.topNForums[i],i,&numberOfCommunities,&errorFlag);
		if(errorFlag == -1){
			continue;
		}
		if(errorFlag == EXIT_VALUE){
			printf("Main Thread: GirvanNewman failed!\n");
			destroySharedDataStructs();
			if(pthread_detach(sig_thr_id) != 0){ //Detach Exit Handler
				printf("Main Thread: Failed to Detach Exit Handler Thread!\n");
			}
			if(Results != NULL){
				for(i=0; i < numberOfCommunities; i++){
					if(Results[i].members != NULL){
						free(Results[i].members);
					}
				}
				free(Results);
			}
			return 0;
		}

		for(i=0; i < numberOfCommunities; i++){
			printf("==========COMMUNITY: %d===========\n",i);
			if(Results[i].members == NULL){
				printf("Main Thread: This community has NULL members pointer!\n");
			}
			else{
				for(j=0; j < Results[i].numberOfMembers; j++){
					if(Results[i].numberOfMembers-1 == j){
						printf("|%d|\n",Results[i].members[j]);
					}
					else{
						printf("|%d|->",Results[i].members[j]);
					}
				}
				free(Results[i].members);
				printf("\n");
			}
		}
		free(Results);

	} while(1);

	destroySharedDataStructs();
///////////////////////////////////////////////////////////////////////////
	if(pthread_detach(sig_thr_id) != 0){ //Detach Exit Handler
		printf("Main Thread: Failed to Detach Exit Handler Thread!\n");
	}

	return 0;

}
