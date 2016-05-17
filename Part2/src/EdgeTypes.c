#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EdgeTypes.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: EdgeTypes.c
 MAIN AUTHOR: All of us														

 Details: Here we have gathered all the different kinds of Edges that will be created
 based on the DataSet during execution.

================================================================================================*/

//////////////////////////////PERSON EDGES///////////////////////////////////////////////

/////PERSON KNOWS PERSON/////

Edge* setPersonKnowsPersonProperties(int startNodeID, int endNodeID, char* type, int weight, int* errorFlag) { /* Initialise a Person Knows Person Edge */

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonKnowsPersonProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonKnowsPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setIntegerProperty(weight, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonKnowsPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonKnowsPersonProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPersonKnowsPersonProperties:: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonKnowsPersonProperties(Edge* e) {

	int* tempInt;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				tempInt = (int*) e->properties->arrayP[1].attribute;
					printf("PersonKnowsPerson - startID: |%d| - endID: |%d| - Type: |%s| - Weight: |%d|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute, *tempInt);
			}
			else{
				printf("printPersonKnowsPersonProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonKnowsPersonProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonKnowsPersonProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

/////PERSON HAS INTEREST TAG ////

Edge* setPersonHasInterestTagProperties(int startNodeID,int endNodeID, char* type, int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonHasInterestTagProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonHasInterestTagProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonHasInterestTagProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPersonHasInterestTagProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonHasInterestTagProperties(Edge* e) {

	int* tempInt;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PersonHasInterest - startID: |%d| - endID: |%d| - Type: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printPersonHasInterestTagProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonHasInterestTagProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonHasInterestTagProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

////PERSON LIKES POST//////

Edge* setPersonLikesPostProperties(int startNodeID,int endNodeID, char* type, char* creationDate, int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonLikesPostProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonLikesPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setStringProperty(creationDate, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonLikesPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonLikesPostProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPersonLikesPostProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonLikesPostProperties(Edge* e) {

	int* tempInt;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PersonLikesPost - startID: |%d| - endID: |%d| - Type: |%s| - CreationDate: |%s| \n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute, (char*) e->properties->arrayP[1].attribute);
			}
			else{
				printf("printPersonLikesPostProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonLikesPostProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonLikesPostProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

/////PERSON IS LOCATED IN PLACE //////

Edge* setPersonIsLocatedInPlaceProperties(int startNodeID,int endNodeID,char* type, int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonIsLocatedInPlaceProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonIsLocatedInPlaceProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonIsLocatedInPlaceProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPersonIsLocatedInPlaceProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonIsLocatedInPlaceProperties(Edge* e) {

	int* tempInt;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PersonIsLocatedInPlace - startID: |%d| - endID: |%d| - Type: |%s| \n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printPersonIsLocatedInPlaceProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonIsLocatedInPlaceProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonIsLocatedInPlaceProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

///////PERSON STUDY AT ORGANISATION ////////
Edge* setPersonStudyAtOrganisationProperties(int startNodeID, int endNodeID, char* type, char* classYear, int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonStudyAtOrganisationProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonStudyAtOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setStringProperty(classYear, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPersonStudyAtOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPersonStudyAtOrganisationProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPersonStudyAtOrganisationProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonStudyAtOrganisationProperties(Edge* e){

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PersonStudiesAtOrganisation - startID: |%d| - endID: |%d| - Type: |%s| - classYear: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute,(char*) e->properties->arrayP[1].attribute);
			}
			else{
				printf("printPersonStudyAtOrganisationProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonStudyAtOrganisationProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonStudyAtOrganisationProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

//////PERSON WORK AT ORGANISATION ////
Edge* setPersonWorkAtOrganisationProperties(int startNodeID, int endNodeID, char* type, char* workFrom, int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPersonWorkAtOrganisationProperties: createProperties failed! - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("printPersonWorkAtOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setStringProperty(workFrom, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("printPersonWorkAtOrganisationProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("printPersonWorkAtOrganisationProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("printPersonWorkAtOrganisationProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPersonWorkAtOrganisationProperties(Edge* e){

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PersonWorksAtOrganisation - startID: |%d| - endID: |%d| - Type: |%s| - workFrom: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute,(char*)e->properties->arrayP[1].attribute);
			}
			else{
				printf("printPersonWorkAtOrganisationProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPersonWorkAtOrganisationProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPersonWorkAtOrganisationProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

////////////////TRUST EDGES ////////////

Edge* setTrustEdgeProperties(int startNodeID,int endNodeID,char* type,double trustWeight, int* errorFlag){

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setTrustEdgeProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setTrustEdgeProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setDoubleProperty(trustWeight, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setTrustEdgeProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setTrustEdgeProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setTrustEdgeProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printTrustEdgeProperties(Edge* e) {

	double* trustWeight;

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				trustWeight = (double*) e->properties->arrayP[1].attribute;
				printf("trustEdge - startID: |%d| - endID: |%d| - Type: |%s| - trustWeight: |%lf| \n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute,*trustWeight);
			}
			else{
				printf("printTrustEdgeProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printTrustEdgeProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printTrustEdgeProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

/////////////////////////////////FORUM EDGES ////////////////////////////

Edge* setForumContainerPostProperties(int startNodeID, int endNodeID, char* type, int* errorFlag){ /* Used to create edges from Forum to Post */

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setForumContainerPostProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setForumContainerPostProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setForumContainerPostProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setForumContainerPostProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printForumContainerPostProperties(Edge* e) {

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
					printf("ForumContainsPost - startID: |%d| - endID: |%d| - Type: |%s| \n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printForumContainerPostProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printForumContainerPostProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printForumContainerPostProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;

}

Edge* setForumHasMemberProperties(int startNodeID, int endNodeID, char* type, char* joinDate, int* errorFlag) { /* Initialise a Person Knows Person Edge */

    Properties* prop = createProperties(2,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setForumHasMemberProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setForumHasMemberProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

	if(setStringProperty(joinDate, 1, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setForumHasMemberProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setForumHasMemberProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setForumHasMemberProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printForumHasMemberProperties(Edge* e) {

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
					printf("ForumHasMember - startID: |%d| - endID: |%d| - Type: |%s| - joinDate: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute, (char*) e->properties->arrayP[1].attribute);
			}
			else{
				printf("printForumHasMemberProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printForumHasMemberProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printForumHasMemberProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

///////////////////////////////////COMMENT EDGES/////////////////////////////////

Edge* setCommentHasCreatorPersonProperties(int startNodeID, int endNodeID, char* type, int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setCommentHasCreatorPersonProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentHasCreatorPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setCommentHasCreatorPersonProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setCommentHasCreatorPersonProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printCommentHasCreatorPersonProperties(Edge* e){

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
					printf("CommentHasCreator - startID: |%d| - endID: |%d| - Type: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printCommentHasCreatorPersonProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printCommentHasCreatorPersonProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printCommentHasCreatorPersonProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

Edge* setCommentReplyOfPostProperties(int startNodeID, int endNodeID, char* type, int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setCommentReplyOfPostPropertiess: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setCommentReplyOfPostPropertiess: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setCommentReplyOfPostPropertiess: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setCommentReplyOfPostPropertiess: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printCommentReplyOfPostProperties(Edge* e){

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
					printf("CommentReplyOfPost - startID: |%d| - endID: |%d| - Type: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printCommentReplyOfPostProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printCommentReplyOfPostProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printCommentReplyOfPostProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

////////////////////////////////POST EDGES///////////////////////////////////////
Edge* setPostHasCreatorPersonProperties(int startNodeID,int endNodeID, char* type, int* errorFlag){

    Properties* prop = createProperties(1,errorFlag); /* Allocated memory for property set */

	if(prop == NULL){
		printf("setPostHasCreatorPersonProperties: Error creating Properties!\n");
		*errorFlag = EXIT_VALUE;
		return ((Edge*) (NULL));
	}

	if(setStringProperty(type, 0, prop) == EXIT_VALUE){ /* Allocated memory for string property */
		printf("setPostHasCreatorPersonProperties: Error creating Property!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    /*create an edge*/
    Edge* e = createEdge(startNodeID, endNodeID, prop, errorFlag);
	
	if(*errorFlag == EXIT_VALUE){
		printf("setPostHasCreatorPersonProperties: createEdge failed! - Exiting!\n");
		if(e != NULL){
			destroyEdge(&e);
		}
		else{
			destroyProperties(&prop);
		}
		return ((Edge*) (NULL));
	}

	if(e == NULL){
		printf("setPostHasCreatorPersonProperties: Error Creating Edge - Exiting!\n");
		*errorFlag = EXIT_VALUE;
		destroyProperties(&prop);
		return ((Edge*) (NULL));
	}

    return e;

}

int printPostHasCreatorPersonProperties(Edge* e){

	if(e != NULL){
		if(e->properties != NULL){
			if(e->properties->arrayP != NULL){
				printf("PostHasCreatorPerson - startID: |%d| - endID: |%d| - Type: |%s|\n", e->startNodeID, e->endNodeID, (char*) e->properties->arrayP[0].attribute);
			}
			else{
				printf("printPostHasCreatorPersonProperties: Array of properties is NULL! - Exiting!\n");
				return EXIT_VALUE;
			}			
		}
		else{
			printf("printPostHasCreatorPersonProperties: Properties set is NULL! - Exiting!\n");
			return EXIT_VALUE;
		}
	}
	else{
		printf("printPostHasCreatorPersonProperties: Edge Pointer is NULL! - Exiting!\n");
		return EXIT_VALUE;
	}

	return 0;
}

