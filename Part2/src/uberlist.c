#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"

/*============================NOTES ABOUT THIS FILE ============================================

 TITLE: uberlist.c
 MAIN AUTHOR: PANAGIOTIS PAPAGEORGIOU														

 Details: In order to avoid implementing many linked Lists, 1 for every Datatype we will need
 We chose to create a Double-Linked List with void* data in each Node. This List is provided 
 upon its creation with proper functions for allocation,destruction etc of a Node. And uses them 
 when needed to properly handle the void* data pointer

================================================================================================*/

/* Generic List functions*/

void createList(ListPtr listp, char* type_name, int (*allocateNode) (void**, void*), int (*destroyNode) (void**), int (*compareNode) (void*, void*), int (*assignNode) (void*, void*), int (*printNode) (void*)){ /* Initialise Generic List using the proper functions */

	listp->items = 0;
	listp->Head = (Node*) NULL;
	listp->Current = (Node*) NULL;
	listp->Last = (Node*) NULL;

	listp->type_name = malloc(sizeof(char) * (strlen(type_name) + 1));
	strcpy(listp->type_name,type_name);

	listp->allocateNode = allocateNode; /* Will be used to allocate the Data of the Node */
	listp->destroyNode = destroyNode;	/* To destroy the data of the Node */
	listp->compareNode = compareNode;	/* etc */
	listp->assignNode = assignNode;
	listp->printNode = printNode;

	//printf("A |%s| List has been created!\n",listp->type_name);
}

int isEmptyList(List list){ /* Check if List is empty */

	if(list.items == 0) return 1;
	else return 0;
}

void* frontList(List list){ /* Returns a pointer to the Data of the first Node */

	if(list.items > 0)
		return list.Head->data;
	else
		return NULL;
}

int addLastList(ListPtr listp,void* data){ /* Adds a Node to the end of the List */

	int i=0;

	if(listp->items == 0){ /* No items yet */

		listp->Head = malloc(sizeof(Node));
		listp->allocateNode(&(listp->Head->data),data);

		listp->Head->next = (Node*) NULL;
		listp->Head->previous = (Node*) NULL;

		listp->Current = listp->Head;
		listp->Last = listp->Head;

		(listp->items)++;

		return 0;
	}
	else if(listp->items > 0){

		listp->Last->next = malloc(sizeof(Node));

		listp->Last->next->previous = listp->Last;
		listp->Last = listp->Last->next;
		listp->Last->next = (Node*) NULL;
	
		listp->allocateNode(&(listp->Last->data),data);

		(listp->items)++;

		listp->Current = listp->Last;


		return 0;
	}
	else{
		printf("List is not properly initialised...\n");
		return -1;
	}

}

int addMiddleList(ListPtr listp,NodePtr position,void* data){ /* Add new Node in the middle of the Queue */

	int i=0;

	//there is always a previous and a next Node
	if(listp->items > 0){
		if(position == NULL) {
			printf("Position not valid...\n");
			return -1;
		}
		else {
			listp->Current = position->previous; //Current is the previous Node 
		}

		listp->Current->next = malloc(sizeof(Node));
		listp->Current->next->previous = listp->Current;
		listp->Current->next->next = position;
		position->previous = listp->Current->next;
		
		listp->allocateNode(&(listp->Current->next->data),data);

		listp->Current = listp->Head;

		(listp->items)++;

	}
	else{
		printf("List is not properly initialised...\n");
		return -1;
	}
}

int addFirstList(ListPtr listp,void* data){ /* Add new Node as Head of the Queue */

	int i=0;

	if(listp->items == 0){
		listp->Head = malloc(sizeof(Node));
		listp->allocateNode(&(listp->Head->data),data);

		listp->Head->next = (Node*) NULL;
		listp->Head->previous = (Node*) NULL;
		listp->Current = listp->Head;
		listp->Last = listp->Head;

		(listp->items)++;
		return 0;
	}
	else if(listp->items > 0){

		listp->Head->previous = malloc(sizeof(Node));
		listp->Head->previous->next = listp->Head;
		listp->Head = listp->Head->previous;
		
		listp->allocateNode(&(listp->Head->data),data);

		listp->Head->previous = (Node*) NULL;

		listp->Current = listp->Head;

		(listp->items)++;

	}
	else{
		printf("List is not properly initialised...\n");
		return -1;
	}
}

int searchList(List list,void* item_ptr,void** location){ /* Search the list for a specific item */
	
	if(list.items > 0){
		list.Current = list.Head;
		while(list.Current != NULL){ /* Use the proper comparison function for this List */
			if(list.compareNode(list.Current->data,item_ptr) == 0){
				*location = list.Current->data;
				return 1;
			}
			list.Current = list.Current->next;
		}
	}
	*location = NULL;

	return 0;

}

void printList(List list){ /* Prints List */

	int i=0;

	if(list.items > 0){
		list.Current = list.Head;
		while(list.Current->next != NULL){
			printf("|");
			list.printNode(list.Current->data);
			printf("|->");
			list.Current = list.Current->next;
		}
		printf("|");
		list.printNode(list.Current->data);
		printf("|->NULL");
	}
	else
		printf("printList: Nothing to print in List...\n");

}


void popList(ListPtr listp){ /* Removes 1 Node from the End of the List */

	if(listp->items > 0){

		listp->Current = listp->Head;

		listp->Head = listp->Head->next; /* Make Head point to its next and make the previous of new Head be NULL */
		if(listp->Head != NULL)
			listp->Head->previous = (Node*) NULL;
		
		if(listp->Current != NULL){
			listp->destroyNode(&(listp->Current->data));
			free(listp->Current);
			(listp->items)--;
		}

		listp->Current = listp->Head;

	}

}

void removeLastList(ListPtr listp){ /* Removes 1 Node from the Start of the List */

	if(listp->items > 0){

		if(listp->items == 1){ 
			popList(listp);
		}
		else{

			listp->Current = listp->Last->previous;

			if(listp->Last != NULL){

				listp->destroyNode(&(listp->Last->data));

				free(listp->Last);

				(listp->items)--;
			}

			listp->Last = listp->Current;

			listp->Last->next = (Node*) NULL;

		}

	}

}

void removeCurrentNode(ListPtr listp){ /* Removes the Node Current points to */

	Node* temp;

	if(listp->items > 0){

		if(listp->items == 1){ 
			popList(listp);
		}
		else{

			if(listp->Current == listp->Last){
				removeLastList(listp);
			}
			else{
				if(listp->Current != NULL){

					listp->destroyNode(&(listp->Current->data));

					temp = listp->Current->next;
					listp->Current->previous->next = listp->Current->next; /* Make the previous of the Current have as next the next of Current and vice versa */
					listp->Current->next->previous = listp->Current->previous;
			
					free(listp->Current);
				
					listp->Current = temp;
		
					(listp->items)--; //TODO: TEST AND FIX MIDDLE

				}
			}

		}

	}

}

void destroyList(ListPtr listp){ /* Destroys List */

	while(listp->items > 0) popList(listp);
	//printf("List was successfully destroyed!\n");
	if(listp->type_name != NULL){
		free(listp->type_name);
		listp->type_name = (char*) NULL;
	}

}
