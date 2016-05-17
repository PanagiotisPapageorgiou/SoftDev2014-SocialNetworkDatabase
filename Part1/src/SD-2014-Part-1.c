/*
 ============================================================================
 Name        : SD-2014-Part-1.c
 Version     :
 Copyright   : Your copyright notice
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gtools.h"
#include "defines.h"

#define PERSON_PROPERTIES_NUM 3
#define PERSON_REL_PROPERTIES_NUM 2

#define CHECK(text, actual, expected)                         \
  if (actual != expected) {                                   \
    printf("%-30s: Failed | actual = %3d, expected = %3d\n",  \
           text,                                              \
           actual,                                            \
           expected);                                         \
  } else {                                                    \
    printf("%-30s: Success\n", text);                     \
  }

int main(void) {

    int m = 2;
    int c = 3;

    /*create empty graph*/
    Graph* g = createGraph(m, c);

	printf("\nCreating nodes!\n");

    /*create node and set node properties*/
    GraphNode* n1 = setPersonProperties(5, "lonely", "loner", 29);
    GraphNode* n2 = setPersonProperties(1, "herald", "kllapi", 22); 
    GraphNode* n3 = setPersonProperties(2, "marialena", "kiriakidi", 25); 
    GraphNode* n4 = setPersonProperties(10, "antonia", "saravanou", 18); 
    GraphNode* n5 = setPersonProperties(6, "manos", "karvounis", 19); 
    GraphNode* n6 = setPersonProperties(3, "giannis", "chronis", 20); 
    GraphNode* n7 = setPersonProperties(4, "christoforos", "sfiggos", 16); 
    GraphNode* n8 = setPersonProperties(7, "stamatis", "xristoforidis", 24); 
    GraphNode* n9 = setPersonProperties(8, "xristos", "mallios", 29); 
    GraphNode* n10 = setPersonProperties(14, "johnny", "depp", 35); 
    GraphNode* n11 = setPersonProperties(12, "fox", "mulder", 29); 
    GraphNode* n12 = setPersonProperties(16, "dana", "scully", 25); 

	printf("\nFinished creating nodes!\n");
    /*print person*/
    //	printPersonProperties(n1);

    /*insert nodes in graph*/
    insertNode(n1, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n1);

    insertNode(n2, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n2);

    insertNode(n3, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n3);

    insertNode(n4, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n4);

    insertNode(n5, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n5);

    insertNode(n6, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n6);

    insertNode(n7, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n7);

    insertNode(n8, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n8);

    insertNode(n9, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n9);

    insertNode(n10, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n10);

    insertNode(n11, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n11);

    insertNode(n12, g);
	printf("Just inserted GraphNode: \n");
	printPersonProperties(n12);


	printf("\n==========================FINAL HASHTABLE===============================\n");
	printHashTable(&(g->hashtable));
	printf("\n========================================================================\n\n");

	printf("\nInserted all the nodes!\n");

	printf("\nCreating Edges...\n");

    /* Create edges and set properties */
    Edge* e1 = setEdgeProperties(1, 6, "knows", 30);
    Edge* e2 = setEdgeProperties(6, 1, "knows", 30);
    Edge* e3 = setEdgeProperties(1, 2, "knows", 20);
    Edge* e4 = setEdgeProperties(2, 1, "knows", 20);
    Edge* e5 = setEdgeProperties(1, 4, "knows", 30);
    Edge* e6 = setEdgeProperties(4, 1, "knows", 30);
    Edge* e7 = setEdgeProperties(2, 6, "knows", 10);
    Edge* e8 = setEdgeProperties(6, 2, "knows", 10);
    Edge* e9 = setEdgeProperties(4, 3, "knows", 30);
    Edge* e10 = setEdgeProperties(3, 4, "knows", 30);
    Edge* e11 = setEdgeProperties(4, 7, "knows", 30);
    Edge* e12 = setEdgeProperties(7, 4, "knows", 30);
    Edge* e13 = setEdgeProperties(4, 8, "knows", 10);
    Edge* e14 = setEdgeProperties(8, 4, "knows", 10);
    Edge* e15 = setEdgeProperties(3, 10, "knows", 30);
    Edge* e16 = setEdgeProperties(10, 3, "knows", 30);
    Edge* e17 = setEdgeProperties(10, 7, "knows", 30);
    Edge* e18 = setEdgeProperties(7, 10, "knows", 30);
    Edge* e19 = setEdgeProperties(10, 14, "knows", 50);
    Edge* e20 = setEdgeProperties(14, 10, "knows", 50);
    Edge* e21 = setEdgeProperties(14, 12, "knows", 30);
    Edge* e22 = setEdgeProperties(12, 14, "knows", 30);
    Edge* e23 = setEdgeProperties(12, 16, "knows", 30);
    Edge* e24 = setEdgeProperties(16, 12, "knows", 30);
    Edge* e25 = setEdgeProperties(16, 14, "knows", 30);
    Edge* e26 = setEdgeProperties(14, 16, "knows", 30);
	Edge* e27 = setEdgeProperties(3, 8, "knows", 30);
    Edge* e28 = setEdgeProperties(8, 3, "knows", 30);
	Edge* e29 = setEdgeProperties(8, 10, "knows", 10);
    Edge* e30 = setEdgeProperties(10, 8, "knows", 10);
	
	printf("\nFinished creating Edges!\n");

	printf("\nInserting Edges...\n");
    /* Insert edges in graph */
    insertEdge(1, e1, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e1);

    insertEdge(6, e2, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e2);

    insertEdge(1, e3, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e3);

    insertEdge(2, e4, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e4);

    insertEdge(1, e5, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e5);

    insertEdge(4, e6, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e6);

    insertEdge(2, e7, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e7);

    insertEdge(6, e8, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e8);

    insertEdge(4, e9, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e9);

    insertEdge(3, e10, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e10);

    insertEdge(4, e11, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e11);

    insertEdge(7, e12, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e12);

    insertEdge(4, e13, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e13);

    insertEdge(8, e14, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e14);

    insertEdge(3, e15, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e15);

    insertEdge(10, e16, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e16);

    insertEdge(10, e17, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e17);

    insertEdge(7, e18, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e18);

    insertEdge(10, e19, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e19);

    insertEdge(14, e20, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e20);

    insertEdge(14, e21, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e21);

    insertEdge(12, e22, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e22);

    insertEdge(12, e23, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e23);

    insertEdge(16, e24, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e24);

    insertEdge(16, e25, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e25);

    insertEdge(14, e26, g);
	printf("Just inserted Edge: \n");
	printEdgeProperties(e26);


	printf("\nFinished inserting Edges!\n");

    /* Perform lookups in the graph */
	printf("\nTesting lookUpNode...\n\n");
    GraphNode* nl1 = lookUpNode(12, g);
    printPersonProperties(nl1);
    GraphNode* nl2 = lookUpNode(16, g);
    printPersonProperties(nl2);
	printf("\nLet's check Graph Traversal now...\n\n");

    /* Find shortest path 1-1 */
    int spt1 = reachNode1(1, 12, g);
    CHECK("Shortest path between nodes (1,12)", spt1, 5);

    int spt2 = reachNode1(14, 14, g);
    CHECK("Shortest path between nodes (14,14)", spt2, 0);

    int spt3 = reachNode1(3, 16, g);
    CHECK("Shortest path between nodes (3,16)", spt3, 3);

    int spt4 = reachNode1(5, 3, g);
    CHECK("Shortest path between nodes (5,3)", spt4, INFINITY_REACH_NODE);
    //reachNode1 must return INFINITY_REACH_NODE defined in defines.h when two nodes are not connected

	printf("\nNow beginning ResultSet tests!!\n");
    /* Find shortest paths 1-N */
    ResultSet* res = reachNodeN(1, g);
    int i = 0;

    Pair results[10] = {
        {2, 1},
        {6, 1},
        {4, 1},
        {3, 2},
        {7, 2},
        {8, 2},
        {10, 3},
        {14, 4},
        {16, 5},
        {12, 5}
    };

    Pair pair;
    int k;
    int counter = 0;
    while (next(res, &pair)) {
		//printf("PAIR: (%d-%d)\n",pair.ID,pair.distance);

        ++counter;
        for (k = 0; k < 10; ++k) {
            if (results[k].ID == pair.ID) {
                if (results[k].distance == pair.distance) {
                    printf("Shortest path between nodes (%d,%d): Success\n", 1, results[k].ID);
                } else {
                    printf("Shortest path between nodes (%d,%d): Failed | actual = %3d, expected = %3d\n", 1, results[k].ID, pair.distance, results[k].distance);
                }
                break;
            }
        }
        if (k == 10) {
            printf("ReachNodeN : Failed | Your returned an extra Pair(%d,%d) ", pair.ID, pair.distance);
        }
    }
    CHECK("Number of pairs in the ResultSet", counter, 10);

	/* Destroy ResultSet */
	destroyResultSet(&(res));

	/* Destroy Graph */
	destroyGraph(g);

	/* Destroy all Nodes */
	destroyGraphNode(&n1);
	destroyGraphNode(&n2);
	destroyGraphNode(&n3);
	destroyGraphNode(&n4);
	destroyGraphNode(&n5);
	destroyGraphNode(&n6);
	destroyGraphNode(&n7);
	destroyGraphNode(&n8);
	destroyGraphNode(&n9);
	destroyGraphNode(&n10);
	destroyGraphNode(&n11);
	destroyGraphNode(&n12);

	/* Destroy all edges */
	destroyEdge(&e1);
	destroyEdge(&e2);
	destroyEdge(&e3);
	destroyEdge(&e4);
	destroyEdge(&e5);
	destroyEdge(&e6);
	destroyEdge(&e7);
	destroyEdge(&e8);
	destroyEdge(&e9);
	destroyEdge(&e10);
	destroyEdge(&e11);
	destroyEdge(&e12);
	destroyEdge(&e13);
	destroyEdge(&e14);
	destroyEdge(&e15);
	destroyEdge(&e16);
	destroyEdge(&e17);
	destroyEdge(&e18);
	destroyEdge(&e19);
	destroyEdge(&e20);
	destroyEdge(&e21);
	destroyEdge(&e22);
	destroyEdge(&e23);
	destroyEdge(&e24);
	destroyEdge(&e25);
	destroyEdge(&e26);
	destroyEdge(&e27);
	destroyEdge(&e28);
	destroyEdge(&e29);
	destroyEdge(&e30);

    return EXIT_SUCCESS;
}
