#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "GraphQueries.h"
#include "fileReader.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: fileReader.c
 MAIN AUTHOR: GEORGE LAMPRAKOPOYLOS														

 Details: This file contains all the functions used to handle reading and loading data from the
 DataSet. For each kind of GraphNode of Edge that exists we have created a different function
 to read the data from the corresponding file. 

 Keep in mind here that currently we are using the following policy: Any file that creates 
 GraphNodes such as Forums, Person, Post, Comment etc has a Graph created for it in order
 to allow for faster search. Functions for Edges such as forum has person member will add
 an Edge in the Forum Graph with StartID the Forum and EndID the ID of the Person.

 Finally we keep the addresses of the GraphNodes and Edges in Lists that will be used by main
 later for proper de-allocation of these memory structs. We do so because our Graphs don't
 have any responsibility for GraphNode/Edge allocation/deallocation.

 ***RECENT UPDATE: When adding an Edge for example from Forum to Person we also add the opposite
 Edge to allow for better flexibility in searching the desired info. Note also that in this case
 you are not required to have both personGraph and forumGraph initialised. The function will
 see if they are initialised and will add the edge to the proper Graph. This means that you can
 have an Edge from Person to Forum but no from Forum to Person if the Forum Graph does not exist

 //TODO: MODIFY GRAPHNODE AND EDGES TO HOLD LONG INT
 //TODO: SPEEDUP FILEREADING BY REDUCING COMPARISONS
 //TODO: CHANGE MAX_CHARS
 //TODO: CHECK FOR MEMORY LEAKS
================================================================================================*/

int setNULLDataSetStructs(AllDataSetStructs* allDataSetStructs){

	if(allDataSetStructs != NULL){
		allDataSetStructs->personGraph = NULL,allDataSetStructs->postGraph = NULL,allDataSetStructs->forumGraph = NULL,allDataSetStructs->tagGraph = NULL,allDataSetStructs->commentGraph = NULL,allDataSetStructs->organisationGraph = NULL,allDataSetStructs->placeGraph = NULL;

		allDataSetStructs->personList = NULL,allDataSetStructs->postList = NULL,allDataSetStructs->forumList = NULL,allDataSetStructs->tagList = NULL,allDataSetStructs->commentList = NULL,allDataSetStructs->organisationList = NULL,allDataSetStructs->placeList = NULL,allDataSetStructs->personKnowsPersonList = NULL,allDataSetStructs->personIsLocatedInPlaceList = NULL,allDataSetStructs->personLikesPostList = NULL,allDataSetStructs->personStudyAtList = NULL,allDataSetStructs->personWorksAtList = NULL,allDataSetStructs->personHasInterestTagList = NULL,allDataSetStructs->forumHasMemberPersonList = NULL,allDataSetStructs->forumContainerPostList = NULL,allDataSetStructs->commentHasCreatorPersonList,allDataSetStructs->commentReplyOfPostList = NULL,allDataSetStructs->postHasCreatorPersonList = NULL;

	}

	return 0;

}

int emptyAndDestroyList(List* list,char* nameList,int type){ //type == 1 means destroy Graphnode List

	if(list != NULL){
		if(list->items > 0){
			list->Current = list->Head;
			while(list->Current != NULL){
				if(type == 1){
					destroyGraphNode( (GraphNode**) &(list->Current->data) );
				}
				else{
					destroyEdge( (Edge**) &(list->Current->data) );
				}
				(list)->Current = (list)->Current->next;
			}
		}
		destroyList((list));
		free((list));
		printf("destroyDataSetStructs: Destroyed %s!\n",nameList);
	}

	return 0;

}

int destroyDataSetStructs(AllDataSetStructs* allDataSetStructs){

	printf("destroyDataSetStructs: Freeing DataSet resources!\n");

	if(allDataSetStructs != NULL){
		if(allDataSetStructs->personGraph != NULL){
			destroyGraph(allDataSetStructs->personGraph);
			printf("destroyDataSetStructs: Destroyed personGraph!\n");
		}
		if(allDataSetStructs->postGraph != NULL){
			destroyGraph(allDataSetStructs->postGraph);
			printf("destroyDataSetStructs: Destroyed postGraph!\n");
		}
		if(allDataSetStructs->forumGraph != NULL){
			destroyGraph(allDataSetStructs->forumGraph);
			printf("destroyDataSetStructs: Destroyed forumGraph!\n");
		}
		if(allDataSetStructs->tagGraph != NULL){
			destroyGraph(allDataSetStructs->tagGraph);
			printf("destroyDataSetStructs: Destroyed tagGraph!\n");
		}
		if(allDataSetStructs->commentGraph != NULL){
			destroyGraph(allDataSetStructs->commentGraph);
			printf("destroyDataSetStructs: Destroyed commentGraph!\n");
		}
		if(allDataSetStructs->organisationGraph != NULL){
			destroyGraph(allDataSetStructs->organisationGraph);
			printf("destroyDataSetStructs: Destroyed organisationGraph!\n");
		}
		if(allDataSetStructs->placeGraph != NULL){
			destroyGraph(allDataSetStructs->placeGraph);
			printf("destroyDataSetStructs: Destroyed placeGraph!\n\n");
		}

		if(allDataSetStructs->personList != NULL){
			emptyAndDestroyList(allDataSetStructs->personList,"personList",1);
		}
		if(allDataSetStructs->postList != NULL){
			emptyAndDestroyList(allDataSetStructs->postList,"postList",1);
		}
		if(allDataSetStructs->forumList != NULL){
			emptyAndDestroyList(allDataSetStructs->forumList,"forumList",1);
		}
		if(allDataSetStructs->tagList != NULL){
			emptyAndDestroyList(allDataSetStructs->tagList,"tagList",1);
		}
		if(allDataSetStructs->commentList != NULL){
			emptyAndDestroyList(allDataSetStructs->commentList,"commentList",1);
		}
		if(allDataSetStructs->organisationList != NULL){
			emptyAndDestroyList(allDataSetStructs->organisationList,"organisationList",1);
		}
		if(allDataSetStructs->placeList != NULL){
			emptyAndDestroyList(allDataSetStructs->placeList,"placeList",1);
		}
		if(allDataSetStructs->personKnowsPersonList != NULL){
			emptyAndDestroyList(allDataSetStructs->personKnowsPersonList,"personKnowsPersonList",2);
		}
		if(allDataSetStructs->personIsLocatedInPlaceList != NULL){
			emptyAndDestroyList(allDataSetStructs->personIsLocatedInPlaceList,"personIsLocatedInPlaceList",2);
		}
		if(allDataSetStructs->personLikesPostList != NULL){
			emptyAndDestroyList(allDataSetStructs->personLikesPostList,"personLikesPostList",2);
		}
		if(allDataSetStructs->personStudyAtList != NULL){
			emptyAndDestroyList(allDataSetStructs->personStudyAtList,"personStudyAtList",2);
		}
		if(allDataSetStructs->personWorksAtList != NULL){
			emptyAndDestroyList(allDataSetStructs->personWorksAtList,"personWorksAtList",2);
		}
		if(allDataSetStructs->personHasInterestTagList != NULL){
			emptyAndDestroyList(allDataSetStructs->personHasInterestTagList,"personHasInterestTagList",2);
		}
		if(allDataSetStructs->forumHasMemberPersonList != NULL){
			emptyAndDestroyList(allDataSetStructs->forumHasMemberPersonList,"forumHasMemberPersonList",2);
		}
		if(allDataSetStructs->forumContainerPostList != NULL){
			emptyAndDestroyList(allDataSetStructs->forumContainerPostList,"forumContainerPostList",2);
		}
		if(allDataSetStructs->commentHasCreatorPersonList != NULL){
			emptyAndDestroyList(allDataSetStructs->commentHasCreatorPersonList,"commentHasCreatorPersonList",2);
		}
		if(allDataSetStructs->commentReplyOfPostList != NULL){
			emptyAndDestroyList(allDataSetStructs->commentReplyOfPostList,"commentReplyOfPostList",2);
		}
		if(allDataSetStructs->postHasCreatorPersonList != NULL){
			emptyAndDestroyList(allDataSetStructs->postHasCreatorPersonList,"postHasCreatorPersonList",2);
		}

	}

	return 0;

}

