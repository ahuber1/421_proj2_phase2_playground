#ifndef PSJF_AVL_H
#define PSJF_AVL_H

#define TREE_INSERT_STATUS_NEW_NODE_CREATED 0
#define TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND 1
//#define TREE_INSERT_STATUS_FAILURE 2

#define TREE_DELETION_STATUS_NODE_ALTERED 0
#define TREE_DELETION_STATUS_NODE_REMOVED 1
#define TREE_DELETION_STATUS_FAILURE 3

struct psjf_avl_tree {
    long numNodes;
    struct psjf_avl_tree_node * root;
    struct psjf_avl_tree_node * min;
    struct psjf_avl_tree_node * max;
};

struct psjf_avl_tree_node {
    struct psjf_avl_tree_node * parent;
    struct psjf_avl_tree_node * left;
    struct psjf_avl_tree_node * right;
    long height;
    void* data;
};

struct psjf_avl_tree_deletion_result {
    void * dataRemoved;
    void * dataToStoreInNode;
};

struct psjf_avl_tree_deletion_result_final {
    int status;
    void * dataRemoved;
    void * dataToStoreInNode;
};

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete(
    struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData));

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete_min(
    struct psjf_avl_tree * tree,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData),
    void * (*unencapsulateData)(void * encapsulatedData));

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete_max(
    struct psjf_avl_tree * tree,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData),
    void * (*unencapsulateData)(void * encapsulatedData));

void * psjf_avl_tree_get_min(struct psjf_avl_tree * tree);

void * psjf_avl_tree_get_max(struct psjf_avl_tree * tree);

long psjf_avl_tree_get_num_nodes(struct psjf_avl_tree * tree);

void psjf_avl_tree_in_order_traversal(struct psjf_avl_tree * tree,
    void (*onNodeTouched)(void *));

int psjf_avl_tree_insert(struct psjf_avl_tree * tree,
    void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    void* (*onNewNodeCreated)(void *),
    void* (*onPreviousNodeFound)(void * nodeData, void * data));

struct psjf_avl_tree_deletion_result psjf_avl_tree_make_deletion_result(
    void * dataRemoved, void * dataToStoreInNode);

struct psjf_avl_tree * psjf_avl_tree_make_empty_tree(void);

void * psjf_avl_tree_search(struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData));

#endif
