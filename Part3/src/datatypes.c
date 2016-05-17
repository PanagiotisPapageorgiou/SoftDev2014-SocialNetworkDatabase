#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: datatypes.c
 MAIN AUTHOR: PANAGIOTIS PAPAGEORGIOU														

 Details: Used to equip the Generic List with any Datatype we want. Effectively allows to have 
 any List we want, all we have to do is write separate functions for allocation, destruction, 
 comparison, assignment and print for the data of the Node. To understand the format of the
 function written below note the following:

 - void** target - translates to (Node of the Generic List -> data). This data field in the
 Node of the Generic List is void* and to allocate memory on it we call these functions with
 &data, so void** is formed.
 
 - void* source - translates to: Whatever you want to send from outside in order to initialise
 the target. Depends on the datatype you are using the List on.

 - Memory allocation/de-allocation : The Generic List by itself will only take care of the
 memory resources used to allocate its Nodes. It will not and cannot free the memory resource
 of the void* data of a node. The responsibility for this falls on us to write proper
 destroyNode functions aka destroyInt below and provide them in createList( ) in order
 for the List to know them and call them while destroying a Node.

 The policy also used to allocate/free the data of a Node is not the same everywhere.
 In allocateInt we allocate a copy of the int given as source and we free that copy in
 destroyInt respectively.
 On the contrary, in allocateGraphNodePtr the GraphNode has been allocated outside the List
 and will be de-allocated also 

================================================================================================*/


////////////////////////////////////////////////INTEGER ///////////////////
/* Int functions for List */

int allocateInt(void** target, void* source){ /* Target is the void* data of the current Node in the uberlist, source is whatever you may wish to use for initialisation */
	//printf("THIS IS INT ALLOCATION!\n");
	int** temp = (int**) (target); /* We need to cast the pointers before we allocate */
	int* temp2 = (int*) (source);

	(*temp) = malloc(sizeof(int)); /* Allocated the new int */
	if(*temp == NULL){
		printf("allocateInt: Failed to allocate memory for Int!\n");
		printf("allocateInt: Sending EXIT Value!\n");
		return EXIT_VALUE;
	}

	(**temp) = (*temp2);
	//printf("Added Int: %d to List of Ints!\n", **temp);

	return 0;

}

int destroyInt(void** target){
	//printf("THIS IS INT DESTRUCTION!\n");
	int** temp = (int**) target;
	//printf("Deleting Int: %d from List of Ints!\n", **temp);
	if(temp != NULL)
		if((*temp) != NULL)
			free(*temp);

	return 0;

}

int compareInts(void* target, void* source){
	int* tempA = (int*) (target);
	int* tempB = (int*) (source);
	if((*tempA) == (*tempB)) return 0;
	if((*tempA) < (*tempB)) return 1;
	if((*tempA) > (*tempB)) return -1;
}

int assignInt(void* target, void* source){
	int* tempA = (int*) target;
	int* tempB = (int*) source;
	*tempA = *tempB;

	return 0;

}

int printInt(void* target){
	int* temp = (int*) (target);
	printf("%d",*temp);

	return 0;

}

//////////////////////////////////////////CHAR */////////////////////////////////////////////////////////

/* Datatype functions for char* (used for GraphNode attributes */

int allocateString(void** target, void* source){

	char** temp = (char**) (target);
	char* temp2 = (char*) (source);

	(*temp) = malloc(sizeof(char)*(strlen(temp2)+1));
	strcpy(*temp, temp2);
	//printf("Added string: |%s| to List of Strings!\n", *temp);

	return 0;

}

int destroyString(void** target){
	//printf("THIS IS INT DESTRUCTION!\n");
	char** temp = (char**) target;
	//printf("Deleting Int: %d from List of Ints!\n", **temp);
	if(temp != NULL)
		if((*temp) != NULL)
			free(*temp);

	return 0;

}

int compareStrings(void* target, void* source){

	char* tempA = (char*) (target);
	char* tempB = (char*) (source);

	if((tempA != NULL) && (tempB != NULL))
		return strcmp(tempA,tempB);
	else{
		printf("compareStrings: NULL strings comparison!\n");
		return EXIT_VALUE;
	}

}