int createGraphsAndLists(Graph** graphPtrA,Graph** graphPtrB,List** listPtr,int type,int mode,int m ,int c,char* dataSetFolder,char* nameOfItem,char* filename,int (*readerFunction) (char*, List*, Graph*, Graph*)){

	int errorFlag = 0;

	if((type != 1) && (type != 2)){
		printf("createGraphsAndLists: Invalid type: Expected 1==GraphNode or 2==Edge! - Exiting!\n");
		return EXIT_VALUE;
	}

	*listPtr = NULL;
	*listPtr = malloc(sizeof(List));
	if(*listPtr == NULL){
		printf("createGraphsAndLists: Failed to allocate memory for %sList! - Exiting!\n",nameOfItem);
		return EXIT_VALUE;
	}

	if(type == 1){
		*graphPtrA = NULL;
		*graphPtrA = createGraph(m,c,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("createGraphsAndLists: createGraph failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		printf("createGraphsAndLists: Created %sGraph!\n",nameOfItem);

		createList(*listPtr,"GraphNodePtr",&allocateGraphNodePtr,&destroyGraphNodePtr,&compareGraphNodesPtr,&assignGraphNodePtr,&printGraphNodePtr);
		if(loadGraphItems("GraphNode",*graphPtrA,NULL,*listPtr,mode,dataSetFolder,nameOfItem,filename,readerFunction) == EXIT_VALUE){
			printf("createGraphsAndLists: loadGraphItems failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		printf("createGraphsAndLists: Created %sList!\n",nameOfItem);
	}
	else{
		createList(*listPtr,"EdgePtr",&allocateEdgePtr,&destroyEdgePtr,&compareEdgePtr,&assignEdgePtr,&printEdgePtr);
		if(graphPtrB == NULL){
			if(loadGraphItems("Edge",*graphPtrA,NULL,*listPtr, mode,dataSetFolder,nameOfItem,filename,readerFunction) == EXIT_VALUE){
				printf("createGraphsAndLists: loadGraphItems failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
		else{
			if(loadGraphItems("Edge",*graphPtrA,*graphPtrB,*listPtr, mode,dataSetFolder,nameOfItem,filename,readerFunction) == EXIT_VALUE){
				printf("createGraphsAndLists: loadGraphItems failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	}

}

int loadDataSet(char* dataSetFolder,int mode,AllDataSetStructs* allStructs,int m,int c){

	/////LOAD GRAPHNODES////
	if(createGraphsAndLists(&(allStructs->personGraph),NULL,&(allStructs->personList),1,mode,m,c,dataSetFolder,"person","person.csv",&personReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->postGraph),NULL,&(allStructs->postList),1,mode,m,c,dataSetFolder,"post","post.csv",&postReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at postReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->forumGraph),NULL,&(allStructs->forumList),1,mode,m,c,dataSetFolder,"forum","forum.csv",&forumReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at forumReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->commentGraph),NULL,&(allStructs->commentList),1,mode,m,c,dataSetFolder,"comment","comment.csv",&commentReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at commentReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->placeGraph),NULL,&(allStructs->placeList),1,mode,m,c,dataSetFolder,"place","place.csv",&placeReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at placeReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->organisationGraph),NULL,&(allStructs->organisationList),1,mode,m,c,dataSetFolder,"organisation","organisation.csv",&organisationReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at organisationReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->tagGraph),NULL,&(allStructs->tagList),1,mode,m,c,dataSetFolder,"tag","tag.csv",&tagReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at tagReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}

	/////LOAD EDGES//////
	if(createGraphsAndLists(&(allStructs->personGraph),NULL,&(allStructs->personKnowsPersonList),2,mode,m,c,dataSetFolder,"personKnowsPersonEdge","person_knows_person.csv",&personKnowsPersonReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personKnowsPersonReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->personGraph),&(allStructs->placeGraph),&(allStructs->personIsLocatedInPlaceList),2,mode,m,c,dataSetFolder,"personIsLocatedInPlaceEdge","person_isLocatedIn_place.csv",&personIsLocatedInPlaceReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personIsLocatedInPlaceReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->personGraph),&(allStructs->postGraph),&(allStructs->personLikesPostList),2,mode,m,c,dataSetFolder,"personLikesPostEdge","person_likes_post.csv",&personLikesPostReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personLikesPostReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->personGraph),&(allStructs->organisationGraph),&(allStructs->personWorksAtList),2,mode,m,c,dataSetFolder,"personWorksAtEdge","person_workAt_organisation.csv",&personWorkAtOrganisationReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personWorkAtOrganisationReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->personGraph),&(allStructs->organisationGraph),&(allStructs->personStudyAtList),2,mode,m,c,dataSetFolder,"personStudiesAtEdge","person_studyAt_organisation.csv",&personStudyAtOrganisationReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personStudyAtOrganisationReader failed! - Exiting!\n");
		return EXIT_VALUE;		
	}
	if(createGraphsAndLists(&(allStructs->personGraph),&(allStructs->tagGraph),&(allStructs->personHasInterestTagList),2,mode,m,c,dataSetFolder,"personHasInterestTagList","person_hasInterest_tag.csv",&personHasInterestTagReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at personHasInterestTagReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->forumGraph),&(allStructs->personGraph),&(allStructs->forumHasMemberPersonList),2,mode,m,c,dataSetFolder,"forumHasMemberPersonList","forum_hasMember_person.csv",&forumHasMemberPersonReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at forumHasMemberPersonReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->forumGraph),&(allStructs->postGraph),&(allStructs->forumContainerPostList),2,mode,m,c,dataSetFolder,"forumContainerPostList","forum_containerOf_post.csv",&forumContainerPostReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at forumContainerPostReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->commentGraph),&(allStructs->personGraph),&(allStructs->commentHasCreatorPersonList),2,mode,m,c,dataSetFolder,"commentHasCreatorPersonList","comment_hasCreator_person.csv",&commentHasCreatorPersonReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at commentHasCreatorPersonReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->commentGraph),&(allStructs->postGraph),&(allStructs->commentReplyOfPostList),2,mode,m,c,dataSetFolder,"commentReplyOfPostList","comment_replyOf_comment.csv",&commentReplyOfPostReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at commentReplyOfPostReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}
	if(createGraphsAndLists(&(allStructs->postGraph),&(allStructs->personGraph),&(allStructs->postHasCreatorPersonList),2,mode,m,c,dataSetFolder,"postHasCreatorPersonList","post_hasCreator_person.csv",&postHasCreatorPersonReader) == EXIT_VALUE){
		printf("loadDataSet: createGraphsAndLists at postHasCreatorPersonReader failed! - Exiting!\n");
		return EXIT_VALUE;	
	}

}

