#include "GraphLib.h"

//////////////////////////////////////////PERSON GRAPHNODE/////////////////////////////////////////
GraphNode* setPersonProperties(int id, char* name, char* surname, char* gender, char* birthday, char* creationDate,char* IP,char* browser,int* errorFlag);
int printPersonProperties(GraphNode* n);

//////////////////////////////////////////FORUM GRAPHNODE//////////////////////////////////////////
GraphNode* setForumProperties(int id, char* title, char* creationDate,int* errorFlag);
int printForumProperties(GraphNode* n);

/////////////////////////////////////////PLACE GRAPHNODE////////////////////////////////////////////
GraphNode* setPlaceProperties(int id,char* name,char* url,char* type,int* errorFlag);
int printPlaceProperties(GraphNode* n);

////////////////////////////////////////COMMENT GRAPHNODE//////////////////////////////////////////
GraphNode* setCommentProperties(int id,char* creationDate,char* locationIP,char* browserUsed,char* content,int* errorFlag);
int printCommentProperties(GraphNode* n);

////////////////////////////////////////ORGANISATION GRAPHNODE//////////////////////////////////////////
GraphNode* setOrganisationProperties(int id,char* type,char* name,char* url,int* errorFlag);
int printOrganisationProperties(GraphNode* n) ;

////////////////////////////////////////POST GRAPHNODE//////////////////////////////////////////
GraphNode* setPostProperties(int id,char* imageFile,char* creationDate,char* locationIP,char* browserUsed,char* language,char* content,int* errorFlag);
int printPostProperties(GraphNode* n);

////////////////////////////////////////TAG GRAPHNODE//////////////////////////////////////////
GraphNode* setTagProperties(int id,char* name,char* url,int* errorFlag);
int printTagProperties(GraphNode* n);
