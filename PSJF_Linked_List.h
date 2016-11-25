#ifndef PSJF_LINKED_LIST_H
#define PSJF_LINKED_LIST_H

struct psjf_linked_list {
    unsigned long numNodes;
    struct psjf_linked_list_node * head;
    struct psjf_linked_list_node * tail;
};

struct psjf_linked_list_node {
    struct psjf_linked_list_node * previous;
    struct psjf_linked_list_node * next;
    void* data;
};

void* psjf_linked_list_delete(
    struct psjf_linked_list * linkedList,
    void* data,
    int (*compareNodeData)(void * dataInNode, void * data));

void* psjf_linked_list_get_head_data(
    struct psjf_linked_list * linkedList);

unsigned long psjf_linked_list_get_num_nodes(
    struct psjf_linked_list * linkedList);

void* psjf_linked_list_get_tail_data(
    struct psjf_linked_list * linkedList);

void psjf_linked_list_insert_at_head(
    struct psjf_linked_list * linkedList,
    void* data);

void psjf_linked_list_insert_at_tail(
    struct psjf_linked_list * linkedList,
    void * data);

struct psjf_linked_list * psjf_linked_list_make_empty_list(void);

void psjf_linked_list_print_contents(struct psjf_linked_list * list,
    void (*printItem)(void * dataInNode));

void* psjf_linked_list_search(
    struct psjf_linked_list * linkedList,
    void * data,
    int (*compareNodeData)(void * dataInNode, void * data));

#define PSJF_ENQUEUE(list, ptr) \
    psjf_linked_list_insert_at_tail(list, ptr);


#define PSJF_DEQUEUE(list, compareNodeData) \
    psjf_linked_list_delete(list, psjf_linked_list_get_head_data(list), \
        compareNodeData);


#define PSJF_PUSH(list, ptr) \
    psjf_linked_list_insert_at_tail(list, ptr);


#define PSJF_POP(list, compareNodeData) \
    psjf_linked_list_delete(list, psjf_linked_list_get_tail_data(list), \
        compareNodeData);

#define PSJF_QUEUE_PEEK(list) psjf_linked_list_get_head_data(list);

#define PSJF_STACK_PEEK(list) psjf_linked_list_get_tail_data(list);

#endif
