/*
 * GraphLib.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "GraphLib.h"
#include "defines.h"

//////////////////////////////////////////////PROPERTIES FUNCTIONS/////////////////////////////////////////

/**
 * Creates a properties object
 * number: the number of properties
 * return value: a properties object
 */
Properties* createProperties(int number) {

	int i=0;

	Properties* properties;
	properties = malloc(sizeof(Properties));
	
	if(properties == NULL){
		printf("createProperties: Failed to allocate memory for property set!\n");
		exit(1);
	}
	
	properties->num_properties = number;
	properties->arrayP = malloc(sizeof(Property) * number);

	if(properties->arrayP == NULL){
		printf("createProperties: Failed to allocate memory for property array!\n");
		exit(1);
	}

	for(i=0; i<number; i++){
		properties->arrayP[i].attribute = NULL;
		properties->arrayP[i].type = NULL;
	}

    return properties;

}

/**
 * Sets a string property "property" in position "index"
 * property: the type of the property
 * index: the position of the property
 * p: the properties object
 */
void setStringProperty(char* property, int index, Properties* p) {

	int i=0;
	char** attributePtr;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setStringProperty: Properties is not properly initialised (1)!\n");
			exit(1);
		}
		if(p->arrayP == NULL){
			printf("setStringProperty: Properties is not properly initialised (2)!\n");
			exit(1);
		}
		
		if((p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == NULL) ){
			
			attributePtr = (char**) (&(p->arrayP[index].attribute));

			*attributePtr = malloc(sizeof(char) * (strlen(property) + 1) );
			if(*attributePtr == NULL){
				printf("setStringProperty: Could not allocate memory for char* attribute!\n");
				exit(1);
			}

			strcpy(*attributePtr, property);

			p->arrayP[index].type = malloc(sizeof(char) * (strlen("string") + 1) );
			if(p->arrayP[index].type == NULL){
				printf("setStringProperty: Could not allocate memory for type field!\n");
				exit(1);
			}

			strcpy(p->arrayP[index].type, "string");

		}
		else{ //TODO: Support assignment?
			/*if(p->arrayP[index]->type != NULL){
				if(!strcmp(p->arrayP[index]->type, "string")){
			}*/
			printf("setStringProperty: Currently works for initialisation only! Assignment not supported!\n");
			exit(1);
		}
	}
	else{
		printf("setStringProperty: Properties set is NULL!\n");
		exit(1);
	}

}

/**
 * Sets an integer property "property" in position "index"
 * property: the value of the property
 * index: the position of the property
 * p: the properties object
 */
void setIntegerProperty(int property, int index, Properties* p) {
    //TODO
	int** attributePtr;

	if(p != NULL){
		if(p->num_properties <= index){
			printf("setIntegerProperty: Properties is not properly initialised (1)!\n");
			exit(1);
		}
		if(p->arrayP == NULL){
			printf("setIntegerProperty: Properties is not properly initialised (2)!\n");
			exit(1);
		}
		
		if( (p->arrayP[index].attribute == NULL) && (p->arrayP[index].type == NULL) ){
			
			attributePtr = (int**) (&(p->arrayP[index].attribute));

			*attributePtr = malloc(sizeof(int));

			if(*attributePtr == NULL){
				printf("setIntegerProperty: Could not allocate memory for int attribute!\n");
				exit(1);
			}

			**attributePtr = property; /* Assignment */

			p->arrayP[index].type = malloc(sizeof(char) * (strlen("int") + 1) );
			if(p->arrayP[index].type == NULL){
				printf("setIntegerProperty: Could not allocate memory for type field!\n");
				exit(1);
			}

			strcpy(p->arrayP[index].type, "int");

		}
		else{ //TODO: Support assignment?
			/*if(p->arrayP[index]->type != NULL){
				if(!strcmp(p->arrayP[index]->type, "string")){
			}*/
			printf("setIntegerProperty: Currently works for initialisation only! Assignment not supported!\n");
			exit(1);
		}
	}
	else{
		printf("setIntegerProperty: Properties set is NULL!\n");
		exit(1);
	}

}

/**
 * Gets a string property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the type of the property
 */
