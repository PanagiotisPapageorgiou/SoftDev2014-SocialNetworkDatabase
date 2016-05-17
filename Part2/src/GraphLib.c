/*
 * GraphLib.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "GraphLib.h"
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
					*errorFlag = EXIT_VALUE;
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

int destroyProperties(Properties** proptr){

	int i=0;

	if(proptr != NULL){
		if(*proptr != NULL){
			if( ((*proptr)->num_properties > 0) && ((*proptr)->arrayP != NULL) ){
				for(i=0; i<(*proptr)->num_properties; i++){
					if((*proptr)->arrayP[i].attribute != NULL){
						free((*proptr)->arrayP[i].attribute);
					}
				}
				free((*proptr)->arrayP);
			}
			free(*proptr);
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

	if(gnodeptr != NULL){
		if(*gnodeptr != NULL){
			destroyProperties(&((*gnodeptr)->properties));
			destroyList(&((*gnodeptr)->EdgeSetList));
			free(*gnodeptr);
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

	gnode->EdgeSetList.Current = gnode->EdgeSetList.Head;
	while(gnode->EdgeSetList.Current != NULL){
		tempEdgeSet = (EdgeSet*) gnode->EdgeSetList.Current->data;	
		if(tempEdgeSet == NULL){
			printf("findEdgeSet: EdgeSet is NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return ((EdgeSet*) (NULL));
		}
		if(!strcmp(tempEdgeSet->relation, relation)){ //EdgeSet located
			return tempEdgeSet;
		}
		gnode->EdgeSetList.Current = gnode->EdgeSetList.Current->next;
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
		if(success == 1){
			printf("reachNode1: Double_BFS found at least a minimal path with distance: %d\n",total_distance);
			printf("reachNode1: Let's see all the possible paths found!\n");
			for(i=0; i < number_of_paths; i++){
				if(pathSet[i].items > 0){
					printf("PATH %d: ",path_counter);
					printList(pathSet[i]);
					path_counter++;
					printf("\n");
				}
			}
		}
		else{
			printf("reachNode1: Double_BFS didn't find any path!\n");
		}
		for(i=0; i < number_of_paths; i++){
			destroyList(&(pathSet[i]));
		}
		free(pathSet);
	}

	if(*errorFlag == EXIT_VALUE){
		printf("reachNode1: Double BFS Failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
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

	GraphNode *gnodeptr;

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

	Pair* temp;

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
			*pair = resultSet->currentResult;
			return 1;
		}
		if(*errorFlag == EXIT_VALUE){
			printf("next(): BFS_Step return EXIT_VALUE - Exiting!\n");
			return EXIT_VALUE;
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
		free(ptrG);
		printf("Destroyed the Graph!\n\n");
	}
	return 0;
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

///////////////////////////////////////////////////////////////////////////////

int findAllCoherentGraphs(Graph* g,List* listOfSubGraphs,int* errorFlag) {

 
        int i=0,number_of_cc=0;
        GraphNode* currentGNode = (GraphNode*) NULL;
        GraphNode* tempGNode = (GraphNode*) NULL;
        ResultSet* result_set = (ResultSet*) NULL;
        List listOfResultSets;//H findCoherentGraphs krataei mia lista apo resultSets. Kathe resultSet einai enas upografos me th morfh (ID,apostash) gia kathe kombo tou.
        Pair currentResult;
        Graph* subGraph; //create temporary graph for the coherent graph
 
        //*max_size=0;
 
        if(g != NULL){
                if(g->hashtable.hashmap != NULL){ 
                        createList(&listOfResultSets, "ResultSetPtr", &allocateResultSetPtr, &destroyResultSetPtr, &compareResultSetPtr, &assignResultSetPtr, &printResultSetPtr);
                        while ( (currentGNode = pickExcludedNode(listOfResultSets, g, errorFlag) ) != NULL){ //Edw epilegei kombo tou arxikou grafou p dn uparxei mexri stigmhs s kapoio upografo Wste na ksekinisei kainourgia anazhthsh me reachNodeN gia neo upografo
                                if(*errorFlag == EXIT_VALUE){                                                                                                    
                                        printf("findCoherentGraphs: errorFlag! Could not allocate ResultSet! - Exiting!\n");
                                        return EXIT_VALUE;
                                }
 
                                if(currentGNode == NULL){ //oxi result_set ==Null afou panta NULL tha nai stin arxi
                                        printf("findCoherentGraphs: Could not pick a currentGNode! - Exiting!\n");
                                        *errorFlag = EXIT_VALUE;
                                }
                                else { 
                                	printf("Current node to start reachNodeN is %d\n",currentGNode->id);
                                }
                                /* New coherent subgraph located! Let's explore it */
 								
                                result_set = reachNodeN(currentGNode->id, g, errorFlag); /* Prepare a 1 to N search starting with new node */ //Neo reachNodeN == Neos upografos
                                if(*errorFlag == EXIT_VALUE){
                                        printf("findCoherentGraphs: reachNodeN failed! - Exiting!\n");
                                        *errorFlag = EXIT_VALUE;
                                        return EXIT_VALUE;
                                }
                                if(addLastList(&listOfResultSets, result_set) == EXIT_VALUE){ /* Add the set of results of this search to a list to keep all the results */
                                        printf("findCoherentGraphs: addLastList failed! - Exiting!\n");
                                        *errorFlag = EXIT_VALUE;
                                        return EXIT_VALUE;
                                }
                                
                               /* A new coherent subgraph was located */ 
                                //Se antistoixeia me to neo reachNodeN esu prepei na dhmiourgeis me createGraph edw pera enan proswrino neo Grafo
                                subGraph = createGraph(3,4,errorFlag); //m,c ?
                               //Auto metraei ton arithmo twn upografwn p exoun anakalufthei - Dld ton arithmo twn reachNodeN p eginan
                                number_of_cc++; 
 
                                while( next(result_set, &currentResult, errorFlag) ) { //continue; 
                                /* Explore the latest set as much as possible and create a coherent subgraph by doing so */
                                //Proxwraei oso borei to resultSet k anakaluptei ton grafo
                                                                                                                   
                              		// tropopoihseis to loop apo panw p anakaluptei kainourgia Pairs k na pairneis to currentResult. 
                                	//Na eksageis to ID apo to currentResult.
                                                           
                                	if(*errorFlag == EXIT_VALUE) {
										printf("findCoherentGraphs:next failed!\n");
										*errorFlag = EXIT_VALUE;
										return 	EXIT_VALUE;
									} 
									//Afou to eksageis tha kaneis tempGraphNode = lookUpNode(IdPair,g) gia na fereis ton graphnode apo ton arxiko grafo
									tempGNode = lookUpNode(currentResult.ID, g,errorFlag); 
									if(*errorFlag == EXIT_VALUE){
										printf("findCoherentGraphs: lookUpNode failed!\n");
										*errorFlag = EXIT_VALUE;
										return 	EXIT_VALUE;
									}                             
                                	//Afou ton fereis prepei na ton kaneis insertNode sto tempGraph p ftiaksame pio panw
                                	insertNode(tempGNode,subGraph); //vaze ena ena mesa sto while tous komvous tou synektikou=kovoi subgraph
                                }//while	
                                	
                                //Otan teleiwsei to loop twn while next tote tha prepei na kaneis addLastList(listOfSubGraphs,tempGraph) 
                                //gia na baleis ton neo upografo sth Lista me tous deiktes s Grafous P eipame na ftiaksoume gia uberlist
                                
                                addLastList(listOfSubGraphs,subGraph); //vale grapho sti lista pou epistrefeis
                                
                                //Prosekse mia leptomereia: Edw tha kaneis createGraph p kanei malloc mesa kainourgio Graph, -done line 131
                                //tha kaneis insert -done 155
                                // alla dn tha kaneis destroy. Gia na min exoume polla antigrafa kathe grafou. 
                                //Ousiastika tha kaneis tempGraph = createGraph, tha ton gemizeis k meta tha kaneis addLastList k h uberlist me deiktes s grafous dn tha ftianxei dikia tis
                                //copia me malloc stin allocateGraphPtr gt tote tha prepei na antigrapsoume apo tin arxh oti exei o grafos! Akolouthise tin logikh tou GraphNodePtr gia to GraphPtr s uberlist-done den kanei malloc i allocate!
                               
                                if(*errorFlag == EXIT_VALUE){
                                        printf("findCoherentGraphs: Could not allocate ResultSet! - Exiting!\n");
                                        return EXIT_VALUE;
                                }
               
                                //if(result_set->set.num_records > *max_size)
                                        //*max_size = result_set->set.num_records; /* A subgraph with more nodes has been located! */
 
                        }//while yparxoun komvoi pou den einai se ypografo
 
                        if(*errorFlag == EXIT_VALUE){
                                printf("findCoherentGraphs: Could not allocate ResultSet! - Exiting!\n");
                                return EXIT_VALUE;
                        }
 
 
                        /* Free all resources allocated by findCoherentGraphs */
 
                        listOfResultSets.Current = listOfResultSets.Head;
                        while(listOfResultSets.Current != NULL){
                                destroyResultSet( (ResultSet**) &(listOfResultSets.Current->data));
                                listOfResultSets.Current = listOfResultSets.Current->next;
                        }
 
                        destroyList(&listOfResultSets);
 
                        return number_of_cc;
 
                }
                else{
                        printf("findCoherentGraphs: HashMap is NULL! - Exiting!\n");
                        *errorFlag = EXIT_VALUE;
                        return EXIT_VALUE;
                }
        }
        else{
                printf("findCoherentGraphs: Graph Pointer is NULL! - Exiting!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
}







