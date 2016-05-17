typedef struct Node* NodePtr;
typedef struct Node{
	void* data;
	NodePtr next;
	NodePtr previous;
} Node;

typedef struct List* ListPtr;
typedef struct List{
	int items;
	NodePtr Head;
	NodePtr Current;
	NodePtr Last;
	char* type_name;
	int (*allocateNode) (void**, void*);
	int (*destroyNode) (void**);
	int (*compareNode) (void*, void*);
	int (*assignNode) (void*, void*);
	int (*printNode) (void*);
} List;

void createList(ListPtr,char* type_name, int (*allocateNode) (void**, void*), int (*destroyNode) (void**), int (*compareNode) (void*, void*), int (*assignNode) (void*,void*), int (*printNode) (void*) );
int isEmptyList(List);
void* frontList(List);
int addLastList(ListPtr,void*);
int addFirstList(ListPtr,void*);
int addMiddleList(ListPtr,NodePtr,void*);
int searchList(List,void*,void**);
void printList(List);
void popList(ListPtr);
void destroyList(ListPtr);
void removeLastList(ListPtr);
void removeCurrentNode(ListPtr);
int appendLists(ListPtr listp1,ListPtr listp2);
int listsAreDuplicated(List listA,List listB,int compareMode);