char* getStringProperty(int index, Properties* p) {
    //TODO

	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != NULL) ){
				if(!strcmp(p->arrayP[index].type, "string" ) ){
					return ((char*) (p->arrayP[index].attribute));
				}
				else{
					printf("getStringProperty: There is no string attribute in the specified index!\n");
					exit(1);
				}
			}
			else{
				printf("getStringProperty: The specified index has NULL attribute or type!\n");
				exit(1);
			}
		}
		else{
			printf("getStringProperty: Array of properties if NULL!\n");
			exit(1);
		}
	}
	else{
		printf("getStringProperty: Properties pointer is NULL!\n");
		exit(1);
	}
    
}

/**
 * Gets an integer property "property" from position "index"
 * index: the position of the requested property
 * p: the properties object
 * return value: the value of the property
 */
int getIntegerProperty(int index, Properties* p) {

	int* numberptr;

	if(p != NULL){
		if(p->arrayP != NULL){
			if( (p->arrayP[index].attribute != NULL) && (p->arrayP[index].type != NULL) ){
				if(!strcmp(p->arrayP[index].type, "int" ) ){
					numberptr = (int*) (p->arrayP[index].attribute);
					return (*numberptr);

				}
				else{
					printf("getIntegerProperty: There is no int attribute in the specified index!\n");
					exit(1);
				}
			}
			else{
				printf("getIntegerProperty: The specified index attribute or type is NULL!\n");
				exit(1);
			}
		}
		else{
			printf("getIntegerProperty: Array of properties if NULL!\n");
			exit(1);
		}
	}
	else{
		printf("getIntegerProperty: Properties pointer is NULL!\n");
		exit(1);
	}

}

int destroyProperties(Properties** proptr){

	int i=0;

	if(proptr != NULL){
		if(*proptr != NULL){
			if( ((*proptr)->num_properties > 0) && ((*proptr)->arrayP != NULL) ){
				for(i=0; i<(*proptr)->num_properties; i++){
					if((*proptr)->arrayP[i].type != NULL){
						free((*proptr)->arrayP[i].type);
					}
					if((*proptr)->arrayP[i].attribute != NULL){
						free((*proptr)->arrayP[i].attribute);
					}
				}
				free((*proptr)->arrayP);
			}
			free(*proptr);
		}
	}

}

///////////////////////////GRAPH NODE FUNCTIONS/////////////////////////////////////////

/**
 * Creates a  graph node with specific properties
 * id: the id of the graph node
 * p: the properties of the graph node
 * return value: the requested graph node
 */
GraphNode* createGraphNode(int id, Properties* p) {

	GraphNode* gnode = NULL;

	gnode = malloc(sizeof(GraphNode));
	if(gnode == NULL){
		printf("createNode: Could not allocate memory for GraphNode!\n");
		exit(1);
	}

	gnode->id = id;
	gnode->properties = p;
	createList(&((gnode)->EdgeSetList),"EdgeSetList",&allocateEdgeSet,&destroyEdgeSet,&compareEdgeSet,&assignEdgeSet,&printEdgeSet); /* Each node might have many different
																																types of Edges so we hold a List of Lists
																															 	of Edges for better categorisation! */
	gnode->num_edges = 0;

	return gnode;

}


GraphNode* setPersonProperties(int id, char* name, char* surname, int age) {

    /*create properties*/
    Properties* prop = createProperties(3); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonProperties: Error creating Properties!\n");
		exit(1);
	}

    setStringProperty(name, 0, prop); /* Allocated memory for string property */
    setStringProperty(surname, 1, prop);
	setIntegerProperty(age, 2, prop);


    /*create node*/
    GraphNode* n = createGraphNode(id, prop);

	if(n == NULL){
		printf("setPersonProperties: Error Creating GraphNode!\n");
		exit(1);
	}

    return n;
}

void printPersonProperties(GraphNode* n) {
    
	int* tempInt;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					tempInt = (int*) n->properties->arrayP[2].attribute;
					printf("Person - ID: |%d| - Name: |%s| - Surname: |%s| - Age: |%d|\n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute, (*tempInt));
			}
			else{
				printf("printPersonProperties: Array of properties is NULL!\n");
				exit(1);
			}			
		}
		else{
			printf("printPersonProperties: Properties set is NULL!\n");
			exit(1);
		}
	}
	else{
		printf("printPersonProperties: GraphNode Pointer is NULL!\n");
		exit(1);
	}

}

