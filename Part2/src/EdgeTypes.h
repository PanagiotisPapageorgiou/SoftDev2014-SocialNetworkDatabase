#include "GraphLib.h"

/////////////////////////////PERSON EDGES//////////////////////////
Edge* setPersonHasInterestTagProperties(int startNodeID,int endNodeID, char* type, int* errorFlag);
int printPersonHasInterestTagProperties(Edge* e);
Edge* setPersonLikesPostProperties(int startNodeID, int endNodeID, char* type,char* creationDate, int* errorFlag);
int printPersonLikesPostProperties(Edge* e);
Edge* setPersonKnowsPersonProperties(int startNodeID, int endNodeID, char* type, int weight, int* errorFlag);
int printPersonKnowsPersonProperties(Edge* n);
Edge* setPersonIsLocatedInPlaceProperties(int startID,int endID,char* type, int* errorFlag);
int printPersonIsLocatedInPlaceProperties(Edge* n);
Edge* setPersonStudyAtOrganisationProperties(int startNodeID, int endNodeID, char* type, char* classYear, int* errorFlag);
int printPersonStudyAtOrganisationProperties(Edge* e);
Edge* setPersonWorkAtOrganisationProperties(int startNodeID, int endNodeID, char* type, char* workFrom, int* errorFlag);
int printPersonWorkAtOrganisationProperties(Edge* e);
Edge* setTrustEdgeProperties(int startNodeID,int endNodeID,char* type,double trustWeight, int* errorFlag);
int printTrustEdgeProperties(Edge* e);

/////////////////////////////FORUM EDGES///////////////////////////
Edge* setForumContainerPostProperties(int startNodeID, int endNodeID, char* type, int* errorFlag);
int printForumContainerPostProperties(Edge* e);
Edge* setForumHasMemberProperties(int startNodeID, int endNodeID, char* type, char* joinDate, int* errorFlag);
int printForumHasMemberProperties(Edge* e);

////////////////////////////COMMENT EDGES///////////////////////////
Edge* setCommentHasCreatorPersonProperties(int startNodeID, int endNodeID, char* type, int* errorFlag);
int printCommentHasCreatorPersonProperties(Edge* e);
Edge* setCommentReplyOfPostProperties(int startNodeID, int endNodeID, char* type, int* errorFlag);
int printCommentReplyOfPostProperties(Edge* e);

////////////////////////////POST EDGES///////////////////////////////
Edge* setPostHasCreatorPersonProperties(int startNodeID,int endNodeID, char* type, int* errorFlag);
int printPostHasCreatorPersonProperties(Edge* e);
