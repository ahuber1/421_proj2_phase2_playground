#include <stdlib.h>

#include "PSJF_AVL_Tree.h"

//////////////////////////////////////////////////////////////////////////////
// Structs
//////////////////////////////////////////////////////////////////////////////

struct psjf_avl_tree_insertion_result {
    int status;
    struct psjf_avl_tree_node * addedNode;
};

//////////////////////////////////////////////////////////////////////////////
// Function Headers
//////////////////////////////////////////////////////////////////////////////

void psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node);

struct psjf_avl_tree_deletion_result_inner
    psjf_avl_tree_make_deletion_result_inner(struct
        psjf_avl_tree_deletion_result);

struct psjf_avl_tree_insertion_result psjf_avl_tree_make_insertion_result(
    int status, struct psjf_avl_tree_node * addedNode);

struct psjf_avl_tree_node * psjf_avl_tree_make_tree_node(
    struct psjf_avl_tree_node * parent, struct psjf_avl_tree_node * left,
    struct psjf_avl_tree_node * right, long height, void * data);

long psjf_avl_tree_max(long num1, long num2);

struct psjf_avl_tree_deletion_result_inner psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData));

struct psjf_avl_tree_insertion_result psjf_avl_tree_insert_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    void * (*onNewNodeCreated)(void *),
    void * (*onPreviousNodeFound)(void * nodeData, void * data));

void psjf_avl_tree_perform_deletion(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData));

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node);

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node);

void * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    void * data, int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData));

void psjf_avl_tree_update_height(struct psjf_avl_tree_node * node);

//////////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////////

struct psjf_avl_tree_deletion_result_inner psjf_avl_tree_delete(
    struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData))
{
    struct psjf_avl_tree_deletion_result_inner result =
        psjf_avl_tree_delete_helper(tree, tree->root, data,
            compareEncapsulatedNodeData, onNodeFound);

    if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
        tree->numNodes = tree->numNodes - 1;
    }

    return result;
}

void * psjf_avl_tree_get_max(struct psjf_avl_tree * tree) {
    return tree->max->data;
}

void * psjf_avl_tree_get_min(struct psjf_avl_tree * tree) {
    return tree->min->data;
}

int psjf_avl_tree_insert(struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    void * (*onNewNodeCreated)(void *),
    void * (*onPreviousNodeFound)(void * nodeData, void * data))
{
    struct psjf_avl_tree_insertion_result insertionResult =
        psjf_avl_tree_insert_helper(
            // the tree
            tree,

            // the starting point of the search process involved with every
            // insertion; the root of the tree
            tree->root,

            // the parent of the starting point (i.e., root), which is null
            0,

            // the data that we will insert
            data,

            // a function pointer that will be used to compare items in the tree
            // with the data that is being added
            compareEncapsulatedNodeData,

            // a function pointer that will be used if a new node needs to be
            // created in order to insert the data into the tree
            onNewNodeCreated,

            // a function pointer that will be used if a node containing the
            // data to be inserted already exists in the tree
            onPreviousNodeFound);

    if (tree->min == 0 ||
        compareEncapsulatedNodeData(tree->min->data, data) > 0)
    {
        tree->min = insertionResult.addedNode;
    }

    if (tree->max == 0 ||
        compareEncapsulatedNodeData(tree->max->data, data) < 0)
    {
        tree->max = insertionResult.addedNode;
    }

    if (insertionResult.addedNode->parent == 0) {
        tree->root = insertionResult.addedNode;
    }

    if (insertionResult.status == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
        tree->numNodes = tree->numNodes + 1;
    }

    return insertionResult.status;
}

struct psjf_avl_tree_deletion_result psjf_avl_tree_make_deletion_result(
    void * dataRemoved, void * dataToStoreInNode)
{
    struct psjf_avl_tree_deletion_result returnVal = {
        .dataRemoved = dataRemoved,
        .dataToStoreInNode = dataToStoreInNode,
    };

    return returnVal;
}

void * psjf_avl_tree_search(struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData))
{
    return psjf_avl_tree_search_helper(tree->root, data,
        compareEncapsulatedNodeData);
}

//////////////////////////////////////////////////////////////////////////////
// Helper Functions
//////////////////////////////////////////////////////////////////////////////

void psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node)
{
    long leftHeight = psjf_avl_tree_calculate_left_height(node) + 1;
    long rightHeight = psjf_avl_tree_calculate_right_height(node) + 1;
    long balanceFactor = -leftHeight + rightHeight;

    if (balanceFactor < -1) {
        psjf_avl_tree_rotate_right(tree, node);
    }
    else if (balanceFactor > 1) {
        psjf_avl_tree_rotate_left(tree, node);
    }
    else {
        psjf_avl_tree_update_height(node);
    }
}

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node) {
    if (node->left == 0) {
        return 0;
    }
    else {
        return node->left->height;
    }
}

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node) {
    if (node->right == 0) {
        return 0;
    }
    else {
        return node->right->height;
    }
}

struct psjf_avl_tree_deletion_result_inner
    psjf_avl_tree_make_deletion_result_inner(
        struct psjf_avl_tree_deletion_result result)
{
    struct psjf_avl_tree_deletion_result_inner returnVal;

    returnVal.result = result;

    if (result.dataRemoved == 0 && result.dataToStoreInNode != 0) {
        returnVal.status = TREE_DELETION_STATUS_NODE_ALTERED;
    }
    else if (result.dataRemoved != 0 && result.dataToStoreInNode == 0) {
        returnVal.status = TREE_DELETION_STATUS_NODE_REMOVED;
    }
    else {
        returnVal.status = TREE_DELETION_STATUS_FAILURE;
    }

    return returnVal;
}

struct psjf_avl_tree_insertion_result psjf_avl_tree_make_insertion_result(
    int status, struct psjf_avl_tree_node * addedNode)
{
    struct psjf_avl_tree_insertion_result returnVal = {
        .status = status,
        .addedNode = addedNode
    };

    return returnVal;
}

struct psjf_avl_tree_node * psjf_avl_tree_make_tree_node(
    struct psjf_avl_tree_node * parent, struct psjf_avl_tree_node * left,
    struct psjf_avl_tree_node * right, long height, void * data)
{
    struct psjf_avl_tree_node * newNode =
        (struct psjf_avl_tree_node*) malloc(sizeof(
            struct psjf_avl_tree_node));

    newNode->parent = parent;
    newNode->left   = left;
    newNode->right  = right;
    newNode->height = height;
    newNode->data   = data;

    return newNode;
}

long psjf_avl_tree_max(long num1, long num2) {
    if (num1 > num2) {
        return num1;
    }
    else {
        return num2;
    }
}

struct psjf_avl_tree_deletion_result_inner psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData))
{
    if (node == 0) {
        return psjf_avl_tree_make_deletion_result_inner(
            psjf_avl_tree_make_deletion_result(0, 0));
    }
    else if (compareEncapsulatedNodeData(node->data, data) > 0) {
        struct psjf_avl_tree_deletion_result_inner returnVal =
            psjf_avl_tree_delete_helper(tree, node->left, data,
                compareEncapsulatedNodeData, onNodeFound);

        psjf_avl_tree_update_height(node);

        return returnVal;
    }
    else if (compareEncapsulatedNodeData(node->data, data) < 0) {
        struct psjf_avl_tree_deletion_result_inner returnVal =
            psjf_avl_tree_delete_helper(tree, node->right, data,
                compareEncapsulatedNodeData, onNodeFound);

        psjf_avl_tree_update_height(node);

        return returnVal;
    }
    else {
        struct psjf_avl_tree_deletion_result_inner result =
            psjf_avl_tree_make_deletion_result_inner(onNodeFound(node->data));

        if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
            psjf_avl_tree_perform_deletion(tree, node, data,
                compareEncapsulatedNodeData, onNodeFound);
        }

        return result;
    }
}

struct psjf_avl_tree_insertion_result psjf_avl_tree_insert_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    void * (*onNewNodeCreated)(void *),
    void * (*onPreviousNodeFound)(void * nodeData, void * data))
{
    if (node == 0) {
        struct psjf_avl_tree_node * addedNode =
            psjf_avl_tree_make_tree_node(
                parent,                // parent node
                0,                     // left node
                0,                     // right node
                0,                     // height
                onNewNodeCreated(data) // node data
            );

        return psjf_avl_tree_make_insertion_result(
            TREE_INSERT_STATUS_NEW_NODE_CREATED, addedNode);
    }
    else if (compareEncapsulatedNodeData(node->data, data) == 0) {
        node->data = onPreviousNodeFound(node->data, data);
        return psjf_avl_tree_make_insertion_result(
            TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND, node);
    }
    else {
        struct psjf_avl_tree_node * nextNode;

        if (compareEncapsulatedNodeData(node->data, data) > 0) {
            nextNode = node->left;
        }
        else {
            nextNode = node->right;
        }

        struct psjf_avl_tree_insertion_result insertionResult =
            psjf_avl_tree_insert_helper(tree, nextNode, node, data,
                compareEncapsulatedNodeData, onNewNodeCreated,
                onPreviousNodeFound);

        if (insertionResult.status == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
            psjf_avl_tree_balance_tree_if_necessary_at(tree, node);
        }

        return insertionResult;
    }
}