int getNumberOfGraphNodeEdges(GraphNode* gnode){

	if(gnode == NULL){
		printf("getNumberOfGraphNodeEdges: GraphNode is NULL!\n");
		exit(1);		
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

}

////////////////////////////////////EDGE SET FUNCTIONS////////////////////////////////////////

int insertInEdgeSet(GraphNode* ptrN, Edge* edgeptr){

	int found = 0;
	EdgeSet* currentEdgeSet=NULL;
	Edge* currentEdge=NULL;

	if((ptrN != NULL) && (edgeptr != NULL)){
		if(ptrN->EdgeSetList.items == 0){
			addLastList(&(ptrN->EdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute); /* Create a new set of Edges, e.g set of "knows" edges */
			currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Head->data;
			addLastList(&(currentEdgeSet->edges), edgeptr);			    /* Add the Edge in this EdgeSet */
		}
		else{
			ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Head;
			while(ptrN->EdgeSetList.Current != NULL){
				currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
				if(!strcmp(currentEdgeSet->relation, edgeptr->properties->arrayP[0].attribute)){ /* An EdgeSet of this relation already exists! */
					found = 1;
					addLastList(&(currentEdgeSet->edges), edgeptr);			    /* Add the Edge in this EdgeSet */
					break;
				}
			
			}
			
			if(found == 0){
				addLastList(&(ptrN->EdgeSetList),(char*) edgeptr->properties->arrayP[0].attribute); /* Create a new set of Edges, e.g set of "knows" edges */
				currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
				addLastList(&(currentEdgeSet->edges), edgeptr);			    /* Add the Edge in this EdgeSet */
			}

		}

		ptrN->num_edges++;

	}
	else{
		printf("insertInEdgeSet: GraphNode or Edge Pointer are NULL!\n");
		exit(1);
	}

}


////////////////////////////////////EDGE FUNCTIONS////////////////////////////////////////////

/**
 * Creates an edge with specific properties
 * startID: the id of the start node
 * endID: the id of the end node
 * p: the properties of the edge
 * return value: the requested edge
 */
Edge* createEdge(int startID, int endID, Properties* p) {
    
	Edge* edge = NULL;

	edge = malloc(sizeof(Edge));
	if(edge == NULL){
		printf("createEdge: Could not allocate memory for Edge!\n");
		exit(1);
	}

	edge->startNodeID = startID;
	edge->endNodeID = endID;
	edge->properties = p;

	return edge;

}

int getEdgeStartID(Edge* e){ /* Returns the StartID of an Edge */

	if(e != NULL){
		return e->startNodeID;
	}
	else{
		printf("getEdgeStartID: Edge pointer is NULL!\n");
		exit(1);
	}

}

int getEdgeEndID(Edge* e){ /* Returns the EndID of an Edge */

	if(e != NULL){
		return e->endNodeID;
	}
	else{
		printf("getEdgeEndID: Edge pointer is NULL!\n");
		exit(1);
	}

}

Edge* getFirstEdge(EdgeSet* edgeSet){ 

	if(edgeSet != NULL){
		if(edgeSet->edges.Head == NULL) return NULL;
		return  (Edge*) edgeSet->edges.Head->data;
	}
	else{
		printf("getFirstEdge: EdgeSet is NULL!\n");
		exit(1);		
	}

}

Edge* getLastEdge(EdgeSet* edgeSet){ 

	if(edgeSet != NULL){
		if(edgeSet->edges.Last == NULL) return NULL;
		return (Edge*) edgeSet->edges.Last->data;
	}
	else{
		printf("getLastEdge: EdgeSet is NULL!\n");
		exit(1);		
	}

}

Edge* getNextEdge(EdgeSet* edgeSet,Edge* edge){ 

	Node* tempNode = NULL;
	Edge* tempEdge = NULL;

	if(edgeSet == NULL){
		printf("getNextEdge: EdgeSet is NULL!\n");
	}

	tempNode = edgeSet->edges.Head;
	if(tempNode == NULL) return NULL;

	tempEdge = (Edge*) tempNode->data;
	if(tempEdge == NULL) return NULL;

	while(tempNode != NULL){

		if(tempEdge == edge){
			tempNode = tempNode->next;
			if(tempNode == NULL) return NULL;
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL) return NULL;
			return tempEdge;
		}

		tempNode = tempNode->next;
		if(tempNode != NULL){
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL) return NULL;
		}

	}
	return NULL;

}

