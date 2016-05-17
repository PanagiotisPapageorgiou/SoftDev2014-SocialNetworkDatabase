#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtools.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: gtools.c
 MAIN AUTHOR: PANAGIOTIS PAPAGEORGIOU														

 Details: This file contains all functions required in order to perform graph traversal
 from one node to another (reachNode1) or from one to all others (reachNodeN). Our graph
 traversal is BFS and uses a frontier called queued and a set called set that is a HashTable
 in order to speedup the search for explored nodes or common nodes etc.
 The fields: distance and parentID for GraphNode struct become useful here in order to remember
 what node acted as an ancestor to another and to keep the distance of new nodes from the
 a starting point node.

================================================================================================*/

/*==================PART 1 TOOLS =======================*/

int BFS_Step(ResultSet* result_set){ /* Moves ResultSet by one step BFS style */

	GraphNode* gnodeptr=NULL;
	Pair tempResult;

	if(!isEmptyList(result_set->queue)){ /* While new nodes exist in the frontier */

		gnodeptr = (GraphNode*) (result_set->queue.Head->data); /* Extract the Head node */

		if(gnodeptr == NULL){
			printf("BFS_Step: NULL Pointer to GraphNode!\n");
			exit(1);
		}
		
		tempResult.ID = gnodeptr->id;
		tempResult.distance = gnodeptr->distanceA;		
		
		checkBFSNeighbours(result_set->ptrG,gnodeptr,&(result_set->set),&(result_set->queue),gnodeptr->distanceA+1,1); /* Insert the neighbours of this GraphNode into queue and set */

		popList(&(result_set->queue)); /* Remove the Head of the Queue - This normally becomes earlier but since we are using pointers to GraphNodes here gnodeptr would be NULL if we performed popList before checkBFSNeighbours and this would result in segmentation fault */

		result_set->currentResult = tempResult;

		//printf("With Starting Point: %d - DISCOVERED: Node: %d with Distance %d\n",result_set->startID,result_set->currentResult.ID,result_set->currentResult.distance);

		if(tempResult.ID == result_set->startID){ /*Do one more step here to skip the initial if we were in the initial */

			BFS_Step(result_set);

		}
		
	}
	else{
		//printf("Nothing left to expand! ResultSet should be full!\n");
		return -1;
	}
	return 0;

}

int destroyResultSet(ResultSet** result_set){ /* Frees Result Set */

	if(result_set != NULL){
		if((*result_set) != NULL){
			destroyList(&((*result_set)->queue)); /* Destroy frontier */
			clearHashTable(&((*result_set)->set)); /* Destroy set */
			(*result_set)->ptrG = NULL; /* NULL the association with a certain Graph */
		
			free((*result_set)); /* Free result set */
		}
	}
	printf("\nResult Set was destroyed!\n");
}

int printResultSet(ResultSet result_set){ /* Prints the explored Nodes (set) */
	
	printHashTable(&(result_set.set));

}

