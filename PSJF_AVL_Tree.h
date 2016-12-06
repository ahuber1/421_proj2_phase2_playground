#ifndef PSJF_AVL_H
#define PSJF_AVL_H

#define TREE_INSERT_STATUS_NEW_NODE_CREATED 0
#define TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND 1

#define TREE_DELETION_STATUS_NODE_ALTERED 0
#define TREE_DELETION_STATUS_NODE_REMOVED 1
#define TREE_DELETION_STATUS_FAILURE 3

struct psjf_avl_tree {
    long numNodes;
    struct psjf_avl_tree_node * root;
};

struct psjf_avl_tree_node {
    struct psjf_avl_tree_node * left;
    struct psjf_avl_tree_node * right;
    long height;
    void* data;
};

void * psjf_avl_tree_delete(
    struct psjf_avl_tree * tree,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToDelete));

void psjf_avl_tree_delete_all(struct psjf_avl_tree * tree);

long psjf_avl_tree_get_num_nodes(struct psjf_avl_tree * tree);

void psjf_avl_tree_in_order_traversal(struct psjf_avl_tree * tree,
    void (*onNodeTouched)(void *));

int psjf_avl_tree_insert(struct psjf_avl_tree * tree,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToDelete));

struct psjf_avl_tree * psjf_avl_tree_make_empty_tree(void);

void * psjf_avl_tree_search(struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToSearch));

#endif
