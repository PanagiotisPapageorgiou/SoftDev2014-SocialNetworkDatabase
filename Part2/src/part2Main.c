#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "fileReader.h"
#include "GraphQueries.h"
#include "defines.h"

//TODO: FREE MEMORY ACCORIDINGTO EXIT

int main(void) { /* This is Part 2 Main using DataSets */

	int m=2,c=3;

	AllDataSetStructs allStructs; /*Contains all Graphs and Lists that can be obtained through the DataSet */

	setNULLDataSetStructs(&allStructs); /*Set to NULL all pointers inside the struct */

	printf("Importing DataSet...\n");
	if(loadDataSet("./DataSet/",DATASET_DEBUG,&allStructs,m,c) == EXIT_VALUE){ /* Reads everything in DataSet folder - DATASET_DEBUG flag means it will ask for permission before reading */
		printf("main: loadDataSet failed! - Exiting!\n");
		printf("Destroying DataSet resources...\n");	
		destroyDataSetStructs(&allStructs); /* Free all DataSet related resources */
		exit(1);
	}
	////Query 1///////
	//Matches* match;
	//GraphNode* dateNode;
	//dateNode = lookUpNode(633, allStructs.personGraph);
	//dateNode = lookUpNode(3755, personGraph);
	//int commonInterests =1;
	//int ageDiff = 30;
	//int acquaintanceHops = 3;
	//int matchesNum = 1;
	
	//match =  matchSuggestion(dateNode,commonInterests,ageDiff,acquaintanceHops,matchesNum,allStructs.personGraph);

// ------------------ Query 3 -------------------
	char **menArray=NULL, **womenArray=NULL;
	int errorFlag=0, k=3, i=0;

	menArray = malloc(k * sizeof(char*));
	womenArray = malloc(k * sizeof(char*));
	assert (menArray != NULL); 
	assert (womenArray != NULL);
	for (i=0 ; i<k ; i++) {
		menArray[i] = (char*) malloc(64 * sizeof(char));
		womenArray[i] = (char*) malloc(64 * sizeof(char));
	}

	findTrends(k, allStructs.personGraph, allStructs.tagGraph, menArray, womenArray);

// ----------------------------------------------

// ------------------ Query 2 -------------------

	Stalkers *st=NULL;

	st = malloc(5 * sizeof(Stalkers));
	assert (st != NULL);
	assert (allStructs.personGraph != NULL);

	//getTopStalkers(5, 10, 1, allStructs.personGraph, st, &errorFlag);

// ----------------------------------------------

	if(errorFlag == EXIT_VALUE){
		if(st != NULL) free(st);
		destroyDataSetStructs(&allStructs); /* Free all DataSet related resources */
		exit(1);
	}
	if(st != NULL) free(st);

	printf("Destroying DataSet resources...\n");	
	destroyDataSetStructs(&allStructs); /* Free all DataSet related resources */
	return 0;
}