int assignString(void* target, void* source){

	char* tempA = (char*) target;
	char* tempB = (char*) source;
	
	if(tempA != NULL){

		free(tempA);
		if(tempB != NULL){
			tempA = malloc(sizeof(char) * (strlen(tempB) + 1));
			strcpy(tempA,tempB);
		}
		else{ 
			tempA = (char*) NULL;
			printf("assignString: WARNING assigned NULL string!\n");
		}

	}
	else{		
		tempA = malloc(sizeof(char) * (strlen(tempB) + 1));
		strcpy(tempA,tempB);
	}

}

int printString(void* target){

	char* temp = (char*) (target);
	printf("%s",temp);

	return 0;

}

////////////////////////////////////////////////RESULTS SET PAIR ////////////////////////////////////////
/* ResultSetPair functions */

int allocateResultPair(void** target, void* source){

	Pair** tempA = (Pair**) (target);
	Pair* tempB = (Pair*) (source);
	
	(*tempA) = malloc(sizeof(Pair));
	if((*tempA) == NULL){
		printf("allocateResultPair: Failed to allocate memory for Pair! Sending EXIT Value!\n");
		return EXIT_VALUE;
	}

	(*tempA)->ID = tempB->ID;
	(*tempA)->distance = tempB->distance;

	//printf("Added ResultSetInfo with ID: %d to List of ResultSetInfo!\n", (*tempA)->id);

	return 0;

}

int destroyResultPair(void** target){

	Pair** temp = (Pair**) (target);
	//printf("Deleting ResultSetInfo with ID: %d from List of ResultSetInfo!\n", (*temp)->id);
	if(temp != NULL)
		if(*temp != NULL)
			free(*temp);

	return 0;

}

int compareResultPair(void* target, void* source){

	Pair* tempA = (Pair*) (target);
	Pair* tempB = (Pair*) (source);

	if((tempA == NULL) || (tempB == NULL)){
		printf("compareResultPair: NULL ResultPair Pointers given!\nSending EXIT value!\n");
		return EXIT_VALUE;
	}
	if((tempA->ID == tempB->ID) && (tempA->distance == tempB->distance)) return 0;
	else return 1;

}

int assignResultPair(void* target,void* source){

	Pair* tempA = (Pair*) (target);
	Pair* tempB = (Pair*) (source);

	if((tempA != NULL) && (tempB != NULL)){
		tempA->ID = tempB->ID;
		tempA->distance = tempB->distance;
	
		return 0;
	}
	else{
		printf("assignResultPair: Result Pair pointer is NULL! Sending EXIT Value!\n");
		return EXIT_VALUE;
	}

}


int printResultPair(void* target){

	Pair* tempA = (Pair*) target;
	//printf("ID: %d - Distance: %d\n",tempA->id,tempA->distance);
	if(tempA == NULL){
		printf("printResultPair: Result Pair pointer is NULL! Sending EXIT Value!\n");
		return EXIT_VALUE;
	}

	printf("(%d,%d)",tempA->ID,tempA->distance);

	return 0;

}

////////////////////////////////////////////GRAPH NODE Ptr///////////////////////////////////////////////////////////
/* GraphNodePtr functions for List */

int allocateGraphNodePtr(void** target, void* source){ /* Allocation takes place outside */

	GraphNode** tempA = (GraphNode**) (target);
	GraphNode* tempB = (GraphNode*) (source);
	
	(*tempA) = tempB;

	return 0;

}

int destroyGraphNodePtr(void** target){ /* De-allocation takes place outside */

	GraphNode** temp = (GraphNode**) (target);
	//printf("Deleting GraphNode with ID: %d from List of GraphNodes!\n", (*temp)->id);
	*temp = (GraphNode*) NULL;

	return 0;

}

int compareProperties(Properties* propA, Properties* propB){

	int i=0;

	if( (propA != NULL) && (propB != NULL) ){
		if(propA->num_properties == propB->num_properties){
			for(i=0; i < propA->num_properties; i++){
				if(propA->arrayP[i].type != propB->arrayP[i].type){
					return -1;
				}
				else{
					if(propA->arrayP[i].type == STRING){
						if(strcmp( (char*) propA->arrayP[i].attribute, (char*) propB->arrayP[i].attribute) ){
							return -1;
						}
					}
					else if (propA->arrayP[i].type == INT){
						int* p1 = (int*) (propA->arrayP[i].attribute);
						int* p2 = (int*) (propB->arrayP[i].attribute);

						if(*p1 != *p2) return -1;

					}
					else if (propA->arrayP[i].type == FLOAT){
						float* p3 = (float*) (propA->arrayP[i].attribute);
						float* p4 = (float*) (propB->arrayP[i].attribute);

						if(*p3 != *p4) return -1;
					}
					else if (propA->arrayP[i].type == DOUBLE){
						double* p5 = (double*) (propA->arrayP[i].attribute);
						double* p6 = (double*) (propB->arrayP[i].attribute);

						if(*p5 != *p6) return -1;					
					}
					else
						return -1;
				}
			}
			return 0;
		}
		else return -1;
	}
	else{
		printf("compareProperties: Can't compare NULL properties!\n");
		printf("compareProperties: Sending EXIT Value!\n");
		return EXIT_VALUE;	
	}

}