Edge* getPreviousEdge(EdgeSet* edgeSet,Edge* edge){ 

	Node* tempNode = NULL;
	Edge* tempEdge = NULL;

	if(edgeSet == NULL){
		printf("getNextEdge: EdgeSet is NULL!\n");
	}

	tempNode = edgeSet->edges.Head;
	if(tempNode == NULL) return NULL;

	tempEdge = (Edge*) tempNode->data;
	if(tempEdge == NULL) return NULL;

	while(tempNode != NULL){

		if(tempEdge == edge){
			tempNode = tempNode->previous;
			if(tempNode == NULL) return NULL;
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL) return NULL;
			return tempEdge;
		}

		tempNode = tempNode->previous;
		if(tempNode != NULL){
			tempEdge = (Edge*) tempNode->data;
			if(tempEdge == NULL) return NULL;
		}

	}
	return NULL;

}

Edge* setEdgeProperties(int startNodeID, int endNodeID, char* type, int weight) {

    /*create edge properties*/
    Properties* propEdge = createProperties(PERSON_REL_PROPERTIES_NUM);
    setStringProperty(type, 0, propEdge);
    setIntegerProperty(weight, 1, propEdge);

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, propEdge);
    return e;
}

Properties* getEdgeProperties(Edge* e){ /* Returns the Properties of an Edge */

	if(e != NULL){
		if(e->properties == NULL){
			printf("getEdgeProperties: Edge has no properties yet!\n");
			exit(1);
		}
		return e->properties;
	}
	else{
		printf("getEdgeStartID: Edge pointer is NULL!\n");
		exit(1);
	}

}

void printEdgeProperties(Edge* e) {

	int* tempInt;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				tempInt = (int*) e->properties->arrayP[1].attribute;
					printf("Edge - startID: |%d| - endID: |%d| - Type: |%s| - Weight: |%d|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute, *tempInt);
			}
			else{
				printf("printEdgeProperties: Array of properties is NULL!\n");
				exit(1);
			}			
		}
		else{
			printf("printEdgeProperties: Properties set is NULL!\n");
			exit(1);
		}
	}
	else{
		printf("printEdgeProperties: Edge Pointer is NULL!\n");
		exit(1);
	}

}

int destroyEdge(Edge** edgeptr){

	if(edgeptr != NULL){
		if(*edgeptr != NULL){
			destroyProperties(&((*edgeptr)->properties));
			free(*edgeptr);
		}
	}

}

//////////////////////////////////INITIALISE A PERSON /////////////////////////////////


////////////////////////////////////////////////////////////////////////////

/************************************/
/* Implement for part 1 */
/************************************/

/* Creates an empty graph
 * m: size of hashtable
 * c: size of cells in a bucket
 * return value: an empty graph*/
Graph* createGraph(int m, int c) {

	Graph* ptrG;
	int i;

	if((c <= 0) || (m <= 0)){
		printf("Error! Need positive values for hashtable size and bucket size!\n");
		return NULL;
	}

	ptrG = malloc(sizeof(Graph));
	if(ptrG == NULL){
		printf("Error! Could not allocate enough memory for Graph!\n");
		exit(1);
	}

	ptrG->num_edges = 0;
	
	initHashTable(&(ptrG->hashtable),m,c); //Initialise this HashTable

	ptrG->relation = malloc(sizeof(char) * (strlen("knows") + 1));

	strcpy(ptrG->relation, "knows");

	return ptrG;

}

/* Inserts a node in the graph
 * n: the node to insert
 * g: the graph
 * return value: success/fail status */
int insertNode(const GraphNode* const ptrN, Graph* ptrG) {

	insertNodeHashTable(ptrN, &(ptrG->hashtable));
	//printf("CHECK!!!insertNode: i = %d\n",ptrG->hashtable.i);

}

/* Inserts an edge in the graph
 * startID: the id of the start node
 * e: the edge we want to add to the start node
 * g: the graph
 * return value: success/fail status */
int insertEdge(int startID, const Edge* const edgeptr, Graph* ptrG) {

	GraphNode* gnodeptr = NULL;

	//printf("STARTID: %d\n",startID);

	gnodeptr = lookUpNode(startID, ptrG);

	if(gnodeptr == NULL){
		printf("Initial Node was not found...can't add Edge!\n");
		exit(1);
	}

	insertInEdgeSet(gnodeptr,edgeptr);

	ptrG->num_edges++;

	return 0;

}

