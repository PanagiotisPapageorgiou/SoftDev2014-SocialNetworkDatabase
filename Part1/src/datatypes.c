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
		exit(1);
	}

	(**temp) = (*temp2);
	//printf("Added Int: %d to List of Ints!\n", **temp);
}

int destroyInt(void** target){
	//printf("THIS IS INT DESTRUCTION!\n");
	int** temp = (int**) target;
	//printf("Deleting Int: %d from List of Ints!\n", **temp);
	if(temp != NULL)
		if((*temp) != NULL)
			free(*temp);
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
}

void printInt(void* target){
	int* temp = (int*) (target);
	printf("%d",*temp);
}

//////////////////////////////////////////CHAR */////////////////////////////////////////////////////////

/* Datatype functions for char* (used for GraphNode attributes */

int allocateString(void** target, void* source){

	char** temp = (char**) (target);
	char* temp2 = (char*) (source);

	(*temp) = malloc(sizeof(char)*(strlen(temp2)+1));
	strcpy(*temp, temp2);
	//printf("Added string: |%s| to List of Strings!\n", *temp);
}

int destroyString(void** target){
	//printf("THIS IS INT DESTRUCTION!\n");
	char** temp = (char**) target;
	//printf("Deleting Int: %d from List of Ints!\n", **temp);
	if(temp != NULL)
		if((*temp) != NULL)
			free(*temp);
}

int compareStrings(void* target, void* source){

	char* tempA = (char*) (target);
	char* tempB = (char*) (source);

	if((tempA != NULL) && (tempB != NULL))
		return strcmp(tempA,tempB);
	else{
		printf("compareStrings: NULL strings comparison!\n");
		return -2;
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
			tempA = NULL;
			printf("assignString: WARNING assigned NULL string!\n");
		}

	}
	else{		
		tempA = malloc(sizeof(char) * (strlen(tempB) + 1));
		strcpy(tempA,tempB);
	}

}

void printString(void* target){

	char* temp = (char*) (target);
	printf("%s",temp);

}

////////////////////////////////////////////////RESULTS SET PAIR ////////////////////////////////////////
/* ResultSetPair functions */

int allocateResultPair(void** target, void* source){

	Pair** tempA = (Pair**) (target);
	Pair* tempB = (Pair*) (source);
	
	(*tempA) = malloc(sizeof(Pair));
	(*tempA)->ID = tempB->ID;
	(*tempA)->distance = tempB->distance;

	//printf("Added ResultSetInfo with ID: %d to List of ResultSetInfo!\n", (*tempA)->id);

}

int destroyResultPair(void** target){

	Pair** temp = (Pair**) (target);
	//printf("Deleting ResultSetInfo with ID: %d from List of ResultSetInfo!\n", (*temp)->id);
	if(temp != NULL)
		if(*temp != NULL)
			free(*temp);

}

int compareResultPair(void* target, void* source){

	Pair* tempA = (Pair*) (target);
	Pair* tempB = (Pair*) (source);
	if((tempA->ID == tempB->ID) && (tempA->distance == tempB->distance)) return 0;
	else return 1;

}

int assignResultPair(void* target,void* source){

	Pair* tempA = (Pair*) (target);
	Pair* tempB = (Pair*) (source);

	if((tempA != NULL) && (tempB != NULL)){
		tempA->ID = tempB->ID;
		tempA->distance = tempB->distance;
	}
	else{
		printf("assingResultPair: Result Pair pointer is NULL!\n");
		exit(1);
	}

}


void printResultPair(void* target){

	Pair* tempA = (Pair*) target;
	//printf("ID: %d - Distance: %d\n",tempA->id,tempA->distance);
	if(tempA == NULL){
		printf("printResultPair: Result Pair pointer is NULL!\n");
		exit(1);
	}

	printf("(%d,%d)",tempA->ID,tempA->distance);

}

////////////////////////////////////////////GRAPH NODE Ptr///////////////////////////////////////////////////////////
/* GraphNodePtr functions for List */

int allocateGraphNodePtr(void** target, void* source){ /* Allocation takes place outside */

	GraphNode** tempA = (GraphNode**) (target);
	GraphNode* tempB = (GraphNode*) (source);
	
	(*tempA) = tempB;

}

int destroyGraphNodePtr(void** target){ /* De-allocation takes place outside */

	GraphNode** temp = (GraphNode**) (target);
	//printf("Deleting GraphNode with ID: %d from List of GraphNodes!\n", (*temp)->id);
	*temp = NULL;

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
					if(!strcmp(propA->arrayP[i].type, "string")){
						if(strcmp( (char*) propA->arrayP[i].attribute, (char*) propB->arrayP[i].attribute) ){
							return -1;
						}
					}
					else if (!strcmp(propA->arrayP[i].type, "int")){
						int* p1 = (int*) (propA->arrayP[i].attribute);
						int* p2 = (int*) (propB->arrayP[i].attribute);

						if(*p1 != *p2) return -1;

					}
					else if (!strcmp(propA->arrayP[i].type, "float")){
						float* p3 = (float*) (propA->arrayP[i].attribute);
						float* p4 = (float*) (propB->arrayP[i].attribute);

						if(*p3 != *p4) return -1;
					}
					else if (!strcmp(propA->arrayP[i].type, "double")){
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
		exit(1);	
	}

}