int compareGraphNodesPtr(void* target, void* source){ /* NOTE: This only compares the IDs (might be insufficient */

	GraphNode* tempA = (GraphNode*) (target);
	GraphNode* tempB = (GraphNode*) (source);
	if((tempA == NULL) || (tempB == NULL)){
		printf("compareGraphNodesPtr: GraphNode comparison cannot occur with NULL pointers!\n");
		printf("compareGraphNodesPtr: Sending EXIT Value!\n");
		return EXIT_VALUE;
	}
	if((tempA->id == tempB->id)) return 0;
	else if((tempA->id < tempB->id)) return 1;
	else return -1;
	
}

int assignGraphNodePtr(void* target,void* source){ //TODO: NOT READY, REMEMBER TO ALSO ASSIGN LISTS

	GraphNode* tempA = (GraphNode*) (target);
	GraphNode* tempB = (GraphNode*) (source);

	printf("assignGraphNodePtr: WARNING assignGraphNodePtr is under construction\nEdge assignment is not ready yet!\nSending EXIT Value!\n");
	return EXIT_VALUE;

}

int printGraphNodePtr(void* target){

	GraphNode* tempA = (GraphNode*) target;

	if(tempA != NULL)
		printf("%d",tempA->id);
	else{
		printf("\nGraphNode pointer is NULL, can't print!\n");
		printf("\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}
	
	return 0;

}


////////////////////////////////////GRAPH NODE EDGE PTR///////////////////////////////////

/* TODO: In the future in order to represent multiple e.g (1,2) edges but with different relations we can use a list */

int allocateEdgePtr(void** target, void* source){ /* Allocation takes place outside */

	Edge** tempA = (Edge**) (target);
	Edge* tempB = (Edge*) (source);
	
	(*tempA) = tempB;

	return 0;

}

int destroyEdgePtr(void** target){ /* De-allocation takes place outside */

	Edge** temp = (Edge**) (target);
	//printf("Deleting GraphNode with ID: %d from List of GraphNodes!\n", (*temp)->id);
	*temp = (Edge*) NULL;

	return 0;

}

int compareEdgePtr(void* target, void* source){ /* NOTE: This only compares the IDs (might be insufficient */

	Edge* tempA = (Edge*) (target);
	Edge* tempB = (Edge*) (source);
	if((tempA == NULL) || (tempB == NULL)){
		printf("compareEdgePtr: Edge comparison cannot occur with NULL pointers!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}

	if( (tempA->startNodeID == tempB->startNodeID) && (tempA->endNodeID == tempB->endNodeID) && (compareProperties(tempA->properties, tempB->properties) != -1) ){
		return 0;
	}
	else
		return -1;
	
}

int assignEdgePtr(void* target,void* source){ //TODO: NOT READY, REMEMBER TO ALSO ASSIGN LISTS

	Edge* tempA = (Edge*) (target);
	Edge* tempB = (Edge*) (source);

	printf("assignEdgePtr: WARNING assignEdgePtr is under construction\nEdge assignment is not ready yet!\nSending EXIT Value");
	return EXIT_VALUE;

}

int printEdgePtr(void* target){

	int* tempInt;

	Edge* tempA = (Edge*) target;

	if(tempA != NULL){
		tempInt = (int*) tempA->properties->arrayP[1].attribute;
		printf("|sID: %d-eID: %d-T: %s-W: %d\n|",tempA->startNodeID,tempA->endNodeID,(char*) tempA->properties->arrayP[0].attribute, *tempInt);

		return 0;
	}
	else{
		printf("\nprintEdgePtr: Edge pointer is NULL, can't print!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}

}

/* EdgeSet functions for List */
int allocateEdgeSet(void** new_edge_set, void* relation){

	EdgeSet** tempEdgeSetPtr = (EdgeSet**) new_edge_set;
	char* relationTemp = (char*) relation;

	*tempEdgeSetPtr = malloc(sizeof(EdgeSet));
	if(*tempEdgeSetPtr == NULL){
		printf("allocateEdgeSet: Could not allocate memory for EdgeSet!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}

	(*tempEdgeSetPtr)->relation = malloc(sizeof(char) * (strlen(relationTemp) + 1));
	if( (*tempEdgeSetPtr)->relation == NULL){
		printf("allocateEdgeSet: Could not allocate memory for relation of EdgeSet!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}
	strcpy( (*tempEdgeSetPtr)->relation, relationTemp);

	createList(&( (*tempEdgeSetPtr)->edges),"EdgePtr",&allocateEdgePtr,&destroyEdgePtr,&compareEdgePtr,&assignEdgePtr,&printEdgePtr);

	return 0;
}

int destroyEdgeSet(void** edge_set_ptr){

	EdgeSet** tempEdgeSetPtr = (EdgeSet**) edge_set_ptr;

	if(tempEdgeSetPtr != NULL){
		if(*tempEdgeSetPtr != NULL){
			destroyList(& ((*tempEdgeSetPtr)->edges));
			if((*tempEdgeSetPtr)->relation != NULL)
				free((*tempEdgeSetPtr)->relation);
			free(*tempEdgeSetPtr);
		}
	}

	return 0;

}

int compareEdgeSet(void* target, void* source){

	printf("compareEdgeSet: NOT READY!\nSending EXIT Value!\n");
	return EXIT_VALUE;
}

int assignEdgeSet(void* target,void* source){

	printf("assignEdgeSet: NOT READY!\nSending EXIT Value!\n");
	return EXIT_VALUE;

}

int printEdgeSet(void* EdgeSetPtr){

	EdgeSet* tempEdgeSetPtr = (EdgeSet*) EdgeSetPtr;

	printf("printEdgeSet: Let's print this EdgeSet!\n");

	if(tempEdgeSetPtr == NULL){
		printf("printEdgeSet: This EdgeSet is NULL!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}

	if(tempEdgeSetPtr->relation == NULL){
		printf("printEdgeSet: This EdgeSet's relation is NULL!\nSending EXIT Value!\n");
		return EXIT_VALUE;
	}

	printf("Relation Type: %s\n",tempEdgeSetPtr->relation);
	printf("List of Edges: ");
	printList(tempEdgeSetPtr->edges);
	printf("\n\n");

	return 0;
}

////////////////////////////////////////////////MATCH ////////////////////////////////////////
/* Matches functions */

int allocateMatch(void** target, void* source){

	Match** tempA = (Match**) (target);
	Match* tempB = (Match*) (source);
	
	(*tempA) = malloc(sizeof(Match));
	(*tempA)->ID = tempB->ID;
	(*tempA)->similarity_score = tempB->similarity_score;

	return 0;

}

int destroyMatch(void** target){

	Match** temp = (Match**) (target);
	printf("Deleting Match with ID: %d from List of Matches!\n", (*temp)->ID);
	if(temp != NULL)
		if(*temp != NULL)
			free(*temp);

	return 0;

}

int compareMatch(void* target, void* source){

	Match* tempA = (Match*) (target);
	Match* tempB = (Match*) (source);
	if((tempA->ID == tempB->ID) && (tempA->similarity_score == tempB->similarity_score)) return 0;
	else return 1;

}

int assignMatch(void* target,void* source){

	Match* tempA = (Match*) (target);
	Match* tempB = (Match*) (source);

	if((tempA != NULL) && (tempB != NULL)){
		tempA->ID = tempB->ID;
		tempA->similarity_score = tempB->similarity_score;
	}
	else{
		printf("assignMatch:Match pointer is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;

}


int printMatch(void* target){

	Match* tempA = (Match*) target;
	//printf("ID: %d - Similarity Score: %d\n",tempA->id,tempA->similarity_score);
	if(tempA == NULL){
		printf("printMatch: Match pointer is NULL!\n");
		return EXIT_VALUE;
	}

	printf("(%d,%.2f)",tempA->ID,tempA->similarity_score);

	return 0;

}

///////////////////////////////////////ARGUMENT ///////////////////////////////////////

/* Argument functions for List */
int allocateArgumentPtr(void** target, void* source){

	Argument** newArgument = (Argument**) target;
	Argument* sourceArgument = (Argument*) source;

	if(newArgument == NULL){
		printf("allocateArgumentPtr: newArgument Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	*newArgument = malloc(sizeof(Argument));
	if(*newArgument == NULL){
		printf("allocateArgumentPtr: Failed to allocate memory for Argument! - Exiting!\n");
		return EXIT_VALUE;
	}

	(*newArgument)->type = sourceArgument->type;
	if((*newArgument)->type == INT){
	 	(*newArgument)->value = malloc(sizeof(int));
		int* intAptr = (*newArgument)->value;
		int* intBptr = sourceArgument->value;
		if(sourceArgument->value == NULL){
			printf("allocateArgumentPtr: Value of source Argument is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
		*intAptr = *intBptr;
	}
	else{
		printf("allocateArgumentPtr: No other argument types ready yet! - Exiting!\n");
		return EXIT_VALUE;
	}

}

int destroyArgumentPtr(void** target){

	Argument** targetArgument = (Argument**) target;

	if(targetArgument != NULL){
		if(*targetArgument != NULL){
			if((*targetArgument)->value != NULL){
				free((*targetArgument)->value);
			}
			free(*targetArgument);
		}
	}

	return 0;

}

int printArgumentPtr(void* target){

	printf("printArgumentPtr: Error printArgumentPtr is NOT READY!\n");

	exit(1);
	/*Argument* targetJob = (Argument*) target;

	if(targetArgument != NULL){
		printf("|JOB: id: %d|-",targetJob->jobID);
	}
	else{
		printf("printJobPtr: Job Pointer is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;*/
}

///////////////////////////////////////JOB ///////////////////////////////////////

/* Job functions for List */
int allocateJobPtr(void** target, void* source){

	Job** newJob = (Job**) target;
	Job* sourceJob = (Job*) source;

	if(newJob == NULL){
		printf("allocateJobPtr: newJob Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	*newJob = malloc(sizeof(Job));
	if(*newJob == NULL){
		printf("allocateJobPtr: Failed to allocate memory for Job! - Exiting!\n");
		return EXIT_VALUE;
	}

	(*newJob)->jobID = sourceJob->jobID;
	(*newJob)->priority = sourceJob->priority;
	(*newJob)->type = sourceJob->type;
	(*newJob)->status = sourceJob->status;
	createList(&((*newJob)->argumentList),"argumentList",&allocateArgumentPtr,&destroyArgumentPtr,NULL,NULL,&printArgumentPtr);
	appendLists(&((*newJob)->argumentList),&(sourceJob->argumentList));

	return 0;

}

int destroyJobPtr(void** target){

	Job** targetJob = (Job**) target;

	if(targetJob != NULL){
		if(*targetJob != NULL){
			destroyList(&((*targetJob)->argumentList));
			free(*targetJob);
		}
	}

	return 0;

}

int printJobPtr(void* target){

	Job* targetJob = (Job*) target;

	if(targetJob != NULL){
		printf("|JOB: id: %ld|-",targetJob->jobID);
	}
	else{
		printf("printJobPtr: Job Pointer is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;
}

/* List functions for List (List used as datatype in List!) */
int allocateListPtr(void** target, void* source){ //For when we want a List to carry a List in each Node

	List** targetList = (List**) target;
	List* sourceList = (List*) source;

	if(targetList == NULL){
		printf("allocateListPtr: Error targetList pointer is Null!\n");
		exit(1);
	}

	*targetList = malloc(sizeof(List)); //This new List must know what kind of List it will be,so it sees the source argument to determine
	if(*targetList == NULL){
		printf("allocateListPtr: failed to allocate memory!\n");
		exit(1);
	}
	if(!strcmp(sourceList->type_name,"ListOfLists")){
		*targetList = sourceList;
	}
	createList(*targetList,sourceList->type_name,sourceList->allocateNode,sourceList->destroyNode,sourceList->compareNode,sourceList->assignNode,sourceList->printNode);
	appendLists(*targetList,sourceList); //Copy all records of List 2 to List1

	return 0;

}

int destroyListPtr(void** target){

	List** targetList = (List**) target;

	if(targetList != NULL){
		if(*targetList != NULL){
			destroyList(*targetList);
			free(*targetList);
			*targetList = NULL;
		}
	}

	return 0;

}

int printListPtr(void* target){

	List* targetList = (List*) target;

	if(targetList == NULL){
		printf("printListPtr: targetList pointer is NULL!\n");
		return EXIT_VALUE;
	}

	printf("List of Lists: (");
	printList(*targetList);
	printf(")\n");

	return 0;

}
