#ifndef PSJF_AVL_H
#define PSJF_AVL_H

#define TREE_INSERT_STATUS_NEW_NODE_CREATED = 0
#define TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND = 1
#define TREE_INSERT_STATUS_FAILURE = 2

struct psjf_avl_tree {
    long numNodes;
    struct psjf_avl_tree_node * root;
    struct psjf_avl_tree_node * min;
    struct psjf_avl_tree_node * max;
};

struct psjf_avl_tree_node {
    struct psjf_avl_tree_node* parent;
    struct psjf_avl_tree_node* left;
    struct psjf_avl_tree_node* right;
    long height;
    void* data;
};

int psjf_avl_tree_insert(struct psjf_avl_tree * tree, void* data,
    int (*compareEncapsulatedNodeData)(*void encapsulatedData,
        *void unencapsulatedData),
    void* (*onNewNodeCreated)(*void),
    void* (*onPreviousNodeFound)(*void nodeData, *void data));

void* psjf_avl_tree_search(struct psjf_avl_tree * tree, void* data,
    int (*compareEncapsulatedNodeData)(*void encapsulatedData,
        *void unencapsulatedData));

void* psjf_avl_tree_delete(struct psjf_avl_tree * tree, void* data,
    int (*compareEncapsulatedNodeData)(*void encapsulatedData,
        *void unencapsulatedData));

void* psjf_avl_tree_get_min(struct psjf_avl_tree * tree);

void* psjf_avl_tree_get_max(struct psjf_avl_tree * tree);

#endif
