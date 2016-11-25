#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "PSJF_AVL_Tree.h"
#include "PSJF_Linked_List.h"
#include "PSJF_Shared.h"

#define MIN = 1
#define MAX = 15
#define NUMBER_OF_NUMBERS = MAX - MIN
#define ASCENDING_FOR int i; for(i = MIN; i <= MAX; i++)
#define DESCENDING_FOR int i; for(i = MAX; i >= MIN, i++)
#define MAP_INDEX(num) num - MIN

int compareInts(void * ptr1, void * ptr2);
void printInt(void * ptr);
int compareEncapsulatedNodeData(void * encapsulatedData,
    void * unencapsulatedData);
void * onNewNodeCreated(void * unencapsulatedData);
void * onPreviousNodeFound(void * nodeData, void * data);
void printQueue(void * nodeData);
void insert(struct psjf_avl_tree * tree);
void randomRemove(struct psjf_avl_tree * tree);
struct psjf_avl_tree_deletion_result onNodeFoundDeletion(void * nodeData);

int removed[NUMBER_OF_NUMBERS];

int main(int argc, char const *argv[]) {
    struct psjf_avl_tree * tree = psjf_avl_tree_make_empty_tree();

    insert(tree);
    //insert(tree);

    return 0;
}

void randomRemove(struct psjf_avl_tree * tree) {
    int num;
    int count = 0;
    srand(time(NULL));
    while (count < NUMBER_OF_NUMBERS) {
        num = (rand() % NUMBER_OF_NUMBERS) + MIN
        if (removed[MAP_INDEX(num)] == FALSE) {
            struct psjf_avl_tree_deletion_result_final =
                psjf_avl_tree_delete(tree, &num, &compareEncapsulatedNodeData,
                    &onNodeFoundDeletion);
        }

    }
}

void insert(struct psjf_avl_tree * tree) {
    int * ptr;
    ASCENDING_FOR {
        ptr = malloc(sizeof(int));
        *ptr = i;
        int result = psjf_avl_tree_insert(tree, ptr,
            &compareEncapsulatedNodeData, &onNewNodeCreated,
            &onPreviousNodeFound);

        const char * status;

        switch (result) {
            case TREE_INSERT_STATUS_NEW_NODE_CREATED:
                status = "NEW_NODE_CREATED";
                break;
            case TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND:
                status = "PREVIOUS_NODE_FOUND";
                break;
            default:
                status = "UNKNOWN";
        }

        printf("Inserted %d with status %s\n\n", i, status);
        printf("MIN NODE: ");
        printQueue(psjf_avl_tree_get_min(tree));
        printf("\n");
        printf("MAX NODE: ");
        printQueue(psjf_avl_tree_get_max(tree));
        printf("\n");
        printf("NUM NODES: %ld\n", psjf_avl_tree_get_num_nodes(tree));
        printf("-----------------------------------------------------------\n");

        removed[MAP_INDEX(i)] = FALSE;
    }
}

int compareInts(void * ptr1, void * ptr2) {
    return *((int*) ptr1) - *((int*) ptr2);
}

void printInt(void * ptr) {
    printf("%d", *((int*) ptr));
}

int compareEncapsulatedNodeData(void * encapsulatedData,
    void * unencapsulatedData)
{
    struct psjf_linked_list * list =
        (struct psjf_linked_list *) encapsulatedData;
    void * ptr1 = PSJF_QUEUE_PEEK(list);

    return compareInts(ptr1, unencapsulatedData);
}

void * onNewNodeCreated(void * unencapsulatedData) {
    struct psjf_linked_list * list = psjf_linked_list_make_empty_list();
    PSJF_ENQUEUE(list, unencapsulatedData);

    return list;
}

void * onPreviousNodeFound(void * nodeData, void * data) {
    struct psjf_linked_list * list =
        (struct psjf_linked_list *) nodeData;
    PSJF_ENQUEUE(list, data);

    return list;
}

void printQueue(void * nodeData) {
    struct psjf_linked_list * list =
        (struct psjf_linked_list *) nodeData;

    psjf_linked_list_print_contents(list, &printInt);
}

struct psjf_avl_tree_deletion_result onNodeFoundDeletion(void * nodeData) {
    struct psjf_linked_list * list =
        (struct psjf_linked_list *) nodeData;
    struct psjf_avl_tree_deletion_result returnVal;
    returnVal.dataRemoved = PSJF_DEQUEUE(list, &compareInts);

    if (psjf_linked_list_get_num_nodes(list) == 0) {
        returnVal.dataToStoreInNode = 0;
    }
    else {
        returnVal.dataToStoreInNode = list;
    }

    return returnVal;
}