void psjf_avl_tree_perform_deletion(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData))
{
    // If a node has no children
    if (node->left == 0 && node->right == 0) {
        if (node == tree->root) {
            tree->root = 0;
        }
        else if (node->parent->left == node) {
            node->parent->left = 0;
        }
        else {
            node->parent->right = 0;
        }

        free(node);
    }

    // If a node has two children
    else if (node->left != 0 && node->right != 0) {
        if (node->left->right == 0 && node->right->left == 0) {
            if (node == tree->root) {
                tree->root = node->left;

                // Unnecessary
                //psjf_avl_tree_update_height(tree->root);
            }
            else if (node->parent->left == node) {
                node->parent->left = node->left;
                node->left->parent = node->parent;
                psjf_avl_tree_update_height(node->parent->left);
            }
            else {
                node->parent->right = node->left;
                node->right->parent = node->parent;
                psjf_avl_tree_update_height(node->parent->right);
            }
        }
        else if (node->left->right != 0) {
            node->data = node->left->right->data;
            psjf_avl_tree_delete_helper(tree, node->left->right, data,
                compareEncapsulatedNodeData, onNodeFound);
            psjf_avl_tree_update_height(node);
        }
        else {
            node->data = node->right->left->data;
            psjf_avl_tree_delete_helper(tree, node->right->left, data,
                compareEncapsulatedNodeData, onNodeFound);
            psjf_avl_tree_update_height(node);
        }
    }

    // If a node has one child on the left
    else if (node->right == 0) {
        if (node == tree->root) {
            tree->root = node->left;
            node->left->parent = 0;
        }
        else if (node->parent->left == node) {
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }
        else {
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }

        psjf_avl_tree_update_height(node->left);
        free(node);
    }

    // If a node has one child on the right
    else {
        if (node == tree->root) {
            tree->root = node->right;
            node->right->parent = 0;
        }
        else if (node->parent->left == node) {
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }
        else {
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }

        psjf_avl_tree_update_height(node->right);
        free(node);
    }
}

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node)
{
    struct psjf_avl_tree_node * temp = node->right;

    node->right = node->right->left;
    temp->left = node;

    node->parent = temp;
    node->right->parent = node;

    node->height = psjf_avl_tree_max(node->left->height,
        node->right->height) + 1;

    node->parent->height = psjf_avl_tree_max(node->parent->left->height,
        node->parent->right->height) + 1;

    if (node == tree->root) {
        tree->root = node->parent;
    }
}

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node)
{
    struct psjf_avl_tree_node * temp = node->left;

    node->left = node->left->right;
    temp->right = node;

    node->parent = temp;
    node->left->parent = node;

    node->height = psjf_avl_tree_max(node->left->height,
        node->right->height) + 1;

    node->parent->height = psjf_avl_tree_max(node->parent->left->height,
        node->parent->right->height) + 1;

    if (node == tree->root) {
        tree->root = node->parent;
    }
}

void * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    void * data, int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData))
{
    if (node == 0) {
        return 0;
    }
    else if (compareEncapsulatedNodeData(node->data, data) > 0) {
        return psjf_avl_tree_search_helper(node->left, data,
            compareEncapsulatedNodeData);
    }
    else if (compareEncapsulatedNodeData(node->data, data) < 0) {
        return psjf_avl_tree_search_helper(node->right, data,
            compareEncapsulatedNodeData);
    }
    else {
        return node->data;
    }
}

void psjf_avl_tree_update_height(struct psjf_avl_tree_node * node) {
    node->height = psjf_avl_tree_max(
        (psjf_avl_tree_calculate_left_height(node) + 1),
        (psjf_avl_tree_calculate_right_height(node) + 1));
}