int loadGraphItems(char* item_type,Graph* graph,Graph* secondGraph,List* itemList,int mode,char* dataSetFolder,char* nameOfItem,char* filename,int (*readerFunction) (char*, List*, Graph*, Graph*)){

	int answer=0;
	char* tempPath= (char*) NULL;

	if(graph != NULL){
		if(itemList == NULL){
			printf("loadGraphItems: Given List is NULL!\n");
			return EXIT_VALUE;	
		}

		if(mode == DATASET_DEBUG){
			if(!strcmp(item_type,"GraphNode")){
				printf("\nloadDataSet: Fill %sGraph with GraphNodes from %s? Yes=1|No=0\n\n",nameOfItem,filename);
			}
			else if(!strcmp(item_type,"Edge")){
				printf("\nloadDataSet: Fill Graph with Edge from %s? Yes=1|No=0\n\n",filename);
			}
			else{
				printf("\nloadDataSet: Error item type is neither Edge nor GraphNode!\n\n");
				return EXIT_VALUE;	
			}
			answer=0;
			do{
				scanf("%d",&answer);
			}while((answer != 0) && (answer != 1));		
			if(answer == 1){
				printf("\nloadDataSet: Opening %s...\n",filename);
				if(dataSetFolder == NULL){
					if(readerFunction(filename, itemList, graph, secondGraph) == EXIT_VALUE){
						return EXIT_VALUE;	
					}
				}
				else{
					tempPath = (char*) NULL;
					tempPath = malloc(sizeof(char)*(strlen(filename) + strlen(dataSetFolder) + 1));
					if(tempPath == NULL){
						printf("loadDataSet: Failed to allocate memory for file pathname!\n");
						return EXIT_VALUE;	
					}
					strcpy(tempPath, dataSetFolder);
					strcat(tempPath, filename);
					if(readerFunction(tempPath, itemList, graph, secondGraph) == EXIT_VALUE){
						if(tempPath != NULL){
							free(tempPath);
							tempPath = (char*) NULL;
						}			
						return EXIT_VALUE;	
					}
					if(tempPath != NULL){
						free(tempPath);
						tempPath = (char*) NULL;
					}
				}
				if(!strcmp(item_type,"GraphNode")){
					printf("\nloadDataSet: We added a total of %d GraphNodes in %sGraph!\n\n", itemList->items, nameOfItem);	
				}
				else if(!strcmp(item_type,"Edge")){
					printf("\nloadDataSet: We added a total of %d Edges in Graph!\n\n", itemList->items);	
				}
				else{
					printf("\nloadDataSet: Error item type is neither Edge nor GraphNode!\n\n");
					return EXIT_VALUE;	
				}
			}
		}
		else{
			printf("\nloadDataSet: Opening %s...\n",filename);
			if(dataSetFolder == NULL){
				if(readerFunction(filename, itemList, graph, secondGraph) == EXIT_VALUE){
					return EXIT_VALUE;	
				}
			}
			else{
				tempPath = (char*) NULL;
				tempPath = malloc(sizeof(char)*(strlen(filename) + strlen(dataSetFolder) + 1));
				if(tempPath == NULL){
					printf("loadDataSet: Failed to allocate memory for file pathname!\n\n");
					return EXIT_VALUE;	
				}
				strcpy(tempPath, dataSetFolder);
				strcat(tempPath, filename);
				if(readerFunction(tempPath, itemList, graph, secondGraph) == EXIT_VALUE){
					return EXIT_VALUE;	
				}
				if(tempPath != NULL){
					free(tempPath);
					tempPath = (char*) NULL;
				}
			}
			if(!strcmp(item_type,"GraphNode")){
				printf("\nloadDataSet: We added a total of %d GraphNodes in %sGraph!\n\n", itemList->items, nameOfItem);	
			}
			else if(!strcmp(item_type,"Edge")){
				printf("\nloadDataSet: We added a total of %d Edges in Graph!\n\n", itemList->items);	
			}
			else{
				printf("\nloadDataSet: Error item type is neither Edge nor GraphNode!\n\n");
				return EXIT_VALUE;	
			}
		}
	}
	else{
		printf("loadDataSet: Graph is NULL!\n");
		return EXIT_VALUE;	
	}

}

int addCharacterToIntegerField(char fieldBuffer[],char c,int* field_c,int* i,int* resultPtr){

	int result = 0;

	if((c != '|') && (c != '\n') && (c != '\0')){ //Normal character for this field
		//printf("addCharacterToIntegerField: Adding Letter: %c to integer field!\n",c);
		fieldBuffer[*field_c] = c; //Insert character in field buff
		(*field_c)++; //Increase field counter by 1
		fieldBuffer[*field_c] = '\0'; //Required to make this array a printable string
		result = (atoi(fieldBuffer));
		*resultPtr = result;
	}
	else{ //When either | or '\n' has been encountered this field has ended
		//printf("Moving to next line because: |%c| - j: %d - line_length: %d\n",c,j,line_length);
		if(*field_c == 0){ //This field is basically empty because no normal characters were ever encountered
			result = 0;
		}
		else{ //Field has been formed
			fieldBuffer[*field_c] = '\0'; //Finalise string
			result = (atoi(fieldBuffer));
		}
		(*i)++; //Move to the next field
		*resultPtr = result;
		//printf("addCharacterToIntegerField: Finalised integer field to value: %d\n",result);
	}


}

int addCharacterToStringField(char fieldBuffer[],char c,int* field_c,int* i){

	if((c != '|') && (c != '\n') && (c != '\0')){ //Normal character for this field
		//printf("addCharacterToStringField: Adding Letter: %c to string field!\n",c);
		fieldBuffer[*field_c] = c; //Insert character in field buff
		(*field_c)++; //Increase field counter by 1
		fieldBuffer[*field_c] = '\0'; //Required to make this array a printable string
	}
	else{ //When either | or '\n' has been encountered this field has ended
		//printf("Moving to next line because: |%c| - j: %d - line_length: %d\n",c,j,line_length);
		if(*field_c == 0){ //This field is basically empty because no normal characters were ever encountered
			strcpy(fieldBuffer,"N/A");
		}
		else{ //Field has been formed
			fieldBuffer[*field_c] = '\0'; //Finalise string
		}
		(*i)++; //Move to the next field
		//printf("addCharacterToStringField: Finalised string field to value: %s\n",fieldBuffer);
	}

}

//////////////////////////////////////////////////////////////PERSON //////////////////////////////////////////////////////////////////////////////////////