List* Double_BFS_Graph(Graph* ptrG,int start,int goal,int* success,int* total_distance,int* number_of_paths,int mode){ /* Used in reachNode 1 */
                                                                                         /* Double BFS receives as input the start node and goal node and the graph. Returns
																							a flag (success) if it was successful or not, the total distance of the shortest 
																						    path (total_distance) and a list of all shortest paths itself if mode ==
																							 PATH_CONSTRUCTION
																						    GraphNodes* */
																					     /* Remember to free its path using destroyList(&(path)) */
                                                                                         /*TODO: Non unidirected graphs need simple BFS */
																						 /*TODO: Make path formation optional in order to increase speed */	

/* Algorithm details: Double BFS uses on BFS from the start node and one BFS from the goal node. When the two algorithms meet, a path has been created. In order to find the
   true shortest path we need to modify BFS a bit. Instead of removing and expanding one node of a frontier at a time (the Head) we need to remove and expand all nodes of the same level in the frontier. The two BFS searches (start and goal) take turns expanding their frontier level by level and then check for possible paths created. If multiple paths exist then we must check for the shortest one. This change in BFS was required in order to avoid finding the first path possible and exiting without giving the chance to nodes of the same level to also expand and possibly form shorter paths. Finally for testing purposes we also form the Path, while constructing it we had to keep in mind that a commonNode between the two BFS Searches will belong both to setA and setB and will have ancestors in both sets so we need to check in both sets for the distance of this node as well as its ancestors. */

	GraphNode* startGNode=NULL,*goalGNode=NULL; /* Start and Goal Graph Nodes */

	int i=0, parentNodeAID=0,parentNodeBID=0,distanceA=0;

	GraphNode* tempNode=NULL;

	GraphNode* tempNodePtr = NULL,
	*commonNodePtr = NULL,
	*currentNodeAPtr = NULL,
	*currentNeighbourAPtr = NULL,
	*currentNodeBPtr = NULL,
	*currentNeighbourBPtr = NULL;

	GraphNode** allcommonNodes=NULL;

	List *pathSet=NULL, queueA, queueB; /* 2 frontiers */
	HashTable setA, setB;			 /* 2 sets */

	*total_distance = 0;

	//printf("\n=================================DOUBLE BFS START: %d - GOAL: %d=================================\n",start,goal);
	if(ptrG == NULL){
		printf("Graph is not ready for Double BFS...NULL Graph!\n");
		*success = -1;
		return NULL;
	}
	if(ptrG->hashtable.num_records == 0){
		printf("Graph is not ready for Double BFS...NULL HashTable\n");
		*success = -1;
		return NULL;
	}
	if(ptrG->hashtable.hashmap == NULL){
		printf("Graph is not ready for Double BFS...NULL HashMap\n");
		*success = -1;
		return NULL;
	}

	//printf("Double BFS: Attempting to locate startGNode...\n");

	startGNode = lookUpNode(start, ptrG); /* Locate start Node */

	if(startGNode == NULL){
		printf("Graph START Node is not valid...\n");
		*success = -1;
		return NULL;
	}

	//printf("Double BFS: Located startGNode!\n");

	//printf("Double BFS: Attempting to locate goalGNode...\n");

	goalGNode = lookUpNode(goal, ptrG); /* Locate goal Node */

	if(goalGNode == NULL){
		printf("Graph GOAL Node is not valid...\n");
		*success = -1;
		return NULL;
	}

	//printf("Double BFS: Located endGNode!\n");


	createList(&queueA,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); /* Create frontier A */
	createList(&queueB,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); /* frontier B */
	initHashTable(&setA,2,10); /* HashTable of 2 bucketChains initially and bucket capacity 10 - setA */
	initHashTable(&setB,2,10);

	//printf("===============DOUBLE BFS - START: %d - GOAL: %d =================\n\n",start,goal);

	startGNode->distanceA = 0;
	startGNode->parentIDA = -1;
	//startGNode->distanceB = 0;
	//startGNode->parentIDB = -1;

	tempNode = startGNode;


	addLastList(&queueA,tempNode); /* Add initial and goal Nodes to queue and set for each BFS */
	insertNodeHashTable(tempNode,&setA);

	//printf("BFS A: Added startGNode: %d to QueueA: ",tempNode->id);
	//printList(queueA);
	//printf("\n");
	//printf("BFS A: Added startGNode: %d to SetA: ",tempNode->id);
	//printHashTable(&setA);
	//printf("\n");

	//goalGNode->distanceA = 0;
	//goalGNode->parentIDA = -1;
	goalGNode->distanceB = 0;
	goalGNode->parentIDB = -1;

	tempNode = goalGNode;


	addLastList(&queueB,tempNode);
	insertNodeHashTable(tempNode,&setB);

	//printf("BFS B: Added goalGNode: %d to QueueB: ",tempNode->id);
	//printList(queueB);
	//printf("\n");
	//printf("BFS B: Added goalGNode: %d to SetB: ",tempNode->id);
	//printHashTable(&setB);
	//printf("\n");

	if( (commonNodePtr = firstCommonInFrontier(&setA,&setB)) != NULL){ /* If already a common Node exists - meaning START == GOAL */

		//printf("Double BFS: Start Node is Goal Node!\n");

		if(mode == PATH_CONSTRUCTION){ /* Only one path exists when startnode == goalnode */
			//printf("Double BFS: Will construct a path!\n");
			*number_of_paths = 1;
			pathSet = malloc(sizeof(List) * (*number_of_paths));
			
			if(pathSet == NULL){ /* Allocate memory for Path */
				printf("Double BFS: Error could not allocate enough memory for path!\n");
				exit(1);
			}

			createList(&(pathSet[0]),"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);

			tempNode = startGNode;

			addFirstList(&(pathSet[0]),tempNode); /* Add to Path - this Path will contain only one Node (start == goal) */

			//printf("Double BFS: Finished constructing path!\n");

		}
		/* Free Resources */
		//printf("Double BFS: Freeing up resources...\n");

		destroyList(&queueA);
		destroyList(&queueB);
		clearHashTable(&setA);
		clearHashTable(&setB);

		*success = 1;
		*total_distance = 0;

		//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);

		return pathSet; /* The caller of Double BFS is responsible for freeing the resources */

	}
	else{ /* Beginning Double BFS */

		while((!(isEmptyList(queueA))) && (!(isEmptyList(queueB)))){ /* In a properly defined Graph where at least one path exists between goal and start this will never occur */
	
			/*======================BFS A ==========================*/
			//printf("Double BFS: Expanding A...\n");
			BFSLevelExpansion(&queueA,&setA,ptrG,1);

			/*=====================BFS B ============================*/
			//printf("Double BFS: Expanding B...\n");
			BFSLevelExpansion(&queueB,&setB,ptrG,2);

			/*=========================Checking for Paths=========================*/
			/* Since both BFS have been given a chance to expand by one level now we will see if paths have been formed */
			if((allcommonNodes = shortestCommonInFrontier(&setA,&setB,mode,number_of_paths)) != NULL){ /* Will return the common node that forms the shortest path */

				//if(mode == PATH_CONSTRUCTION)
				//	printf("Double BFS: Fetched all shortest common Nodes between A and B!\n");
				//else
				//	printf("Double BFS: Fetched one shortest common Node between A and B!\n");
				//for(i=0; i < (*number_of_paths); i++){
					//printf("CommonNode: %d - ParentA: %d - DistanceA: %d - ParentB: %d - DistanceB: %d\n",allcommonNodes[i]->id,getParentOfNode(&setA,allcommonNodes[i],1),getDistanceOfNode(&setA,allcommonNodes[i],1),getParentOfNode(&setB,allcommonNodes[i],2),getDistanceOfNode(&setB,allcommonNodes[i],2));
				//}

				distanceA = getDistanceOfNode(&setA,allcommonNodes[0],1); /* All shortest commonNodes share the same  total distance */
				/*======================Forming the Path==========================*/
				/* In order to properly form the Path between the 2 BFS searches we need to find the Parent of the commonNode in setA and in setB and build from there */
				if( ((parentNodeAID = getParentOfNode(&setA,allcommonNodes[0],1)) != -2) && ((parentNodeBID = getParentOfNode(&setB,allcommonNodes[0],2)) != -2) && (distanceA != -1)){

					if(mode == PATH_CONSTRUCTION){
						/*======================Forming the Path==========================*/
						/* In order to properly form a Path between the 2 BFS searches we need to find 
						the Parent of the commonNode in setA and in setB and build from there */
			
						//printf("Double BFS: Now forming all possible paths!\n");

						pathSet = malloc(sizeof(List) * (*number_of_paths));
						if(pathSet == NULL){
							printf("Double_BFS: Could not allocate memory for pathSet!\n");
							exit(1);
						}

						for(i=0; i < (*number_of_paths); i++){
							createList(&(pathSet[i]),"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);
							commonNodePtr = allcommonNodes[i];
							//printf("Building Path: %d - CommonNode was: %d\n",i+1,allcommonNodes[i]->id);

							if(commonNodePtr == NULL){
								printf("Double_BFS: NULL Pointer to Common GraphNode!\n");
								exit(1);
							}

							distanceA = getDistanceOfNode(&setA,commonNodePtr,1);

							if( ((parentNodeAID = getParentOfNode(&setA,commonNodePtr,1)) != -2) && 
								((parentNodeBID = getParentOfNode(&setB,commonNodePtr,2)) != -2) && (distanceA != -1)){

								addFirstList(&(pathSet[i]),commonNodePtr);

								fillPathWithSet(&(pathSet[i]),&setA,parentNodeAID,1,1); /* Build the part of the path BFS A formed */
								fillPathWithSet(&(pathSet[i]),&setB,parentNodeBID,2,2); /* Build the part of the path BFS B formed */

								//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);

							}
							else{
								printf("Double_BFS: Error constructing a path in pathSet!\n");
								exit(1);					
							}
						}

						cutNonUniquePaths(&pathSet,*number_of_paths); /* Different commonNodes don't always result in different paths */

						*total_distance = distanceA + getDistanceOfNode(&setB,commonNodePtr,2);

						//printf("Double BFS: Formed all possible paths! Freeing resources!\n");

						destroyList(&queueA);
						destroyList(&queueB);
						clearHashTable(&setA);
						clearHashTable(&setB);
						free(allcommonNodes);

						*success = 1;
						return pathSet;
											
					}
					else{
						*total_distance = distanceA + getDistanceOfNode(&setB, allcommonNodes[0],2);
						//printf("SUSPECT ID: %d - Distance A: %d - Distance B: %d\n",allcommonNodes[0]->id,distanceA,getDistanceOfNode(&setB,allcommonNodes[0]));
						//printf("Double BFS: Freeing resources!\n");
						destroyList(&queueA);
						destroyList(&queueB);
						clearHashTable(&setA);
						clearHashTable(&setB);
						*success = 1;
						if(allcommonNodes != NULL)
							free(allcommonNodes);
						//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);
						return NULL;
					}
				}
			}

		}	
	}

	/* Free resources in case of failure to form a path*/

	destroyList(&queueA);
	destroyList(&queueB);
	clearHashTable(&setA);
	clearHashTable(&setB);
	if(allcommonNodes != NULL)
		free(allcommonNodes);

	*total_distance = 0;
	*success = -1;
	//printf("Double BFS: Returning with success: %d and total distance: INFINITE\n",*success);

	return NULL;

}

int comparePaths(List pathA, List pathB){ /* Determine if 2 Paths of equal length are the same */

	int i=0,j=0;
	GraphNode* tempA,* tempB;

	if((pathA.items <= 0) || (pathB.items <= 0)){
		printf("comparePaths: Can't compare empty paths!\n");
		exit(1);
	}

	if(pathA.items != pathB.items){
		printf("comparePaths: Can't compare paths of different length!\n");
		exit(1);
	}

	pathA.Current = pathA.Head;
	pathB.Current = pathB.Head;
	for(i=0; i < pathA.items; i++){

		if((pathA.Current == NULL) || (pathB.Current == NULL)){
			printf("comparePaths: Error! Paths have not been constructed properly!\n");
			exit(1);
		}

		tempA = (GraphNode*) pathA.Current->data;
		tempB = (GraphNode*) pathB.Current->data;

		if((tempA == NULL) || (tempB == NULL)){
			printf("comparePaths: Error! Paths contain NULL Graphnodes!\n");
			exit(1);
		}

		if(tempA->id != tempB->id) return 1; /* Paths are different! */

		pathA.Current = pathA.Current->next;
		pathB.Current = pathB.Current->next;

	}

	return 0;

}

int cutNonUniquePaths(List** pathSet,int number_of_paths){ /* Cut duplicate paths in a set of Paths */

	int all_unique = 1,different=0,i=0,j=0;

	if(pathSet != NULL){
		if(*pathSet != NULL){
			if(number_of_paths > 0){
				for(i=0; i< number_of_paths; i++){
					if((*((*pathSet)+i)).items > 0){
						for(j=0; j < number_of_paths; j++){
							if(i != j){
								if((*((*pathSet)+j)).items > 0){
									different = comparePaths( (*((*pathSet)+i)), (*((*pathSet)+j)) );
									if(different == 0){
										destroyList(&(*((*pathSet)+j)));
									}
								}
							}
						}
					}
				}
			}
		}
	}

}

int BFSLevelExpansion(List* queueptr,HashTable* setptr,Graph* ptrG,int round){ /* Used by Double BFS - Expands all nodes of the same level */

	int distance = 0;

	if((setptr == NULL) || (queueptr == NULL)){
		printf("BFSLevelExpansion: Can't use NULL frontier or queue!\n");
		exit(1);	
	}

	GraphNode* tempNodePtr = NULL;
	tempNodePtr = (GraphNode*) (queueptr->Head->data); /* Get Head of frontier */

	do{

		if(tempNodePtr == NULL){
			printf("Double_BFS: Could not access Head of queue!\n");
			exit(1);
		}

		if(round == 1)
			distance = tempNodePtr->distanceA;
		else
			distance = tempNodePtr->distanceB;

		checkBFSNeighbours(ptrG,tempNodePtr,setptr,queueptr,distance+1,round); /* Fetch Neighbours, neighbours will be set to distance of current Node + 1 */

		popList(queueptr);

		if(isEmptyList(*queueptr)) break; /* No neighbours were added, nothing left to expand for this BFS */

		tempNodePtr = (GraphNode*) (queueptr->Head->data); /* Get the new head of the frontier */

		if(round == 1){
			if(distance != tempNodePtr->distanceA) break;
		}
		else{
			if(distance != tempNodePtr->distanceB) break;
		}

	}while(1 > 0); /* Do the same till all nodes of the same level (same distance) have expanded */

}

int checkBFSNeighbours(Graph* ptrG,GraphNode* currentNode,HashTable* setptr,List* queueptr,int distance,int round){ /* Visits the neighbour nodes of a node */

	int i=0,currentNeighbourID=0,expanded=0,j=0;
	Edge* edgeptr=NULL;
	EdgeSet* currentEdgeSet=NULL;
	GraphNode* gnodeptr=NULL;
	GraphNode *neighbour=NULL,*result=NULL;

	if(ptrG == NULL){
		printf("checkBFSNeighbours: Graph is not ready for BFS!\n");
		exit(1);
	}

	if(ptrG->hashtable.hashmap == NULL){
		printf("checkBFSNeighbours: Graph is not ready for BFS! NULL HashMap!\n");
		exit(1);
	}

	if(currentNode == NULL){
		printf("checkBFSNeighbours: Current Node is NULL!\n");
		exit(1);
	}

	if((setptr == NULL) || (queueptr == NULL)){
		printf("checkBFSNeighbours: Queue and Set must be initialised!\n");
		exit(1);
	}

	currentNode->EdgeSetList.Current = currentNode->EdgeSetList.Head; /* Each GraphNode can have many different types of Edges, each type belongs to an EdgeSet */
																	  /* When expanding to new GraphNodes we must pick only the Edges that the Graph represents */
																	  /* If we need the "knows" relation we must pick the "knows" Edges */

	//printf("checkBFSNeighbours: Now checking the neighbours of ID: %d,distanceA: %d,parentA: %d\n",currentNode->id,currentNode->distanceA,currentNode->parentIDA);

	for(i=0; i < currentNode->EdgeSetList.items; i++){ /* For all EdgeSets the Current GraphNode has */

		currentEdgeSet = (EdgeSet*) currentNode->EdgeSetList.Current->data; /* Extract the EdgeSet */

		if(currentEdgeSet->relation == NULL){
			printf("checkBFSNeighbours: currentEdgeSet relation is NULL!\n");
			exit(1);
		}

		if(ptrG->relation == NULL){
			printf("checkBFSNeighbours: ptrG relation is NULL!\n");
			exit(1);
		}

		if(!strcmp(currentEdgeSet->relation, ptrG->relation)){ /* If the relation this EdgeSet represents is the one the graph we traverse represents */

			currentEdgeSet->edges.Current = currentEdgeSet->edges.Head; /* Now we will look at all the Edges of this type */
			
			for(j=0; j < currentEdgeSet->edges.items; j++){  /* For every Edge */

				edgeptr = (Edge*) currentEdgeSet->edges.Current->data; /* Extract Edge */

				if(edgeptr == NULL){
					printf("checkBFSNeighbours: Can't access Edge!\n");
					exit(1);
				}

				currentNeighbourID = edgeptr->endNodeID; /* Extract neighbour ID from edge */

				//printf("checkBFSNeighbours: Found neighbours with ID: %d\n",currentNeighbourID);

				neighbour = lookUpNode(currentNeighbourID,ptrG); /* Find neighbour in graph */
				if(neighbour == NULL){
					//printf("checkBFSNeighbours: Something went wrong with lookUpNode...\nEdge but no Node?!\n");
					exit(1);
				}

				result = lookUpNodeHashTable(neighbour->id, setptr); /* Check to see if current Neighbour has been visited again */

				if(result == NULL){ /* Neighbour has not been visited before */

					if(round == 1){
						neighbour->parentIDA = currentNode->id; /* Neighbour's father is the current Node */
						neighbour->distanceA = distance;        /* Distance will be the distance of the current Node + 1 */
					}
					else{
						neighbour->parentIDB = currentNode->id; /* Neighbour's father is the current Node */
						neighbour->distanceB = distance;        /* Distance will be the distance of the current Node + 1 */
					}

					insertNodeHashTable(neighbour,setptr); /* Add it to Set */
					addLastList(queueptr,neighbour); /* Add it to frontier in order to avoid having multiple same nodes in the frontier */
					expanded++;
					//printf("Neighbour: %d ID, was added with ParentID: %d and Distance: %d\n",neighbour->id,neighbour->parentIDA,neighbour->distanceA);
					//printf("checkBFSNeighbours: This is a new neighbour! Added to Queue and Set!\n");
					//printf("Queue: ");
					//printList(*queueptr);
					//printf("\nSet: ");
					//printHashTable(setptr);
					//printf("\n");
				}
				//else
					//printf("\t- This neighbour has already been opened or added in the Queue before!\n");
				currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;

			}

			return expanded; /* Returns the number of neighbours met */

		}
		currentNode->EdgeSetList.Current = currentNode->EdgeSetList.Current->next;
	}
	return expanded;

}

GraphNode** shortestCommonInFrontier(HashTable* setA, HashTable* setB,int mode,int* number_of_paths){ /* Will run through all the GraphNodes in setA and see if any of them exist in set B. Then, if common
 																		Nodes exist, paths have been formed and we have to return with the shortest path of all */
	
	int encountered=0,total_distance=0,i=0,j=0,k=0,num_of_commons=0;
	GraphNode *tempA,*tempB,*commonNode=NULL,*currentNodeA=NULL;
	Bucket* currentBucket=NULL;
	GraphNode** allCommonNodes=NULL;

	for(i=0; i < setA->size; i++){ /* For all entries in HashTable */

		if(setA->hashmap[i] != NULL){ /* BucketChain exists here */

			goToHeadBucket(setA->hashmap[i]);
			currentBucket = getCurrentBucket(setA->hashmap[i]);

			for(j=0; j < setA->hashmap[i]->bucket_chain.items; j++){ /* For all buckets in this chain */
				
				for(k=0; k < currentBucket->items; k++){ /* For all records in this bucket */

					currentNodeA = currentBucket->GNArray[k]; /* Get a record (GraphNode) */
		
					if(currentNodeA == NULL){
						printf("shortestCommonInFrontier: NULL Pointer in Bucket! Error in Insertion!\n");
						exit(1);
					}

					commonNode = NULL;
					commonNode = lookUpNodeHashTable(currentNodeA->id,setB); /* Look if current record exists in the other hashtable too */
		
					if(commonNode != NULL){ /* Record exists! */
						if(encountered == 0){ /* First common */
							total_distance = commonNode->distanceA + commonNode->distanceB; /* Total distance is the distance of the path formed by the 2 BFS */

							num_of_commons = 1;
							allCommonNodes = malloc(sizeof(GraphNode*) * 1);
							if(allCommonNodes == NULL){
								printf("shortestCommonInFrontier: Error allocating memory for commonNodes!\n");
								exit(1);
							}

							allCommonNodes[0] = commonNode;
						}
						else{
							if(total_distance > (commonNode->distanceA + commonNode->distanceB)){ /* A better (shorter) path is formed with this common Node */
								total_distance = commonNode->distanceA + commonNode->distanceB;

								if(mode == PATH_CONSTRUCTION){ /* We need to locate all possible shortest paths ---> all possible shortest nodes */
									if(allCommonNodes != NULL)
										free(allCommonNodes);
									else{
										printf("shortestCommonInFrontier: Error freeing memory for commonNodes!\n");
										exit(1);
									}

									num_of_commons = 1;

									if(allCommonNodes == NULL){
										printf("shortestCommonInFrontier: Error allocating memory for commonNodes!\n");
										exit(1);
									}

									allCommonNodes[0] = commonNode;									
								}
								else{ /* We just need one of the shortest */
									allCommonNodes[0] = commonNode;
								}			
							}
							if(total_distance == (commonNode->distanceA + commonNode->distanceB)){ /* A Path of the same distance was found! 
																									Add one more commonNode in the array */
								if(mode == PATH_CONSTRUCTION){
									num_of_commons++; /* One more shortest commonNode was found */
									allCommonNodes = realloc(allCommonNodes, (num_of_commons)*sizeof(GraphNode*));

									if(allCommonNodes == NULL){
										printf("shortestCommonInFrontier: Realloc Failed!\n");
										exit(1);
									}

									allCommonNodes[num_of_commons-1] = commonNode;
								}

							}
						}
						encountered++;
					}
				}

				goToNextBucket(setA->hashmap[i]); /* Move to the next Bucket in chain */
				currentBucket = getCurrentBucket(setA->hashmap[i]);

			}
		}

	}

	if(encountered == 0) return NULL; /* No common nodes exist */
	else{
		*number_of_paths = num_of_commons;
		return allCommonNodes;
	}

}

int getParentOfNode(HashTable* set,GraphNode* node,int round){ /* Returns the parent of a graph node in a specific set */
													 /* This is important because a graph node might have 2 parents, one in setA and one in setB in Double BFS */

	GraphNode* temp=NULL;

	if(node == NULL){
		//printf("getParentOfNode: NULL Pointer to GraphNode!\n");
		//exit(1);
		return -2;
	}

	temp = lookUpNodeHashTable(node->id,set); /* Locate node in the given set */

	if(temp == NULL) return -1;
	else{
		if(round == 1)
			return temp->parentIDA; /* Return the parent according to this set */
		else
			return temp->parentIDB;
	}

}

GraphNode* firstCommonInFrontier(HashTable* setA, HashTable* setB){ /* Same as shortest common in frontier with the difference that it returns the first common found */

	int i=0,j=0,k=0;
	GraphNode* commonNode=NULL, *currentNodeA=NULL;
	Bucket* currentBucket=NULL;

	for(i=0; i < setA->size; i++){ /* For all entries in HashTable */

		if(setA->hashmap[i] != NULL){ /* BucketChain exists here */

			goToHeadBucket(setA->hashmap[i]);
			currentBucket = getCurrentBucket(setA->hashmap[i]);

			for(j=0; j < setA->hashmap[i]->bucket_chain.items; j++){ /* For all buckets in this chain */
				
				for(k=0; k < currentBucket->items; k++){ /* For all records in this bucket */

					currentNodeA = currentBucket->GNArray[k];
		
					if(currentNodeA == NULL){
						printf("shortestCommonInFrontier: NULL Pointer in Bucket! Error in Insertion!\n");
						exit(1);
					}

					commonNode = NULL;
					commonNode = lookUpNodeHashTable(currentNodeA->id,setB); /* Look if current record exists in the other hashtable too */
		
					if(commonNode != NULL){ /* Record exists! */
						return commonNode;
					}
				}

				goToNextBucket(setA->hashmap[i]);
				currentBucket = getCurrentBucket(setA->hashmap[i]);

			}
		}

	}

	return NULL;

}

int getDistanceOfNode(HashTable* set, GraphNode* node,int round){ /* Same logic as with getParentOfNode */
	
	GraphNode* temp;

	if(node == NULL){
		printf("getDistanceOfNode: NULL Pointer to GraphNode! (1)\n");
		return -1;
	}

	temp = lookUpNodeHashTable(node->id, set);

	if(temp == NULL) return -1;
	else{
		if(round == 1)
			return temp->distanceA; /* Return the parent according to this set */
		else
			return temp->distanceB;
	}

}

void fillPathWithSet(List* pathptr,HashTable* set,int parentID,int mode,int round){ /* This function is used to form the path in Double BFS search */
																		  /* A commonNode will have a parentA and a parentB and setA and setB it belong to.
																		     By providing the pair of set and parent this function will find all the ancestors of the
																			 commonNode in the given set and add them properly in the Path
																		  */
	GraphNode* temp = NULL;

	while(parentID != -1){
		
		temp = lookUpNodeHashTable(parentID, set);

		if(temp == NULL){
			printf("fillPathWithSet: Parent does not exist in set!\n");
			exit(1);
		}

		if(mode == 2){
			addLastList(pathptr,temp);
		}
		else{ 
			addFirstList(pathptr,temp);
		}
	
		if(round == 1)
			parentID = temp->parentIDA;
		else
			parentID = temp->parentIDB;

	}

}

/*==============================PART 2 TOOLS =============================*/

GraphNode* pickExcludedNode(List listOfResultSets, Graph* g){ /* Will return a node of Graph g that was not reached by any result set up to now */

	int i=0,exists = 0,num_buckets=0,num_items=0,j=0,k=0;
	GraphNode* currentGNode=NULL;
	ResultSet* currentResultSet=NULL;
	Bucket* currentBucket=NULL;
	BucketChain* currentChain=NULL;

	if(g != NULL){

		if(g->hashtable.hashmap != NULL){

			for(i=0; i < g->hashtable.size; i++){ /* For every bucket chain in hashtable */

				if(g->hashtable.hashmap[i] != NULL){

					currentChain = g->hashtable.hashmap[i]; /* Get the current chain */

					num_buckets = currentChain->bucket_chain.items;

					goToHeadBucket(currentChain);
					currentBucket = getCurrentBucket(currentChain);

					for(j=0; j < num_buckets; j++){ /* For every bucket in the chain */
							
						num_items = currentBucket->items;

						for(k = 0; k < num_items; k++){

							if(currentBucket->GNArray[k] != NULL){

								currentGNode = currentBucket->GNArray[k];

								if(listOfResultSets.items > 0){

									exists = 0;

									listOfResultSets.Current = listOfResultSets.Head;
	
									while(listOfResultSets.Current != NULL){

										currentResultSet = (ResultSet*) listOfResultSets.Current->data;

										if(currentResultSet->set.num_records != g->hashtable.num_records - 1){ /*If a ResultSet already contains the whole graph no point in 
																										   looking anymore */
											if( lookUpNodeHashTable(currentGNode->id, &(currentResultSet->set)) != NULL){ /* ID is located - Move on to the next one */
												exists = 1;
												break;
											}

										}
										else{ /* Stop searching for more coherent graphs - At least one result set has the whole graph */
											return NULL;
										}

										listOfResultSets.Current = listOfResultSets.Current->next;

									}

									if(exists == 0){ /* This node does not exist in any result set, let's return it! */
										return currentGNode;
									}

								}
								else{ /* No result set exists yet so any GraphNode is free to use */
									return currentGNode;
								}


							}

						}

						goToNextBucket(currentChain);
						currentBucket = getCurrentBucket(currentChain);
					}
				}
			}
			
			return NULL; /*No GraphNodes Left to check - stop searching */

		}
		else{
			printf("pickExcludedNode: HashMap is NULL!\n");
			exit(1);
		}
	}
	else{
		printf("pickExcludedNode: Graph is NULL!\n");
		exit(1);
	}
}

int findCoherentGraphs(Graph* g,int *max_size){ /* TODO: Speed up */

	int i=0,number_of_cc=0;
	GraphNode* currentGNode=NULL;
	ResultSet* result_set=NULL;
	List listOfResultSets;
	Pair* currentResult=NULL;

	*max_size=0;

	if(g != NULL){
		if(g->hashtable.hashmap != NULL){
			createList(&listOfResultSets, "ResultSetPtr", &allocateResultSetPtr, &destroyResultSetPtr, &compareResultSetPtr, &assignResultSetPtr, &printResultSetPtr);
			while ( (currentGNode = pickExcludedNode(listOfResultSets, g) ) != NULL){
				if(result_set == NULL){
					printf("findCoherentGraphs: Could not allocate ResultSet!\n");
					exit(1);
				}
				/* New coherent subgraph located! Let's explore it */

				result_set = reachNodeN(currentGNode->id, g); /* Prepare a 1 to N search starting with new node */
				addLastList(&listOfResultSets, result_set); /* Add the set of results of this search to a list to keep all the results */
				number_of_cc++; /* A new coherent subgraph was located */

				while( next(result_set, currentResult) ) continue; /* Explore the latest set as much as possible and create a coherent subgraph by doing so */
		
				if(result_set->set.num_records > *max_size)
					*max_size = result_set->set.num_records; /* A subgraph with more nodes has been located! */

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
			printf("findCoherentGraphs: HashMap is NULL!\n");
			exit(1);
		}
	}
	else{
		printf("findCoherentGraphs: Graph Pointer is NULL!\n");
		exit(1);
	}

}

int nodeIsInPath(int id,List path){

	GraphNode* currentNode=NULL;

	if(path.items > 0){
		path.Current = path.Head;
		while(path.Current != NULL){
			currentNode = (GraphNode*) path.Current->data;
			if(currentNode == NULL){
				printf("nodeIsInPath: Node is NULL!\n");
				exit(1);
			}
			if(currentNode->id == id) return 1;
			path.Current = path.Current->next;
		}
		return 0;
	}
	else{
		printf("nodeIsInPath: Path is empty!\n");
		exit(1);
	}

}

////////////////////////////////////////RESULTSET PTR FOR LIST ////////////////////////

/* ResultSetPtr functions for List */

int allocateResultSetPtr(void** target, void* source){

	ResultSet** tempA = (ResultSet**) target;
	ResultSet* tempB = (ResultSet*) source;

	*tempA = tempB;

}

int destroyResultSetPtr(void** target){

	ResultSet** tempA = (ResultSet**) target;

	*tempA = NULL;

}

int compareResultSetPtr(void* target,void* source){

	printf("compareResultSetPtr: Is not ready for comparisons between two result sets!\n");
	exit(1);

}

int assignResultSetPtr(void* target,void* source){

	printf("assignResultSetPtr: Is not ready for assignment between two result sets!\n");
	exit(1);

}

void printResultSetPtr(void* target){

	printf("printResultSetPtr: Is not ready for print!\n");
	exit(1);

}
