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

//TODO: NOT ALL PART 1 AND PART 2 FUNCTIONS THAT INVOLVE OPENING EDGES WILL WORK PROPERLY WITH PART 3 YET BECAUSE OF BANNED EDGE SET CHANGES AND LOOKUPNODE NULL LOGIC CHANGE

/*==================PART 1 TOOLS =======================*/

int BFS_Step(ResultSet* result_set,int* errorFlag){ /* Moves ResultSet by one step BFS style */

	GraphNode* gnodeptr = (GraphNode*) NULL;
	Pair tempResult;

	if(!isEmptyList(result_set->queue)){ /* While new nodes exist in the frontier */

		gnodeptr = (GraphNode*) (result_set->queue.Head->data); /* Extract the Head node */

		if(gnodeptr == NULL){
			printf("BFS_Step: NULL Pointer to GraphNode! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		
		tempResult.ID = gnodeptr->id;
		tempResult.distance = gnodeptr->distanceA;		
		
		checkBFSNeighbours(result_set->ptrG,gnodeptr,&(result_set->set),&(result_set->queue),gnodeptr->distanceA+1,1, errorFlag); /* Insert the neighbours of this GraphNode into queue and set */
		if(*errorFlag == EXIT_VALUE){
			printf("BFS_Step: checkBFSNeighbours failed! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		popList(&(result_set->queue)); /* Remove the Head of the Queue - This normally becomes earlier but since we are using pointers to GraphNodes here gnodeptr would be NULL if we performed popList before checkBFSNeighbours and this would result in segmentation fault */

		result_set->currentResult = tempResult;

		//printf("With Starting Point: %d - DISCOVERED: Node: %d with Distance %d\n",result_set->startID,result_set->currentResult.ID,result_set->currentResult.distance);

		if(tempResult.ID == result_set->startID){ /*Do one more step here to skip the initial if we were in the initial */

			BFS_Step(result_set, errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("BFS_Step: BFS_Step failed! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return EXIT_VALUE;
			}

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
			(*result_set)->ptrG = (Graph*) NULL; /* NULL the association with a certain Graph */
		
			free((*result_set)); /* Free result set */
		}
	}
	printf("\nResult Set was destroyed!\n");

	return 0;

}

int printResultSet(ResultSet result_set){ /* Prints the explored Nodes (set) */
	
	return printHashTable(&(result_set.set));

}

List* Double_BFS_Graph(Graph* ptrG,int start,int goal,int* success,int* total_distance,int* number_of_paths,int mode,int* errorFlag){ /* Used in reachNode 1 */
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

	GraphNode* startGNode = (GraphNode*) NULL,*goalGNode = (GraphNode*) NULL; /* Start and Goal Graph Nodes */

	int i=0, parentNodeAID=0,parentNodeBID=0,distanceA=0;

	GraphNode* tempNode = (GraphNode*) NULL;

	GraphNode* tempNodePtr = (GraphNode*) NULL,
	*commonNodePtr = (GraphNode*) NULL,
	*currentNodeAPtr = (GraphNode*) NULL,
	*currentNeighbourAPtr = (GraphNode*) NULL,
	*currentNodeBPtr = (GraphNode*) NULL,
	*currentNeighbourBPtr = (GraphNode*) NULL;

	GraphNode** allcommonNodes = (GraphNode**) NULL;

	List *pathSet = (List*) NULL, queueA, queueB; /* 2 frontiers */
	HashTable setA, setB;			 /* 2 sets */

	*total_distance = 0;

	//printf("\n=================================DOUBLE BFS START: %d - GOAL: %d=================================\n",start,goal);
	if(ptrG == NULL){
		printf("Double BFS: Graph is not ready for Double BFS...NULL Graph! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(ptrG->hashtable.num_records == 0){
		printf("Double BFS: Graph is not ready for Double BFS...NULL HashTable - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(ptrG->hashtable.hashmap == NULL){
		printf("Double BFS: Graph is not ready for Double BFS...NULL HashMap - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	//printf("Double BFS: Attempting to locate startGNode...\n");

	startGNode = lookUpNode(start, ptrG, errorFlag); /* Locate start Node */
	if(*errorFlag == EXIT_VALUE){
		printf("Double BFS: lookUpNode Failed... - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	if(startGNode == NULL){
		printf("Double BFS: Graph START Node is not valid... - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	//printf("Double BFS: Located startGNode!\n");

	//printf("Double BFS: Attempting to locate goalGNode...\n");

	goalGNode = lookUpNode(goal, ptrG, errorFlag); /* Locate goal Node */
	if(*errorFlag == EXIT_VALUE){
		printf("Double BFS: lookUpNode Failed... - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	if(goalGNode == NULL){
		printf("Double BFS: Graph GOAL Node is not valid... - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	//printf("Double BFS: Located endGNode!\n");

	createList(&queueA,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); /* Create frontier A */
	createList(&queueB,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr); /* frontier B */
	if(initHashTable(&setA,2,10) == EXIT_VALUE){ /* HashTable of 2 bucketChains initially and bucket capacity 10 - setA */
		printf("Double BFS: initHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(initHashTable(&setB,2,10) == EXIT_VALUE){
		printf("Double BFS: initHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	//printf("===============DOUBLE BFS - START: %d - GOAL: %d =================\n\n",start,goal);

	startGNode->distanceA = 0;
	startGNode->parentIDA = -1;
	//startGNode->distanceB = 0;
	//startGNode->parentIDB = -1;

	tempNode = startGNode;

	if(addLastList(&queueA,tempNode) == EXIT_VALUE){ /* Add initial and goal Nodes to queue and set for each BFS */
		printf("Double BFS: addLastList failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(insertNodeHashTable(tempNode,&setA) == EXIT_VALUE){
		printf("Double BFS: insertNodeHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

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


	if(addLastList(&queueB,tempNode) == EXIT_VALUE){
		printf("Double BFS: addLastList failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(insertNodeHashTable(tempNode,&setB) == EXIT_VALUE){
		printf("Double BFS: insertNodeHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}

	//printf("BFS B: Added goalGNode: %d to QueueB: ",tempNode->id);
	//printList(queueB);
	//printf("\n");
	//printf("BFS B: Added goalGNode: %d to SetB: ",tempNode->id);
	//printHashTable(&setB);
	//printf("\n");

	if( (commonNodePtr = firstCommonInFrontier(&setA,&setB,errorFlag)) != NULL){ /* If already a common Node exists - meaning START == GOAL */

		if(*errorFlag == EXIT_VALUE){
			printf("Double BFS: firstCommonInFrontier failed! - Exiting!\n");
			*success = -1;
			*errorFlag = EXIT_VALUE;
			return ((List*) NULL);
		}
		//printf("Double BFS: Start Node is Goal Node!\n");

		if(mode == PATH_CONSTRUCTION){ /* Only one path exists when startnode == goalnode */
			//printf("Double BFS: Will construct a path!\n");
			pathSet = malloc(sizeof(List));
			createList(pathSet,"ListOfLists",&allocateListPtr,&destroyListPtr,NULL,NULL,&printListPtr);

			if(pathSet == NULL){ /* Allocate memory for Path */
				printf("Double BFS: Error could not allocate enough memory for path! - Exiting!\n");
				*success = -1;
				*errorFlag = EXIT_VALUE;
				return ((List*) NULL);
			}

			List path;
			createList(&path,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);

			tempNode = startGNode;

			if(addFirstList(&path,tempNode) == EXIT_VALUE){ /* Add to Path - this Path will contain only one Node (start == goal) */
				printf("Double BFS: addFirstList failed! - Exiting!\n");
				*success = -1;
				*errorFlag = EXIT_VALUE;
				return ((List*) NULL);
			}
			//printf("Double BFS: Finished constructing path!\n");
			addLastList(pathSet,&path);
			destroyList(&path);
		}
		/* Free Resources */
		//printf("Double BFS: Freeing up resources...\n");

		destroyList(&queueA);
		destroyList(&queueB);
		if(clearHashTable(&setA) == EXIT_VALUE){
			printf("Double BFS: clearHashTable failed! - Exiting!\n");
			*success = -1;
			*errorFlag = EXIT_VALUE;
			return ((List*) NULL);
		}
		if(clearHashTable(&setB) == EXIT_VALUE){
			printf("Double BFS: clearHashTable failed! - Exiting!\n");
			*success = -1;
			*errorFlag = EXIT_VALUE;
			return ((List*) NULL);
		}

		*success = 1;
		*total_distance = 0;

		//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);

		return pathSet; /* The caller of Double BFS is responsible for freeing the resources */

	}
	else{ /* Beginning Double BFS */

		while((!(isEmptyList(queueA))) && (!(isEmptyList(queueB)))){ /* In a properly defined Graph where at least one path exists between goal and start this will never occur */
	
			/*======================BFS A ==========================*/
			//printf("Double BFS: Expanding A...\n");
			if(BFSLevelExpansion(&queueA,&setA,ptrG,1) == EXIT_VALUE){
				printf("Double BFS: BFSLevelExpansion failed! - Exiting!\n");
				*success = -1;
				*errorFlag = EXIT_VALUE;
				return ((List*) NULL);
			}

			/*=====================BFS B ============================*/
			//printf("Double BFS: Expanding B...\n");
			if(BFSLevelExpansion(&queueB,&setB,ptrG,2) == EXIT_VALUE){
				printf("Double BFS: BFSLevelExpansion failed! - Exiting!\n");
				*success = -1;
				*errorFlag = EXIT_VALUE;
				return ((List*) NULL);
			}

			/*=========================Checking for Paths=========================*/
			/* Since both BFS have been given a chance to expand by one level now we will see if paths have been formed */
			if((allcommonNodes = shortestCommonInFrontier(&setA,&setB,mode,number_of_paths,errorFlag)) != NULL){ /* Will return the common node that forms the shortest path */
				if(*errorFlag == EXIT_VALUE){
					printf("Double BFS: shortestCommonInFrontier failed! - Exiting!\n");
					*success = -1;
					return ((List*) NULL);
				}
				//if(mode == PATH_CONSTRUCTION)
					//printf("Double BFS: Fetched all shortest common Nodes between A and B!\n");
				//else
					//printf("Double BFS: Fetched one shortest common Node between A and B!\n");
				//for(i=0; i < (*number_of_paths); i++){
					//printf("CommonNode: %d - ParentA: %d - DistanceA: %d - ParentB: %d - DistanceB: %d\n",allcommonNodes[i]->id,getParentOfNode(&setA,allcommonNodes[i],1,errorFlag),getDistanceOfNode(&setA,allcommonNodes[i],1,errorFlag),getParentOfNode(&setB,allcommonNodes[i],2,errorFlag),getDistanceOfNode(&setB,allcommonNodes[i],2,errorFlag));

				distanceA = getDistanceOfNode(&setA,allcommonNodes[0],1,errorFlag); /* All shortest commonNodes share the same  total distance */

				if(*errorFlag == EXIT_VALUE){
					printf("Double BFS: getDistanceOfNode failed! - Exiting!\n");
					*success = -1;
					*errorFlag = EXIT_VALUE;
					return ((List*) NULL);
				}
				/*======================Forming the Path==========================*/
				/* In order to properly form the Path between the 2 BFS searches we need to find the Parent of the commonNode in setA and in setB and build from there */
				if( ((parentNodeAID = getParentOfNode(&setA,allcommonNodes[0],1,errorFlag)) != EXIT_VALUE) && ((parentNodeBID = getParentOfNode(&setB,allcommonNodes[0],2,errorFlag)) != EXIT_VALUE) && (distanceA != EXIT_VALUE)){

					if(*errorFlag == EXIT_VALUE){
						printf("Double BFS: getParentOfNode failed! - Exiting!\n");
						*success = -1;
						*errorFlag = EXIT_VALUE;
						return ((List*) NULL);
					}
					if(mode == PATH_CONSTRUCTION){
						/*======================Forming the Path==========================*/
						/* In order to properly form a Path between the 2 BFS searches we need to find 
						the Parent of the commonNode in setA and in setB and build from there */
			
						//printf("Double BFS: Now forming all possible paths!\n");

						pathSet = malloc(sizeof(List));
						createList(pathSet,"ListOfLists",&allocateListPtr,&destroyListPtr,NULL,NULL,&printListPtr);

						if(pathSet == NULL){ /* Allocate memory for Path */
							printf("Double BFS: Error could not allocate enough memory for path! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);
						}

						List path;	

						for(i=0; i < (*number_of_paths); i++){
							createList(&path,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);
							commonNodePtr = allcommonNodes[i];
							//printf("Building Path: %d - CommonNode was: %d\n",i+1,allcommonNodes[i]->id);

							if(commonNodePtr == NULL){
								printf("Double_BFS: NULL Pointer to Common GraphNode! - Exiting!\n");
								*success = -1;
								*errorFlag = EXIT_VALUE;
								return ((List*) NULL);
							}

							distanceA = getDistanceOfNode(&setA,commonNodePtr,1,errorFlag);
							if(*errorFlag == EXIT_VALUE){
								printf("Double_BFS: getDistanceOfNode failed! - Exiting!\n");
								*success = -1;
								*errorFlag = EXIT_VALUE;
								return ((List*) NULL);
							}
							if( ((parentNodeAID = getParentOfNode(&setA,commonNodePtr,1,errorFlag)) != -2) && 
								((parentNodeBID = getParentOfNode(&setB,commonNodePtr,2,errorFlag)) != -2) && (distanceA != -1)){

								if(*errorFlag == EXIT_VALUE){
									printf("Double_BFS: getParentOfNode failed! - Exiting!\n");
									*success = -1;
									*errorFlag = EXIT_VALUE;
									return ((List*) NULL);
								}							
								if(addFirstList(&path,commonNodePtr) == EXIT_VALUE){
									printf("Double_BFS: addFirstList failed! - Exiting!\n");
									*success = -1;
									*errorFlag = EXIT_VALUE;
									return ((List*) NULL);
								}

								if(fillPathWithSet(&path,&setA,parentNodeAID,1,1) == EXIT_VALUE){ /* Build the part of the path BFS A formed */
									printf("Double_BFS: fillPathWithSet failed! - Exiting!\n");
									*success = -1;
									*errorFlag = EXIT_VALUE;
									return ((List*) NULL);								
								}
								if(fillPathWithSet(&path,&setB,parentNodeBID,2,2) == EXIT_VALUE){ /* Build the part of the path BFS B formed */
									printf("Double_BFS: fillPathWithSet failed! - Exiting!\n");
									*success = -1;
									*errorFlag = EXIT_VALUE;
									return ((List*) NULL);
								}
								//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);
								addLastList(pathSet,&path);
								destroyList(&path);	
							}
							else{
								printf("Double_BFS: Error constructing a path in pathSet! - Exiting!\n");
								*success = -1;
								*errorFlag = EXIT_VALUE;
								return ((List*) NULL);					
							}
						}

						if(cutNonUniquePaths(pathSet,*number_of_paths) == EXIT_VALUE){ /* Different commonNodes don't always result in different paths */
							printf("Double_BFS: cutNonUniquePaths failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}

						*total_distance = distanceA + getDistanceOfNode(&setB,commonNodePtr,2,errorFlag);
						if(*errorFlag == EXIT_VALUE){
							printf("Double_BFS: getDistanceOfNode failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}

						//printf("Double BFS: Formed all possible paths! Freeing resources!\n");

						destroyList(&queueA);
						destroyList(&queueB);
						if(clearHashTable(&setA) == EXIT_VALUE){
							printf("Double_BFS: clearHashTable failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}
						if(clearHashTable(&setB) == EXIT_VALUE){
							printf("Double_BFS: clearHashTable failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}
						free(allcommonNodes);

						*success = 1;
						return pathSet;
											
					}
					else{
						*total_distance = distanceA + getDistanceOfNode(&setB, allcommonNodes[0],2, errorFlag);
						if(*errorFlag == EXIT_VALUE){
							printf("Double_BFS: getDistanceOfNode failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}
						//printf("SUSPECT ID: %d - Distance A: %d - Distance B: %d\n",allcommonNodes[0]->id,distanceA,getDistanceOfNode(&setB,allcommonNodes[0]));
						//printf("Double BFS: Freeing resources!\n");
						destroyList(&queueA);
						destroyList(&queueB);
						if(clearHashTable(&setA) == EXIT_VALUE){
							printf("Double_BFS: clearHashTable failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}
						if(clearHashTable(&setB) == EXIT_VALUE){
							printf("Double_BFS: clearHashTable failed! - Exiting!\n");
							*success = -1;
							*errorFlag = EXIT_VALUE;
							return ((List*) NULL);	
						}
						*success = 1;
						if(allcommonNodes != NULL)
							free(allcommonNodes);
						//printf("Double BFS: Returning with success: %d and total distance: %d\n",*success,*total_distance);
						return ((List*) NULL);
					}
				}
				else{
					printf("Double_BFS: getParent/getDistance failed! - Exiting!\n");
					*success = -1;
					*errorFlag = EXIT_VALUE;
					return ((List*) NULL);	
				}
			}

		}	
	}

	/* Free resources in case of failure to form a path*/

	destroyList(&queueA);
	destroyList(&queueB);
	if(clearHashTable(&setA) == EXIT_VALUE){
		printf("Double_BFS: clearHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(clearHashTable(&setB) == EXIT_VALUE){
		printf("Double_BFS: clearHashTable failed! - Exiting!\n");
		*success = -1;
		*errorFlag = EXIT_VALUE;
		return ((List*) NULL);
	}
	if(allcommonNodes != NULL)
		free(allcommonNodes);

	*total_distance = 0;
	*success = -1;
	//printf("Double BFS: Returning with success: %d and total distance: INFINITE\n",*success);

	return ((List*) NULL);

}

int comparePaths(List pathA, List pathB){ /* Determine if 2 Paths of equal length are the same */

	int i=0,j=0;
	GraphNode* tempA,* tempB;

	if((pathA.items <= 0) || (pathB.items <= 0)){
		printf("comparePaths: Can't compare empty paths! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(pathA.items != pathB.items){
		printf("comparePaths: Can't compare paths of different length! - Exiting!\n");
		return EXIT_VALUE;
	}

	pathA.Current = pathA.Head;
	pathB.Current = pathB.Head;
	for(i=0; i < pathA.items; i++){

		if((pathA.Current == NULL) || (pathB.Current == NULL)){
			printf("comparePaths: Error! Paths have not been constructed properly! - Exiting!\n");
			return EXIT_VALUE;
		}

		tempA = (GraphNode*) pathA.Current->data;
		tempB = (GraphNode*) pathB.Current->data;

		if((tempA == NULL) || (tempB == NULL)){
			printf("comparePaths: Error! Paths contain NULL Graphnodes! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(tempA->id != tempB->id) return 1; /* Paths are different! */

		pathA.Current = pathA.Current->next;
		pathB.Current = pathB.Current->next;

	}

	return 0;

}

int cutNonUniquePaths(List* pathSet,int number_of_paths){ /* Cut duplicate paths in a set of Paths */

	int all_unique = 1,different=0,i=0,j=0;

	Node* myCurrent = (Node*) NULL;
	Node* myCurrent2 = (Node*) NULL;
	List* pathA = (List*) NULL;
	List* pathB = (List*) NULL;

	if(pathSet != NULL){
		if(pathSet->items > 1){
			myCurrent = pathSet->Head;
			while(myCurrent != NULL){
				pathA = (List*) myCurrent->data;
				if(pathA != NULL){
					myCurrent2 = pathSet->Head;
					while(myCurrent2 != NULL){
						pathB = (List*) myCurrent2->data;
						if(pathB != NULL){
							if(pathA != pathB){
								different = comparePaths(*pathB,*pathA);
								if(different == 0){
									destroyList(pathA);
									free(pathA);
									myCurrent->data = NULL;
									break;
								}
							}
						}
						myCurrent2 = myCurrent2->next;
					}
				}
				myCurrent = myCurrent->next;
			}
		}

		return 0;
	}

	return EXIT_VALUE;

}

int BFSLevelExpansion(List* queueptr,HashTable* setptr,Graph* ptrG,int round){ /* Used by Double BFS - Expands all nodes of the same level */

	int distance = 0;
	int errorFlag=0;

	if((setptr == NULL) || (queueptr == NULL)){
		printf("BFSLevelExpansion: Can't use NULL frontier or queue! - Exiting!\n");
		return EXIT_VALUE;	
	}

	GraphNode* tempNodePtr = (GraphNode*) NULL;
	tempNodePtr = (GraphNode*) (queueptr->Head->data); /* Get Head of frontier */

	do{

		if(tempNodePtr == NULL){
			printf("BFSLevelExpansion: Could not access Head of queue! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(round == 1)
			distance = tempNodePtr->distanceA;
		else
			distance = tempNodePtr->distanceB;

		checkBFSNeighbours(ptrG,tempNodePtr,setptr,queueptr,distance+1,round,&errorFlag); /* Fetch Neighbours, neighbours will be set to distance of current Node + 1 */
		if(errorFlag == EXIT_VALUE){
			printf("BFSLevelExpansion: checkBFSNeighbours failed! - Exiting!\n");
			return EXIT_VALUE;
		}

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

int checkBFSNeighbours(Graph* ptrG,GraphNode* currentNode,HashTable* setptr,List* queueptr,int distance,int round,int* errorFlag){ /* Visits the neighbour nodes of a node */

	int i=0,currentNeighbourID=0,expanded=0,j=0,bannedFlag=0;
	Edge* edgeptr = (Edge*) NULL;
	EdgeSet* currentEdgeSet = (EdgeSet*) NULL;
	Node* myCurrent=NULL;
	Node* myCurrent2=NULL;
	GraphNode* gnodeptr = (GraphNode*) NULL;
	GraphNode *neighbour = (GraphNode*) NULL,*result = (GraphNode*) NULL;

	if(ptrG == NULL){
		printf("checkBFSNeighbours: Graph is not ready for BFS! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(ptrG->hashtable.hashmap == NULL){
		printf("checkBFSNeighbours: Graph is not ready for BFS! NULL HashMap! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(currentNode == NULL){
		printf("checkBFSNeighbours: Current Node is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if((setptr == NULL) || (queueptr == NULL)){
		printf("checkBFSNeighbours: Queue and Set must be initialised! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	myCurrent = currentNode->EdgeSetList.Head;
																	  /* Each GraphNode can have many different types of Edges, each type belongs to an EdgeSet */
																	  /* When expanding to new GraphNodes we must pick only the Edges that the Graph represents */
																	  /* If we need the "knows" relation we must pick the "knows" Edges */

	//printf("checkBFSNeighbours: Now checking the neighbours of ID: %d,distanceA: %d,parentA: %d\n",currentNode->id,currentNode->distanceA,currentNode->parentIDA);

	for(i=0; i < currentNode->EdgeSetList.items; i++){ /* For all EdgeSets the Current GraphNode has */

		currentEdgeSet = (EdgeSet*) myCurrent->data; /* Extract the EdgeSet */

		if(currentEdgeSet->relation == NULL){
			printf("checkBFSNeighbours: currentEdgeSet relation is NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		if(!strcmp(currentEdgeSet->relation, "knows")){ /* If the relation this EdgeSet represents is "knows" (used by person) */

			myCurrent2 = currentEdgeSet->edges.Head; /* Now we will look at all the Edges of this type */
			
			for(j=0; j < currentEdgeSet->edges.items; j++){  /* For every Edge */

				edgeptr = (Edge*) myCurrent2->data; /* Extract Edge */

				if(edgeptr == NULL){
					printf("checkBFSNeighbours: Can't access Edge! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}

				bannedFlag=0;
			
				bannedFlag = isBannedEdge(ptrG, edgeptr);
				if(bannedFlag == EXIT_VALUE){
					printf("checkBFSNeighbours: isBannedEdge failed! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;					
				}
				else if(bannedFlag == 0){
					currentNeighbourID = edgeptr->endNodeID; /* Extract neighbour ID from edge */

					//printf("checkBFSNeighbours: Found neighbours with ID: %d\n",currentNeighbourID);

					neighbour = lookUpNode(currentNeighbourID,ptrG, errorFlag); /* Find neighbour in graph */

					if(*errorFlag == EXIT_VALUE){
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;
					}

					if(neighbour != NULL){ //If neighbour is NULL this is not a problem neccessarily, it might mean the current node "knows" a person from another graph!
						result = lookUpNodeHashTable(neighbour->id, setptr, errorFlag); /* Check to see if current Neighbour has been visited again */
						if(*errorFlag == EXIT_VALUE){
							printf("checkBFSNeighbours: lookUpNodeHashTable failed!\n");
							*errorFlag = EXIT_VALUE;
							return EXIT_VALUE;
						}

						if(result == NULL){ /* Neighbour has not been visited before */

							if(round == 1){
								neighbour->parentIDA = currentNode->id; /* Neighbour's father is the current Node */
								neighbour->distanceA = distance;        /* Distance will be the distance of the current Node + 1 */
							}
							else{
								neighbour->parentIDB = currentNode->id; /* Neighbour's father is the current Node */
								neighbour->distanceB = distance;        /* Distance will be the distance of the current Node + 1 */
							}

							if(insertNodeHashTable(neighbour,setptr) == EXIT_VALUE){ /* Add it to Set */
								printf("checkBFSNeighbours: insertNodeHashTable failed!\n");
								*errorFlag = EXIT_VALUE;
								return EXIT_VALUE;
							}
							if(addLastList(queueptr,neighbour) == EXIT_VALUE){ /* Add it to frontier in order to avoid having multiple same nodes in the frontier */
								printf("checkBFSNeighbours: addLastList failed!\n");
								*errorFlag = EXIT_VALUE;
								return EXIT_VALUE;
							}
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
					}
				}
				myCurrent2 = myCurrent2->next;

			}
		}
		myCurrent = myCurrent->next;
	}

	return expanded;

}

GraphNode** shortestCommonInFrontier(HashTable* setA, HashTable* setB,int mode,int* number_of_paths,int* errorFlag){ /* Will run through all the GraphNodes in setA and see if any of them exist in set B. Then, if common
 																		Nodes exist, paths have been formed and we have to return with the shortest path of all */
	
	int encountered=0,total_distance=0,i=0,j=0,k=0,num_of_commons=0;
	GraphNode *tempA,*tempB,*commonNode = (GraphNode*) NULL,*currentNodeA = (GraphNode*) NULL;
	Bucket* currentBucket = (Bucket*) NULL;
	GraphNode** allCommonNodes = (GraphNode**) NULL;

	for(i=0; i < setA->size; i++){ /* For all entries in HashTable */

		if(setA->hashmap[i] != NULL){ /* BucketChain exists here */

			currentBucket = getHeadBucket(setA->hashmap[i], errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("shortestCommonInFrontier: NULL Pointer in Bucket! Error in Insertion! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return ((GraphNode**) NULL);
			}

			for(j=0; j < setA->hashmap[i]->bucket_chain.items; j++){ /* For all buckets in this chain */
				
				for(k=0; k < currentBucket->items; k++){ /* For all records in this bucket */

					currentNodeA = currentBucket->GNArray[k]; /* Get a record (GraphNode) */
	
					if(currentNodeA != NULL){

						commonNode = (GraphNode*) NULL;
						commonNode = lookUpNodeHashTable(currentNodeA->id,setB, errorFlag); /* Look if current record exists in the other hashtable too */
						if(*errorFlag == EXIT_VALUE){
							printf("shortestCommonInFrontier: NULL Pointer after lookUpNodeHashTable!\n");
							return ((GraphNode**) NULL);
						}

						if(commonNode != NULL){ /* Record exists! */
							if(encountered == 0){ /* First common */
								total_distance = commonNode->distanceA + commonNode->distanceB; /* Total distance is the distance of the path formed by the 2 BFS */

								num_of_commons = 1;
								allCommonNodes = malloc(sizeof(GraphNode*) * 1);
								if(allCommonNodes == NULL){
									printf("shortestCommonInFrontier: Error allocating memory for commonNodes! - Exiting!\n");
									*errorFlag = EXIT_VALUE;
									return ((GraphNode**) NULL);
								}

								allCommonNodes[0] = commonNode;
							}
							else{
								if(total_distance > (commonNode->distanceA + commonNode->distanceB)){ /* A better (shorter) path is formed with this common Node */
									total_distance = commonNode->distanceA + commonNode->distanceB;

									if(mode == PATH_CONSTRUCTION){ /* We need to locate all possible shortest paths ---> all possible shortest nodes */
										if(allCommonNodes != NULL){
											free(allCommonNodes);
											allCommonNodes=NULL;
										}

										num_of_commons = 1;

										allCommonNodes = malloc(sizeof(GraphNode*) * 1);
										if(allCommonNodes == NULL){
											printf("shortestCommonInFrontier: Error allocating memory for commonNodes! - Exiting!\n");
											*errorFlag = EXIT_VALUE;
											return ((GraphNode**) NULL);
										}

										if(allCommonNodes == NULL){
											printf("shortestCommonInFrontier: Error allocating memory for commonNodes! - Exiting!\n");
											*errorFlag = EXIT_VALUE;
											return ((GraphNode**) NULL);
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
											printf("shortestCommonInFrontier: Realloc Failed! - Exiting!\n");
											*errorFlag = EXIT_VALUE;
											return ((GraphNode**) NULL);
										}

										allCommonNodes[num_of_commons-1] = commonNode;
									}

								}
							}
							encountered++;
						}
					}
				}

				currentBucket = getNextBucket(setA->hashmap[i], currentBucket, errorFlag); /* Move to the next Bucket in chain */
				if(*errorFlag == EXIT_VALUE){
					printf("shortestCommonInFrontier: getNextBucket Failed! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return ((GraphNode**) NULL);
				}

			}
		}

	}

	if(encountered == 0) return ((GraphNode**) NULL); /* No common nodes exist */
	else{
		*number_of_paths = num_of_commons;
		return allCommonNodes;
	}

}

int getParentOfNode(HashTable* set,GraphNode* node,int round, int* errorFlag){ /* Returns the parent of a graph node in a specific set */
													 /* This is important because a graph node might have 2 parents, one in setA and one in setB in Double BFS */

	GraphNode* temp = (GraphNode*) NULL;

	if(node == NULL){
		//printf("getParentOfNode: NULL Pointer to GraphNode!\n");
		//exit(1);
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	temp = lookUpNodeHashTable(node->id,set,errorFlag); /* Locate node in the given set */
	if(*errorFlag == EXIT_VALUE){
		printf("getParentOfNode: lookUpNodeHashTable Failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(temp == NULL) return EXIT_VALUE;
	else{
		if(round == 1)
			return temp->parentIDA; /* Return the parent according to this set */
		else
			return temp->parentIDB;
	}

}

GraphNode* firstCommonInFrontier(HashTable* setA, HashTable* setB, int* errorFlag){ /* Same as shortest common in frontier with the difference that it returns the first common found */

	int i=0,j=0,k=0;
	GraphNode* commonNode = (GraphNode*) NULL, *currentNodeA = (GraphNode*) NULL;
	Bucket* currentBucket = (Bucket*) NULL;

	for(i=0; i < setA->size; i++){ /* For all entries in HashTable */

		if(setA->hashmap[i] != NULL){ /* BucketChain exists here */

			currentBucket = getHeadBucket(setA->hashmap[i], errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("firstCommonInFrontier: NULL Pointer in Bucket! Error in Insertion! - Exiting!\n");
				*errorFlag = EXIT_VALUE;
				return ((GraphNode*) NULL);
			}

			for(j=0; j < setA->hashmap[i]->bucket_chain.items; j++){ /* For all buckets in this chain */
				
				for(k=0; k < currentBucket->items; k++){ /* For all records in this bucket */

					currentNodeA = currentBucket->GNArray[k];
	
					if(currentNodeA != NULL){

						commonNode = (GraphNode*) NULL;
						commonNode = lookUpNodeHashTable(currentNodeA->id,setB,errorFlag); /* Look if current record exists in the other hashtable too */
						if(*errorFlag == EXIT_VALUE){
							printf("firstCommonInFrontier: lookUpNodeHashTable failed! - Exiting!\n");
							*errorFlag = EXIT_VALUE;
							return ((GraphNode*) NULL);
						}
						if(commonNode != NULL){ /* Record exists! */
							return commonNode;
						}
					}
				}

				currentBucket = getNextBucket(setA->hashmap[i], currentBucket, errorFlag); /* Move to the next Bucket in chain */
				if(*errorFlag == EXIT_VALUE){
					printf("firstCommonInFrontier: getNextBucket Failed! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return ((GraphNode*) NULL);
				}
			}
		}

	}

	return ((GraphNode*) NULL);

}

int getDistanceOfNode(HashTable* set, GraphNode* node,int round,int* errorFlag){ /* Same logic as with getParentOfNode */
	
	GraphNode* temp = (GraphNode*) NULL;

	if(node == NULL){
		printf("getDistanceOfNode: NULL Pointer to GraphNode! (1) - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	temp = lookUpNodeHashTable(node->id, set, errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("getDistanceOfNode: lookUpNodeHashTable Failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(temp == NULL) return EXIT_VALUE;
	else{
		if(round == 1)
			return temp->distanceA;
		else
			return temp->distanceB;
	}

}

int fillPathWithSet(List* pathptr,HashTable* set,int parentID,int mode,int round){ /* This function is used to form the path in Double BFS search */
																		  /* A commonNode will have a parentA and a parentB and setA and setB it belong to.
																		     By providing the pair of set and parent this function will find all the ancestors of the
																			 commonNode in the given set and add them properly in the Path
																		  */
	GraphNode* temp = (GraphNode*) NULL;
	int errorFlag=0;

	while(parentID != -1){
		
		temp = lookUpNodeHashTable(parentID, set, &errorFlag);

		if(temp == NULL){
			printf("fillPathWithSet: Parent does not exist in set! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(mode == 2){
			if(addLastList(pathptr,temp) == EXIT_VALUE){
				printf("fillPathWithSet: addLastList Failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
		else{ 
			if(addFirstList(pathptr,temp) == EXIT_VALUE){
				printf("fillPathWithSet: addFirstList Failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	
		if(round == 1)
			parentID = temp->parentIDA;
		else
			parentID = temp->parentIDB;

	}

}

/*==============================PART 2 TOOLS =============================*/

GraphNode* pickExcludedNode(List listOfResultSets, Graph* g,int* errorFlag){ /* Will return a node of Graph g that was not reached by any result set up to now */

	int i=0,exists = 0,num_buckets=0,num_items=0,j=0,k=0;
    GraphNode* currentGNode = (GraphNode*) NULL;
    ResultSet* currentResultSet = (ResultSet*) NULL;
    Bucket* currentBucket = (Bucket*) NULL;
    BucketChain* currentChain = (BucketChain*) NULL;

	//printf("pickExcludedNode: I will try to find a new GraphNode that does not exist in any resultSet!\n");

	//printHashTable(&(g->hashtable));

	if(g != NULL){
    	if(g->hashtable.hashmap != NULL){
 
			for(i=0; i < g->hashtable.size; i++){ /* For every bucket chain in hashtable */
					
					if(g->hashtable.hashmap[i] != NULL){
 
						currentChain = g->hashtable.hashmap[i]; /* Get the current chain */
						num_buckets = currentChain->bucket_chain.items;
						currentBucket = getHeadBucket(currentChain,errorFlag);
 
						if(*errorFlag == EXIT_VALUE){
                        	printf("pickExcludeNode: getHeadBucket failed! - Exiting!\n");
                            return ((GraphNode*) NULL);
                        }
						
						for(j=0; j < num_buckets; j++){ /* For every bucket in the chain */
								
							num_items = currentBucket->items;
							
							for(k = 0; k < num_items; k++){
										
									if(currentBucket->GNArray[k] != NULL){
										
										currentGNode = currentBucket->GNArray[k];
										//printf("pickExcludedNode: Currently looking at GraphNode: %d\n",currentGNode->id);

										if(listOfResultSets.items > 0){
											exists = 0;
 											listOfResultSets.Current = listOfResultSets.Head;
       										
											//printf("pickExcludedNode: ResultSets exist so let's check if %d exists in any ResultSet!\n",currentGNode->id);

       										while(listOfResultSets.Current != NULL){
		 										currentResultSet = (ResultSet*) listOfResultSets.Current->data;
 												
												//printf("pickExcludedNode: Picked a resulSet!\n");

 												if(currentResultSet->set.num_records != g->hashtable.num_records){ 
 												/*If a ResultSet already contains the whole graph no point in lookin anymore*/
													
													if( lookUpNodeHashTable(currentGNode->id, &(currentResultSet->set), errorFlag) != NULL){ 
													/* ID is located - Move on to the next one */
														 //printf("pickExcludedNode: GraphNode exists inside this resultSet!\n");
                                                         exists = 1;
                                                         break;
                                                     }
 													 else{
														//printf("pickExcludedNode: This ResultSet does not contain the GraphNode...we have to check more...\n");
														
													 }

                                                     if(*errorFlag == EXIT_VALUE){
                                                            printf("pickExcludeNode: lookUpNodeHashTable failed! - Exiting!\n");
                                                            return ((GraphNode*) NULL);
                                                     }
 
 
                                                  }
                                                  else{ /* Stop searching for more coherent graphs - At least one result set has the whole graph */
														//printf("pickExcludedNode: This resultSet holds the whole graph! No point in searching any more!\n");
                                                        return ((GraphNode*) NULL);
                                                  }
 
                                                  listOfResultSets.Current = listOfResultSets.Current->next;
 
                                              }
 
                                              if(exists == 0){ /* This node does not exist in any result set, let's return it! */
                                              	//printf("pickExcludedNode: GraphNode %d does not exist in any ResultSet! It is a new one!\n",currentGNode->id);
                                                     return currentGNode;
                                               }
 
                                         }
                                         else{ /* No result set exists yet so any GraphNode is free to use */
											//printf("pickExcludedNode: No ResultSets exists! GraphNode %d will be returned by default!\n",currentGNode->id);
                                                return currentGNode;
                                         }
 
 
                                   }
 
                            }//for
                            currentBucket = getNextBucket(currentChain,currentBucket,errorFlag);
                            if(*errorFlag == EXIT_VALUE){
                                   printf("pickExcludeNode: getNextBucket failed! - Exiting!\n");
                                   return ((GraphNode*) NULL);
                             }
 
                        }
                  }
       		}
                       
       		return ((GraphNode*) NULL); /*No GraphNodes Left to check - stop searching */
 
    	}
    	else{
         	printf("pickExcludedNode: HashMap is NULL! - Exiting!\n");
         	*errorFlag = EXIT_VALUE;
          	return ((GraphNode*) NULL);
     	}
  	}
  	else{
      	printf("pickExcludedNode: Graph is NULL! - Exiting!\n");
      	*errorFlag = EXIT_VALUE;
    	return ((GraphNode*) NULL);
	}
}

int findCoherentGraphs(Graph* g,int* max_size,List* listOfSubGraphs,int mode,int* errorFlag) { //Mode 1 creates a Graph for every Coherent SubGraph in the original
																								  //Mode 2 creates community List for every coherent subgraph of hyperNodes
																								  //Mode 3 creates community List for every coherent subgraph of normal Nodes
																								  //Mode 0 simply calculates max_size of coherent subgraph and number of
  																								  //coherent subgraphs
	int i=0,number_of_cc=0;
	GraphNode* currentGNode = (GraphNode*) NULL;
	GraphNode* tempGNode = (GraphNode*) NULL;
	ResultSet* result_set = (ResultSet*) NULL;
	List listOfResultSets;
	Pair currentResult;
	Graph* subGraph = (Graph*) NULL;
	Community* subComm = (Community*) NULL;	
	Node* myCurrent = (Node*) NULL;
	GraphNode* subGraphNode = (GraphNode*) NULL;
	List* aClique = (List*) NULL;
	*max_size=0;
 
	if(g != NULL){
		if(g->hashtable.hashmap != NULL){ 
			createList(&listOfResultSets, "ResultSetPtr", &allocateResultSetPtr, &destroyResultSetPtr, &compareResultSetPtr, &assignResultSetPtr, &printResultSetPtr);
            while ( (currentGNode = pickExcludedNode(listOfResultSets, g, errorFlag) ) != NULL){
            	if(*errorFlag == EXIT_VALUE){                                                                                                    
                	printf("findCoherentGraphs: errorFlag! Could not allocate ResultSet! - Exiting!\n");
                    return EXIT_VALUE;
                }
                if(currentGNode == NULL){ 
                	printf("findCoherentGraphs: Could not pick a currentGNode! - Exiting!\n");
                    *errorFlag = EXIT_VALUE;
                }
                //else { 
                	//printf("Current node to start reachNodeN is %d\n",currentGNode->id);
               //	}
                /* New coherent subgraph located! Let's explore it */
                result_set = reachNodeN(currentGNode->id, g, errorFlag); /* Prepare a 1 to N search starting with new node */
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
                
				if(mode == 1){ //Graph Construct mode
					if(listOfSubGraphs == NULL){
		           		printf("findCoherentGraphs: NULL List of SubGraphs! - Exiting!\n");
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
                	subGraph = createGraph(3,4,errorFlag);                   	
                    insertNode(currentGNode,subGraph); 	
				}
				if(mode == 2){ //SubGraph contains hyperNodes -cliqueNodes
					if(listOfSubGraphs == NULL){
		           		printf("findCoherentGraphs: NULL List of SubGraphs! - Exiting!\n");
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					subComm = malloc(sizeof(Community));
					if(subComm == NULL){
		           		printf("findCoherentGraphs: subComm is NULL! - Exiting!\n");
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					subComm->members = NULL;
					subComm->members = malloc(sizeof(int) * 1000); //Maximum size of members we can find - I wanted to avoid reallocs or Lists
					if(subComm->members == NULL){
		           		printf("findCoherentGraphs: subComm members is NULL! - Exiting!\n");
						free(subComm);
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					for(i=0; i < 1000; i++){
						subComm->members[i] = -1;
					}
					subComm->numberOfMembers = 0;

					myCurrent = (Node*) NULL;
					aClique = (List*) currentGNode->properties->arrayP[0].attribute;
					//printf("Clique in GraphNode: %d - \n",currentGNode->id);
					//printList(*aClique);
					//printf("\n");

					myCurrent = aClique->Head;
					while(myCurrent != NULL){
						subGraphNode = (GraphNode*) myCurrent->data;
						subComm->members[subComm->numberOfMembers] = subGraphNode->id;
						subComm->numberOfMembers++;

						myCurrent = myCurrent->next;
					}

				}
				if(mode == 3){ //SubGraph contains normal nodes, each subgraph is a community
					if(listOfSubGraphs == NULL){
		           		printf("findCoherentGraphs: NULL List of SubGraphs! - Exiting!\n");
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					subComm = malloc(sizeof(Community));
					if(subComm == NULL){
		           		printf("findCoherentGraphs: subComm is NULL! - Exiting!\n");
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					subComm->members = NULL;
					subComm->members = malloc(sizeof(int) * g->hashtable.num_records); //Maximum size of members we can find - I wanted to avoid reallocs or Lists
					if(subComm->members == NULL){
		           		printf("findCoherentGraphs: subComm members is NULL! - Exiting!\n");
						free(subComm);
		                *errorFlag = EXIT_VALUE;
		                return EXIT_VALUE;
					}
					for(i=0; i < g->hashtable.num_records; i++){
						subComm->members[i] = -1;
					}
					subComm->numberOfMembers = 0;

					subComm->members[subComm->numberOfMembers] = currentGNode->id;
					subComm->numberOfMembers++;
				}

               	number_of_cc++; 
 
                while( next(result_set, &currentResult, errorFlag) ) { //continue; 
                	/* Explore the latest set as much as possible and create a coherent subgraph by doing so */
					if(*errorFlag == EXIT_VALUE) {
						printf("findCoherentGraphs:next failed!\n");
						*errorFlag = EXIT_VALUE;
						return 	EXIT_VALUE;
					} 

					if(mode == 1){ //Graph Construct mode
						tempGNode = lookUpNode(currentResult.ID, g, errorFlag); //Access the GraphNode of this ID in order to insert it in subgraph
						if(*errorFlag == EXIT_VALUE){
							printf("findCoherentGraphs: lookUpNode failed!\n");
							*errorFlag = EXIT_VALUE;
							return 	EXIT_VALUE;
						}                     
						if(lookUpNode(currentResult.ID, subGraph, errorFlag) != NULL){
							printf("findCoherentGraphs: ReachNodeN needs debugging! Records already in subGraph!\n");
							exit(1);
						}
          	
                        insertNode(tempGNode,subGraph); 	
                    }
					if(mode == 2){
						tempGNode = lookUpNode(currentResult.ID, g, errorFlag);
						myCurrent = (Node*) NULL;
						aClique = (List*) tempGNode->properties->arrayP[0].attribute;
						myCurrent = aClique->Head;
						int exists=0;
						//printf("Clique in GraphNode: %d - \n",currentGNode->id);
						//printList(*aClique);
						//printf("\n");
						while(myCurrent != NULL){
							subGraphNode = (GraphNode*) myCurrent->data;
							exists=0;
							for(i=0; i < subComm->numberOfMembers; i++){
								if(subComm->members[i] == subGraphNode->id){
									exists = 1;
									break;
								}
							}
							if(exists == 0){					
								subComm->members[subComm->numberOfMembers] = subGraphNode->id;
								subComm->numberOfMembers++;
							}

							myCurrent = myCurrent->next;
						}
					}
					if(mode == 3){
						int exists=0;
						for(i=0; i < subComm->numberOfMembers; i++){
							if(subComm->members[i] == currentResult.ID){
								exists = 1;
								break;
							}
						}
						if(exists == 0){					
							subComm->members[subComm->numberOfMembers] = currentResult.ID;
							subComm->numberOfMembers++;
						}
					}

				}

				if(mode == 1){
					addLastList(listOfSubGraphs,subGraph);
				}      
				if((mode == 2) || (mode == 3)){
					addLastList(listOfSubGraphs,subComm);
				}

                if(*errorFlag == EXIT_VALUE){
                	printf("findCoherentGraphs: Could not allocate ResultSet! - Exiting!\n");
                    return EXIT_VALUE;
                }
               
                if(result_set->set.num_records > *max_size)
                	*max_size = result_set->set.num_records; /* A subgraph with more nodes has been located! */
			}
 
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

int nodeIsInPath(int id,List path){

	GraphNode* currentNode = (GraphNode*) NULL;

	if(path.items > 0){
		path.Current = path.Head;
		while(path.Current != NULL){
			currentNode = (GraphNode*) path.Current->data;
			if(currentNode == NULL){
				printf("nodeIsInPath: Node is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}
			if(currentNode->id == id) return 1;
			path.Current = path.Current->next;
		}
		return 0;
	}
	else{
		printf("nodeIsInPath: Path is empty! - Exiting!\n");
		return EXIT_VALUE;
	}

}

int edgeIsInPath(int startNodeID,int endNodeID,List path){

	GraphNode* currentNode = (GraphNode*) NULL;

	if(path.items > 0){
		if(path.items > 1){
			Node* myCurrent = path.Head;
			while(myCurrent != NULL){
				currentNode = (GraphNode*) myCurrent->data;
				if(currentNode == NULL){
					printf("edgeIsInPath: Node is NULL! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(currentNode->id == startNodeID){
					if(myCurrent == path.Last){ //This Node is the last in path so no way this edge is in the path
						return 0;
					}
					myCurrent = myCurrent->next;
					if(myCurrent == NULL) return 0;
					currentNode = (GraphNode*) myCurrent->data;
					if(currentNode == NULL){
						printf("edgeIsInPath: Node is NULL! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(currentNode->id == endNodeID){ //Edge exists in path!
						return 1;
					}
					else{ //Mission failed!
						return 0;
					}
				}
				myCurrent = myCurrent->next;
			}
			return 0;
		}
		return 0;
	}
	else{
		printf("edgeIsInPath: Path is empty! - Exiting!\n");
		return EXIT_VALUE;
	}

}

int trustBFSExpansion(List* frontier,HashTable* explored,Graph* trustGraph,int goalID,double* goalTrustValue,int* errorFlag){ /* Used by TidalTrust - TODO: Create a simple BFS customizable for all needs */

	int distance = 0;

	if((frontier == NULL) || (explored == NULL)){
		printf("trustBFSLevelExpansion: Can't use NULL frontier or explored set! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	GraphNode* currentNode = (GraphNode*) NULL;

	currentNode = (GraphNode*) (frontier->Head->data); /* Get Head of frontier */

	if(currentNode == NULL){
		printf("trustBFSLevelExpansion: Could not access Head of queue! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(currentNode->id == goalID){
		*goalTrustValue = currentNode->trustValue;
		return 1;
	}

	distance = currentNode->distanceA;
	checkTrustBFSNeighbours(trustGraph,currentNode,explored,frontier,distance+1,errorFlag); /* Fetch Neighbours, neighbours will be set to distance of current Node + 1 */
	if(*errorFlag == EXIT_VALUE){
		printf("trustBFSLevelExpansion: checkTrustBFSNeighbours failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	popList(frontier);															  /* Remove Head of the frontier */
	if(isEmptyList(*frontier)) return -1; 											  /* No neighbours were added, nothing left to expand for this BFS */

	return 0;
}

int checkTrustBFSNeighbours(Graph* trustGraph,GraphNode* currentNode,HashTable* explored,List* frontier,int distance,int* errorFlag){ /* Used by TidalTrust Visits the neighbour nodes of a node */

	int i=0,currentNeighbourID=0,expanded=0,j=0;
	double trustWeight, * trustWeightPtr;
	Edge* edgeptr = (Edge*) NULL,*currentEdge = (Edge*) NULL;
	EdgeSet* currentEdgeSet = (EdgeSet*) NULL;
	GraphNode* gnodeptr = (GraphNode*) NULL;
	GraphNode *neighbour = (GraphNode*) NULL,*result = (GraphNode*) NULL;

	if(currentNode == NULL){
		printf("checkTrustBFSNeighbours: Current Node is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if((frontier == NULL) || (explored == NULL)){
		printf("checkTrustBFSNeighbours: Queue and Set must be initialised! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	currentNode->EdgeSetList.Current = currentNode->EdgeSetList.Head;
	for(i=0; i < currentNode->EdgeSetList.items; i++){ /* For all EdgeSets the Current GraphNode has */
		currentEdgeSet = (EdgeSet*) currentNode->EdgeSetList.Current->data; /* Extract the EdgeSet */
		if(currentEdgeSet->relation == NULL){
			printf("checkTrustBFSNeighbours: currentEdgeSet relation is NULL! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		if(!strcmp(currentEdgeSet->relation, "trusts")){ /* If the relation this EdgeSet represents is "trusts" (used by person) */
			currentEdgeSet->edges.Current = currentEdgeSet->edges.Head; /* Now we will look at all the Edges of this type */
			for(j=0; j < currentEdgeSet->edges.items; j++){  /* For every Edge */
				edgeptr = (Edge*) currentEdgeSet->edges.Current->data; /* Extract Edge */
				if(edgeptr == NULL){
					printf("checkTrustBFSNeighbours: Can't access Edge! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}
				currentNeighbourID = edgeptr->endNodeID; /* Extract neighbour ID from edge */
				trustWeightPtr = (double*) edgeptr->properties->arrayP[1].attribute;
				trustWeight = *trustWeightPtr;

				neighbour = lookUpNode(currentNeighbourID,trustGraph,errorFlag); /* Find neighbour in graph */
				if(*errorFlag== EXIT_VALUE){
					printf("checkTrustBFSNeighbours: lookUpNode failed! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;
				}

				if(neighbour == NULL){
					printf("checkTrustBFSNeighbours: Something went wrong with lookUpNode...\nEdge but no Node?! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;					
				}

				result = lookUpNodeHashTable(neighbour->id, explored, errorFlag); /* Check to see if current Neighbour has been visited again */
				if(*errorFlag == EXIT_VALUE){
					printf("checkTrustBFSNeighbours: lookUpNodeHashTable failed! - Exiting!\n");
					*errorFlag = EXIT_VALUE;
					return EXIT_VALUE;	
				}

				if(result == NULL){ /* Neighbour has not been visited before */
					neighbour->parentIDA = currentNode->id;
					neighbour->distanceA = distance;
					neighbour->trustValue = currentNode->trustValue * trustWeight; /* Neighbour's trustValue is current's trustValue * trustWeight of edge */
					if(insertNodeHashTable(neighbour,explored) == EXIT_VALUE){ /* Add it to Set */
						printf("checkTrustBFSNeighbours: insertNodeHashTable failed! - Exiting!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;	
					}
					if(addLastList(frontier,neighbour) == EXIT_VALUE){ /* Add it to frontier in order to avoid having multiple same nodes in the frontier */
						printf("checkTrustBFSNeighbours: addLastList failed! - Exiting!\n");
						*errorFlag = EXIT_VALUE;
						return EXIT_VALUE;	
					}
					expanded++;
				}
				else{ //Neighbour is already explored
					if(result->distanceA == distance){ //Same node with same distance
						if(result->parentIDA != currentNode->id){ //But different parents
							if(result->trustValue < currentNode->trustValue* trustWeight){ //This path provides greater trust value
								result->trustValue = currentNode->trustValue* trustWeight; //Keep in explored the one with greater trust value
								result->parentIDA = currentNode->id;					   //And its parent in order to properly assess the path
							}
						}
					}
				}
				currentEdgeSet->edges.Current = currentEdgeSet->edges.Current->next;
			}

			return expanded; /* Returns the number of neighbours met */

		}
		currentNode->EdgeSetList.Current = currentNode->EdgeSetList.Current->next;
	}
	return expanded;

}

int getNumberOfLikes(GraphNode* personLikes,GraphNode* personIsLiked,Graph* postGraph,Graph* personGraph,int forumID,int* errorFlag){

	int postID=0,likes=0,forumFound=0;
	GraphNode* currentPerson = (GraphNode*) NULL;
	GraphNode* postNode = (GraphNode*) NULL;
	EdgeSet* postEdgeSet = (EdgeSet*) NULL, *likeEdgeSet = (EdgeSet*) NULL, *forumEdgeSet = (EdgeSet*) NULL;
	Edge* postEdge = (Edge*) NULL;
	Edge* likeEdge = (Edge*) NULL;
	Edge* forumEdge = (Edge*) NULL;

	if(postGraph == NULL){
		printf("getNumberOfLikes: postGraph is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(personGraph == NULL){
		printf("getNumberOfLikes: personGraph is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	postEdgeSet = findEdgeSet(personIsLiked, "createdPost",errorFlag); //Let's see all posts this user has created
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
		return EXIT_VALUE;
	}
	
	if(postEdgeSet == NULL){ //Person who is Liked has not created any posts
		return 0;
	}

	likeEdgeSet = findEdgeSet(personLikes, "likes",errorFlag); //Let's see all the likes this person has performed
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	if(likeEdgeSet == NULL){ //Person who Likes has not liked any posts
		return 0;
	}

	postEdge = getFirstEdge(postEdgeSet,errorFlag); 						  //Get first post
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
		return EXIT_VALUE;
	}

	while(postEdge != NULL){ 									  //For all posts Person who Likes has created
		postID = postEdge->endNodeID; 						  //Get the id of the post
		postNode = lookUpNode(postID, postGraph,errorFlag); 				  //Access more info about this post
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfLikes: lookUpNode failed! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		if(postNode == NULL){
			printf("getNumberOfLikes: Edge from Person to Post exists but Post does not! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		forumEdgeSet = findEdgeSet(postNode, "containedByForum",errorFlag); //We must find the forum of this post
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(forumEdgeSet == NULL){ 								  //Posts are not contained in any forum
			return 0;
		}
		forumEdge = getFirstEdge(forumEdgeSet,errorFlag);					  //Get first forum
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		while(forumEdge != NULL){
			if(forumEdge->endNodeID == forumID){ 				  //Post is contained by desired forum
				forumFound = 1;
				likeEdge = getFirstEdge(likeEdgeSet,errorFlag);             //Get first Like
				if(*errorFlag == EXIT_VALUE){
					printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				while(likeEdge != NULL){
					if(likeEdge->endNodeID == postID){            //PersonLikes has liked this Post
						likes++;
						if(likeEdgeSet->edges.items == 1) return likes; //No other likes from this user
						break;
					}
					likeEdge = getNextEdge(likeEdgeSet, likeEdge, errorFlag);
					if(*errorFlag == EXIT_VALUE){
						printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
						return EXIT_VALUE;
					}
				}	
			}
			if(forumFound == 1) break;							 //If we found the desired forum there is no meaning in searching more
			forumEdge = getNextEdge(forumEdgeSet, forumEdge, errorFlag);
			if(*errorFlag == EXIT_VALUE){
				printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}	
		postEdge = getNextEdge(postEdgeSet,postEdge,errorFlag);            //Get to the next post
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
			return EXIT_VALUE;
		}
	}

	return likes;

}

int getNumberOfReplies(GraphNode* personReplies,GraphNode* personIsReplied,Graph* postGraph,Graph* personGraph,Graph* commentGraph,int forumID,int* errorFlag){

	int postID=0,replies=0,commentID,forumFound=0,replyID;
	GraphNode* currentPerson = (GraphNode*) NULL;
	EdgeSet* postEdgeSet = (EdgeSet*) NULL, *commentEdgeSet = (EdgeSet*) NULL,*forumEdgeSet = (EdgeSet*) NULL,*replyEdgeSet;
	Edge* postEdge = (Edge*) NULL;
	Edge* forumEdge = (Edge*) NULL;
	Edge* commentEdge = (Edge*) NULL;
	Edge* replyEdge = (Edge*) NULL;
	GraphNode* commentNode = (GraphNode*) NULL;
	GraphNode* postNode = (GraphNode*) NULL;
	GraphNode* replyNode = (GraphNode*) NULL;

	if(postGraph == NULL){
		printf("getNumberOfReplies: postGraph is NULL! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(personGraph == NULL){
		printf("getNumberOfReplies: personGraph is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	postEdgeSet = findEdgeSet(personIsReplied, "createdPost",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}

	if(postEdgeSet == NULL){ 	//Person who gets Replies has not created any posts
		return 0;
	}
	commentEdgeSet = findEdgeSet(personReplies, "createdComment",errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	if(commentEdgeSet == NULL){ //Person who Replies has not replied to any post
		return 0;
	}

	postEdge = getFirstEdge(postEdgeSet,errorFlag);
	if(*errorFlag == EXIT_VALUE){
		printf("getNumberOfReplies: getFirstEdgefailed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return EXIT_VALUE;
	}
	while(postEdge != NULL){																//Search through all posts
		postID = postEdge->endNodeID; 														//Get the id of the post
		postNode = lookUpNode(postID, postGraph,errorFlag); 											//Access more info about this post
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: lookUpNodes failed! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		if(postNode == NULL){
			printf("getNumberOfReplies: Edge from Person to Post exists but Post does not! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}

		forumEdgeSet = findEdgeSet(postNode, "containedByForum",errorFlag); 							//We must find the forum of this post
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}	
		if(forumEdgeSet == NULL){ 								  							//Posts are not contained in any forum
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}
		forumFound = 0;
		forumEdge = getFirstEdge(forumEdgeSet,errorFlag);					  							//We must locate the desired forum
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
			*errorFlag = EXIT_VALUE;
			return EXIT_VALUE;
		}	
		while(forumEdge != NULL){
			if(forumEdge->endNodeID == forumID){ 				 							//Post is contained by desired forum
				forumFound = 1;		
				replyEdgeSet = findEdgeSet(postNode, "PostHasComment",errorFlag);
				if(*errorFlag == EXIT_VALUE){
					printf("getNumberOfReplies: findEdgeSet failed! - Exiting!\n");
					return EXIT_VALUE;
				}			
				if(replyEdgeSet != NULL){ //Get all replies to this post
					replyEdge = getFirstEdge(replyEdgeSet,errorFlag);	
					if(*errorFlag == EXIT_VALUE){
						printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
						return EXIT_VALUE;
					}				
					while(replyEdge != NULL){
						replyID = replyEdge->endNodeID;										//ID of a comment that acts as reply to this post
						replyNode = lookUpNode(replyID, commentGraph, errorFlag);						//Locate more info about this comment
						if(*errorFlag == EXIT_VALUE){
							printf("getNumberOfReplies: lookUpNode failed! - Exiting!\n");
							return EXIT_VALUE;
						}
						if(replyNode == NULL){
							printf("getNumberOfReplies: Edge from Post to Comment leads to NULL replyNode! - Exiting!\n");
							return EXIT_VALUE;
						}
								
						commentEdge = getFirstEdge(commentEdgeSet,errorFlag); //Run through all comments this user has created
						if(*errorFlag == EXIT_VALUE){
							printf("getNumberOfReplies: getFirstEdge failed! - Exiting!\n");
							return EXIT_VALUE;
						}
						while(commentEdge != NULL){
							if(replyID == commentEdge->endNodeID){	//The reply was created by this user!
								replies++;							//NOTE: A user might have replied multiple times to a post
								break;
							}
							commentEdge = getNextEdge(commentEdgeSet,commentEdge,errorFlag);
							if(*errorFlag == EXIT_VALUE){
								printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
								return EXIT_VALUE;
							}
						}
						replyEdge = getNextEdge(replyEdgeSet, replyEdge, errorFlag);
						if(*errorFlag == EXIT_VALUE){
							printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
			if(forumFound == 1) break;							 //If we found the desired forum there is no meaning in searching more
			forumEdge = getNextEdge(forumEdgeSet, forumEdge, errorFlag);    //Move to the next forum
			if(*errorFlag == EXIT_VALUE){
				printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}	
		postEdge = getNextEdge(postEdgeSet,postEdge, errorFlag); //Check next post of person who gets replies
		if(*errorFlag == EXIT_VALUE){
			printf("getNumberOfReplies: getNextEdge failed! - Exiting!\n");
			return EXIT_VALUE;
		}
	}

	return replies;

}


////////////////////////////////////////RESULTSET PTR FOR LIST ////////////////////////

/* ResultSetPtr functions for List */

int allocateResultSetPtr(void** target, void* source){

	ResultSet** tempA = (ResultSet**) target;
	ResultSet* tempB = (ResultSet*) source;

	*tempA = tempB;

	return 0;
}

int destroyResultSetPtr(void** target){

	ResultSet** tempA = (ResultSet**) target;

	*tempA = (ResultSet*) NULL;

	return 0;
}

int compareResultSetPtr(void* target,void* source){

	printf("compareResultSetPtr: Is not ready for comparisons between two result sets! - Exiting!\n");
	return EXIT_VALUE;

}

int assignResultSetPtr(void* target,void* source){

	printf("assignResultSetPtr: Is not ready for assignment between two result sets! - Exiting!\n");
	return EXIT_VALUE;

}

int printResultSetPtr(void* target){

	printf("printResultSetPtr: Is not ready for print! - Exiting!\n");
	return EXIT_VALUE;

}