int personReader(char* filename, List* personList, Graph* g, Graph* leaveNULLGraph){ /* Fill Graph g with GraphNodes that represent people and keeps all GraphNodes in a List for proper de-allocation later*/

	FILE *in= (FILE*) NULL;
	char *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,firstName_c=0,lastName_c=0,gender_c=0,birthday_c=0,creationDate_c=0,locationIP_c=0,browserUsed_c=0,line_length=0;
	char id_array[10];
	char firstName[50];
	char lastName[50];
	char gender[10];
	char birthday[20];
	char creationDate[30];
	char locationIP[20];
	char browserUsed[20];
	char c;
	char str[1024];
	int errorFlag=0;
	
	GraphNode* currentPerson = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\nPersonReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	printf("\npersonReader: Populating Graph with Person from file! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		//printf("personReader: Got Line: %s\n",str);
		i = 1,j=0,id = 0,id_c = 0,firstName_c = 0,lastName_c = 0,gender_c = 0,birthday_c = 0,creationDate_c = 0,locationIP_c = 0,browserUsed_c = 0;

		do{
			c = str[j]; //Get character in line buffer
			//printf("Currently examining Letter: %c - i:%d\n",c,i);
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1: //ID Field
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(firstName,c,&firstName_c,&i);
					break;
				case 3:
					addCharacterToStringField(lastName,c,&lastName_c,&i);
					break;
				case 4:
					addCharacterToStringField(gender,c,&gender_c,&i);
					break;
				case 5:
					addCharacterToStringField(birthday,c,&birthday_c,&i);
					break;
				case 6: 
					addCharacterToStringField(creationDate,c,&creationDate_c,&i);
					break;
				case 7: 
					addCharacterToStringField(locationIP,c,&locationIP_c,&i);
					break;
				case 8: 
					addCharacterToStringField(browserUsed,c,&browserUsed_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));
		currentPerson = setPersonProperties(id, firstName, lastName, gender, birthday, creationDate, locationIP, browserUsed,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("personReader: setPersonProperties Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addLastList(personList, currentPerson) == EXIT_VALUE){
			printf("personReader: addLastList Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentPerson, g) == EXIT_VALUE){
			printf("personReader: insertNode Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printPersonProperties(currentPerson) == EXIT_VALUE){
				printf("personReader: printPersonProperties Failed - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	}

	fclose(in); 
}

int personKnowsPersonReader(char* filename, List* personKnowsPersonList,Graph* g, Graph* leaveNULLGraph){ /* Fill Graph g with Edges that represent that a Person Knows another Person */

	FILE *in=(FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,line_length=0;
	char c;
	char id_array1[10];
	char id_array2[10];
	int errorFlag=0;
	
	Edge* currentEdge = NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\npersonKnowsPersonReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));

	printf("\npersonKnowsPersonReader: Populating with PersonKnowsPerson edges! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		//printf("\npersonKnowsPersonReader: Got Line: |%s|\n",str);
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					//printf("NEVER GOT HERE!!!!\n");
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentEdge = setPersonKnowsPersonProperties(startID, endID, "knows", 0, &errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("personKnowsPersonReader: setPersonKnowsPersonProperties Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addLastList(personKnowsPersonList, currentEdge) == EXIT_VALUE){
			printf("personKnowsPersonReader: addLastList Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertEdge(startID, currentEdge, g) == EXIT_VALUE){
			printf("personKnowsPersonReader: insertEdge Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printPersonKnowsPersonProperties(currentEdge) == EXIT_VALUE){
				printf("personKnowsPersonReader: printPersonKnowsPersonProperties Failed - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	}

	fclose(in);
 
}

int personIsLocatedInPlaceReader(char* filename, List* personIsLocatedInPlaceList,Graph* personGraph,Graph* placeGraph){ /* Fill Graph g with Edges that represent that a Person is Located in a Place and also in Place Graph fill an Edge to the Person that is located in this place for easier access*/
	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,startID=0,endID=0,id_c1,id_c2,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag=0;

	Edge* currentEdge = (Edge*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\npersonIsLocatedInPlaceReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\npersonIsLocatedInPlaceReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npersonIsLocatedInPlaceReader: Populating with personIsLocated and placeHostPerson edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));
		if(personGraph != NULL){ /* Necessary to read through file normally but not attempt to add anything to a non initialised Graph */
								 /* NOTE: We may not care about a certain Graph yet so it may not be initialised, it is not wrong */
			if(personGraph->hashtable.num_records > 0){
				currentEdge = setPersonIsLocatedInPlaceProperties(startID, endID, "isLocatedIn",&errorFlag); /* Relation is "isLocatedIn", (no weight) */	
				if(errorFlag == EXIT_VALUE){
					printf("personIsLocatedInPlaceReader: setPersonIsLocatedInPlaceProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}	
				if(addLastList(personIsLocatedInPlaceList, currentEdge) == EXIT_VALUE){
					printf("personIsLocatedInPlaceReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				//printf("Adding in personGraph Edge: %d-%d\n",startID,endID);
				if(insertEdge(startID, currentEdge, personGraph) == EXIT_VALUE){
					printf("personIsLocatedInPlaceReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printPersonIsLocatedInPlaceProperties(currentEdge) == EXIT_VALUE){
						printf("personIsLocatedInPlaceReader: printPersonIsLocatedInPlaceProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}
		if(symmetric_addition == 1){
			if(placeGraph != NULL){
				if(placeGraph->hashtable.num_records > 0){
					currentEdge = setPersonIsLocatedInPlaceProperties(endID, startID, "hostsPerson",&errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("personIsLocatedInPlaceReader: setPersonIsLocatedInPlaceProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}	
					if(addLastList(personIsLocatedInPlaceList, currentEdge) == EXIT_VALUE){
						printf("personIsLocatedInPlaceReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					//printf("Adding in placeGraph Edge: %d-%d\n",endID,startID);
					//printHashTable(&(placeGraph->hashtable));
					if(insertEdge(endID, currentEdge, placeGraph) == EXIT_VALUE){
						printf("personIsLocatedInPlaceReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPersonIsLocatedInPlaceProperties(currentEdge) == EXIT_VALUE){
							printf("personIsLocatedInPlaceReader: printPersonIsLocatedInPlaceProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}
	fclose(in);
}

int personLikesPostReader(char* filename, List* personLikesPostList, Graph* personGraph, Graph* postGraph) { /* Sets An Edge from Person to Post representing Like and an Edge from Post to Person showing who liked this post */
	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,startID=0,endID=0,id_c1,id_c2,creationDate_c=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char creationDate[30];
	char c;
	int errorFlag=0;

	Edge* currentEdge = (Edge*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}
	int answer=0;
	printf("\npersonLikesPostReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\npersonLikesPostReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npersonLikesPostReader: Populating with personLikesPost and postIsLikedbyPerson edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0,creationDate_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
				case 3:
					addCharacterToStringField(creationDate,c,&creationDate_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		if(personGraph != NULL){
			if(personGraph->hashtable.num_records > 0){
				currentEdge = setPersonLikesPostProperties(startID, endID, "likes", creationDate, &errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("personLikesPostReader: setPersonLikesPostProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(personLikesPostList, currentEdge) == EXIT_VALUE){
					printf("personLikesPostReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, personGraph) == EXIT_VALUE){
					printf("personLikesPostReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printPersonLikesPostProperties(currentEdge) == EXIT_VALUE){
						printf("personLikesPostReader: printPersonLikesPostProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(postGraph != NULL){
				if(postGraph->hashtable.num_records > 0){
					currentEdge = setPersonLikesPostProperties(endID, startID, "isLikedByPerson", creationDate, &errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("personLikesPostReader: setPersonLikesPostProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(personLikesPostList, currentEdge) == EXIT_VALUE){
						printf("personLikesPostReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, postGraph) == EXIT_VALUE){
						printf("personLikesPostReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPersonLikesPostProperties(currentEdge) == EXIT_VALUE){
							printf("personLikesPostReader: printPersonLikesPostProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}
	fclose(in);

}

int personStudyAtOrganisationReader(char* filename, List* personStudyAtOrganisationList,Graph* personGraph, Graph* organisationGraph){ /* Sets An Edge from person to Organisation represent that a person studies there and an edge from organisation to person to know all people that study at an organisation */
	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,startID=0,endID=0,id_c1,id_c2,classYear_c=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char classYear[10];
	char c;
	int errorFlag = 0;

	Edge* currentEdge = (Edge*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}
	int answer=0;
	printf("\npersonStudyAtOrganisationReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\npersonStudyAtOrganisationReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npersonStudyAtOrganisationReader: Populating with personStudiesAtOrganisation and PersonStudiesHere edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0,classYear_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
				case 3:
					addCharacterToStringField(classYear,c,&classYear_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));
		if(personGraph != NULL){
			if(personGraph->hashtable.num_records > 0){
				currentEdge = setPersonStudyAtOrganisationProperties(startID, endID, "studyAt",classYear,&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("personStudyAtOrganisationReader: setPersonStudyAtOrganisationProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(personStudyAtOrganisationList, currentEdge) == EXIT_VALUE){
					printf("personStudyAtOrganisationReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, personGraph) == EXIT_VALUE){
					printf("personStudyAtOrganisationReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printPersonStudyAtOrganisationProperties(currentEdge) == EXIT_VALUE){
						printf("personStudyAtOrganisationReader: printPersonStudyAtOrganisationProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(organisationGraph != NULL){
				if(organisationGraph->hashtable.num_records > 0){
					currentEdge = setPersonStudyAtOrganisationProperties(endID, startID, "PersonStudiesHere",classYear,&errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("personStudyAtOrganisationReader: setPersonStudyAtOrganisationProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(personStudyAtOrganisationList, currentEdge) == EXIT_VALUE){
						printf("personStudyAtOrganisationReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, organisationGraph) == EXIT_VALUE){
						printf("personStudyAtOrganisationReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPersonStudyAtOrganisationProperties(currentEdge) == EXIT_VALUE){
							printf("personStudyAtOrganisationReader: printPersonStudyAtOrganisationProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}
	}
	fclose(in);
 
}

int personWorkAtOrganisationReader(char* filename, List* personWorkAtOrganisationList,Graph* personGraph,Graph* organisationGraph){

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,startID=0,endID=0,id_c1,id_c2,workFrom_c=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char workFrom[10];
	char c;
	int errorFlag = 0;
	
	Edge* currentEdge = (Edge*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}
	int answer=0;
	printf("\npersonWorkAtOrganisationReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\npersonWorkAtOrganisationReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npersonWorkAtOrganisationReader: Populating with personWorksAtOrganisation and PersonWorksHere edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0,workFrom_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
				case 3:
					addCharacterToStringField(workFrom,c,&workFrom_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));
		if(personGraph != NULL){
			if(personGraph->hashtable.num_records > 0){
				currentEdge = setPersonWorkAtOrganisationProperties(startID, endID, "workAt",workFrom,&errorFlag); /* Relation is "workAt", (no weight) */
				if(errorFlag == EXIT_VALUE){
					printf("personWorkAtOrganisationReader: setPersonWorkAtOrganisationProperties Failed - Exiting!\n");
					return EXIT_VALUE;				
				}
				if(addLastList(personWorkAtOrganisationList, currentEdge) == EXIT_VALUE){
					printf("personWorkAtOrganisationReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, personGraph) == EXIT_VALUE){
					printf("personWorkAtOrganisationReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printPersonWorkAtOrganisationProperties(currentEdge) == EXIT_VALUE){
						printf("personWorkAtOrganisationReader: printPersonWorkAtOrganisationProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(organisationGraph != NULL){
				if(organisationGraph->hashtable.num_records > 0){
					currentEdge = setPersonWorkAtOrganisationProperties(endID, startID, "PersonWorksHere",workFrom,&errorFlag); /* Relation is "workAt", (no weight) */
					if(errorFlag == EXIT_VALUE){
						printf("personWorkAtOrganisationReader: setPersonWorkAtOrganisationProperties Failed - Exiting!\n");
						return EXIT_VALUE;				
					}
					if(addLastList(personWorkAtOrganisationList, currentEdge) == EXIT_VALUE){
						printf("personWorkAtOrganisationReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, organisationGraph) == EXIT_VALUE){
						printf("personWorkAtOrganisationReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPersonWorkAtOrganisationProperties(currentEdge) == EXIT_VALUE){
							printf("personWorkAtOrganisationReader: printPersonWorkAtOrganisationProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}
	fclose(in);
}

int personHasInterestTagReader(char* filename, List* personHasInterestTagList,Graph* personGraph,Graph* tagGraph){ 

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,startID=0,endID=0,id_c1,id_c2,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag=0;

	Edge* currentEdge = (Edge*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\npersonHasInterestTagReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\nEnable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npersonHasInterestTagReader: Populating with personHasInterest and interestsPerson edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;
	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));
		if(personGraph != NULL){
			if(personGraph->hashtable.num_records > 0){
				currentEdge = setPersonHasInterestTagProperties(startID, endID, "hasInterest",&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("personHasInterestTagReader: setPersonHasInterestTagProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(personHasInterestTagList, currentEdge) == EXIT_VALUE){
					printf("personHasInterestTagReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, personGraph) == EXIT_VALUE){
					printf("personHasInterestTagReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printPersonHasInterestTagProperties(currentEdge) == EXIT_VALUE){
						printf("personHasInterestTagReader: printPersonHasInterestTagProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}
		if(symmetric_addition == 1){
			if(tagGraph != NULL){
				if(tagGraph->hashtable.num_records > 0){
					currentEdge = setPersonHasInterestTagProperties(endID, startID, "interestsPerson",&errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("personHasInterestTagReader: setPersonHasInterestTagProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}	
					if(addLastList(personHasInterestTagList, currentEdge) == EXIT_VALUE){
						printf("personHasInterestTagReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, tagGraph) == EXIT_VALUE){
						printf("personHasInterestTagReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPersonHasInterestTagProperties(currentEdge) == EXIT_VALUE){
							printf("personHasInterestTagReader: printPersonHasInterestTagProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}
	fclose(in);
}
////////////////////////////////////////////////////////FORUM //////////////////////////////////////////////////////////////////////

int forumReader(char* filename, List* forumList, Graph* forumGraph, Graph* leaveNULLGraph){ /* Reads all Forums from file and creates a Graph of Forums */

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,title_c=0,creationDate_c=0,line_length=0;
	char id_array[10];
	char title[256];
	char creationDate[30];
	char c;
	int errorFlag=0;

	GraphNode* currentForum = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\nforumReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	
	printf("\nforumReader: Populating forumGraph with Forums from file! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,id = 0,id_c = 0,title_c = 0,creationDate_c = 0;
		do{
			c = str[j]; //Get character in line buffer
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1: //ID Field
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(title,c,&title_c,&i);
					break;
				case 3:
					addCharacterToStringField(creationDate,c,&creationDate_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentForum = setForumProperties(id, title, creationDate, &errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("forumReader: setForumProperties Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addLastList(forumList, currentForum) == EXIT_VALUE){
			printf("forumReader: addLastList Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentForum, forumGraph) == EXIT_VALUE){
			printf("forumReader: insertNode Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printForumProperties(currentForum) == EXIT_VALUE){
				printf("forumReader: printForumProperties Failed - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	} 
}

int forumHasMemberPersonReader(char* filename, List* forumHasMemberPersonList,Graph* forumGraph,Graph* personGraph){ /* Fill forumGraph with edges from Forum to Person and from Person to Forum(to know all forums a person belongs to) */

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,joinDate_c=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char joinDate[30];
	char c;
	int errorFlag=0;

	Edge* currentEdge = (Edge*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\nforumHasMemberPersonReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));
	
	int symmetric_addition=0;
	printf("\nforumHasMemberPersonReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\nforumHasMemberPersonReader: Populating with forumHasMember and isMemberOfForum edges! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0,joinDate_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
				case 3:
					addCharacterToStringField(joinDate,c,&joinDate_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		if(forumGraph != NULL){
			if(forumGraph->hashtable.num_records > 0){
				currentEdge = setForumHasMemberProperties(startID, endID, "HasMember", joinDate, &errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("forumHasMemberPersonReader: setForumHasMemberProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(forumHasMemberPersonList, currentEdge) == EXIT_VALUE){
					printf("forumHasMemberPersonReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, forumGraph) == EXIT_VALUE){
					printf("forumHasMemberPersonReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printForumHasMemberProperties(currentEdge) == EXIT_VALUE){
						printf("forumHasMemberPersonReader: printForumHasMemberProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}
		
		if(symmetric_addition == 1){
			if(personGraph != NULL){
				if(personGraph->hashtable.num_records > 0){
					currentEdge = setForumHasMemberProperties(endID, startID, "isMemberOfForum", joinDate, &errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("forumHasMemberPersonReader: setForumHasMemberProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(forumHasMemberPersonList, currentEdge) == EXIT_VALUE){
						printf("forumHasMemberPersonReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, personGraph) == EXIT_VALUE){
						printf("forumHasMemberPersonReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printForumHasMemberProperties(currentEdge) == EXIT_VALUE){
							printf("forumHasMemberPersonReader: printForumHasMemberProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}
	fclose(in);
}

int forumContainerPostReader(char* filename, List*  forumContainerPostList, Graph* forumGraph, Graph* postGraph){ /* Adds an Edge about a Forum Containing a Post in forumGraph and its possite in postGraph */

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag=0;
	
	Edge* currentEdge = (Edge*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\nforumContainerPostReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\nforumContainerPostReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\nforumContainerPostReader: Populating with containsPost and isContained! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		if(forumGraph != NULL){
			if(forumGraph->hashtable.num_records > 0){
				currentEdge = setForumContainerPostProperties(startID, endID, "containsPost",&errorFlag);
				if(errorFlag == EXIT_VALUE){
					printf("ForumContainerPostReader: setForumContainerPostProperties Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(forumContainerPostList, currentEdge) == EXIT_VALUE){
					printf("ForumContainerPostReader: addLastList Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, forumGraph) == EXIT_VALUE){
					printf("ForumContainerPostReader: insertEdge Failed - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printForumContainerPostProperties(currentEdge) == EXIT_VALUE){
						printf("ForumContainerPostReader: printForumContainerPostProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(postGraph != NULL){
				if(postGraph->hashtable.num_records > 0){
					currentEdge = setForumContainerPostProperties(endID, startID, "containedByForum",&errorFlag);
					if(errorFlag == EXIT_VALUE){
						printf("ForumContainerPostReader: setForumContainerPostProperties Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(forumContainerPostList, currentEdge) == EXIT_VALUE){
						printf("ForumContainerPostReader: addLastList Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, postGraph) == EXIT_VALUE){
						printf("ForumContainerPostReader: insertEdge Failed - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printForumContainerPostProperties(currentEdge) == EXIT_VALUE){
							printf("ForumContainerPostReader: printForumContainerPostProperties Failed - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}

	fclose(in);
 
}

////////////////////////////////////////PLACE /////////////////////////////////////////////////////////////////////
//TODO: ID AS LONG
int placeReader(char* filename, List* placeList, Graph* placeGraph, Graph* leaveNULLGraph){ /* Fill placeGraphs with GraphNodes that represent places*/

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,name_c=0,url_c=0,type_c=0,line_length=0;
	char id_array[10];
	char name[100];
	char url[256];
	char type[20];
	char c;
	int errorFlag=0;

	GraphNode* currentPlace = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\nplaceReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	
	printf("\nplaceReader: Populating placeGraph with Places from file! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,id = 0,id_c = 0,name_c = 0,url_c = 0,type_c = 0;
		do{
			c = str[j]; //Get character in line buffer
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1: //ID Field
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(name,c,&name_c,&i);
					break;
				case 3:
					addCharacterToStringField(url,c,&url_c,&i);
					break;
				case 4:
					addCharacterToStringField(type,c,&type_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentPlace = setPlaceProperties(id, name, url, type,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("placeReader: setPlaceProperties Failed - Exiting!\n");
			return EXIT_VALUE;
		}

		if(addLastList(placeList, currentPlace) == EXIT_VALUE){
			printf("placeReader: addLastList Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentPlace, placeGraph) == EXIT_VALUE){
			printf("placeReader: insertNode Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printPlaceProperties(currentPlace) == EXIT_VALUE){
				printf("placeReader: printPlaceProperties Failed - Exiting!\n");
				return EXIT_VALUE;
			}
		}

	} 

	fclose(in);
 
}

///////////////////////////////////COMMENT /////////////////////////////////////////////

int commentReader(char* filename, List* commentList, Graph* commentGraph, Graph* leaveNULLGraph){

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,creationDate_c=0,locationIP_c=0,browserUsed_c=0,content_c=0,line_length=0;
	char id_array[10];
	char creationDate[30];
	char locationIP[20];
	char browserUsed[20];
	char content[1024];
	char c;
	int errorFlag=0;
	
	GraphNode* currentComment = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\ncommentReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	
	printf("\ncommentReader: Populating commentGraph with Comments from file! Please go bake a cake until we finish!\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,id = 0,id_c = 0,creationDate_c = 0,locationIP_c = 0,browserUsed_c = 0,content_c=0;
		do{
			c = str[j]; //Get character in line buffer
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1: //ID Field
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(creationDate,c,&creationDate_c,&i);
					break;
				case 3:
					addCharacterToStringField(locationIP,c,&locationIP_c,&i);
					break;
				case 4:
					addCharacterToStringField(browserUsed,c,&browserUsed_c,&i);
					break;
				case 5:
					addCharacterToStringField(content,c,&content_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentComment = setCommentProperties(id, creationDate, locationIP, browserUsed, content,&errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("commentReader: setCommentProperties Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addLastList(commentList, currentComment) == EXIT_VALUE){
			printf("commentReader: addLastList Failed - Exiting!\n");
			return EXIT_VALUE;	
		}
		if(insertNode(currentComment, commentGraph) == EXIT_VALUE){
			printf("commentReader: insertNode Failed - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printCommentProperties(currentComment) == EXIT_VALUE){
				printf("commentReader: sprintCommentProperties Failed - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	} 

	fclose(in);
 
}

int commentHasCreatorPersonReader(char* filename, List* commentHasCreatorPersonList,Graph* commentGraph, Graph* personGraph){ /* Adds an Edge about creation of Comment by Person in commentGraph and its opposite in personGraph */

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag=0;

	Edge* currentEdge = (Edge*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\ncommentHasCreatorPersonReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\ncommentHasCreatorPersonReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\ncommentHasCreatorPersonReader: Populating with commentHasCreatorPerson and personCreatedComment edges! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));;

		if(commentGraph != NULL){
			if(commentGraph->hashtable.num_records > 0){
				currentEdge = setCommentHasCreatorPersonProperties(startID, endID, "HasCreatorPerson",&errorFlag); //Add Edge from Comment to Person
				if(errorFlag == EXIT_VALUE){
					printf("commentHasCreatorPersonReader: setCommentHasCreatorPersonProperties failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(commentHasCreatorPersonList, currentEdge) == EXIT_VALUE){
					printf("commentHasCreatorPersonReader: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, commentGraph) == EXIT_VALUE){
					printf("commentHasCreatorPersonReader: insertNode failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printCommentHasCreatorPersonProperties(currentEdge) == EXIT_VALUE){
						printf("commentHasCreatorPersonReader: printCommentHasCreatorPersonProperties failed! - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(personGraph != NULL){
				if(personGraph->hashtable.num_records > 0){
					currentEdge = setCommentHasCreatorPersonProperties(endID, startID, "createdComment",&errorFlag); //Add Edge from Person to Comment
					if(errorFlag == EXIT_VALUE){
						printf("commentHasCreatorPersonReader: setCommentHasCreatorPersonProperties failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(commentHasCreatorPersonList, currentEdge) == EXIT_VALUE){
						printf("commentHasCreatorPersonReader: addLastList failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, personGraph) == EXIT_VALUE){
						printf("commentHasCreatorPersonReader: insertNode failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printCommentHasCreatorPersonProperties(currentEdge) == EXIT_VALUE){
							printf("commentHasCreatorPersonReader: printCommentHasCreatorPersonProperties failed! - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}

	fclose(in);
 
}

int commentReplyOfPostReader(char* filename, List* commentReplyOfPostList,Graph* commentGraph,Graph* postGraph){ /* Fill commentGraph g with Edges that represent that a Comment is a reply to a Post and postGraph with Edges that represent a post contains a comment */

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,line_length;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag=0;
	
	Edge* currentEdge = (Edge*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\ncommentReplyOfPostReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	int symmetric_addition=0;
	printf("\ncommentReplyOfPostReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\ncommentReplyOfPostReader: Populating with commentReplyOfPost and PostHasReplyComment edges! Please wait...\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));;

		if(commentGraph != NULL){
			if(commentGraph->hashtable.num_records > 0){
				currentEdge = setCommentReplyOfPostProperties(startID, endID, "ReplyOfPost",&errorFlag); //Create Edge from commentGraph to Post Graph
				if(errorFlag == EXIT_VALUE){
					printf("commentReplyOfPostReader: setCommentReplyOfPostProperties failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(commentReplyOfPostList, currentEdge) == EXIT_VALUE){
					printf("commentReplyOfPostReader: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, commentGraph) == EXIT_VALUE){
					printf("commentReplyOfPostReader: insertNode failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(answer == 1){
					if(printCommentReplyOfPostProperties(currentEdge) == EXIT_VALUE){
						printf("commentReplyOfPostReader: printPostProperties failed! - Exiting!\n");
						return EXIT_VALUE;
					}
				}
			}
		}

		if(symmetric_addition == 1){
			if(postGraph != NULL){
				if(postGraph->hashtable.num_records > 0){
					currentEdge = setCommentReplyOfPostProperties(endID, startID, "PostHasComment",&errorFlag); //Create Edge from commentGraph to Post Graph
					if(errorFlag == EXIT_VALUE){
						printf("commentReplyOfPostReader: setCommentReplyOfPostProperties failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(commentReplyOfPostList, currentEdge) == EXIT_VALUE){
						printf("commentReplyOfPostReader: addLastList failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, postGraph) == EXIT_VALUE){
						printf("commentReplyOfPostReader: insertNode failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printCommentReplyOfPostProperties(currentEdge) == EXIT_VALUE){
							printf("commentReplyOfPostReader: printPostProperties failed! - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}

	fclose(in);

}

///////////////////////////////////////////////////POST ///////////////////////////////////////////////////
int postReader(char* filename, List* postList, Graph* postGraph, Graph* leaveNULLGraph){

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,imageFile_c=0,creationDate_c=0,locationIP_c=0,browserUsed_c=0,language_c=0,content_c=0,line_length=0;
	char id_array[10];
	char imageFile[100];
	char creationDate[30];
	char locationIP[20];
	char browserUsed[20];
	char language[10];
	char content[1024];
	char c;
	int errorFlag=0;
	GraphNode* currentPost = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("Failed to open: %s\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\npostReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	
	printf("\npostReader: Populating postGraph with Posts from file! Come back tomorrow!\n\n");

	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,id = 0,id_c = 0,imageFile_c=0,creationDate_c=0,locationIP_c=0,browserUsed_c=0,language_c=0,content_c=0,line_length=0;
		do{
			c = str[j]; //Get character in line buffer
			//printf("Current letter: |%c|\n",c);
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1: //ID Field
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(imageFile,c,&imageFile_c,&i);
					break;
				case 3:
					addCharacterToStringField(creationDate,c,&creationDate_c,&i);
					break;
				case 4:
					addCharacterToStringField(locationIP,c,&locationIP_c,&i);
					break;
				case 5:
					addCharacterToStringField(browserUsed,c,&browserUsed_c,&i);
					break;
				case 6:
					addCharacterToStringField(language,c,&language_c,&i);
					break;
				case 7:
					addCharacterToStringField(content,c,&content_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentPost = setPostProperties(id, imageFile, creationDate, locationIP, browserUsed, language, content, &errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("postReader: setPostProperties failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(addLastList(postList, currentPost) == EXIT_VALUE){
			printf("postReader: addLastList failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentPost, postGraph) == EXIT_VALUE){
			printf("postReader: insertNode failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printPostProperties(currentPost) == EXIT_VALUE){
				printf("postReader: printPostProperties failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}

	} 

	fclose(in);

}

int postHasCreatorPersonReader(char* filename, List* postHasCreatorPersonList,Graph* postGraph, Graph* personGraph){ /* Fill postGraph with Edges that represent that a Post has been created by a Person and personGraph with Edges tha represent what posts a Person has created - Keep both types of Edges in postHasCreatorPersonList for easier de-allocation later */
	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_edges = 0,startID=0,endID=0,id_c1=0,id_c2=0,line_length=0;
	char id_array1[10];
	char id_array2[10];
	char c;
	int errorFlag = 0;
	
	Edge* currentEdge = (Edge*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("\npostHasCreatorPersonReader: Failed to open: %s - Exiting!\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\npostHasCreatorPersonReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));

	int symmetric_addition=0;
	printf("\npostHasCreatorPersonReader: Enable symmetric edge addition(Placing Edge from GraphA to GraphB and from GraphB to GraphA)? Y:1 / N:0\n");
	do{
		scanf("%d",&symmetric_addition);
	}while((symmetric_addition != 0) && (symmetric_addition != 1));

	printf("\npostHasCreatorPersonReader: Populating postHasCreatorPerson and personCreatedPost edges! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){
		i = 1,j=0,startID=0,endID=0,id_c1=0,id_c2=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array1,c,&id_c1,&i,&startID);
					break;
				case 2:
					addCharacterToIntegerField(id_array2,c,&id_c2,&i,&endID);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		if(postGraph != NULL){
			if(postGraph->hashtable.num_records){
				currentEdge = setPostHasCreatorPersonProperties(startID, endID, "HasCreatorPerson",&errorFlag); //Create Edge from Post to Person
				if(errorFlag == EXIT_VALUE){
					printf("postHasCreatorPersonReader: setTagProperties failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(addLastList(postHasCreatorPersonList, currentEdge) == EXIT_VALUE){ //Add to the List
					printf("postHasCreatorPersonReader: addLastList failed! - Exiting!\n");
					return EXIT_VALUE;
				}
				if(insertEdge(startID, currentEdge, postGraph) == EXIT_VALUE){ //Insert in post graph
					printf("postHasCreatorPersonReader: insertEdge failed! - Exiting!\n");
					return EXIT_VALUE;	
				}
				if(answer == 1){
					if(printPostHasCreatorPersonProperties(currentEdge) == EXIT_VALUE){ //Print for Debugging
						printf("postHasCreatorPersonReader: printPostHasCreatorPersonProperties failed! - Exiting!\n");
						return EXIT_VALUE;	
					}
				}
			}
		}
		if(symmetric_addition == 1){
			if(personGraph != NULL){
				if(personGraph->hashtable.num_records){
					currentEdge = setPostHasCreatorPersonProperties(endID, startID, "createdPost",&errorFlag); //Create the opposite Edge in Person Graph
					if(errorFlag == EXIT_VALUE){
						printf("postHasCreatorPersonReader: setPostHasCreatorPersonProperties failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(addLastList(postHasCreatorPersonList, currentEdge) == EXIT_VALUE){ //Add to the List
						printf("postHasCreatorPersonReader: addLastList failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(insertEdge(endID, currentEdge, personGraph) == EXIT_VALUE){ //Insert in post graph
						printf("postHasCreatorPersonReader: insertEdge failed! - Exiting!\n");
						return EXIT_VALUE;
					}
					if(answer == 1){
						if(printPostHasCreatorPersonProperties(currentEdge) == EXIT_VALUE){ //Print for Debugging
							printf("postHasCreatorPersonReader: printPostHasCreatorPersonProperties failed! - Exiting!\n");
							return EXIT_VALUE;
						}
					}
				}
			}
		}

	}

	fclose(in);

}

////////////////////////TAG ////////////////////////////////
int tagReader(char* filename, List* tagList, Graph* tagGraph, Graph* leaveNULLGraph){

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,name_c=0,url_c=0,line_length=0;
	char id_array[10];
	char name[50];
	char url[256];
	char c;
	int errorFlag = 0;
	
	GraphNode* currentTag = (GraphNode*) NULL;
	in = fopen(filename, "r");
	if(in == NULL){
		printf("\ntagReader: Failed to open: %s - Exiting!\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\ntagReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	printf("\ntagReader: Populating tagGraph with Interest-Tags from file! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line		
		i = 1,j=0,id=0,id_c=0,name_c=0,url_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(name,c,&name_c,&i);
					break;
				case 3:
					addCharacterToStringField(url,c,&url_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));

		currentTag = setTagProperties(id, name, url, &errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("tagReader: setTagProperties failed! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(addLastList(tagList, currentTag) == EXIT_VALUE){
			printf("tagReader: addLastList failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentTag, tagGraph) == EXIT_VALUE){
			printf("tagReader: insertNode failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printTagProperties(currentTag) == EXIT_VALUE){
				printf("tagReader: printTagProperties failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}

	}
	fclose(in);
}

///////////////////////ORGANISATION ///////////////////////////
int organisationReader(char* filename, List* organisationList, Graph* organisationGraph, Graph* leaveNULLGraph){

	FILE *in= (FILE*) NULL;
	char str[MAX_CHARS], *tok= (char*) NULL;
	int i=1,j=0,num_of_person = 0,id=0,id_c=0,type_c=0,name_c=0,url_c=0,line_length=0;
	char id_array[10];
	char type[20];
	char name[50];
	char url[256];
	char c;
	int errorFlag=0;

	GraphNode* currentOrganisation = (GraphNode*) NULL;

	in = fopen(filename, "r");
	if(in == NULL){
		printf("\norganisationReader: Failed to open: %s - Exiting!\n", filename);
		return EXIT_VALUE;
	}

	int answer=0;
	printf("\norganisationReader: Printing every record we add from this file might take a while, are you sure you want to print? Y:1 / N:0\n");
	do{
		scanf("%d",&answer);
	}while((answer != 0) && (answer != 1));	

	printf("\norganisationReader: Populating organisationGraph with Organisation-Tags from file! Please wait...\n\n");
	if( fgets(str, MAX_CHARS, in) == NULL) return -1;

	while(fgets (str, MAX_CHARS, in) != NULL){ //Get line
		i = 1,j=0,id=0,id_c=0,type_c=0,name_c=0,url_c=0;
		do{
			c = str[j];
			if(!((c >= 32) && (c < 127)) && (c != '\0')){
				//printf("personReader: Special character detected! Error!\n");
				j++;
				continue;
			}
			switch(i){
				case 1:
					addCharacterToIntegerField(id_array,c,&id_c,&i,&id);
					break;
				case 2:
					addCharacterToStringField(type,c,&type_c,&i);
					break;
				case 3:
					addCharacterToStringField(name,c,&name_c,&i);
					break;
				case 4:
					addCharacterToStringField(url,c,&url_c,&i);
					break;
			}
			j++;
		}while((c != '\n') && (c != '\0'));;

		currentOrganisation = setOrganisationProperties(id, type, name, url, &errorFlag);
		if(errorFlag == EXIT_VALUE){
			printf("organisationReader: setOrganisationProperties failed! - Exiting!\n");
			return EXIT_VALUE;
		}

		if(addLastList(organisationList, currentOrganisation) == EXIT_VALUE){
			printf("organisationReader: addLastList failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(insertNode(currentOrganisation, organisationGraph) == EXIT_VALUE){
			printf("organisationReader: insertNode failed! - Exiting!\n");
			return EXIT_VALUE;
		}
		if(answer == 1){
			if(printOrganisationProperties(currentOrganisation) == EXIT_VALUE){
				printf("organisationReader: printOrganisationProperties failed! - Exiting!\n");
				return EXIT_VALUE;
			}
		}
	}

	fclose(in);
}