/* Retrieves a node from the graph
 * nodeID: the if of node to retrieve
 * g: the graph
 * return value: the requested node */
GraphNode* lookUpNode(int nodeID, const Graph* const ptrG) {

	if(ptrG == NULL){
		printf("lookUpNode: Graph has not been initialised!\n");
		exit(1);	
	}

	return lookUpNodeHashTable(nodeID, &(ptrG->hashtable));

}

/* Finds shortest path distance between two nodes in an undirected graph with no edge weights
 * startID: the id of the start node
 * endID: the id of the end node
 * g: the graph
 * return value: the shortest path length */
int reachNode1(int startID, int endID, const Graph* const ptrG) { //TODO: Check if graph is unidirected

	int success=0,total_distance=0,number_of_paths,i=0,path_counter=1;
	List* pathSet = NULL;

	pathSet = (Double_BFS_Graph(ptrG,startID,endID,&success,&total_distance,&number_of_paths,PATH_CONSTRUCTION));

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

	if(success == 1) return total_distance;
	else return INFINITY_REACH_NODE;

}

/* Finds shortest path distance between all reachable nodes from a given node
 * in an undirected graph with no edge weights
 * startID: the id of the start node
 * g: the graph
 * return value: a resultSet */
ResultSet* reachNodeN(int startID, const Graph* const ptrG) {

	GraphNode *gnodeptr;

	ResultSet* result_set_ptr = NULL;

	if(ptrG == NULL){
		printf("reachNodeN: Graph is not ready for BFS...\n");
		return NULL;
	}

	if(ptrG->hashtable.num_records == 0){
		printf("reachNodeN: HashTable is empty...\n");
		return NULL;
	}

	gnodeptr = lookUpNode(startID, ptrG); /* Find start graphnode in graph */

	if(gnodeptr == NULL){
		printf("reachNodeN: Graph START Node is not valid...\n");
		return NULL;
	}
	
	result_set_ptr = malloc(sizeof(ResultSet)); /* Create a Result Set */
	if(result_set_ptr == NULL){
		printf("reachNodeN: Could not allocate enough memory for result set!\n");
		exit(1);
	}

	result_set_ptr->ptrG = ptrG; /* Associate the given graph with this Result set */

	createList(&(result_set_ptr->queue),"GraphNodePtr",&allocateGraphNodePtr, &destroyGraphNodePtr, &compareGraphNodesPtr, &assignGraphNodePtr, &printGraphNodePtr); /* Creates frontier for BFS_Search */

	initHashTable(&(result_set_ptr->set),2,10); /* Creates set for explored nodes for BFS_Search, using HashTable to speed up search */

	gnodeptr->parentIDA = -1; /* Initial Node has no parent */
	gnodeptr->distanceA = 0; /* Initial Node has no distance from itself */

	result_set_ptr->startID = startID; /* Remember the Id of the initial node */

	addLastList(&(result_set_ptr->queue),gnodeptr); /* Add initial node to frontier and set */
	insertNodeHashTable(gnodeptr,&(result_set_ptr->set));

	return result_set_ptr;

}
/* Checks if another result is available
 * resultSet: the ResultSet returned by a reachNodeN call
 * pair: a Pair instance to hold the next result, the space for the Pair is
 * allocated by the caller
 * return value: 0 when the no more record is available, 1 when 
 */
int next(ResultSet* resultSet, Pair* pair) {

	Pair* temp;

	if(resultSet == NULL){
		printf("next(): Can't use ResultSet - Initialise it!\n");
		exit(1);
	}

	if(resultSet->ptrG == NULL){
		printf("next(): Can't use Graph! - Initialise it!\n");
		exit(1);
	}

	if(resultSet->ptrG->hashtable.num_records == 0){
		printf("next(): Can't use HashTable! - Place a record in it!\n");
		exit(1);
	}

	if(resultSet->ptrG->hashtable.hashmap == NULL){
		printf("next(): Can't use HashMap! - Initialiase it!\n");
		exit(1);
	}

	if(resultSet->set.num_records >= 0){ /* Set is ready for use */
		if(BFS_Step(resultSet) != -1){ 
			*pair = resultSet->currentResult;
			return 1;
		}
		else return 0;	
	}
	else{
		printf("Error ResultSet is not initialised!\n");
		return 0;
	}

}

/* Destroys a graph
 * g: the graph
 * return value: success/fail status */
