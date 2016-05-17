/*
 * GraphQueries.c
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "GraphQueries.h"

/* Creates a graph based on schema's relation person_knows_person
 * bucketsNumber: size of hashtable
 * bucketSize: size of cells in a bucket
 * loads also from data file as attributes anything might be useful
 * mustn't save any query/metric estimation
 * return value: the loaded graph*/
Graph* loadGraph(int bucketsNumber, int bucketSize,int* errorFlag){
  return (Graph*) NULL;
}

/* Get the matches structure from query 1, the requested
 * pair number and returns the person
 * id. If there is no <pairNumber> result return -1
 */
int getMatch(int pairNumber,Matches* match,int* errorFlag){
  //TODO
  return 0;
}

/* Gets the stalker structure from query 2, a
 * position and returns the stalker's person id
 * at current position. If there is no <position> result return -1 
*/
int getStalkerID(int position,Stalkers* stalk,int* errorFlag){
  //TODO
  return 0;
}

/************************************/
/* Implement for metrics purposes */
/************************************/

typedef struct a {	
	int degree;			
	int amount;			// how many nodes with degree, 'degree', there are 
} degreeInfo;

int findDegree(degreeInfo* di, int degree) {
	int loop_limit=0, i, size=0;

	size = di[0].amount;							// get size of array
	loop_limit = di[0].degree;						// loop until reaching this last node
	for (i=1 ; i<=loop_limit ; i++)
		if (di[i].degree == degree) return i;
	if (di[size-1].degree != -1) return -2;			// array is full
	else return -1;									// array is not full
}

#define INITIAL_ELEMS 30

int degreeDistribution(Graph *g,int* errorFlag) {
	int degree, i, j, k, m, buckets_num, nodes_num, total_nodes, size=0, spot=1, count=0;
	BucketChain *chain;
	Bucket *bucket = (Bucket*) NULL;
	GraphNode *node;
	degreeInfo *di;

	assert(g != NULL);
	assert(g->hashtable.hashmap != NULL);	
	di = malloc(INITIAL_ELEMS * sizeof(degreeInfo));				
// ---------- initialise statistics array ------------	
	di[0].degree = 0;								// first node's 'degree' is the place of the last element
	di[0].amount = INITIAL_ELEMS;					// first node's 'amount' holds the size of the array
	for (i=1 ; i<INITIAL_ELEMS ; i++) {
		di[i].degree = -1;
		di[i].amount = 0;	
	}
// ---------------------------------------------------
	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);										// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE){
			return EXIT_VALUE;
		}
		bucket = getCurrentBucket(chain, errorFlag);
		if(*errorFlag == EXIT_VALUE){
			printf("buildTrustGraph: getNextEdge failed!\n");
			return EXIT_VALUE;
		}					
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (k=0 ; k<nodes_num ; k++) {							// and for each one of them
				assert (bucket->GNArray[k] != NULL);
				node = bucket->GNArray[k];					
				degree = getNumberOfGraphNodeEdges(node,errorFlag);
				if(*errorFlag == EXIT_VALUE){
					return EXIT_VALUE;
				} 	
				//printf("Found node %d with degree %d\n", node->id, degree);
//-----------------------------------------------------
				spot = findDegree(di, degree);						// determine the spot to insert
				if (spot == -2)	{									// array is full, needs realloc
					size = di[0].amount;
					di = (degreeInfo*) realloc(di, (size+1) * sizeof(degreeInfo));	// increase the size of array by one
					di[0].degree = size;							// new soon-to-be last element = size 
					di[0].amount ++;								// new size of array is incremented
					spot = size; assert(spot < size+1);				// spot is the new last place of array
				}
				else if (spot == -1) {								// degree doesn't exist in array and the latter isn't full
					di[0].degree ++;								// update the soon-to-be last place
					spot = di[0].degree;							// spot to insert is the new last place
				}
				di[spot].degree = degree;							
				di[spot].amount ++;									// update the quantity 
				/*printf("Printing degree distribution array...\n");
				for (m=0 ; m<di[0].amount ; m++) 
					printf("%d-%d| ", di[m].degree, di[m].amount);	
				printf("\n"); */
//-----------------------------------------------------
			}
			total_nodes += nodes_num;
			bucket = getNextBucket(chain, bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE){
				return EXIT_VALUE;
			}
		} 
	}

	return 0;

}

