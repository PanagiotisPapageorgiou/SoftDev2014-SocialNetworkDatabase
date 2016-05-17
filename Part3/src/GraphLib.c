/*
 * GraphLib.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "gtools.h"
#include "defines.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: GraphLib.c
 MAIN AUTHOR: All of us														

 Details: Mainly all the Part 1 functions and some tools.

================================================================================================*/

//////////////////////////////////////////////PROPERTIES FUNCTIONS/////////////////////////////////////////

/**
 * Creates a properties object
 * number: the number of properties
 * return value: a properties object
 */
Properties* createProperties(int number,int* errorFlag) {

	int i=0;

	Properties* properties;
	properties = malloc(sizeof(Properties));
	
	if(properties == NULL){
		printf("createProperties: Failed to allocate memory for property set! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Properties*) (NULL));
	}
	
	properties->num_properties = number;
	properties->arrayP = malloc(sizeof(Property) * number);

	if(properties->arrayP == NULL){
		printf("createProperties: Failed to allocate memory for property array! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Properties*) (NULL));
	}

	for(i=0; i<number; i++){
		properties->arrayP[i].attribute = NULL;
		properties->arrayP[i].type = EMPTY;
	}

    return properties;

}

/**
 * Sets a string property "property" in position "index"
 * property: the type of the property
 * index: the position of the property
 * p: the properties object
 */
int setStringProperty(char* property, int index, Properties* p) {

	int i=0;
	char** attributePtr;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setStringProperty: Properties is not properly initialised (1)! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(p->arrayP == NULL){
			printf("setStringProperty: Properties is not properly initialised (2)! - Exiting!\n");
			return EXIT_VALUE;
		}
		
		if((p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == EMPTY) ){
			
			attributePtr = (char**) (&(p->arrayP[index].attribute));

			*attributePtr = malloc(sizeof(char) * (strlen(property) + 1) );
			if(*attributePtr == NULL){
				printf("setStringProperty: Could not allocate memory for char* attribute! - Exiting!\n");
				return EXIT_VALUE;
			}

			strcpy(*attributePtr, property);
			p->arrayP[index].type = STRING;

		}
		else{ //TODO: Support assignment?
			/*if(p->arrayP[index]->type != NULL){
				if(!strcmp(p->arrayP[index]->type, "string")){
			}*/
			printf("setStringProperty: Currently works for initialisation only! Assignment not supported! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("setStringProperty: Properties set is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

/**
 * Sets an integer property "property" in position "index"
 * property: the value of the property
 * index: the position of the property
 * p: the properties object
 */
int setIntegerProperty(int property, int index, Properties* p) {
    //TODO
	int** attributePtr;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setIntegerProperty: Properties is not properly initialised (1)! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(p->arrayP == NULL){
			printf("setIntegerProperty: Properties is not properly initialised (2)! - Exiting!\n");
			return EXIT_VALUE;
		}
		
		if( (p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == EMPTY) ){
			
			attributePtr = (int**) (&(p->arrayP[index].attribute));

			*attributePtr = malloc(sizeof(int));

			if(*attributePtr == NULL){
				printf("setIntegerProperty: Could not allocate memory for int attribute! - Exiting!\n");
				return EXIT_VALUE;
			}

			**attributePtr = property; /* Assignment */
			p->arrayP[index].type = INT;

		}
		else{ //TODO: Support assignment?
			/*if(p->arrayP[index]->type != NULL){
				if(!strcmp(p->arrayP[index]->type, "string")){
			}*/
			printf("setIntegerProperty: Currently works for initialisation only! Assignment not supported! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("setIntegerProperty: Properties set is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

int setDoubleProperty(double property, int index, Properties* p) {
    //TODO
	double** attributePtr;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setDoubleProperty: Properties is not properly initialised (1)! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(p->arrayP == NULL){
			printf("setDoubleProperty: Properties is not properly initialised (2)! - Exiting!\n");
			return EXIT_VALUE;
		}
		
		if( (p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == EMPTY) ){
			
			attributePtr = (double**) (&(p->arrayP[index].attribute));

			*attributePtr = malloc(sizeof(double));

			if(*attributePtr == NULL){
				printf("setDoubleProperty: Could not allocate memory for double attribute! - Exiting!\n");
				return EXIT_VALUE;
			}

			**attributePtr = property; /* Assignment */
			p->arrayP[index].type = DOUBLE;

		}
		else{ //TODO: Support assignment?
			/*if(p->arrayP[index]->type != NULL){
				if(!strcmp(p->arrayP[index]->type, "string")){
			}*/
			printf("setDoubleProperty: Currently works for initialisation only! Assignment not supported! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("setDoubleProperty: Properties set is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

/**
 * Gets a string property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the type of the property
 */
char* getStringProperty(int index, Properties* p,int* errorFlag) {
    //TODO

	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != EMPTY) ){
				if(p->arrayP[index].type == STRING){
					return ((char*) (p->arrayP[index].attribute));
				}
				else{
					printf("getStringProperty: There is no string attribute in the specified index! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return ((char*) (NULL));
				}
			}
			else{
				printf("getStringProperty: The specified index has NULL attribute or type! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return ((char*) (NULL));
			}
		}
		else{
			printf("getStringProperty: Array of properties if NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((char*) (NULL));
		}
	}
	else{
		printf("getStringProperty: Properties pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((char*) (NULL));
	}
    
}

/**
 * Gets an integer property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the value of the property
 */
int getIntegerProperty(int index, Properties* p,int* errorFlag) {

	int* numberptr;

	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != EMPTY) ){
				if(p->arrayP[index].type == INT){
					numberptr = (int*) (p->arrayP[index].attribute);
					*errorFlag = EXIT_VALUE;
					return (*numberptr);
				}
				else{
					printf("getIntegerProperty: There is no int attribute in the specified index! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}
			}
			else{
				printf("getIntegerProperty: The specified index attribute or type is NULL! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
		}
		else{
			printf("getIntegerProperty: Array of properties if NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
	}
	else{
		printf("getIntegerProperty: Properties pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

}

double getDoubleProperty(int index, Properties* p,int* errorFlag) {

	double* numberptr;

	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != EMPTY) ){
				if(p->arrayP[index].type == DOUBLE){
					numberptr = (double*) (p->arrayP[index].attribute);
					return (*numberptr);
				}
				else{
					printf("getDoubleProperty: There is no double attribute in the specified index!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}
			}
			else{
				printf("getDoubleProperty: The specified index attribute or type is NULL!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
		}
		else{
			printf("getDoubleProperty: Array of properties if NULL!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
	}
	else{
		printf("getDoubleProperty: Properties pointer is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

}

int setListProperty(List* property,int index,Properties* p) { //TODO

	List** attributePtr = (List**) NULL;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setListProperty: Properties is not properly initialised (1)! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(p->arrayP == NULL){
			printf("setListProperty: Properties is not properly initialised (2)! - Exiting!\n");
			return EXIT_VALUE;
		}
		
		if(property == NULL){
			printf("setListProperty: Property List is NULL!\n");
			return EXIT_VALUE;
		}

		if( (p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == EMPTY) ){
			
			attributePtr = (List**) (&(p->arrayP[index].attribute));
			
			*attributePtr = malloc(sizeof(List));
			if(*attributePtr == NULL){
				printf("setListProperty: Failed to allocate memory for List property!\n");
			}

			createList(*attributePtr,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); 	

			if(*attributePtr == NULL){
				printf("setListProperty: Could not allocate memory for List pointer attribute! - Exiting!\n");
				return EXIT_VALUE;
			}
			appendLists(*attributePtr,property);
			p->arrayP[index].type = LIST;
		}
		else{
			printf("setListProperty: Currently works for initialisation only! Assignment not supported! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("setListProperty: Properties set is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

List* getListProperty(int index,Properties* p,int* errorFlag) {

    //TODO
	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != EMPTY) ){
				if(p->arrayP[index].type == LIST){
					*errorFlag = EXIT_VALUE;
					return ((List*) (p->arrayP[index].attribute));
				}
				else{
					printf("getListProperty: There is no string attribute in the specified index! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return ((List*) (NULL));
				}
			}
			else{
				printf("getListProperty: The specified index has NULL attribute or type! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return ((List*) (NULL));
			}
		}
		else{
			printf("getListProperty: Array of properties if NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((List*) (NULL));
		}
	}
	else{
		printf("getListProperty: Properties pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((List*) (NULL));
	}
    
}


int destroyProperties(Properties** proptr){

	int i=0;

	if(proptr != NULL){
		if(*proptr != NULL){
			if( ((*proptr)->num_properties > 0) && ((*proptr)->arrayP != NULL) ){
				for(i=0; i<(*proptr)->num_properties; i++){
					if((*proptr)->arrayP[i].attribute != NULL){
						if((*proptr)->arrayP[i].type == LIST){
							destroyList((*proptr)->arrayP[i].attribute);
						}
						free((*proptr)->arrayP[i].attribute);
						(*proptr)->arrayP[i].attribute = NULL;
					}
				}
				free((*proptr)->arrayP);
				(*proptr)->arrayP = NULL;
			}
			free(*proptr);
			*proptr = NULL;
		}
	}

	return 0;

}

///////////////////////////GRAPH NODE FUNCTIONS/////////////////////////////////////////

/**
 * Creates a  graph node with specific properties
 * id: the id of the graph node
 * p: the properties of the graph node
 * return value: the requested graph node
 */
GraphNode* createGraphNode(int id, Properties* p,int* errorFlag) {

	GraphNode* gnode = (GraphNode*) NULL;

	gnode = malloc(sizeof(GraphNode));
	if(gnode == NULL){
		printf("createNode: Could not allocate memory for GraphNode! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	gnode->id = id;
	gnode->properties = p;
	createList(&((gnode)->EdgeSetList),"EdgeSetList",&allocateEdgeSet,&destroyEdgeSet,&compareEdgeSet,&assignEdgeSet,&printEdgeSet); /* Each node might have many different
																																types of Edges so we hold a List of Lists
																														 	of Edges for better categorisation! */
	gnode->num_edges = 0;

	gnode->topNForumNumEdges = NULL;

	gnode->betweenessCentrality = 0.0;
	gnode->trustValue = 0.0;

	return gnode;

}

int getNumberOfGraphNodeEdges(GraphNode* gnode,int *errorFlag){

	if(gnode == NULL){
		printf("getNumberOfGraphNodeEdges: GraphNode is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;		
	}

	return gnode->num_edges;

}

int destroyGraphNode(GraphNode** gnodeptr){

	Node* myCurrent = (Node*) NULL;
	EdgeSet* hyperEdgeSet = (EdgeSet*) NULL;
	int errorFlag=0;

	if(gnodeptr != NULL){
		if(*gnodeptr != NULL){
			if((*gnodeptr)->properties != NULL){
				destroyProperties(&((*gnodeptr)->properties));
			}
			destroyList(&((*gnodeptr)->EdgeSetList));
			if((*gnodeptr)->topNForumNumEdges != NULL){
				free((*gnodeptr)->topNForumNumEdges);
			}
			free(*gnodeptr);
			*gnodeptr = NULL;
		}
	}

	return 0;

}

////////////////////////////////////EDGE SET FUNCTIONS////////////////////////////////////////

/* An EdgeSet is a struct we have created that exists inside a GraphNode. 
 An EdgeSet keeps a list of all edges of a certain type of Edge.
 The GraphNode keeps a list of EdgeSets called EdgeSetList */

int insertInEdgeSet(GraphNode* ptrN, Edge* edgeptr){

	int found = 0;
	EdgeSet* currentEdgeSet = (EdgeSet*) NULL;
	Edge* currentEdge = (Edge*) NULL;

	if((ptrN != NULL) && (edgeptr != NULL)){
		if(ptrN->EdgeSetList.items == 0){
			if(addLastList(&(ptrN->EdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute) == EXIT_VALUE){ /* Create a new set of Edges, e.g set of "knows" edges */
				printf("insertInEdgeSet: addLastList failed! - Exiting!\n");
				return EXIT_VALUE;
			}
			currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Head->data;
			if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){	/* Add the Edge in this EdgeSet */
				printf("insertInEdgeSet: addLastList failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
		else{
			ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Head;
			while(ptrN->EdgeSetList.Current != NULL){
				currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
				if(!strcmp(currentEdgeSet->relation, edgeptr->properties->arrayP[0].attribute)){ /* An EdgeSet of this relation already exists! */
					found = 1;
					if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){			    /* Add the Edge in this EdgeSet */
						printf("insertInEdgeSet: addLastList failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					break;
				}
				ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Current->next;
			
			}
			
			if(found == 0){
				if(addLastList(&(ptrN->EdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute) == EXIT_VALUE){ /* Create a new set of Edges, e.g set of "knows" edges */
					printf("insertInEdgeSet: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
				if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){	/* Add the Edge in this EdgeSet */
					printf("insertInEdgeSet: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
			}
		}
		ptrN->num_edges++;
	}
	else{
		printf("insertInEdgeSet: GraphNode or Edge Pointer are NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

}

EdgeSet* findEdgeSet(GraphNode* gnode, char* relation,int* errorFlag){ //Located and return the EdgeSet that represents the given relation

	EdgeSet* tempEdgeSet= (EdgeSet*) NULL;

	if(gnode == NULL){
		printf("findEdgeSet: GraphNode is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((EdgeSet*) (NULL));
	}

	Node* myCurrent = NULL;
	myCurrent = gnode->EdgeSetList.Head;

	while(myCurrent != NULL){
		tempEdgeSet = (EdgeSet*) myCurrent->data;	
		if(tempEdgeSet == NULL){
			printf("findEdgeSet: EdgeSet is NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((EdgeSet*) (NULL));
		}
		if(!strcmp(tempEdgeSet->relation, relation)){ //EdgeSet located
			return tempEdgeSet;
		}
		myCurrent = myCurrent->next;
	}
	return (EdgeSet*) NULL;

}

Edge* getFirstEdge(EdgeSet* edgeSet,int* errorFlag){ 

	if(edgeSet != NULL){
		if(edgeSet->edges.Head == NULL){
			return ((Edge*) (NULL));
		}
		return  (Edge*) edgeSet->edges.Head->data;
	}
	else{
		printf("getFirstEdge: EdgeSet is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));	
	}

}

Edge* getLastEdge(EdgeSet* edgeSet,int* errorFlag){ 

	if(edgeSet != NULL){
		if(edgeSet->edges.Last == NULL){ 			
			return ((Edge*) (NULL));
		}
		return (Edge*) edgeSet->edges.Last->data;
	}
	else{
		printf("getLastEdge: EdgeSet is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));		
	}

}

Edge* getNextEdge(EdgeSet* edgeSet,Edge* edge,int* errorFlag){ 

	Node* tempNode = (Node*) NULL;
	Edge* tempEdge = (Edge*) NULL;

	if(edgeSet == NULL){
		printf("getNextEdge: EdgeSet is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	tempNode = edgeSet->edges.Head;
	if(tempNode == NULL){ 
		return ((Edge*) NULL);
	}

	tempEdge = (Edge*) tempNode->data;
	if(tempEdge == NULL){
		return ((Edge*) NULL);
	}

	while(tempNode != NULL){
		if(tempEdge == edge){
			tempNode = tempNode->next;
			if(tempNode == NULL){ 
				return ((Edge*) NULL);
			}
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL){ 
				return ((Edge*) NULL);
			}
			return tempEdge;
		}

		tempNode = tempNode->next;
		if(tempNode != NULL){
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL){ 
				return ((Edge*) NULL);
			}
		}

	}
	return ((Edge*) NULL);

}

Edge* getPreviousEdge(EdgeSet* edgeSet,Edge* edge,int* errorFlag){ 

	Node* tempNode = (Node*) NULL;
	Edge* tempEdge = (Edge*) NULL;

	if(edgeSet == NULL){
		printf("getNextEdge: EdgeSet is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	tempNode = edgeSet->edges.Head;
	if(tempNode == NULL){
		return ((Edge*) NULL);
	}
	tempEdge = (Edge*) tempNode->data;
	if(tempEdge == NULL){ 
		return ((Edge*) NULL);
	}

	while(tempNode != NULL){
		if(tempEdge == edge){
			tempNode = tempNode->previous;
			if(tempNode == NULL){ 
				return ((Edge*) NULL);
			}
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL){ 
				return ((Edge*) NULL);
			}
			return tempEdge;
		}

		tempNode = tempNode->previous;
		if(tempNode != NULL){
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL){ 
				return ((Edge*) NULL);
			}
		}

	}
	return ((Edge*) NULL);

}



////////////////////////////////////EDGE FUNCTIONS////////////////////////////////////////////

/**
 * Creates an edge with specific properties
 * startID: the id of the start node
 * endID: the id of the end node
 * p: the properties of the edge
 * return value: the requested edge
 */
Edge* createEdge(int startID, int endID, Properties* p,int* errorFlag) {
    
	Edge* edge = (Edge*) NULL;

	edge = malloc(sizeof(Edge));
	if(edge == NULL){
		printf("createEdge: Could not allocate memory for Edge! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) NULL);
	}

	edge->startNodeID = startID;
	edge->endNodeID = endID;
	edge->properties = p;

	edge->betweenessCentrality = 0.0;

	return edge;

}

int getEdgeStartID(Edge* e,int* errorFlag){ /* Returns the StartID of an Edge */

	if(e != NULL){
		return e->startNodeID;
	}
	else{
		printf("getEdgeStartID: Edge pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

}

int getEdgeEndID(Edge* e,int* errorFlag){ /* Returns the EndID of an Edge */

	if(e != NULL){
		return e->endNodeID;
	}
	else{
		printf("getEdgeEndID: Edge pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

}

Properties* getEdgeProperties(Edge* e,int* errorFlag){ /* Returns the Properties of an Edge */

	if(e != NULL){
		if(e->properties == NULL){
			printf("getEdgeProperties: Edge has no properties yet! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((Properties*) NULL);
		}
		return e->properties;
	}
	else{
		printf("getEdgeStartID: Edge pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Properties*) NULL);
	}

}

int destroyEdge(Edge** edgeptr){

	if(edgeptr != NULL){
		if(*edgeptr != NULL){
			destroyProperties(&((*edgeptr)->properties));
			free(*edgeptr);
		}
	}

	return 0;

}

////////////////////////////////////////////////////////////////////////////

/************************************/
/* Implement for part 1 */
/************************************/

/* Creates an empty graph
 * m: size of hashtable
 * c: size of cells in a bucket
 * return value: an empty graph*/
Graph* createGraph(int m, int c,int* errorFlag) {

	Graph* ptrG;
	int i;

	*errorFlag = 0;
	if((c <= 0) || (m <= 0)){
		printf("createGraph: Error! Need positive values for hashtable size and bucket size! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return (Graph*) NULL;
	}

	ptrG = malloc(sizeof(Graph));
	if(ptrG == NULL){
		printf("createGraph: Error! Could not allocate enough memory for Graph! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return (Graph*) NULL;
	}

	ptrG->num_edges = 0;
	
	if(initHashTable(&(ptrG->hashtable),m,c) == EXIT_VALUE){ //Initialise this HashTable
		printf("createGraph: Error! Could not initialise HashTable! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return (Graph*) NULL;		
	}

	createList(&((ptrG)->bannedEdgeSetList),"EdgeSetList",&allocateEdgeSet,&destroyEdgeSet,&compareEdgeSet,&assignEdgeSet,&printEdgeSet);

	return ptrG;

}

/* Inserts a node in the graph
 * n: the node to insert
 * g: the graph
 * return value: success/fail status */
int insertNode(GraphNode* ptrN, Graph* ptrG) {

	if(insertNodeHashTable(ptrN, &(ptrG->hashtable)) == EXIT_VALUE){
		printf("insertNode: insertNodeHashTable failed! - Exiting!\n");
		return EXIT_VALUE;
	}
	//printf("CHECK!!!insertNode: i = %d\n",ptrG->hashtable.i);

}

/* Inserts an edge in the graph
 * startID: the id of the start node
 * e: the edge we want to add to the start node
 * g: the graph
 * return value: success/fail status */
int insertEdge(int startID, Edge* edgeptr, Graph* ptrG) {

	GraphNode* gnodeptr = (GraphNode*) NULL;

	//printf("STARTID: %d\n",startID);
	int errorFlag=0;
	gnodeptr = lookUpNode(startID, ptrG, &errorFlag);
	if(errorFlag == EXIT_VALUE){
		printf("\ninsertEdge: lookUpNode failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(gnodeptr == NULL){
		printf("insertEdge: Initial Node was not found...can't add Edge!\n");
		int answer=0;
		printf("\ninsertEdge: Abort execution or Skip record? 1:Abort - 2: Skip\n");
		do{
			scanf("%d",&answer);
		}while((answer != 1) && (answer != 2));
		if(answer == 1){
			return EXIT_VALUE;
		}
		else
			return -1;
	}

	if(insertInEdgeSet(gnodeptr,edgeptr) == EXIT_VALUE){
		printf("insertEdge: insertInEdgeSet failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	ptrG->num_edges++;

	return 0;

}

/* Retrieves a node from the graph
 * nodeID: the if of node to retrieve
 * g: the graph
 * return value: the requested node */
GraphNode* lookUpNode(int nodeID,Graph* ptrG,int* errorFlag) {

	*errorFlag = 0;
	if(ptrG == NULL){
		printf("lookUpNode: Graph has not been initialised! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	GraphNode* result = NULL;
	result = lookUpNodeHashTable(nodeID, &(ptrG->hashtable), errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("lookUpNode: Graph has not been initialised! - Exiting!\n");		
		return ((GraphNode*) (NULL));
	}
	return result;

}

/* Finds shortest path distance between two nodes in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: the shortest path length */
int reachNode1(int startID, int endID,Graph* ptrG,int* errorFlag) { //TODO: Check if graph is unidirected
																  //TODO: WARNING - EXPANDS ONLY KNOWS EDGES and wants a Person Graph as Input

	int success=0,total_distance=0,number_of_paths,i=0,path_counter=1;
	List* pathSet = (List*) NULL;

	pathSet = (Double_BFS_Graph(ptrG,startID,endID,&success,&total_distance,&number_of_paths,PATH_CONSTRUCTION,errorFlag));

	if(pathSet != NULL){
		/*if(success == 1){
			printf("reachNode1: Double_BFS found at least a minimal path with distance: %d\n",total_distance);
			printf("reachNode1: Let's see all the possible paths found!\n");
			Node* myCurrent = (Node*) NULL;
			List* myPath = (List*) NULL;
			myCurrent = pathSet->Head;
			while(myCurrent != NULL){
				myPath = (List*) myPath;
				if(myPath != NULL){
					printf("PATH %d: ",path_counter);
					printList(*myPath);
					path_counter++;
					printf("\n");
				}
				myCurrent = myCurrent->next;
			}
		}
		else{
			printf("reachNode1: Double_BFS didn't find any path!\n");
		}*/
		
		destroyList(pathSet);

		free(pathSet);
	}

	if(*errorFlag == EXIT_VALUE){
		printf("reachNode1: Double BFS Failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(success == 1) return total_distance;
	else return INFINITY_REACH_NODE;

}

/* Finds shortest path distance between all reachable nodes from a given node
 * in an undirected graph with no edge weights
 * startID: the id of the start node
 * g: the graph
 * return value: a resultSet */
ResultSet* reachNodeN(int startID,Graph* ptrG,int* errorFlag) {

	GraphNode *gnodeptr=NULL;

	ResultSet* result_set_ptr = (ResultSet*) NULL;

	if(ptrG == NULL){
		printf("reachNodeN: Graph is not ready for BFS... - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	if(ptrG->hashtable.num_records == 0){
		printf("reachNodeN: HashTable is empty... - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	gnodeptr = lookUpNode(startID, ptrG, errorFlag); /* Find start graphnode in graph */
	if(*errorFlag == EXIT_VALUE){
		printf("reachNodeN: lookUpNode failed - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	if(gnodeptr == NULL){
		printf("reachNodeN: Graph START Node is not valid... - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}
	
	result_set_ptr = malloc(sizeof(ResultSet)); /* Create a Result Set */
	if(result_set_ptr == NULL){
		printf("reachNodeN: Could not allocate enough memory for result set! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	result_set_ptr->ptrG = (Graph*) ptrG; /* Associate the given graph with this Result set */

	createList(&(result_set_ptr->queue),"GraphNodePtr",&allocateGraphNodePtr, &destroyGraphNodePtr, &compareGraphNodesPtr, &assignGraphNodePtr, &printGraphNodePtr); /* Creates frontier for BFS_Search */

	if(initHashTable(&(result_set_ptr->set),2,10) == EXIT_VALUE){ /* Creates set for explored nodes for BFS_Search, using HashTable to speed up search */
		printf("reachNodeN: initHashTable failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	gnodeptr->parentIDA = -1; /* Initial Node has no parent */
	gnodeptr->distanceA = 0; /* Initial Node has no distance from itself */

	result_set_ptr->startID = startID; /* Remember the Id of the initial node */

	if(addLastList(&(result_set_ptr->queue),gnodeptr) == EXIT_VALUE){ /* Add initial node to frontier and set */
		printf("reachNodeN: addLastList failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	if(insertNodeHashTable(gnodeptr,&(result_set_ptr->set)) == EXIT_VALUE){
		printf("reachNodeN: insertNodeHashTable returned EXIT_VALUE - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((ResultSet*) NULL);
	}

	return result_set_ptr;

}
/* Checks if another result is available
 * resultSet: the ResultSet returned by a reachNodeN call
 * pair: a Pair instance to hold the next result, the space for the Pair is
 * allocated by the caller
 * return value: 0 when the no more record is available, 1 when 
 */
int next(ResultSet* resultSet, Pair* pair,int* errorFlag) {

	Pair* temp=NULL;

	if(resultSet == NULL){
		printf("next(): Can't use ResultSet - Initialise it! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(resultSet->ptrG == NULL){
		printf("next(): Can't use Graph! - Initialise it! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(resultSet->ptrG->hashtable.num_records == 0){
		printf("next(): Can't use HashTable! - Place a record in it! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(resultSet->ptrG->hashtable.hashmap == NULL){
		printf("next(): Can't use HashMap! - Initialiase it! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(resultSet->set.num_records >= 0){ /* Set is ready for use */
		if(BFS_Step(resultSet,errorFlag) != -1){ 
			if(*errorFlag == EXIT_VALUE){
				printf("next(): BFS_Step return EXIT_VALUE - Exiting!\n");
				return EXIT_VALUE;
			}
			*pair = resultSet->currentResult;
			return 1;
		}
		else return 0;	
	}
	else{
		printf("next(): Error ResultSet is not initialised! - Exiting!\n");
		return EXIT_VALUE;
	}

}

/* Destroys a graph
 * g: the graph
 * return value: success/fail status */
int destroyGraph(Graph* ptrG) {

	int i,destroyedBuckets=0;

	printf("\nDestroying the Graph...\n");
	if(ptrG != NULL){
		if(clearHashTable(&(ptrG->hashtable)) == EXIT_VALUE){
			printf("destroyGraph: clearHashTable returned EXIT_VALUE!- Exiting!\n");
			free(ptrG);
			return EXIT_VALUE;
		}
		destroyList(&((ptrG)->bannedEdgeSetList));

		free(ptrG);
		printf("Destroyed the Graph!\n\n");
	}
	return 0;
}

int banGraphEdge(Graph* ptrG,Edge* e){ //Used in part 3 to form communities, we must remove edges in graph


	int errorFlag=0;

	if(ptrG == NULL){
		printf("\nbanGraphEdge: Graph is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(e == NULL){
		printf("\nbanGraphEdge: Edge is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	int startNodeID = e->startNodeID;
	int endNodeID = e->endNodeID;

	if(lookUpNode(startNodeID, ptrG, &errorFlag) == NULL){
		printf("\nbanGraphEdge: startNodeID does not exist in this Graph! - Exiting!\n");
		return EXIT_VALUE;
	}
	if(errorFlag == EXIT_VALUE){
		printf("\nbanGraphEdge: lookUpNode failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(lookUpNode(endNodeID, ptrG, &errorFlag) == NULL){
		printf("\nbanGraphEdge: endNodeID does not exist in this Graph! - Exiting!\n");
		return EXIT_VALUE;
	}
	if(errorFlag == EXIT_VALUE){
		printf("\nbanGraphEdge: lookUpNode failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(insertInBanSet(ptrG,e) == EXIT_VALUE){
		printf("banGraphEdge: insertInBanSet failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

int printBanSet(Graph* ptrG){

	int errorFlag =0;

	EdgeSet* banSet = (EdgeSet*) NULL;
	banSet = findBanEdgeSet(ptrG, "knows",&errorFlag);
	if(errorFlag == EXIT_VALUE){
		printf("printBanSet: findBanEdgeSet failed!\n");
		return EXIT_VALUE;
	}

	if(banSet == NULL){
		printf("printBanSet: Failed to find banEdgeSet!\n");
		return EXIT_VALUE;		
	}

	printList(banSet->edges);

	printf("\n");

}

int insertInBanSet(Graph* ptrG, Edge* edgeptr){

	int found = 0;
	int errorFlag = 0;

	EdgeSet* currentEdgeSet = (EdgeSet*) NULL;
	Edge* currentEdge = (Edge*) NULL;

	if((ptrG != NULL) && (edgeptr != NULL)){
		if(ptrG->bannedEdgeSetList.items == 0){
			if(addLastList(&(ptrG->bannedEdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute) == EXIT_VALUE){ /* Create a new set of Edges, e.g set of "knows" edges */
				printf("insertInBanSet: addLastList failed! - Exiting!\n");
				return EXIT_VALUE;
			}
			currentEdgeSet = (EdgeSet*) ptrG->bannedEdgeSetList.Head->data;
			if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){	/* Add the Edge in this EdgeSet */
				printf("insertInBanSet: addLastList failed! - Exiting!\n");
				return EXIT_VALUE;
			}
			
			int endNodeID = edgeptr->endNodeID;
			int startNodeID = edgeptr->startNodeID;

			GraphNode* GNode = (GraphNode*) NULL;
			GNode = lookUpNode(endNodeID, ptrG, &errorFlag);
			if(errorFlag == EXIT_VALUE){
				printf("insertInBanSet: lookUpNode failed! - Exiting!\n");
				return EXIT_VALUE;
			}
			if(GNode != NULL){
				currentEdgeSet = findEdgeSet(GNode,"knows",&errorFlag);
				Node* myCurrent = (Node*) NULL;
				if(currentEdgeSet != NULL){
					myCurrent = currentEdgeSet->edges.Head;
					while(myCurrent != NULL){
						currentEdge = (Edge*) myCurrent->data;
						if(currentEdge != NULL){
							if(currentEdge->endNodeID == startNodeID){
								insertInBanSet(ptrG, currentEdge);
							}
						}
						myCurrent = myCurrent->next;
					}
					
				}
			}

		}
		else{
			ptrG->bannedEdgeSetList.Current = ptrG->bannedEdgeSetList.Head;
			while(ptrG->bannedEdgeSetList.Current != NULL){
				currentEdgeSet = (EdgeSet*) ptrG->bannedEdgeSetList.Current->data;
				if(!strcmp(currentEdgeSet->relation, edgeptr->properties->arrayP[0].attribute)){ /* An EdgeSet of this relation already exists! */
					found = 1;
					if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){			    /* Add the Edge in this EdgeSet */
						printf("insertInBanSet: addLastList failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					break;
				}
				ptrG->bannedEdgeSetList.Current = ptrG->bannedEdgeSetList.Current->next;
			
			}
			
			if(found == 0){
				if(addLastList(&(ptrG->bannedEdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute) == EXIT_VALUE){ /* Create a new set of Edges, e.g set of "knows" edges */
					printf("insertInBanSet: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				currentEdgeSet = (EdgeSet*) ptrG->bannedEdgeSetList.Head->data;
				if(addLastList(&(currentEdgeSet->edges), edgeptr) == EXIT_VALUE){	/* Add the Edge in this EdgeSet */
					printf("insertInBanSet: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}

				int endNodeID = edgeptr->endNodeID;
				int startNodeID = edgeptr->startNodeID;

				GraphNode* GNode = (GraphNode*) NULL;
				GNode = lookUpNode(endNodeID, ptrG, &errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("insertInBanSet: lookUpNode failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(GNode != NULL){
					currentEdgeSet = findEdgeSet(GNode,"knows",&errorFlag);
					Node* myCurrent = (Node*) NULL;
					if(currentEdgeSet != NULL){
						myCurrent = currentEdgeSet->edges.Head;
						while(myCurrent != NULL){
							currentEdge = (Edge*) myCurrent->data;
							if(currentEdge != NULL){
								if(currentEdge->endNodeID == startNodeID){
									insertInBanSet(ptrG, currentEdge);
								}
							}
							myCurrent = myCurrent->next;
						}
					
					}
				}

			}
		}
	}
	else{
		printf("insertInBanSet: Graph or Edge Pointer are NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

}

EdgeSet* findBanEdgeSet(Graph* ptrG, char* relation,int* errorFlag){ //Located and return the EdgeSet that represents the given relation

	EdgeSet* tempEdgeSet= (EdgeSet*) NULL;

	if(ptrG == NULL){
		printf("findEdgeSet: GraphNode is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((EdgeSet*) (NULL));
	}

	ptrG->bannedEdgeSetList.Current = ptrG->bannedEdgeSetList.Head;
	while(ptrG->bannedEdgeSetList.Current != NULL){
		tempEdgeSet = (EdgeSet*) ptrG->bannedEdgeSetList.Current->data;	
		if(tempEdgeSet == NULL){
			printf("findEdgeSet: EdgeSet is NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((EdgeSet*) (NULL));
		}
		if(!strcmp(tempEdgeSet->relation, relation)){ //EdgeSet located
			return tempEdgeSet;
		}
		ptrG->bannedEdgeSetList.Current = ptrG->bannedEdgeSetList.Current->next;
	}
	return (EdgeSet*) NULL;

}

int isBannedEdge(Graph* ptrG,Edge* e){

	int errorFlag = 0;
	Node* myCurrent=NULL;
	EdgeSet* targetEdgeSet = NULL;
	Edge* currentEdge=NULL;
	
	if(ptrG == NULL){
		printf("isBannedEdge: Graph is NULL! - Exiting!\n");
		exit(1);
	}

	if(e == NULL){
		printf("isBannedEdge: Edge is NULL! - Exiting!\n");
		exit(1);
	}

	if(e->properties == NULL){
		printf("isBannedEdge: Edge's properties NULL! - Exiting!\n");
		exit(1);
	}

	if(e->properties->arrayP[0].attribute == NULL){
		printf("isBannedEdge: Edge's properties attribute 0 is NULL! - Exiting!\n");
		exit(1);
	}

	if(ptrG->bannedEdgeSetList.items == 0){
		//printf("isBannedEdge!!! No items!!!!\n\n");
		return 0;
	}

	targetEdgeSet = findBanEdgeSet(ptrG,"knows", &errorFlag);
	if(errorFlag == EXIT_VALUE){
		printf("isBannedEdge: findBanEdgeSet failed! - Exiting!\n");
		exit(1);
	}

	if(targetEdgeSet == NULL){
		//printf("\n\nNo Banset exists!\n\n\n");
		return 0;
	}

	if(targetEdgeSet->edges.items > 0){
		myCurrent = targetEdgeSet->edges.Head;
		while(myCurrent != NULL){
			currentEdge = (Edge*) myCurrent->data;
			if(currentEdge == NULL){
				printf("isBannedEdge: Error edge in bannedEdgeSetList is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}
			
			if((e->startNodeID == currentEdge->startNodeID) && (e->endNodeID == currentEdge->endNodeID)){
				//printf("VICTORYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n");
				return 1; //WARNING WE ARE COMPARING MEMORY ADDRESSES HERE BECAUSE THERE EXISTS ONLY 1 COPY OF THIS EDGE AND SO WE ONLY HAVE TO LOCATE ITS ADDRESS IN THE BANNED LIST IF IT EXISTS
			}

			myCurrent = myCurrent->next;
		}
		return 0;
	}
	else{	
		return 0;
	}

}


long getNumberOfGraphNodes(Graph* ptrG,int* errorFlag){ /* Returns the number of Nodes in the Graph */

	if(ptrG == NULL){
		printf("getNumberOfGraphNodes: Graph pointer is NULL!- Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	return ptrG->hashtable.num_records;
}

long getNumberOfGraphEdges(Graph* ptrG,int* errorFlag){ /* Returns the number of Edges in the Graph */

	if(ptrG == NULL){
		printf("getNumberOfGraphEdges: Graph pointer is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	return ptrG->num_edges;
}
