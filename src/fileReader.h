/* fileReader.h */
#define MAX_CHARS 1024
#define DATASET_DEBUG 1

typedef struct AllDataSetStructs{
	//Graphs and their Lists
	Graph* personGraph, *postGraph, *forumGraph, *tagGraph, *commentGraph, *organisationGraph, *placeGraph;
	List *personList, *postList, *forumList, *tagList, *commentList, *organisationList, *placeList;

    //Edges Lists
	List *personKnowsPersonList, *personIsLocatedInPlaceList, *personLikesPostList, *personStudyAtList, *personWorksAtList,*personHasInterestTagList; /* Person */
    List *forumHasMemberPersonList, *forumContainerPostList; /* Forum */ 
    List *commentHasCreatorPersonList, *commentReplyOfPostList; /* Comment */
	List *postHasCreatorPersonList; /* Post */

	pthread_mutex_t dataSetMutexes[25]; //Mutexes for Graphs and Lists of the dataSet to prevent problems
   
} AllDataSetStructs;

/////////////////////////////// FILE READER FUNCTIONS /////////////////////////////////
int setNULLDataSetStructs(AllDataSetStructs* allDataSetStructs);
int emptyAndDestroyList(List* list,char* nameList,int);
int destroyDataSetStructs(AllDataSetStructs* allDataSetStructs);
int createGraphsAndLists(Graph** graphPtrA,Graph** graphPtrB,List** listPtr,int type,int mode,int m ,int c,char* dataSetFolder,char* nameOfItem,char* filename,int (*readerFunction) (char*, List*, Graph*, Graph*));
int loadDataSet(char* dataSetFolder,int mode,AllDataSetStructs* allStructs,int m,int c);
int loadGraphItems(char* item_type,Graph* graph,Graph* secondGraph,List* itemList,int mode,char* dataSetFolder,char* nameOfItem,char* filename,int (*readerFunction) (char*, List*, Graph*, Graph*));
int addCharacterToStringField(char fieldBuffer[],char c,int* field_c,int* i);
int addCharacterToIntegerField(char fieldBuffer[],char c,int* field_c,int* i,int* resultPtr);
/////PERSON ////
int personReader(char* filename, List* personList, Graph* g, Graph* leaveNULLGraph);
int personKnowsPersonReader(char* filename, List* personKnowsPersonList,Graph* g, Graph* leaveNULLGraph);
int personIsLocatedInPlaceReader(char* filename, List* personIsLocatedInPlaceList,Graph* personGraph,Graph* placeGraph);
int personLikesPostReader(char* filename, List* personLikesPostList, Graph* personGraph, Graph* postGraph);
int personStudyAtOrganisationReader(char* filename, List* personStudyAtOrganisationList,Graph* personGraph, Graph* organisationGraph);
int personWorkAtOrganisationReader(char* filename, List* personWorkAtOrganisationList,Graph* personGraph,Graph* organisationGraph);
int personHasInterestTagReader(char* filename, List* personHasInterestTagList,Graph* personGraph,Graph* tagGraph);

////FORUM ////
int forumReader(char* filename, List* forumList, Graph* forumGraph, Graph* leaveNULLGraph);
int forumHasMemberPersonReader(char* filename, List* forumHasMemberPersonList,Graph* forumGraph,Graph* personGraph);
int forumContainerPostReader(char* filename, List*  forumContainerPostList, Graph* forumGraph, Graph* postGraph);

///PLACE ////
int placeReader(char* filename, List* placeList, Graph* placeGraph, Graph* leaveNULLGraph);

///COMMENT ///
int commentReader(char* filename, List* postList, Graph* commentGraph, Graph* leaveNULLGraph);
int commentHasCreatorPersonReader(char* filename, List* commentHasCreatorPersonList,Graph* commentGraph, Graph* personGraph);
int commentReplyOfPostReader(char* filename, List* commentReplyOfPostList,Graph* commentGraph,Graph* postGraph);

///POST ///
int postReader(char* filename, List* postList, Graph* postGraph, Graph* leaveNULLGraph);
int postHasCreatorPersonReader(char* filename, List* postHasCreatorPersonList,Graph* postGraph, Graph* personGraph);

///TAG ///
int tagReader(char* filename, List* tagList, Graph* tagGraph, Graph* leaveNULLGraph);

///ORGANISATION ///
int organisationReader(char* filename, List* organisationList, Graph* organisationGraph, Graph* leaveNULLGraph);

