#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>

#include "PSJF_Shared.h"
#include "PSJF_Linked_List.h"

///////////////////////////////////////////////////////////////////////////////
// Function headers
///////////////////////////////////////////////////////////////////////////////
struct psjf_linked_list_node * psjf_linked_list_search_helper(
    struct psjf_linked_list * linkedList,
    void* data,
    int (*compareNodeData)(void * dataInNode, void * data));

struct psjf_linked_list_node * psjf_linked_list_make_node(
    struct psjf_linked_list_node * previous,
    struct psjf_linked_list_node * next,
    void* data);

///////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////
void* psjf_linked_list_delete(
    struct psjf_linked_list * linkedList,
    void * data,
    int (*compareNodeData)(void * dataInNode, void * data))
{
    struct psjf_linked_list_node * node =
        psjf_linked_list_search_helper(linkedList, data, compareNodeData);

    if (node == 0) {
        return 0;
    }
    else {
        void* returnVal = node->data;

        if (node == linkedList->head) {
            linkedList->head = node->next;
        }
        if (node == linkedList->tail) {
            linkedList->tail = node->previous;
        }
        if (node->previous != 0) {
            node->previous->next = node->next;
        }
        if (node->next != 0) {
            node->next->previous = node->previous;
        }

        linkedList->numNodes = linkedList->numNodes - 1;

        free(node);

        if ((linkedList->head == 0 || linkedList->tail == 0) && linkedList->numNodes > 0) {
            printf("HERE!\n");
        }

        return returnVal;
    }
}

void* psjf_linked_list_get_head_data(
    struct psjf_linked_list * linkedList)
{
    return linkedList->head->data;
}

unsigned long psjf_linked_list_get_num_nodes(
    struct psjf_linked_list * linkedList)
{
    return linkedList->numNodes;
}

void* psjf_linked_list_get_tail_data(
    struct psjf_linked_list * linkedList)
{
    return linkedList->tail->data;
}

void psjf_linked_list_insert_at_head(
    struct psjf_linked_list * linkedList,
    void* data)
{
    linkedList->head = psjf_linked_list_make_node(0, linkedList->head, data);

    if (linkedList->head->next != 0) {
        linkedList->head->next->previous = linkedList->head;
    }
    if (linkedList->tail == 0) {
        linkedList->tail = linkedList->head;
    }

    linkedList->numNodes = linkedList->numNodes + 1;
}

void psjf_linked_list_insert_at_tail(
    struct psjf_linked_list * linkedList,
    void * data)
{
    linkedList->tail = psjf_linked_list_make_node(linkedList->tail, 0, data);

    if (linkedList->tail->previous != 0) {
        linkedList->tail->previous->next = linkedList->tail;
    }
    if (linkedList->head == 0) {
        linkedList->head = linkedList->tail;
    }

    linkedList->numNodes = linkedList->numNodes + 1;

    if ((linkedList->head == 0 || linkedList->tail == 0) && linkedList->numNodes > 0) {
        printf("HERE!\n");
    }
}

struct psjf_linked_list * psjf_linked_list_make_empty_list(void) {
    struct psjf_linked_list * list =
        (struct psjf_linked_list *) myalloc(sizeof(struct psjf_linked_list));

    list->numNodes = 0;
    list->head = 0;
    list->tail = 0;

    return list;
}

void psjf_linked_list_print_contents(struct psjf_linked_list * list,
    void (*printItem)(void * dataInNode))
{
    struct psjf_linked_list_node * node = list->head;

    printf("[");
    while (node != 0) {
        printItem(node->data);
        node = node->next;

        if (node != 0)
            printf(", ");
    }
    printf("]");
}

void * psjf_linked_list_search(
    struct psjf_linked_list * linkedList,
    void * data,
    int (*compareNodeData)(void * dataInNode, void * data))
{
    return psjf_linked_list_search_helper(linkedList, data,
        compareNodeData)->data;
}

///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////
struct psjf_linked_list_node * psjf_linked_list_search_helper(
    struct psjf_linked_list * linkedList,
    void * data,
    int (*compareNodeData)(void * dataInNode, void * data))
{
    struct psjf_linked_list_node * node = linkedList->head;

    while (node != 0 && compareNodeData(node->data, data) != 0) {
        node = node->next;
    }

    return node;
}

struct psjf_linked_list_node * psjf_linked_list_make_node(
    struct psjf_linked_list_node * previous,
    struct psjf_linked_list_node * next,
    void* data)
{
    struct psjf_linked_list_node * node =
        (struct psjf_linked_list_node *) myalloc(sizeof(
            struct psjf_linked_list_node));

    node->previous = previous;
    node->next = next;
    node->data = data;

    return node;
}