int diameter(Graph* g,int *errorFlag) { /*Returns greatest distance between any two nodes of the graph*/

	int i,j,ID,max=0;
	ResultSet* result_set;
	Pair pair;
	Bucket* bucket = (Bucket*) NULL;
	
	/*Checks*/
	if(g == NULL){
		printf("diameter: HashMap in Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.num_records == 0){
		printf("diameter: HashTable in Graph is empty!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("diameter: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	
	/*for every node in graph..*/	
	i=0;
	while (g->hashtable.hashmap[i] != NULL) { 
		
		bucket = getHeadBucket(g->hashtable.hashmap[i], errorFlag);
		if(*errorFlag == EXIT_VALUE){
			printf("diameter: getHeadBucket failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}		
		while(bucket != NULL) {	
			for(j=0; j < bucket->items; j++) {					
					
					ID = bucket->GNArray[j]->id;
					//printf("diameter:ID: %d\n",ID);
					
					/*..find distance to any other node*/
   					result_set = reachNodeN(ID,g,errorFlag);
					if(*errorFlag == EXIT_VALUE){
						printf("diameter: reachNodeN failed!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					} 							
					while( next(result_set,&pair, errorFlag) ) {
						if(*errorFlag == EXIT_VALUE){
							printf("diameter: next failed!\n");
							*errorFlag = EXIT_VALUE;
							return EXIT_VALUE;
						} 			
						if (pair.distance > max) {
							max = pair.distance;
						}
					}
					if(*errorFlag == EXIT_VALUE){
						printf("diameter: next failed!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					} 			
					
					printf("diameter:Max %d \n",max);		
			}
					
			bucket = getNextBucket(g->hashtable.hashmap[i],bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("diameter: getNextBucket failed!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			} 			
		}
		
		i++;	
	}
	
	return max;	

}

double averagePathLength(Graph* g, int* errorFlag) { /*Returns average distance between any two nodes of the graph*/

	int i,j,ID,counter=0;
	long nodePathSum = 0;
	double totalAverage = 0;
	ResultSet* result_set;
	Pair pair;
	Bucket* bucket = (Bucket*) NULL;
	
	/*checks*/
	if(g == NULL){
		printf("averagePathLength: HashMap in Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.num_records == 0){
		printf("averagePathLength: HashTable in Graph is empty!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("averagePathLength: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	
	/*for every node in graph..*/	
	i=0;
	while (g->hashtable.hashmap[i] != NULL) { 
		
		bucket = getHeadBucket(g->hashtable.hashmap[i],errorFlag);
		if(*errorFlag == EXIT_VALUE){
			printf("averagePathLength: getHeadBucket failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
	
		while(bucket != NULL) {	
			for(j=0; j < bucket->items; j++) {
					
					ID = bucket->GNArray[j]->id;
					//printf("averagePathLength:ID: %d\n",ID);
   					
   					/*..find distance to any other node*/
   					result_set = reachNodeN(ID,g,errorFlag); 
					if(*errorFlag == EXIT_VALUE){
						printf("diameter: reachNodeN failed!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					} 										
					while( next(result_set,&pair,errorFlag) ) {
						if(*errorFlag == EXIT_VALUE){
							printf("diameter: next failed!\n");
							*errorFlag = EXIT_VALUE;
							return EXIT_VALUE;
						} 							
						nodePathSum +=  pair.distance; 
						counter++;
					}
					if(*errorFlag == EXIT_VALUE){
						printf("diameter: next failed!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					} 							
					totalAverage += ((double)nodePathSum)/counter;
					//printf("Average: %f \n",totalAverage);
					nodePathSum = 0;
					counter = 0;			
			}
					
			bucket = getNextBucket(g->hashtable.hashmap[i],bucket,errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("diameter: getNextBucket failed!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			} 						
		}
		
		i++;	
	}	
	return totalAverage	;	
}

int numberofCCs(Graph* g,int* errorFlag){ /* Returns the number of coherent subgraphs in the given Graph */

	int max_size=0;
	int numberofCC=0;

	if(g != NULL){
		if(g->hashtable.num_records != 0){
			if(g->hashtable.hashmap != NULL){
				numberofCC = findCoherentGraphs(g, &max_size,NULL,0,errorFlag); /* findCoherentGraphs returns the number of coherent subgraphs it has found */
				if(*errorFlag == EXIT_VALUE){
					printf("numberofCCs: findCoherentGraphs failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				return numberofCC;
			}
			else{
				printf("numberofCCs: HashMap in Graph is NULL!\n");
				return EXIT_VALUE;
			}
		}
		else{
			printf("numberofCCs: HashTable in Graph is empty!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("numberofCCs: Graph is not initialised!\n");
		return EXIT_VALUE;
	}

}

int maxCC(Graph* g,int* errorFlag){ /* Returns the number of nodes the biggest coherent subgraph in the given graph has */

	int max_size=0;

	if(g != NULL){
		if(g->hashtable.num_records != 0){
			if(g->hashtable.hashmap != NULL){
				findCoherentGraphs(g, &max_size,NULL,0, errorFlag); /* findCoherentGraphs returns the number of coherent subgraphs it has found */
				if(*errorFlag == EXIT_VALUE){
					printf("maxCC: findCoherentGraphs failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				return max_size;
			}
			else{
				printf("maxCC: HashMap in Graph is NULL!\n");
				return EXIT_VALUE;
			}
		}
		else{
			printf("maxCC: HashTable in Graph is empty!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("maxCC: Graph is not initialised!\n");
		return EXIT_VALUE;
	}

}

double density(Graph* g,int* errorFlag) { /*Returns density of a non-directed graph*/

	double density = 0;
	
	if(g != NULL){
		if(g->hashtable.num_records > 0) {		
			if(g->hashtable.num_records > 1){ /*at least two nodes,otherwise denominator is zero*/
				printf("Edges: %ld Nodes: %ld \n",g->num_edges,g->hashtable.num_records);
				density =((double)(2*g->num_edges)/(g->hashtable.num_records * (g->hashtable.num_records-1)));

				return density;
			}
			else {
				printf("density: Could not calculate because denominator is zero!Number of nodes must be > 1)");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
		}
		else {
			printf("density: HashTable is Empty!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
	}
	else {
		printf("density: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}			

}

double closenessCentrality(GraphNode* gnode, Graph* g,int* errorFlag){ /* Will calculate the closeness centrality of a given node */
 
        ResultSet* result_set = (ResultSet*) NULL;
        Pair currentResult;
        double total_distance = 0.0;
 
        if(g == NULL){
                printf("closenessCentrality: HashMap in Graph is NULL!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        if(g->hashtable.num_records == 0){
                printf("closenessCentrality: HashTable in Graph is empty!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        if(g->hashtable.hashmap == NULL){
                printf("closenessCentrality: Graph is not initialised!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        if(lookUpNode(gnode->id,g,errorFlag) == NULL){
                printf("closenessCentrality: GraphNode does not exist in Graph!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        if(*errorFlag == EXIT_VALUE){
                printf("closenessCentrality: lookUpNode failed!!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        result_set = reachNodeN(gnode->id, g, errorFlag); /* Prepare to reach all possible to reach nodes from this node */
 
        if(*errorFlag == EXIT_VALUE){
                printf("closenessCentrality: reachNodeN failed!!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        while( next(result_set, &currentResult, errorFlag) ){
                if(*errorFlag == EXIT_VALUE){
                        printf("closenessCentrality: next failed!\n");
                        *errorFlag = EXIT_VALUE;
                        return EXIT_VALUE;
                }
                total_distance = total_distance + (double) (1 / (currentResult.distance) ); /* Add all distances from nodes it can reach */
        }
 
        if(*errorFlag == EXIT_VALUE){
                printf("closenessCentrality: next failed!\n");
                *errorFlag = EXIT_VALUE;
                return EXIT_VALUE;
        }
 
        return (total_distance / (g->hashtable.num_records - 1) ); /* Normalise */
 
}


double betweennessCentrality(GraphNode* gnode, Graph* g,int* errorFlag){ //TODO: SPEED UP IN UNIDIRECTION

	int i=0,ID1,ID2,num_buckets1=0,num_buckets2=0,num_items1=0,num_items2=0,num_paths=0,num_inc_paths=0,success,number_of_paths,total_distance,j,k,t,s;
	double betweenessTotal=0.0;
	Bucket* bucket1 = (Bucket*) NULL,* bucket2 = (Bucket*) NULL;
	List* pathSet = (List*) NULL;

	if(g == NULL){
		printf("betweennessCentrality: HashMap in Graph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(g->hashtable.hashmap == NULL){
		printf("betweennessCentrality: Graph is not initialised!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(lookUpNode(gnode->id,g,errorFlag) == NULL){
		printf("betweennessCentrality: GraphNode does not exist in Graph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(*errorFlag == EXIT_VALUE){
		printf("betweennessCentrality: GraphNode does not exist in Graph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	for(i=0; i < g->hashtable.size; i++){ //TODO: CUT DUPLICATES / DIPLOTUPA - PATH APO I SE J == PATH APO J SE I
		
		if(g->hashtable.hashmap[i] != NULL){
			
			bucket1 = getHeadBucket(g->hashtable.hashmap[i], errorFlag);
			if(*errorFlag == EXIT_VALUE){
				return EXIT_VALUE;
			}
			while(bucket1 != NULL){
		
				for(j=0; j < bucket1->items; j++){
					if(bucket1->GNArray[j] == NULL) continue;
					for(k = 0; k < g->hashtable.size; k++){
						
						if(g->hashtable.hashmap[k] != NULL){
							
							bucket2 = getHeadBucket(g->hashtable.hashmap[k], errorFlag);
							if(*errorFlag == EXIT_VALUE){
								return EXIT_VALUE;
							}
							while(bucket2 != NULL){
								
								for(t=0; t < bucket2->items; t++){ /* To cut off duplicates id1 < id2 */
									if(bucket2->GNArray[t] == NULL) continue;
									if((bucket1->GNArray[j]->id != bucket2->GNArray[t]->id) && (bucket1->GNArray[j]->id != gnode->id)){

										pathSet = (List*) NULL;
										success = 0;
										total_distance = 0;
										number_of_paths = 0;

										pathSet = (Double_BFS_Graph(g,bucket1->GNArray[j]->id,bucket2->GNArray[t]->id,&success,&total_distance,&number_of_paths,PATH_CONSTRUCTION,errorFlag));

										if(*errorFlag == EXIT_VALUE){
											printf("betweennessCentrality: DOUBLE_BFS Failed!\n");
											*errorFlag = EXIT_VALUE;
											return EXIT_VALUE;
										}

										if(pathSet != NULL){
											if(success == 1){
												num_paths = 0;
												num_inc_paths = 0;
												Node* myCurrent = (Node*) NULL;
												List* myPath = (List*) NULL;
												myCurrent = pathSet->Head;
												while(myCurrent != NULL){
													myPath = (List*) myPath;
													if(myPath != NULL){
														if(nodeIsInPath(gnode->id,*myPath) == 1){
															num_inc_paths++;
														}
														num_paths++;
													}
													myCurrent = myCurrent->next;
												}
	
												betweenessTotal = betweenessTotal + (double) (num_inc_paths / num_paths);
											}
											else{
												printf("reachNode1: Double_BFS didn't find any path!\n");
											}
		
											destroyList(pathSet);

											free(pathSet);
											
										}

									}

								}

								bucket2 = getNextBucket(g->hashtable.hashmap[k],bucket2,errorFlag);

								if(*errorFlag == EXIT_VALUE){
									printf("betweennessCentrality: getNextBucket Failed!\n");
									*errorFlag = EXIT_VALUE;
									return EXIT_VALUE;
								}


							}

						}

					}

				}

				bucket1 = getNextBucket(g->hashtable.hashmap[i],bucket1,errorFlag);

				if(*errorFlag == EXIT_VALUE){
					printf("betweennessCentrality: getNextBucket Failed!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}

			}

		}

	}

	return (betweenessTotal / ( (g->hashtable.num_records-2) * (g->hashtable.num_records-1) / 2));
}

/************************************/
/* Implement for queries purposes */
/************************************/

/////////////////////////////////////////////////QUERY 1///////////////////////////////////////////////////////////////////
Matches* matchSuggestion(GraphNode* ptrN,int k,int h,int x,int limit,Graph* ptrG,int* errorFlag) {

	int bachelorID=-1, bachelorPlaceID=-1, bachelorOrganisationID=-1, bachelorTagID=-1, bachelorYearOfBirth=0;	
	int *matchPtr, matchPlaceID=-1, matchOrganisationID=-1, matchTagID=-1, matchYearOfBirth =0;
	char* bachelorGender=NULL, *Birthday=NULL,*tok=NULL;

	int foundIsLocatedIn=0, foundStudyAt=0, foundWorkAt=0, isAround=0, inserted=0, tagCounter=0, i;
	int foundHasInterest=0, foundMatchHasInterest=0, foundMatchIsLocatedIn=0, foundMatchOrganisation =0;
	
	List bachelorOrganisationsList,bachelorTagsList;
	GraphNode* matchNodePtr;
	
	void* index;
	double jaccard;	
	
	Matches MatchesList, *matchesPtr;	//Matches is  a List
	Match* newMatch = NULL, *currentMatch = NULL;
	Pair pair;
	ResultSet* result_set;
	Edge* edgeptr=NULL;
	EdgeSet* currentEdgeSet=NULL;
	


	//////////////////Find Bachelor's properties/////////////////
	createList(&bachelorOrganisationsList,"int",&allocateInt,&destroyInt,&compareInts,&assignInt,&printInt);
	createList(&bachelorTagsList,"int",&allocateInt,&destroyInt,&compareInts,&assignInt,&printInt);
	
	/*Get Bachelor's ID*/
	if(ptrN != NULL) {
		bachelorID = ptrN->id;
		printf("\nmatchSuggestion:Person with ID %d ",bachelorID);
		
	}
	else {
		printf("ID not found!\n");
		*errorFlag = EXIT_VALUE;
		return ((Matches*)NULL);
	}
	
	
	/*Get Bachelor's gender*/
	if(ptrN->properties->arrayP[2].attribute != NULL) { //we know that gender is the 4th property of a person
		bachelorGender = malloc(sizeof(char) * (strlen(ptrN->properties->arrayP[2].attribute) + 1) );
		strcpy(bachelorGender,ptrN->properties->arrayP[2].attribute);
		printf("is a single %s ",bachelorGender);
	}
	else {
		printf("matchSuggestion:No information provided about gender of %d !\n",bachelorID);
	}
	
	
	/*Get Bachelor's Year of Birth*/
	if(ptrN->properties->arrayP[3].attribute != NULL) { //we know that birthday is the 5th property of a person
		Birthday = malloc(sizeof(char) * (strlen(ptrN->properties->arrayP[3].attribute) + 1) );
		strcpy(Birthday,ptrN->properties->arrayP[3].attribute);
		tok = strtok(Birthday, "-");
		bachelorYearOfBirth = atoi(tok);
		printf("born in %d\n",bachelorYearOfBirth);
		//printf("matchSuggestion: bachelor's year of birth: %s\n",bachelorGender);
	}
	else {
		printf("No information provided about birthday of %d !\n",bachelorID);
	}
	
	
	/*Find in which place Bachelor is Located in
	 Bachelor is located only in one place*/
	 
	ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Head; 
	while(ptrN->EdgeSetList.Current != NULL){
		currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
		if(!strcmp(currentEdgeSet->relation,"isLocatedIn")) { 
			foundIsLocatedIn = 1;
			//printf("matchSuggestion:isLocatedIn found!\n");
			edgeptr = currentEdgeSet->edges.Head->data	;	   
			break;
		}
		ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Current->next;
			
	}
	if(edgeptr != NULL) { 						// Bachelor is Located only in one place
			bachelorPlaceID = edgeptr->endNodeID;
			printf("matchSuggestion: PlaceID: %d\n",bachelorPlaceID);
	}
	else {
		printf("No information provided about in which place %d is located in!\n",bachelorID);
	}

		
	/*Find the List of organisations Bachelor works or studies at.	
	Bachelor can work or study at many organisations.
	Store all these organisations in a list to compare later
	with potentional matches*/
		
	ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Head;
	while(ptrN->EdgeSetList.Current != NULL) { 
		currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
		if (   (!strcmp(currentEdgeSet->relation,"studyAt")) || (!strcmp(currentEdgeSet->relation,"workAt")) ){ 
					
			if (!strcmp(currentEdgeSet->relation,"studyAt")) { 			//keep separete flags in case we ever need to check if he/she works or studies
				foundStudyAt = 1;	
				//printf("matchSuggestion:studyAt found!\n");
			}
			else {
				foundWorkAt = 1;
				//printf("matchSuggestion:workAt relation found!\n");
			}
			
			/*Find all edges of this relation*/
			currentEdgeSet->edges.Current = currentEdgeSet->edges.Head;
			while(currentEdgeSet->edges.Current != NULL){ 			
				edgeptr = currentEdgeSet->edges.Current->data;
				if(edgeptr != NULL) { 						
					bachelorOrganisationID = edgeptr->endNodeID;
					//printf("matchSuggestion: bachelorOrganisationID: %d\n",bachelorOrganisationID); 
					
					/*Store all IDs together.We dont'care if he/she is studying or working at the organisation*/ 
					if(addLastList(&bachelorOrganisationsList,&bachelorOrganisationID)== EXIT_VALUE){
						printf("matchSuggestion: addLastList failed!\n");
						*errorFlag = EXIT_VALUE;
						return ((Matches*) NULL);
					}
				}
				currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;
			}   
		}		
		if((foundStudyAt) && (foundWorkAt)) { 								//if both relations are found dont search thre rest of the EdgeList
			break;
		}
		ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Current->next;
	}
	if((!foundStudyAt) && (!foundWorkAt)) { /*no organisations found*/
		printf("No information provided about at which organisation %d is working or studying at!\n",bachelorID);
	}
	else {
		printf("matchSuggestion: OrganisationIDs:");
		printList(bachelorOrganisationsList);
		printf("\n");
	}
	
	
	/*Find the List of Bachelor's interests */
	ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Head;
	while(ptrN->EdgeSetList.Current != NULL){ 
		currentEdgeSet = (EdgeSet*) ptrN->EdgeSetList.Current->data;
		if (!strcmp(currentEdgeSet->relation,"hasInterest")){ 		
			foundHasInterest = 1;
			//printf("matchSuggestion:HasInterest found!\n");			
			
			/*Find all edges of this relation*/
			currentEdgeSet->edges.Current = currentEdgeSet->edges.Head;
			while(currentEdgeSet->edges.Current != NULL){ 								//while there are edges
				edgeptr = currentEdgeSet->edges.Current->data;
				if(edgeptr != NULL) { 													//if there are data in the edge
					bachelorTagID = edgeptr->endNodeID;
					//printf("matchSuggestion: bachelorTagID: %d\n",bachelorTagID); 
					
					/*Store all interest tags' IDs*/
					if(addLastList(&bachelorTagsList,&bachelorTagID) == EXIT_VALUE){
						printf("matchSuggestion: addLastList failed!\n");
						*errorFlag = EXIT_VALUE;
						return ((Matches*) NULL);
					}
					
					
				}
				currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;
			} 																			//all edges of hasInterest added  

		}
				
		if(foundHasInterest) { 															//if relation found don't search the rest of the EdgeList
			break;
		}
		ptrN->EdgeSetList.Current = ptrN->EdgeSetList.Current->next;

	}
	
	if(!foundHasInterest){ 																//no interest found
		printf("No information provided about which Tags %d is interested in!\n",bachelorID);
	}
	else {
		printf("matchSuggestion: TagsIDs:");
		printList(bachelorTagsList);
		printf("\n\n");
	}
	///////////////////////////////////////////////////////////////////////////
	
	/*Create List of Matches IDs.
	List is ordered (Matches are being inserted according to their similarity score)
	and has max size of limit,because we only want to return the top-limit Matches*/
	
	createList(&MatchesList,"int",&allocateMatch,&destroyMatch,&compareMatch,&assignMatch,&printMatch); 
	
	/*Find a person that is maximum h hops away = potentional match*/
	result_set =reachNodeN(bachelorID,ptrG,errorFlag);
	if(*errorFlag == EXIT_VALUE) {
		printf("matchSuggestion:reachNodeN failed!\n");
		*errorFlag = EXIT_VALUE;
		return ((Matches*)NULL);
	} 							
	while( next(result_set,&pair,errorFlag) ) {
		if(*errorFlag == EXIT_VALUE) {
			printf("matchSuggestion:next failed!\n");
			*errorFlag = EXIT_VALUE;
			return ((Matches*)NULL);
		} 
	
        if(pair.distance <= h) { 
        	//printf("Distance %d <= %d \n",pair.distance,h);
			matchNodePtr = lookUpNode(pair.ID,ptrG,errorFlag);
			if(*errorFlag == EXIT_VALUE) {
				printf("matchSuggestion:lookupNode failed!\n");
				*errorFlag = EXIT_VALUE;
				return ((Matches*)NULL);
			} 
			//printf("MatchNode has ID %d\n",matchNodePtr->id);
			
			/*Find properties of this potentional match
			Start by exluding persons of the same gender and with age difference greater than 'x' 
			in order to avoid comparisons in the organisation and interest list*/
			
			/*if this person's gender is different ...*/			
			if(matchNodePtr->properties->arrayP[2].attribute != NULL) { 		//(although we have already checked that properties are set correclty
				if(strcmp(bachelorGender,matchNodePtr->properties->arrayP[2].attribute)){
					
					/*if the age differece with this person is less than x....*/
					if(matchNodePtr->properties->arrayP[3].attribute != NULL) { //we know that birthday is always the 4th property of a person
						strcpy(Birthday,matchNodePtr->properties->arrayP[3].attribute);
						tok = strtok(Birthday, "-");
						matchYearOfBirth = atoi(tok);
						if(abs(bachelorYearOfBirth-matchYearOfBirth) <= x) {
							//printf("matchSuggestion: Potentional match is a %s ",(char*)matchNodePtr->properties->arrayP[2].attribute);
							//printf("born in %d! \n",matchYearOfBirth);
							
							/*Find relations of potentional Match
							First we search all EdgeSets of the potentional matchfor 'isLocatedIn','studyAt','workAt' to narrow down the results
							and then we will search for the 'isInterestedIn'*/
							
						
							matchNodePtr->EdgeSetList.Current = matchNodePtr->EdgeSetList.Head;
							
							while(matchNodePtr->EdgeSetList.Current != NULL) { 		//if there is a List of EdgeSets
								currentEdgeSet = (EdgeSet*) matchNodePtr->EdgeSetList.Current->data;
								
								/*Place or organisation*/
														
								/*Find the place where potentional match is located in*/
								if (!strcmp(currentEdgeSet->relation,"isLocatedIn")){	
									foundMatchIsLocatedIn= 1;
									//printf("matchSuggestion: isLocatedIn of Match found!\n");			
									
									
									edgeptr = currentEdgeSet->edges.Head->data; 	//only one edge of this relation								
									if(edgeptr != NULL) { 														
													
											matchPlaceID = edgeptr->endNodeID;		//find the ID of the place..
																	
											if(matchPlaceID == bachelorPlaceID) { 	//and check if  this place is where bachelor is located
												isAround = 1;
												printf("Match %d is located also in place : %d!\n",matchNodePtr->id,matchPlaceID);										
												break; 								//we dont need to also check organisation
											}
											else {
												//printf("Match not located in the same place...\n");
											}
									}
								}									
								/*Find organisations where potentional match works or studies at*/
								if ( (!strcmp(currentEdgeSet->relation,"workAt")) || (!strcmp(currentEdgeSet->relation,"studyAt")) ){ 	 
									foundMatchOrganisation = 1;
									//printf("matchSuggestion: workAt or studyAt of Match found!\n");			
									
									
									currentEdgeSet->edges.Current = currentEdgeSet->edges.Head;
									while(currentEdgeSet->edges.Current != NULL){ 	 			//for every edge of the relation..
										edgeptr = currentEdgeSet->edges.Current->data;
										if(edgeptr != NULL) { 	
																
											matchOrganisationID = edgeptr->endNodeID;			//find the ID of the organisation..									
																								 //and search if this tag exists in bachelorTagsList
											if(searchList(bachelorOrganisationsList,&matchOrganisationID,&index)) { 
												isAround =1;
												printf("Match %d works or studies at organisation %d too!\n",matchNodePtr->id,matchOrganisationID);
												break; 											//one organisation in common is enough
												
											}
										}										
										currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;
										
									}															//found every edge of this relation of this match						
								}
								matchNodePtr->EdgeSetList.Current = matchNodePtr->EdgeSetList.Current->next;
					
							}
										
							
							if(isAround) { //Only if the match is located in the same place or study/work at same organisation (isAround =1)
							
							/*Find interests of potentional match*/

							matchNodePtr->EdgeSetList.Current = matchNodePtr->EdgeSetList.Head;
							while(matchNodePtr->EdgeSetList.Current != NULL){ 
								currentEdgeSet = (EdgeSet*) matchNodePtr->EdgeSetList.Current->data;
								if (!strcmp(currentEdgeSet->relation,"hasInterest")){ 		
									foundMatchHasInterest = 1;
									//printf("matchSuggestion:HasInterest of Match found!\n");			
									
									
									currentEdgeSet->edges.Current = currentEdgeSet->edges.Head;
									while(currentEdgeSet->edges.Current != NULL){ 					//for every edge of 'hasInterest'. 
										edgeptr = currentEdgeSet->edges.Current->data;
										if(edgeptr != NULL) { 								
																					
											matchTagID = edgeptr->endNodeID;						//find the ID of the tag..
											
																									//and search if this tag exists in bachelorTagsList
											if(searchList(bachelorTagsList,&matchTagID,&index)) { 	//no need for index.create exist List function?
												tagCounter++;
												printf("Match %d has %d tag in common!\n",matchNodePtr->id,matchTagID);
												//printf("tagCounter %d\n",tagCounter);
												
											}
										}										
										currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;
										
									}																//found every 'hasInterest' edge for this match
				
				
									/*If this match has at least k common interests with our bachelor..	*/	
									if(tagCounter >=  k ) { 
										
										printf(" %d is  a match!\n",matchNodePtr->id);
										printf("Common tags %d ,total tags of Bachelor: %d total tags of Match: %d\n",tagCounter,bachelorTagsList.items,currentEdgeSet->edges.items);
										
										/*Find similarity score of tags using Jaccard*/
										jaccard = tagCounter / ((double)(bachelorTagsList.items + currentEdgeSet->edges.items - tagCounter ));
										printf("Jaccard: %2f \n",jaccard);									
										
										/*If Matches List is empty,allocate a Match and insert at Head of the List without comparing with the similarity score*/
										if(isEmptyList(MatchesList)) { 
											
											/*allocate a new Match*/
											newMatch = malloc(sizeof(struct Match));
											if(newMatch == NULL) {
												printf("matchSuggestion:allocate new match failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Matches*)NULL);
											} 
											newMatch->ID = matchNodePtr->id;
											newMatch->similarity_score = jaccard;

											/*insert first in Matches*/ 
											addFirstList(&MatchesList,newMatch); 
										}
										else {
										
											MatchesList.Current = MatchesList.Head;
											//printf("There is already a match in the list!\n");
											while(MatchesList.Current != NULL && inserted == 0) { 	//stop if end of the list is reached 
																									//or current Match is inserted
												
												currentMatch = (Match*)(MatchesList.Current->data);
												//printf("New match score is %f and old match is %f \n",jaccard,currentMatch->similarity_score);
												
												/*If current Match has similarity score greater than the Current Node of List,allocate a Match*/
												if (jaccard >= currentMatch->similarity_score ) { 
													
													/*allocate a new Match*/
													newMatch = malloc(sizeof(struct Match));
													if(newMatch == NULL) {
														printf("matchSuggestion:allocate new match failed!\n");
														*errorFlag = EXIT_VALUE;
														return ((Matches*)NULL);
													} 
													newMatch->ID = matchNodePtr->id;
													newMatch->similarity_score = jaccard;
										
													/*If current Node is Head,insert Match at Head of the List*/
													if(MatchesList.Current == MatchesList.Head) {
														
														printf("Bigger than Head.Add first\n");
														addFirstList(&MatchesList,newMatch);
														inserted = 1 ;
													}
													/*If current Node is in the middle of the List,insert Match in the middle*/
													else { 
															currentMatch = (Match*)(MatchesList.Last->data);
															//insert in the Middle of Matches
															printf("Bigger than Current.Add middle\n");
															addMiddleList(&MatchesList,MatchesList.Current,newMatch);
															inserted =1;

													}
												
											
												}
												MatchesList.Current = MatchesList.Current->next; 
											}												//last Node of Matches List reached
											
											/*If Match not inserted (has the smallest similarity score),but there is still space in the Matches List
											insert Match at the end of the List*/
											if(inserted == 0 && MatchesList.items < limit) { 
												
												/*allocate a new Match*/
												newMatch = malloc(sizeof(struct Match));
												if(newMatch == NULL) {
													printf("matchSuggestion:allocate new match failed!\n");
													*errorFlag = EXIT_VALUE;
													return ((Matches*)NULL);
												} 
												newMatch->ID = matchNodePtr->id;
												newMatch->similarity_score = jaccard;
																						
												//printf("Smaller than all but no limit reached.Add last\n");			
												addLastList(&MatchesList,newMatch); 
												inserted =1;
												
											}
										}
										/*If an insertion caused an overflow to the list,remove the last node(the one with the smallest similarity score*/
										if(MatchesList.items > limit) { 
											removeLastList(&MatchesList);
										}

										tagCounter = 0;
										inserted = 0;									
									}
									else {
										printf("Not enough common interests found with %d\n",matchNodePtr->id);
									}
								}
								if(foundMatchHasInterest) { 
									break; 								//don't search the other EdgeSets ,found 'hasInterest'
								}
								tagCounter = 0;
								matchNodePtr->EdgeSetList.Current = matchNodePtr->EdgeSetList.Current->next; //next Edgeset
								
							}											//all EdgeSets searched for 'hashInterest'
							
							foundMatchHasInterest = 0;//?
							isAround = 0;
							
							}//isAround
							
							else  {
								//printf("Didn't check for %d interests cause he/she is not in the same place or organisation!\n",matchNodePtr->id);
							}
						}					
					}
				}			
			}	
		}//distance <= h							
	
	}//resultset
	
	if (Birthday != NULL) {
		free(Birthday);
	}
	if(newMatch != NULL) {
		free(newMatch);
	}
	if(!isEmptyList(bachelorOrganisationsList)) {
		destroyList(&bachelorOrganisationsList);
	}
	if(!isEmptyList(bachelorTagsList)) {
		destroyList(&bachelorTagsList);
	}
	
	
	if(isEmptyList(MatchesList)) {
		printf("matchSuggestion: No matches found in Matches!\n");
		exit(1);
	}
	else {		
		printList(MatchesList);		
		printf("\n %d matches found!\n",MatchesList.items);
		//return &MatchesList;
		destroyList(&MatchesList);
	}
		

}

				   /* ###########################################################
			 		  ##						JORGE STUFF					   ##
			 		  ########################################################### */



/* ##############################
   ##			Q2			   ##	
   ############################## */


int whoPostedThis(int postId, Graph *g, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	int i=0, j=0, m=0, n=0, q=0, buckets_num=0, nodes_num=0, total_nodes=0, creator=0, post=0;

	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;	
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;					
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m];
				//-----------search for EdgeSets-----------
				node->EdgeSetList.Current = node->EdgeSetList.Head;						// go to begining of EdgeSet list
				for (n=0 ; n< node->EdgeSetList.items ; n++) {		
					es = (EdgeSet*) node->EdgeSetList.Current->data;			// receive current value
					assert (es != NULL);
					assert (es->relation != NULL);
					if (!strcmp(es->relation, "createdPost")) { 	// search for person_createdPost EdgeSet
						//eList = es->edges; //Na auto einai ena tetoio shmeio
						es->edges.Current = es->edges.Head;
						for (q=0 ; q< es->edges.items ; q++) {
							e = (Edge*) es->edges.Current->data;
							assert (e != NULL);
							creator = e->startNodeID;
							post = e->endNodeID;
							if (post == postId) return creator;
							es->edges.Current = es->edges.Current->next;	// else go to next post
						}
					}
					node->EdgeSetList.Current = node->EdgeSetList.Current->next;			// go to next EdgeSet node of this list
				}
			}
			total_nodes += nodes_num;
			bucket = getNextBucket(chain, bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE) 
				return EXIT_VALUE;	
		} 
	} 
	return -2;														// no such post was found, as 'postId'
}


int acquainted (int oneId, int anotherId, Graph *g, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	int i=0, j=0, m=0, n=0, q=0, buckets_num=0, nodes_num=0, total_nodes=0, creator=0, post=0;

	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;	
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;			
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m];
				//-----------search for EdgeSets-----------
				node->EdgeSetList.Current = node->EdgeSetList.Head;				// go to begining of EdgeSet list
				for (n=0 ; n<node->EdgeSetList.items ; n++) {		
					es = (EdgeSet*) node->EdgeSetList.Current->data;			// receive current value
					assert (es != NULL);
					assert (es->relation != NULL);
					if (!strcmp(es->relation, "knows")) { 			
						es->edges.Current = es->edges.Head;
						for (q=0 ; q<es->edges.items ; q++) {
							e = (Edge*) es->edges.Current->data;
							assert (e != NULL);
							if ( (oneId == e->startNodeID) && (anotherId == e->endNodeID) ) return 1;
							else if ( (oneId == e->endNodeID) && (anotherId == e->startNodeID) ) return 1;
							es->edges.Current = es->edges.Current->next;		// else go to next post
						}
					}
					node->EdgeSetList.Current = node->EdgeSetList.Current->next;	// go to next EdgeSet node of this list
				}
			}
			total_nodes += nodes_num;
			bucket = getNextBucket(chain, bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE) 
				return EXIT_VALUE;	
		} 
	} 
	return 0;														// these two IDs are not acquainted
}

////////////////////////////STRUCTURE TO COUNT SMONE'S LIKES
typedef struct b {
	int stalker;
	int victim;
	int likes;
} creepyData;


int findStalker (creepyData *creep, int stalker, int victim) {
	int loop_limit=0, i, size=0;

	size = creep[0].victim;							// get size of array
	loop_limit = creep[0].stalker;					// loop until reaching this last node
	for (i=1 ; i<=loop_limit ; i++)
		if ( (creep[i].stalker == stalker) && (creep[i].victim == victim) ) return i;
	if (creep[size-1].stalker != -1) return -2;		// array is full
	else return -1;									// array is not full
}
/////////////////////////////////////////////////////////////

void insertStalker(Stalkers *st, Stalkers value, int MEG) {
	int i, j, spot=-1;	

	assert (st != NULL);
	// spot a place to insert
	for (i=0 ; i<MEG ; i++) 
		if (value.centralityScore > st[i].centralityScore) {	// descending order 
			spot = i;											// must be placed on 'spot'
			break;
		}
	if (spot == -1) return;										// abort if there is no place for a number
	// move all the rest one place to the right - dispose of far-right element
	for (i=MEG-2 ; i>=spot ; i--) {	 
		if (st[i].centralityScore == -1) continue;
		st[i+1].centralityScore = st[i].centralityScore;
		st[i+1].id = st[i].id; 
	}
	// place new element to the proper place
	st[spot].centralityScore = value.centralityScore;
	st[spot].id = value.id; 
	/*for (j=0 ; j<MEG ; j++)
		printf("%.0f\n", st[j].centralityScore); 
	printf("\n\n"); */
}


Graph *getTopStalkers(int k, int x, int centralityMode, Graph *g, Stalkers *st, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	creepyData *creep=NULL; 
	Stalkers timCook;
	int i=0, j=0, m=0, n=0, q=0, v=0, spot=0, size=0, buckets_num=0, nodes_num=0, total_nodes=0;
	int accused=0, likedPost=0, victim=0, acquainted_=0, myErrorFlag=0, myErrorFlag2=0;

	creep = malloc(INITIAL_ELEMS * sizeof(creepyData));
	assert (creep != NULL);
// ---------- initialise statistics array ------------	
	creep[0].stalker = 0;										// first node's 'stalker' is the place of the last element
	creep[0].victim = INITIAL_ELEMS;							// first node's 'victim' holds the size of the array
	for (i=1 ; i<INITIAL_ELEMS ; i++) {
		creep[i].stalker = -1;
		creep[i].victim = -1;
		creep[i].likes = 0;	 
	}
// ---------- initialise Stalkers array ------------	
	for (i=0 ; i<k ; i++) { 
		st[i].id = -1;
		st[i].centralityScore = -1;
	}
// ---------------------------------------------------
	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE)
			return ((Graph*) NULL);
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE)
			return ((Graph*) NULL);		
// -------------- check all the graph ----------------
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m];	
			//-----------search for EdgeSets-----------
				node->EdgeSetList.Current = node->EdgeSetList.Head;				// go to begining of EdgeSet list
				for (n=0 ; n < node->EdgeSetList.items ; n++) {		
					es = (EdgeSet*) node->EdgeSetList.Current->data;			// receive current value
					assert (es != NULL);
					assert (es->relation != NULL);
					if (!strcmp(es->relation, "likes")) { 						// search for person_likes_post EdgeSet
						es->edges.Current = es->edges.Head;
						for (q=0 ; q<es->edges.items ; q++) {		
							e = (Edge*) es->edges.Current->data;
							assert (e != NULL);
							accused = e->startNodeID;							// who is the person that likes smone's post
							likedPost = e->endNodeID;							// what is this post's id
							victim = whoPostedThis(likedPost, g, &myErrorFlag);	// who is the person that posted the liked post
							if (victim == EXIT_VALUE) {
								if(creep != NULL) free(creep);
								return ((Graph*) NULL);
							}
							assert (victim != -2);								// if negative, then noone posted this
							acquainted_ = acquainted(accused, victim, g, &myErrorFlag);
							if (acquainted_ == EXIT_VALUE) {
								if(creep != NULL) free(creep);
								return ((Graph*) NULL);
							}
							//printf("Person %d liked post %d written by %d %s\n", accused, likedPost, victim, (acquainted_) ? "" : "(NOT)"); 
							if (acquainted_) { 									// if they know each other, skip
								es->edges.Current = es->edges.Current->next;
								continue;
							}
						// ---------------- update creepy data ---------------
							spot = findStalker(creep, accused, victim);			// determine the spot to update data
							if (spot == -2)	{									// array is full, needs realloc
								size = creep[0].victim;
								creep = (creepyData*) realloc(creep, (size+1) * sizeof(creepyData));	// increase the size of array by one
								creep[size].likes=0;
								creep[0].stalker = size;						// new soon-to-be last element = size 
								creep[0].victim ++;								// new size of array is incremented
								spot = size; assert(spot < size+1);				// spot is the new last place of array
							}
							else if (spot == -1) {								// creep doesn't exist in array and the latter isn't full
								creep[0].stalker ++;							// update the soon-to-be last place
								spot = creep[0].stalker;						// spot to insert is the new last place
							}
							creep[spot].stalker = accused;						// update the stalker id			
							creep[spot].victim = victim;						// update the victim id			
							creep[spot].likes ++;								// update the quantity of likes  
						// ---------------- update stalker data ---------------
							if (creep[spot].likes <= x)	{						// if the number of likes doesn't exceed 'x', skip
								es->edges.Current = es->edges.Current->next;
								continue;		
							}
							timCook.id = accused;								// timCook - imaginary stalker, rightly accused past this point	
							switch (centralityMode) {							// asign proper centrality
								case 1: timCook.centralityScore = closenessCentrality(node, g, &myErrorFlag2);
										break;
								case 2: timCook.centralityScore = betweennessCentrality(node, g, &myErrorFlag2);	
										break;
							}
							insertStalker(st, timCook, k);						// update top stalkers data
							for (j=0 ; j<k ; j++)
								printf("%.0f\n", st[j].centralityScore); 
							printf("\n\n");
							
							es->edges.Current = es->edges.Current->next;
						}
					}printf("1111111111111111111111111111\n");
					assert (node->EdgeSetList.Current != NULL); 
					assert (node->EdgeSetList.Current->next != NULL);
					node->EdgeSetList.Current = node->EdgeSetList.Current->next;	printf("222222222222222222222222\n");		// go to next EdgeSet node of this list
				}
				/*
				ItemType* itemPtr;								 //Deikths ston typo antikeimenou p thes (px GraphNode)
				list.Current = list.Head;						 //Bazei ton Current stin arxh ths Listas
				for(i=0; i < list.items; i++) { 					 //Diatrexei olh thn uberlist
					itemPtr = (ItemType*) list.Current->data;    //Phra to antikeimeno
					list.Current = list.Current->next; 			 //phgaine ston epomeno kombo
				}		*/
			}
			total_nodes += nodes_num;
			bucket = getNextBucket(chain, bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE)
				return ((Graph*) NULL);
		} 
	}
	if(creep != NULL) free(creep);
}



/* ##############################
   ##			Q3			   ##	
   ############################## */

int everybodyHasInterest (int interestId, Graph *g, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	int i=0, j=0, m=0, n=0, q=0, op=0, buckets_num=0, nodes_num=0, total_nodes=0, trend=0, hasIt=0;
	int *trends, spot=0, size=0;

// ------------- initialise trends data --------------
	trends = malloc(5 * sizeof(int));	assert(trends != NULL);
	trends[0] = 5;													// holds trends array size
	trends[1] = 2;													// holds first available spot
	for (i=2 ; i<5 ; i++)
		trends[i] = -1;
// ---------------------------------------------------
	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;	
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;			
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m];
				hasIt = 0;
				//-----------search for EdgeSets-----------
				node->EdgeSetList.Current = node->EdgeSetList.Head;			// go to begining of EdgeSet list
				for (n=0 ; n<node->EdgeSetList.items ; n++) {		
					es = (EdgeSet*) node->EdgeSetList.Current->data;		// receive current value
					assert (es != NULL);
					assert (es->relation != NULL);
					if (!strcmp(es->relation, "hasInterest")) { 			
						es->edges.Current = es->edges.Head;
						for (q=0 ; q<es->edges.items ; q++) {				// check all interests...
							e = (Edge*) es->edges.Current->data;
							assert (e != NULL);
							trend = e->endNodeID;
							if (trend == interestId) {						// mark that he/she likes it
								hasIt=1;	
								break;
							}	
		
							es->edges.Current = es->edges.Current->next;	// else go to next interest
						}
						if (!hasIt) {
							if (trends != NULL) free(trends);
							return 0; 					// smone doesn't have this particular interest. Total failure
						} 
					}
					if (hasIt) break; 					// go swiftly to the next node, we got what we wanted
					node->EdgeSetList.Current = node->EdgeSetList.Current->next;	// go to next EdgeSet node of this list
				}
			}
		} 
	} 
	if (trends != NULL) free(trends);
	return 1;											// only when everybody has this interest, 1 is returned
}



int checkGender(Graph *g, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	char gender[7];
	int i=0, j=0, m=0, n=0, q=0, op=0, buckets_num=0, nodes_num=0, total_nodes=0, manFlag=0, womanFlag=0;

	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;	
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;			
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m];
				strcpy(gender,(char*) node->properties->arrayP[2].attribute);
				if (!strcmp(gender, "male")) {
					if (womanFlag) return 0;
					manFlag = 1;
				}
				if (!strcmp(gender, "female")) {
					if (manFlag) return 0;
					womanFlag = 1;
				}
			} 
		} 
	} 
	if (manFlag) return 1;
	else if (womanFlag) return 2;
}



int graphTrends (Graph *g, int *trends, int *start, int* errorFlag) {
	int i=0, spot=0, size=0, op=0;
	for (i=*start ; i<10000000 ; i++) 
		if ( everybodyHasInterest(i, g, errorFlag) ) {
		// ---------------- update trends data ---------------
			spot = trends[1];							// determine the spot to update data
			if (spot == trends[0])	{					// if spot is the size of the array, a realloc is needed first
				trends[0] ++;							// new size of array is incremented
				*start = i;								// save point to start next time this function is called
				return 0;								// allocate new memory outside
			}
			trends[spot] = i;							// insert the new trend		
			trends[1] ++;								// next spot is the +1. If new spot is beyond boundaries, realloc next time
			/*for (op=0 ; op<trends[0] ; op++)
				printf("%d-", trends[op]); printf("\n"); */
			return 1;
		} 
	return -1;
}


typedef struct op {
	int gender;
	int *tags;
	int size;
} tagHolder;


void insertHolder(tagHolder *st, tagHolder value, int MEG) {
	int i, j, spot=-1;	

	assert (st != NULL);
	// spot a place to insert
	for (i=0 ; i<MEG ; i++) 
		if (value.size > st[i].size) {	// descending order 
			spot = i;											// must be placed on 'spot'
			break;
		}
	if (spot == -1) return;										// abort if there is no place for a number
	// move all the rest one place to the right - dispose of far-right element
	for (i=MEG-2 ; i>=spot ; i--) {	 
		if (st[i].size == -1) continue;
		st[i+1].size = st[i].size;
		st[i+1].gender = st[i].gender;
		st[i+1].tags = st[i].tags;
	}
	// place new element to the proper place
	st[spot].size = value.size;
	st[spot].gender = value.gender;
	st[spot].tags = value.tags; 
	/*for (j=0 ; j<MEG ; j++)
		printf("%.0f\n", st[j].centralityScore); 
	printf("\n\n"); */
}


int whatTagIsThis(int tagId, Graph *g, char *name, int* errorFlag) {
	BucketChain *chain=NULL;
	Bucket *bucket=NULL;
	GraphNode *node=NULL;
	EdgeSet* es=NULL;
	Edge* e=NULL;
	//char name[64];
	int i=0, j=0, m=0, n=0, q=0, buckets_num=0, nodes_num=0, total_nodes=0, creator=0, post=0;

	for (i=0 ; i < g->hashtable.size ; i++) {						// for every bucket chain in hashtable 
		if (g->hashtable.hashmap[i] == NULL) continue; 				// skip any possible "gaps" in hashtable 
		chain = g->hashtable.hashmap[i];							// get the current chain 
		//printBucketChain(chain);
		buckets_num = chain->bucket_chain.items;					// receive total number of buckets in 'chain'
		goToHeadBucket(chain,errorFlag);							// start from first bucket of current chain
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;	
		bucket = getCurrentBucket(chain,errorFlag);	
		if(*errorFlag == EXIT_VALUE) 
			return EXIT_VALUE;					
		for (j=0 ; j<buckets_num ; j++) {							// for every bucket in current chain
			nodes_num = bucket->items;								// receive total number of nodes in this bucket
			for (m=0 ; m<nodes_num ; m++) {							// and for each one of them
				assert (bucket->GNArray[m] != NULL);
				node = bucket->GNArray[m]; 
				if (node->id == tagId) {
					strcpy(name, node->properties->arrayP[0].attribute);
				}
			}
			total_nodes += nodes_num;
			bucket = getNextBucket(chain, bucket, errorFlag);
			if(*errorFlag == EXIT_VALUE) 
				return EXIT_VALUE;	
		} 
	} 
	name = NULL;														// no such post was found
}

	

    void findTrends(int k, Graph *g, Graph *tagg, char **womenTrends, char **menTrends) {
            Graph *subGraph;
            tagHolder *holder=NULL, tempHolder;
            int errorFlag=0, num, i=0, op=0, *trends, size=0, commonInterests=0, gender=0, start=0, graphSize=0;
            int spot=0, j=0, *t=NULL, tagId=0, counter=0, lastPos=0, menFlag=0, womenFlag=0;
            char result[64];
     		int max_size;
            // ----------- Create list of sub-coherent graphs ------------
            List listOfSubGraphs;
            createList(&listOfSubGraphs, "GraphPtr", &allocateGraphPtr, &destroyGraphPtr, &compareGraphPtr, &assignGraphPtr, &printGraphPtr);
            findCoherentGraphs(g,&max_size,&listOfSubGraphs, 1, &errorFlag);
            if(errorFlag == EXIT_VALUE){
                    printf("findAllCoherentSubGraphs: failed!\n");
            }
            // --------------- Initialise tag data holder ----------------
            holder = malloc(listOfSubGraphs.items * sizeof(tagHolder));
            for (i=0 ; i<listOfSubGraphs.items ; i++) {
                    holder[i].gender = -1;
                    holder[i].tags = NULL;
                    holder[i].size = -1;     
            }
            // ------ for each subgraph check its common interests -------
            listOfSubGraphs.Current = listOfSubGraphs.Head;
            for (i=0 ; i<listOfSubGraphs.items ; i++) {            
                    // initialise trends data
                    trends = malloc(5 * sizeof(int));       assert(trends != NULL);
                    trends[0] = 5;                                                                                                  // holds trends array size
                    trends[1] = 2;                                                                                                  // holds first available spot
                    for (j=2 ; j<5 ; j++)
                            trends[j] = -1;                        
                    subGraph = (Graph*) listOfSubGraphs.Current->data;
                    // receive tags of subgraph
                    commonInterests = graphTrends(subGraph, trends, &start, &errorFlag);
                    if (! commonInterests) {                                                                                // array needs to be realloced
                            size = trends[0];              
                            trends = (int*) realloc(trends, (size+1) * sizeof(int));        // increase the size of array by one
                            graphTrends(subGraph, trends, &start, &errorFlag);              // call again function to continue search
                    }
                    // receive gender of subgraph
                    gender = checkGender(subGraph, &errorFlag);                                     // receive gender of this coherent subgraph
                    if (gender == 0)                                                                                        // report that the group is of one gender only
                            if (commonInterests != -1)                                                              // but only if this group has a common interest
                                    printf("Mixed genders problem\n");
                    // receive size of subgraph
                    graphSize = getNumberOfGraphNodes(subGraph, &errorFlag);
                    /*printf("gender: %d\n", gender);
                    printf("Gender %d  with size %d has these interests\n", gender, graphSize);
                    for (op=0 ; op<trends[0] ; op++)
                                    printf("%d-", trends[op]); printf("\n");
                    printf("Size: %d\n", graphSize);*/
                    // insert tag data to appropriate place
                    tempHolder.gender = gender;
                    tempHolder.tags = trends;
                    tempHolder.size = graphSize;
                    insertHolder(holder, tempHolder, listOfSubGraphs.items);
                    listOfSubGraphs.Current = listOfSubGraphs.Current->next;
                    if (trends != NULL) free(trends);
            }
            counter = 0;
            lastPos = 0;
            // men first ( ! savoir vivre )
            for (j=0 ; j<listOfSubGraphs.items ; j++) {
                    if (holder[j].gender != 1) continue;
                    t = holder[j].tags;
                    for (i=2 ; i<t[0] ; i++) {
                            tagId = t[i];
                            whatTagIsThis(tagId, tagg, result, &errorFlag);
                            assert (result != NULL);                                                // NULL means that no tag with such id exists
                            if (tagId == -1)
                                    strcpy(menTrends[lastPos], "n/a");
                            else
                                    strcpy(menTrends[lastPos], result);
                            lastPos ++;
                            counter ++;
                            if (counter >= k) break;
                    }
                    if (counter >= k) break;
            }
            printf("\nΤOP MEN TRENDS\n---------------\n");
            for (i=0 ; i<k ; i++)
                    printf("\t%s\n", menTrends[i]); printf("\n");
            // women now
            counter = 0;
            lastPos = 0;
            for (j=0 ; j<listOfSubGraphs.items ; j++) {
                    if (holder[j].gender != 2) continue;
                    t = holder[j].tags;
                    for (i=2 ; i<t[0] ; i++) {
                            tagId = t[i];
                            whatTagIsThis(tagId, tagg, result, &errorFlag);
                            assert (result != NULL);                                                // NULL means that no tag with such id exists
                            if (tagId == -1)
                                    strcpy(womenTrends[lastPos], "n/a");
                            else
                                    strcpy(womenTrends[lastPos], result);
                            lastPos ++;
                            counter ++;
                            if (counter >= k) break;
                    }
                    if (counter >= k) break;
            }
            printf("\nΤOP WOMEN TRENDS\n---------------\n");
            for (i=0 ; i<k ; i++)
                    printf("\t%s\n", womenTrends[i]); printf("\n");
    }

Graph* buildTrustGraph(char* forum,Graph* personGraph,Graph* forumGraph,Graph* commentGraph,Graph* postGraph,List* trustEdgesList,List* personNodes,int* errorFlag){

	int i,j,k,num_buckets,endID,p,q,r,num_buckets2,num_items,num_items2,num_likes=0,num_replies=0,forumID,knowsID,forumID2;
	double trustWeight = 0.0;
	BucketChain* currentChain = (BucketChain*) NULL,*currentChain2 = (BucketChain*) NULL;
	Bucket* currentBucket = (Bucket*) NULL,*currentBucket2 = (Bucket*) NULL;
	GraphNode* currentGNode = (GraphNode*) NULL,*currentGNode2 = (GraphNode*) NULL;
	GraphNode* forumGNode = (GraphNode*) NULL;
	EdgeSet* currentEdgeSet = (EdgeSet*) NULL,*currentEdgeSet2 = (EdgeSet*) NULL,*currentEdgeSet3 = (EdgeSet*) NULL,*currentEdgeSet4 = (EdgeSet*) NULL;
	Edge* currentEdge = (Edge*) NULL,*currentEdge2 = (Edge*) NULL,*currentEdge3 = (Edge*) NULL,*trustEdge = (Edge*) NULL;
	Graph* trustGraph = (Graph*) NULL;
	GraphNode* currentPerson= (GraphNode*) NULL;
	GraphNode* otherPerson = (GraphNode*) NULL;
	GraphNode* testPerson = (GraphNode*) NULL;

	if(personGraph == NULL){
		printf("buildTrustGraph: No personGraph exists!\n");
		*errorFlag = EXIT_VALUE;
		return ((Graph*) NULL);
	}
	if(forum == NULL){
		printf("buildTrustGraph: NULL forum Name!\n");
		*errorFlag = EXIT_VALUE;
		return ((Graph*) NULL);
	}
	if(forumGraph == NULL){
		printf("buildTrustGraph: No forumGraph exists!\n");
		*errorFlag = EXIT_VALUE;
		return ((Graph*) NULL);
	}
	if(personNodes == NULL){
		printf("buildTrustGraph: personNodes List is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return ((Graph*) NULL);
	}

	createList(trustEdgesList,"EdgePtr",&allocateEdgePtr,&destroyEdgePtr,&compareEdgePtr,&assignEdgePtr,&printEdgePtr);

	personNodes->Current = personNodes->Head; 									//We will be using the List of Person Nodes for easier access!
	for(i=0; i < personNodes->items; i++){ 										//Loop through all people available
		currentPerson = (GraphNode*) personNodes->Current->data; 				//Get this Person
		currentEdgeSet = findEdgeSet(currentPerson, "IsMemberOfForum",errorFlag); 		//See if person is member to any forum
		if(*errorFlag == EXIT_VALUE){
			printf("buildTrustGraph: findEdgeSet failed!\n");
			*errorFlag = EXIT_VALUE;
			return ((Graph*) NULL);
		}
		if(currentEdgeSet != NULL){ 											//This person belongs to a forum
			currentEdge = getFirstEdge(currentEdgeSet,errorFlag); 						//Get first of edge of IsMemberOfForum edges
			if(*errorFlag == EXIT_VALUE){
				printf("buildTrustGraph: getFirstEdge failed!\n");
				*errorFlag = EXIT_VALUE;
				return ((Graph*) NULL);
			}

			while(currentEdge != NULL){ 										//Search through all the forums he is a member of
				forumID = currentEdge->endNodeID;
				forumGNode = lookUpNode(endID, forumGraph,errorFlag); 					//Get all info about this forum  

				if(*errorFlag == EXIT_VALUE){
					printf("buildTrustGraph: lookUpNode failed!\n");
					*errorFlag = EXIT_VALUE;
					return ((Graph*) NULL);
				} 
				if(forumGNode != NULL){
					if(!strcmp((char*) forumGNode->properties->arrayP[0].attribute, forum)){ //The current Person belongs to the desired forum!
						if(trustGraph == NULL){ 											 //Build trustGraph because it does not exist yet
							trustGraph = createGraph(2,3, errorFlag);
							if(*errorFlag == EXIT_VALUE){
								printf("buildTrustGraph: createGraph failed!\n");
								*errorFlag = EXIT_VALUE;
								return ((Graph*) NULL);
							}
						}
						if(insertNode(currentGNode, trustGraph) == EXIT_VALUE){
							printf("buildTrustGraph: lookUpNode failed!\n");
							*errorFlag = EXIT_VALUE;
							return ((Graph*) NULL);
						}
						currentEdgeSet2 = findEdgeSet(currentPerson, "knows",errorFlag); 				//See if this Person knows any other people at all
						if(*errorFlag == EXIT_VALUE){
							printf("buildTrustGraph: findEdgeSet failed!\n");
							*errorFlag = EXIT_VALUE;
							return ((Graph*) NULL);
						}
						if(currentEdgeSet2 != NULL){ 										//He knows other people
							currentEdge2 = getFirstEdge(currentEdgeSet2,errorFlag);					//Look through all the people he knows
							if(*errorFlag == EXIT_VALUE){
								printf("buildTrustGraph: getFirstEdge failed!\n");
								*errorFlag = EXIT_VALUE;
								return ((Graph*) NULL);
							}
							while(currentEdge2 != NULL){
								if(knowsID != currentPerson->id){ 							//Avoid case where Person "KNOWS" himself
									knowsID = currentEdge2->endNodeID;
									otherPerson = lookUpNode(knowsID,personGraph,errorFlag); 			//A Person he knows
									if(*errorFlag == EXIT_VALUE){
										printf("buildTrustGraph: lookUpNode failed!\n");
										*errorFlag = EXIT_VALUE;
										return ((Graph*) NULL);
									}
									
									if(otherPerson != NULL){								//Now let's see if this other person is also a member of the same forum
									
										testPerson = lookUpNode(knowsID, trustGraph,errorFlag); 	    //Test to see if this other person is already in trustGraph
										if(*errorFlag == EXIT_VALUE){
											printf("buildTrustGraph: lookUpNode failed!\n");
											*errorFlag = EXIT_VALUE;
											return ((Graph*) NULL);
										}
										if(testPerson != NULL){  //This Person already belongs in the trustGraph so automatically he is a member of the forum too
											num_likes = getNumberOfLikes(otherPerson,currentPerson,postGraph,personGraph,forumID,errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("buildTrustGraph: getNumberOfLikes failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}
											num_replies = getNumberOfReplies(otherPerson,currentPerson,postGraph,personGraph,commentGraph,forumID,errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("buildTrustGraph: getNumberOfReplies failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}
											trustWeight = (((30* num_likes) / 100) + ((70* num_replies) / 100));
											trustEdge = setTrustEdgeProperties(currentPerson->id,otherPerson->id,"trusts",trustWeight,errorFlag);
											if(*errorFlag == EXIT_VALUE){
												printf("buildTrustGraph: setTrustEdgeProperties failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}

											if(*errorFlag == EXIT_VALUE){
												printf("buildTrustGraph: setTrustEdgeProperties failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}

											if(insertEdge(currentPerson->id, trustEdge, trustGraph) == EXIT_VALUE){
												printf("buildTrustGraph: insertEdge failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}

											if(addLastList(trustEdgesList, trustEdge) == EXIT_VALUE){
												printf("buildTrustGraph: insertEdge failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}
											if(printTrustEdgeProperties(trustEdge) == EXIT_VALUE){
												printf("buildTrustGraph: printTrustEdgeProperties failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}
										}
										else{ //This Person does not belong in the trustGraph yet so we must check if he is a member of the forum
											currentEdgeSet3 = findEdgeSet(otherPerson, "IsMemberOfForum",errorFlag); //We must check all forums this otherPerson belongs to
											if(*errorFlag == EXIT_VALUE){
												printf("buildTrustGraph: findEdgeSet failed!\n");
												*errorFlag = EXIT_VALUE;
												return ((Graph*) NULL);
											}
											if(currentEdgeSet3 != NULL){
												currentEdge3 = getFirstEdge(currentEdgeSet3,errorFlag);
												if(*errorFlag == EXIT_VALUE){
													printf("buildTrustGraph: getFirstEdge failed!\n");
													*errorFlag = EXIT_VALUE;
													return ((Graph*) NULL);
												}
												while(currentEdge3 != NULL){
													forumID2 = currentEdge3->endNodeID;
													if(forumID2 == forumID){ //They belong to the same forum
														num_likes = getNumberOfLikes(otherPerson,currentPerson,postGraph,personGraph,forumID,errorFlag);
														if(*errorFlag == EXIT_VALUE){
															printf("buildTrustGraph: getNumberOfLikes failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}

														num_replies = getNumberOfReplies(otherPerson,currentPerson,postGraph,personGraph,commentGraph,forumID,errorFlag);
														if(*errorFlag == EXIT_VALUE){
															printf("buildTrustGraph: getNumberOfReplies failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}
														trustWeight = (((30* num_likes) / 100) + ((70* num_replies) / 100));
														trustEdge = setTrustEdgeProperties(currentPerson->id,otherPerson->id,"trusts",trustWeight,errorFlag);
														if(*errorFlag == EXIT_VALUE){
															printf("buildTrustGraph: setTrustEdgeProperties failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}


														if(insertEdge(currentPerson->id, trustEdge, trustGraph) == EXIT_VALUE){
															printf("buildTrustGraph: insertEdge failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}

														if(addLastList(trustEdgesList, trustEdge) == EXIT_VALUE){
															printf("buildTrustGraph: addLastList failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}
														if(printTrustEdgeProperties(trustEdge) == EXIT_VALUE){
															printf("buildTrustGraph: printTrustEdgeProperties failed!\n");
															*errorFlag = EXIT_VALUE;
															return ((Graph*) NULL);
														}
													}
													currentEdge3 = getNextEdge(currentEdgeSet3, currentEdge3, errorFlag);
													if(*errorFlag == EXIT_VALUE){
														printf("buildTrustGraph: getNextEdge failed!\n");
														*errorFlag = EXIT_VALUE;
														return ((Graph*) NULL);
													}
												}
											}
										}		
									}
									else{
										printf("buildTrustGraph: Person Knows Person edge leads to NULL Person!\n");
										*errorFlag = EXIT_VALUE;
										return ((Graph*) NULL);
									}					
								}
								currentEdge2 = getNextEdge(currentEdgeSet2, currentEdge2, errorFlag);
								if(*errorFlag == EXIT_VALUE){
									printf("buildTrustGraph: getNextEdge failed!\n");
									*errorFlag = EXIT_VALUE;
									return ((Graph*) NULL);
								}
							}
						}
					}
				}
				else{
					printf("buildTrustGraph: Person to Forum edge leads to NULL forum!\n");
					*errorFlag = EXIT_VALUE;
					return ((Graph*) NULL);
				}
				currentEdge = getNextEdge(currentEdgeSet, currentEdge, errorFlag); //Move to the next forum
				if(*errorFlag == EXIT_VALUE){
					printf("buildTrustGraph: getNextEdge failed!\n");
					*errorFlag = EXIT_VALUE;
					return ((Graph*) NULL);
				}
			}
		}
		personNodes->Current = personNodes->Current->next; //Move to the next Person
	}

	return trustGraph;

}

double estimateTrust(GraphNode* personA,GraphNode* personB, Graph* trustGraph,int* errorFlag){

	if(personA == NULL){
		printf("estimateTrust: PersonA is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(personB == NULL){
		printf("estimateTrust: PersonB is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(trustGraph == NULL){
		printf("estimateTrust: trustGraph is NULL!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(lookUpNode(personA->id, trustGraph, errorFlag) == NULL){
		printf("estimateTrust: START person does not exist in trustGraph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(lookUpNode(personB->id, trustGraph, errorFlag) == NULL){
		printf("estimateTrust: GOAL person does not exist in trustGraph!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	double goalTrustValue = 0.0;
	int success=0;
	List frontier;
	HashTable explored;

	createList(&frontier,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); /* Create frontier */
	if(initHashTable(&explored,2,10) == EXIT_VALUE){
		printf("estimateTrust: initHashTable Failed! Exiting initHashTable\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	personA->trustValue = 1.0;
	personA->parentIDA = -1;
	personB->distanceA = 0;

	if(personA->id == personB->id){
		return (double) personA->trustValue;
	}
	else{
		if(addLastList(&frontier,personA) == EXIT_VALUE){
			printf("estimateTrust: clearHashTable Failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		if(insertNodeHashTable(personA, &explored) == EXIT_VALUE){
			printf("estimateTrust: clearHashTable Failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		GraphNode* currentPerson = (GraphNode*) NULL;
		do{
			success = trustBFSExpansion(&frontier,&explored,trustGraph,personB->id,&goalTrustValue, errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("estimateTrust: trustBFSExpansion Failed!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}
		}while(success == 0);
		if(success == -1){
			printf("estimateTrust: GOAL person was not reached by TidalTrust!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		destroyList(&frontier);
		if(clearHashTable(&explored) == EXIT_VALUE){
			printf("estimateTrust: clearHashTable Failed!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		return goalTrustValue;
	}
	
}