int destroyGraph(Graph* ptrG) {

	int i,destroyedBuckets=0;

	/*printf("=============GRAPH DESTRUCTOR=============\n");*/

	printf("\nDestroying the Graph...\n");

	if(ptrG != NULL){

		clearHashTable(&(ptrG->hashtable));
		//printf("THIS GRAPH SUCKS LET'S GET RID OF IT!\n");
		if(ptrG->relation != NULL) free(ptrG->relation);

		free(ptrG);

		printf("Destroyed the Graph!\n\n");
	}

}

/*=========================================PART 2 FUNCTIONS ===========================================*/

long getNumberOfGraphNodes(Graph* ptrG){ /* Returns the number of Nodes in the Graph */

	if(ptrG == NULL){
		printf("getNumberOfGraphNodes: Graph pointer is NULL!\n");
		exit(1);
	}

	return ptrG->hashtable.num_records;

}

long getNumberOfGraphEdges(Graph* ptrG){ /* Returns the number of Edges in the Graph */

	if(ptrG == NULL){
		printf("getNumberOfGraphEdges: Graph pointer is NULL!\n");
		exit(1);
	}

	return ptrG->num_edges;

}

int numberofCCs(Graph* g){ /* Returns the number of coherent subgraphs in the given Graph */

	int* max_size=0;

	if(g != NULL){
		if(g->hashtable.num_records != 0){
			if(g->hashtable.hashmap != NULL){
				return findCoherentGraphs(g, &max_size); /* findCoherentGraphs returns the number of coherent subgraphs it has found */
			}
			else{
				printf("numberofCCs: HashMap in Graph is NULL!\n");
				exit(1);
			}
		}
		else{
			printf("numberofCCs: HashTable in Graph is empty!\n");
			exit(1);
		}
	}
	else{
		printf("numberofCCs: Graph is not initialised!\n");
		exit(1);
	}

}

int maxCC(Graph* g){ /* Returns the number of nodes the biggest coherent subgraph in the given graph has */

	int max_size=0;

	if(g != NULL){
		if(g->hashtable.num_records != 0){
			if(g->hashtable.hashmap != NULL){
				findCoherentGraphs(g, &max_size); /* findCoherentGraphs returns the number of coherent subgraphs it has found */
				return max_size;
			}
			else{
				printf("maxCC: HashMap in Graph is NULL!\n");
				exit(1);
			}
		}
		else{
			printf("maxCC: HashTable in Graph is empty!\n");
			exit(1);
		}
	}
	else{
		printf("maxCC: Graph is not initialised!\n");
		exit(1);
	}

}

double closenessCentrality(GraphNode* gnode, Graph* g){ /* Will calculate the closeness centrality of a given node */

	ResultSet* result_set = NULL;
	Pair* currentResult = NULL;
	double total_distance = 0.0;

	if(g == NULL){
		printf("closenessCentrality: HashMap in Graph is NULL!\n");
		exit(1);
	}

	if(g->hashtable.num_records == 0){
		printf("closenessCentrality: HashTable in Graph is empty!\n");
		exit(1);
	}

	if(g->hashtable.hashmap == NULL){
		printf("closenessCentrality: Graph is not initialised!\n");
		exit(1);
	}

	if(lookUpNode(gnode->id,g) == NULL){
		printf("closenessCentrality: GraphNode does not exist in Graph!\n");
		exit(1);
	}

	result_set = reachNodeN(gnode->id, g); /* Prepare to reach all possible to reach nodes from this node */

	while( next(result_set, currentResult) ){
		total_distance = total_distance + (double) (1 / (currentResult->distance) ); /* Add all distances from nodes it can reach */
	}

	return (total_distance / (g->hashtable.num_records - 1) ); /* Normalise */

}