int compareGraphNodesPtr(void* target, void* source){ /* NOTE: This only compares the IDs (might be insufficient */

	GraphNode* tempA = (GraphNode*) (target);
	GraphNode* tempB = (GraphNode*) (source);
	if((tempA == NULL) || (tempB == NULL)){
		printf("compareGraphNodesPtr: GraphNode comparison cannot occur with NULL pointers!\n");
		return -1;
	}
	if((tempA->id == tempB->id)) return 0;
	else if((tempA->id < tempB->id)) return 1;
	else return -1;
	
}

int assignGraphNodePtr(void* target,void* source){ //TODO: NOT READY, REMEMBER TO ALSO ASSIGN LISTS

	GraphNode* tempA = (GraphNode*) (target);
	GraphNode* tempB = (GraphNode*) (source);

	printf("assignGraphNodePtr: WARNING assignGraphNodePtr is under construction\nEdge assignment is not ready yet!\n");
	exit(1);

}

void printGraphNodePtr(void* target){

	GraphNode* tempA = (GraphNode*) target;

	if(tempA != NULL)
		printf("%d",tempA->id);
	else{
		printf("\nGraphNode pointer is NULL, can't print!\n");
	}

}


////////////////////////////////////GRAPH NODE EDGE PTR///////////////////////////////////

/* TODO: In the future in order to represent multiple e.g (1,2) edges but with different relations we can use a list */

int allocateEdgePtr(void** target, void* source){ /* Allocation takes place outside */

	Edge** tempA = (Edge**) (target);
	Edge* tempB = (Edge*) (source);
	
	(*tempA) = tempB;

}

int destroyEdgePtr(void** target){ /* De-allocation takes place outside */

	Edge** temp = (Edge**) (target);
	//printf("Deleting GraphNode with ID: %d from List of GraphNodes!\n", (*temp)->id);
	*temp = NULL;

}

int compareEdgePtr(void* target, void* source){ /* NOTE: This only compares the IDs (might be insufficient */

	Edge* tempA = (Edge*) (target);
	Edge* tempB = (Edge*) (source);
	if((tempA == NULL) || (tempB == NULL)){
		printf("compareEdgePtr: Edge comparison cannot occur with NULL pointers!\n");
		return -1;
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

	printf("assignEdgePtr: WARNING assignEdgePtr is under construction\nEdge assignment is not ready yet!\n");
	exit(1);

}

void printEdgePtr(void* target){

	int* tempInt;

	Edge* tempA = (Edge*) target;

	if(tempA != NULL){
		tempInt = (int*) tempA->properties->arrayP[1].attribute;
		printf("|sID: %d-eID: %d-T: %s-W: %d\n|",tempA->startNodeID,tempA->endNodeID,(char*) tempA->properties->arrayP[0].attribute, *tempInt);
	}
	else{
		printf("\nEdge pointer is NULL, can't print!\n");
	}

}

/* EdgeSet functions for List */
int allocateEdgeSet(void** new_edge_set, void* relation){

	EdgeSet** tempEdgeSetPtr = (EdgeSet**) new_edge_set;
	char* relationTemp = (char*) relation;

	*tempEdgeSetPtr = malloc(sizeof(EdgeSet));
	if(*tempEdgeSetPtr == NULL){
		printf("allocateEdgeSet: Could not allocate memory for EdgeSet!\n");
		exit(1);
	}

	(*tempEdgeSetPtr)->relation = malloc(sizeof(char) * (strlen(relationTemp) + 1));
	if( (*tempEdgeSetPtr)->relation == NULL){
		printf("allocateEdgeSet: Could not allocate memory for relation of EdgeSet!\n");
		exit(1);
	}
	strcpy( (*tempEdgeSetPtr)->relation, relationTemp);

	createList(&( (*tempEdgeSetPtr)->edges),"EdgePtr",&allocateEdgePtr,&destroyEdgePtr,&compareEdgePtr,&assignEdgePtr,&printEdgePtr);

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

}

int compareEdgeSet(void* target, void* source){

	printf("compareEdgeSet: NOT READY!\n");
	exit(1);

}

int assignEdgeSet(void* target,void* source){

	printf("assignEdgeSet: NOT READY!\n");
	exit(1);

}

void printEdgeSet(void* EdgeSetPtr){

	EdgeSet* tempEdgeSetPtr = (EdgeSet*) EdgeSetPtr;

	printf("printEdgeSet: Let's print this EdgeSet!\n");

	if(tempEdgeSetPtr == NULL){
		printf("printEdgeSet: This EdgeSet is NULL!\n");
		exit(1);
	}

	if(tempEdgeSetPtr->relation == NULL){
		printf("printEdgeSet: This EdgeSet's relation is NULL!\n");
		exit(1);
	}

	printf("Relation Type: %s\n",tempEdgeSetPtr->relation);
	printf("List of Edges: ");
	printList(tempEdgeSetPtr->edges);
	printf("\n\n");


}
