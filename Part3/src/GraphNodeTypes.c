#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtools.h"
#include "GraphNodeTypes.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: GraphNodeTYpes.c
 MAIN AUTHOR: All of us														

 Details: Here we have gathered all the different kinds of GraphNodes that will be created
 based on the DataSet during execution.

================================================================================================*/

//////////////////////////////////PERSON GRAPHNODE ///////////////////////////////////

GraphNode* setPersonProperties(int id, char* name, char* surname, char* gender, char* birthday, char* creationDate,char* IP,char* browser,int* errorFlag) {

    Properties* prop = createProperties(7,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(name, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(surname, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(gender, 2, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(birthday, 3, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(creationDate, 4, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(IP, 5, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(browser, 6, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setPersonProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printPersonProperties(GraphNode* n) {
    
	int* tempInt;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Person - ID: |%d| - Name: |%s| - Surname: |%s| - Gender: |%s| - Birthday: |%s| - Creation: |%s| - IP: |%s| - Browser: |%s|\n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute, (char*) n->properties->arrayP[2].attribute,(char*) n->properties->arrayP[3].attribute,(char*) n->properties->arrayP[4].attribute,(char*) n->properties->arrayP[5].attribute,(char*) n->properties->arrayP[6].attribute);
			}
			else{
				printf("printPersonProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

///////////////////////////////FORUM GRAPHNODE /////////////////////////////////////

GraphNode* setForumProperties(int id, char* title, char* creationDate,int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setForumProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(title, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setForumProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(creationDate, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setForumProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setForumProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setForumProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printForumProperties(GraphNode* n) {
    
	int* tempInt;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Forum - ID: |%d| - Title: |%s| - CreationDate: |%s| \n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute);
			}
			else{
				printf("printForumProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printForumProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printForumProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

//////////////////////////PLACE GRAPHNODE///////////////////

GraphNode* setPlaceProperties(int id,char* name,char* url,char* type,int* errorFlag){

    Properties* prop = createProperties(3,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPlaceProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(name, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPlaceProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(url, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPlaceProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(type, 2, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPlaceProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPlaceProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setPlaceProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printPlaceProperties(GraphNode* n){

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Place - ID: |%d| - Name: |%s| - URL: |%s| - Type: |%s| \n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute, (char*) n->properties->arrayP[2].attribute);
			}
			else{
				printf("printPlaceProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPlaceProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPlaceProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

///////////////////////COMMENT GRAPHNODE//////////////////
GraphNode* setCommentProperties(int id,char* creationDate,char* locationIP,char* browserUsed,char* content,int* errorFlag){

    Properties* prop = createProperties(4,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setCommentProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(creationDate, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(locationIP, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(browserUsed, 2, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }


    if(setStringProperty(content, 3, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setCommentProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setCommentProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printCommentProperties(GraphNode* n) {
    
	int* tempInt;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Comment - ID: |%d| - creationDate: |%s| - locationIP: |%s| - browserUsed: |%s|\n content: |%s| \n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute,(char*) n->properties->arrayP[2].attribute,(char*) n->properties->arrayP[3].attribute);
			}
			else{
				printf("printCommentProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printCommentProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printCommentProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

///////////////////////ORGANISATION GRAPHNODE//////////////////
GraphNode* setOrganisationProperties(int id,char* type,char* name,char* url,int* errorFlag){

    Properties* prop = createProperties(3,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setOrganisationProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(name, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(url, 2, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setOrganisationProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setOrganisationProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printOrganisationProperties(GraphNode* n) {
    
	int* tempInt;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Organisation - ID: |%d| - Type: |%s| - Name: |%s| - URL: |%s|\n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute,(char*) n->properties->arrayP[2].attribute);
			}
			else{
				printf("printOrganisationProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printOrganisationProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printOrganisationProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

////////////////////////////////////////POST GRAPHNODE//////////////////////////////////////////
GraphNode* setPostProperties(int id,char* imageFile,char* creationDate,char* locationIP,char* browserUsed,char* language,char* content,int* errorFlag){

    Properties* prop = createProperties(6,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPostProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(imageFile, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(creationDate, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(locationIP, 2, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    if(setStringProperty(browserUsed, 3, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(language, 4, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(content, 5, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPostProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setPostProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printPostProperties(GraphNode* n){

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Post - ID: |%d| - imageFile: |%s| - creationDate: |%s| - locationIP: |%s| - browserUsed: |%s| - language: |%s|\nContent: |%s|\n\n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute,(char*) n->properties->arrayP[2].attribute,(char*) n->properties->arrayP[3].attribute,(char*) n->properties->arrayP[4].attribute,(char*) n->properties->arrayP[5].attribute);
			}
			else{
				printf("printPostProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPostProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPostProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

////////////////////////////////////////TAG GRAPHNODE//////////////////////////////////////////
GraphNode* setTagProperties(int id,char* name,char* url,int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setTagProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}

	if(setStringProperty(name, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setTagProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    if(setStringProperty(url, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setTagProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
    }

    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setTagProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setTagProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printTagProperties(GraphNode* n){

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Tag - ID: |%d| - Name: |%s| - URL: |%s|\n", n->id, (char*) n->properties->arrayP[0].attribute, (char*) n->properties->arrayP[1].attribute);
			}
			else{
				printf("printTagProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printTagProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printTagProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

}

////////////////////////////////////////CLIQUE GRAPHNODE//////////////////////////////////////////
GraphNode* setCliqueProperties(int id,List* members,int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set (1 property)*/

	if(prop == NULL){
		printf("setCliqueProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((GraphNode*) (NULL));
	}
	
	if(setListProperty(members, 0, prop) == EXIT_VALUE){ /* Allocated memory for ListPtr property */
		printf("setCliqueProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}


    /*create node*/
    GraphNode* n = createGraphNode(id, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setCliqueProperties: Error creating GraphNode!\n");
		if(n != NULL){
			destroyGraphNode(&n);
		}
		else{
			destroyProperties(&prop);
		}
		return ((GraphNode*) (NULL));
	}

	if(n == NULL){
		printf("setCliqueProperties: Error Creating GraphNode!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((GraphNode*) (NULL));
	}

    return n;

}

int printCliqueProperties(GraphNode* n){

	int i=0;

	if(n != NULL){
		if(n->properties != NULL){
			if(n->properties->arrayP != NULL){
					printf("Clique - ID: |%d| - members:",n->id);
					printList(*(List*)n->properties->arrayP[0].attribute);
					printf("\n");
			}
			else{
				printf("printCliqueProperties: Array of properties is NULL!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printCliqueProperties: Properties set is NULL!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printCliqueProperties: GraphNode Pointer is NULL!\n");
		return EXIT_VALUE;
	}

	return 0;
}
