#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "PSJF_AVL_Tree.h"
#include "PSJF_Linked_List.h"
#include "PSJF_Shared.h"

#define DURATION 30
#define MIN 1
#define MAX 10000
#define RANDOM(min, max) ((rand() % (max - min + 1)) + min)

int compareInts(void * ptr1, void * ptr2);
void printInt(void * ptr);
int compareEncapsulatedNodeData(void * encapsulatedData, void * unencapsulatedData);
void printQueue(void * nodeData);

struct test_results {
    unsigned long treeCleared;
    unsigned long insertStatusNewNodeCreated;
    unsigned long insertStatusPreviousNodeFound;
    unsigned long deletionStatusMismatch;
    unsigned long deletionStatusNodeRemoved;
    unsigned long deletionStatusFailure;
};

int main(int argc, char const *argv[]) {
    srand(time(0));
    struct psjf_avl_tree * tree = psjf_avl_tree_make_empty_tree();
    struct test_results results;
    //struct timeval start_tv;
    //struct timezone start_tz;
    //struct timeval tv;
    //struct timezone tz;

    results.treeCleared = -1;
    results.insertStatusNewNodeCreated = 0;
    results.insertStatusPreviousNodeFound = 0;
    results.deletionStatusMismatch = 0;
    results.deletionStatusNodeRemoved = 0;
    results.deletionStatusFailure = 0;

    //gettimeofday(&start_tv, &start_tz);
    //gettimeofday(&tv, &tz);

    //long timediff = (tv.tv_sec - start_tv.tv_sec);
    
    int insertCounter = 30000;
    int deleteCounter = insertCounter / 2;

    while (insertCounter > 0 || deleteCounter > 0) {

        int * data = (int *) malloc(sizeof(int));
        *data = RANDOM(MIN, MAX);
        int insert;
        long numNodes = psjf_avl_tree_get_num_nodes(tree);
        //int selection = RANDOM(1, 2);

        if (numNodes == 0) {
            results.treeCleared++;
            insert = TRUE;
        }
        else if (insertCounter > 0) {
            insert = TRUE;
        }
        else {
            insert = FALSE; // will be changed
        }
        
        printf("Data = %d\n", *data);

        if (insert == TRUE) {

            printf("[INSERT] ");

            int insertResult = psjf_avl_tree_insert(tree, data, &compareInts);

            switch (insertResult) {
                case TREE_INSERT_STATUS_NEW_NODE_CREATED:
                    printf("New Node Created [%3d]\n", *data);
                    results.insertStatusNewNodeCreated++;
                    break;
                case TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND:
                    printf("Previous Node Found [%3d]\n", *data);
                    results.insertStatusPreviousNodeFound++;
                    break;
            }
            
            insertCounter--;
        }
        else {
            printf("[DELETE] ");

            void * returnVal = psjf_avl_tree_delete(tree, data, &compareInts);

            if (returnVal != 0 && compareInts(returnVal, data) == 0) {
                data = (int *) returnVal;
                printf("Previous Node Found [%3d]\n", *data);
                results.deletionStatusNodeRemoved++;
            }
            else if (returnVal != 0) {
                printf("Mismatch\n");
                results.deletionStatusMismatch++;
            }
            else {
                printf("Failure\n");
                results.deletionStatusFailure++;
            }
            
            deleteCounter--;
        }

        printf("Number of Nodes: %ld\n", psjf_avl_tree_get_num_nodes(tree));
        free(data);
    }

     printf("\nRESULTS:\n");
     printf("\n                     Tree Cleared: %ld\n", results.treeCleared);
     printf("\n   Insert Status New Node Created: %ld\n", results.insertStatusNewNodeCreated);
     printf("\nInsert Status Previous Node Found: %ld\n", results.insertStatusPreviousNodeFound);
     printf("\n     Deletion Status Node Failure: %ld\n", results.deletionStatusFailure);
     printf("\n     Deletion Status Node Removed: %ld\n", results.deletionStatusNodeRemoved);
     printf("\n         Deletion Status Mismatch: %ld\n", results.deletionStatusMismatch);
     printf("\n                     Size of Tree: %ld\n", psjf_avl_tree_get_num_nodes(tree));
     printf("\n                  Calculated Size: %ld\n", results.insertStatusNewNodeCreated - results.deletionStatusNodeRemoved);
}

int compareInts(void * ptr1, void * ptr2) {
    return *((int*) ptr1) - *((int*) ptr2);
}

void printInt(void * ptr) {
    printf("%d", *((int*) ptr));
}

void printQueue(void * nodeData) {
    if (nodeData == 0) {
        printf("null");
    }
    else {
        struct psjf_linked_list * list =
            (struct psjf_linked_list *) nodeData;

        psjf_linked_list_print_contents(list, &printInt);
    }
}