double betweennessCentrality(GraphNode* gnode, Graph* g){

	int i=0,ID1,ID2,num_buckets1=0,num_buckets2=0,num_items1=0,num_items2=0,num_paths=0,num_inc_paths=0,success,number_of_paths,total_distance,j,k,t,s;
	double betweenessTotal=0.0;
	Bucket* bucket1=NULL,* bucket2=NULL;
	List* pathSet=NULL;

	if(g == NULL){
		printf("closenessCentrality: HashMap in Graph is NULL!\n");
		exit(1);
	}

	if(g->hashtable.hashmap == NULL){
		printf("closenessCentrality: Graph is not initialised!\n");
		exit(1);
	}

	if(lookUpNode(gnode->id,g) == NULL){
		printf("closenessCentrality: GraphNode does not exist in Graph!\n");
		exit(1);
	}

	for(i=0; i < g->hashtable.size; i++){ //TODO: CUT DUPLICATES / DIPLOTUPA - PATH APO I SE J == PATH APO J SE I
		
		if(g->hashtable.hashmap[i] != NULL){
			
			bucket1 = getHeadBucket(g->hashtable.hashmap[i]);
			while(bucket1 != NULL){
		
				for(j=0; j < bucket1->items; j++){

					for(k = 0; k < g->hashtable.size; k++){
						
						if(g->hashtable.hashmap[k] != NULL){
							
							bucket2 = getHeadBucket(g->hashtable.hashmap[k]);
							while(bucket2 != NULL){

								for(t=0; t < bucket2->items; t++){ /* To cut off duplicates id1 < id2 */

									if((bucket1->GNArray[j]->id != bucket2->GNArray[t]->id) && (bucket1->GNArray[j]->id != gnode->id)){

										pathSet = NULL;
										success = 0;
										total_distance = 0;
										number_of_paths = 0;

										pathSet = (Double_BFS_Graph(g,bucket1->GNArray[j]->id,bucket2->GNArray[t]->id,&success,&total_distance,&number_of_paths,PATH_CONSTRUCTION));

										if(pathSet != NULL){
											if(success == 1){
												num_paths = 0;
												num_inc_paths = 0;
												for(s=0; s < number_of_paths; s++){
													if(pathSet[s].items > 0){
														//printf("PATH %d: ",num_paths);
														//printList(pathSet[s]);
														if(nodeIsInPath(gnode->id,pathSet[s]) == 1){
															num_inc_paths++;
														}
														num_paths++;
														//printf("\n");
													}
												}
											
												betweenessTotal = betweenessTotal + (double) (num_inc_paths / num_paths);
											}
											else{
												printf("reachNode1: Double_BFS didn't find any path!\n");
											}

											for(s=0; s < number_of_paths; s++){
												destroyList(&(pathSet[s]));
											}

											free(pathSet);
										}

									}

								}

								bucket2 = getNextBucket(g->hashtable.hashmap[k],bucket2);

							}

						}

					}

				}

				bucket1 = getNextBucket(g->hashtable.hashmap[i],bucket1);
			}

		}

	}

	return (betweenessTotal / ( (g->hashtable.num_records-2) * (g->hashtable.num_records-1) / 2));
}

/*
double averagePathLength(Graph* g) {

	int i,j,k,ID,counter;
	long nodePathSum = 0,totalPathSum = 0;
	double nodeAverage = 0,totalAverage = 0;
	ResultSet* result_set;
	Pair* pair;
	
	//checks
	if(g == NULL){
		printf("averagePathLength: HashMap in Graph is NULL!\n");
		exit(1);
	}

	if(g->hashtable.num_records == 0){
		printf("averagePathLength: HashTable in Graph is empty!\n");
		exit(1);
	}

	if(g->hashtable.hashmap == NULL){
		printf("averagePathLength: Graph is not initialised!\n");
		exit(1);
	}
	
	//for every node in graph	
	i=0;
	while (g->hashtable.hashmap[i] != NULL) { 
				
   				
   		// Find shortest paths to every other node 
   		//gia auta pou den exei monopati?
		while( next(result_set, pair) ){
			nodePathSum +=  pair->distance; // Add all distances from nodes it can reach
		}

		return (total_distance / (g->hashtable.num_records - 1) ); 

        			nodeAverage = (double)(nodePathSum/counter)				

}
*/

double density(Graph* g){

	double density = 0;

	if(g != NULL){
		if(g->hashtable.num_records > 0) {		
			if(g->hashtable.num_records > 1){ /*at least two nodes,otherwise denominator is zero*/
				density = (double)((2*g->num_edges)/(g->hashtable.num_records * (g->hashtable.num_records-1)));
				return density;
			}
			else {
				printf("density: Could not calculate because denominator is zero!Number of nodes must be > 1)");
				exit(1);
			}
		}
		else {
			printf("density: HashTable is Empty!\n");
			exit(1);
		}
	}
	else {
		printf("density: Graph is not initialised!\n");
		exit(1);
	}			

}



