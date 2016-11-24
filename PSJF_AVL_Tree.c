#include <stdlib.h>

#include "PSJF_AVL_Tree.h"

//////////////////////////////////////////////////////////////////////////////
// Structs
//////////////////////////////////////////////////////////////////////////////

struct psjf_avl_tree_deletion_result_inner {
    int status;
    void * dataRemoved;
    void * dataToStoreInNode;
    struct psjf_avl_tree_node * nodeRemoved;
};

struct psjf_avl_tree_insertion_result {
    int status;
    struct psjf_avl_tree_node * addedNode;
};

struct psjf_avl_tree_min_and_max {
    struct psjf_avl_tree_node * min;
    struct psjf_avl_tree_node * max;
};

//////////////////////////////////////////////////////////////////////////////
// Function Headers
//////////////////////////////////////////////////////////////////////////////

void psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node);

struct psjf_avl_tree_deletion_result_inner psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData));

struct psjf_avl_tree_deletion_result_inner
    psjf_avl_tree_make_deletion_result_inner(
        struct psjf_avl_tree_deletion_result result,
        struct psjf_avl_tree_node * nodeRemoved);

struct psjf_avl_tree_deletion_result_final
    psjf_avl_tree_make_deletion_result_final(
        struct psjf_avl_tree_deletion_result_inner result);

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

struct psjf_avl_tree_min_and_max
    psjf_avl_tree_update_min_and_max_if_necessary_after_deletion(
        struct psjf_avl_tree * tree, struct psjf_avl_tree_node * nodeDeleted);

//////////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////////

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete(
    struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData))
{
    struct psjf_avl_tree_deletion_result_inner result =
        psjf_avl_tree_delete_helper(
            // The tree
            tree,

            // The starting point of the delete algorithm, the root of the tree
            tree->root,

            // The data that we will search for and delete
            data,

            // Function pointer that compares encapsulated data in the node
            // with the unencapsulated data
            compareEncapsulatedNodeData,

            // Function pointer that returns 0 if the node should be deleted,
            // or a pointer to the data that should be stored in that node
            onNodeFound);

    // If a node was removed
    if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
        tree->numNodes = tree->numNodes - 1;

        struct psjf_avl_tree_min_and_max minAndMax =
            psjf_avl_tree_update_min_and_max_if_necessary_after_deletion(tree,
                result.nodeRemoved);

        tree->min = minAndMax.min;
        tree->max = minAndMax.max;
    }

    return psjf_avl_tree_make_deletion_result_final(result);
}

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete_min(
    struct psjf_avl_tree * tree,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData),
    void * (*unencapsulateData)(void * encapsulatedData))
{
    struct psjf_avl_tree_deletion_result_inner result =
        psjf_avl_tree_delete_helper(
            // The tree
            tree,

            // The min node, the starting point of the delete algorithm
            tree->min,

            // Unencapsulates the data stored at min node, and passes it in
            unencapsulateData(tree->min->data),

            // Function pointer that compares encapsulated data in the node
            // with the unencapsulated data
            compareEncapsulatedNodeData,

            // Function pointer that returns 0 if the node should be deleted,
            // or a pointer to the data that should be stored in that node
            onNodeFound);

    // If a node was removed
    if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
        tree->numNodes = tree->numNodes - 1;

        struct psjf_avl_tree_min_and_max minAndMax =
            psjf_avl_tree_update_min_and_max_if_necessary_after_deletion(tree,
                result.nodeRemoved);

        tree->min = minAndMax.min;
        tree->max = minAndMax.max;
    }

    return psjf_avl_tree_make_deletion_result_final(result);
}

struct psjf_avl_tree_deletion_result_final psjf_avl_tree_delete_max(
    struct psjf_avl_tree * tree,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData),
    struct psjf_avl_tree_deletion_result (*onNodeFound)(void * nodeData),
    void * (*unencapsulateData)(void * encapsulatedData))
{
    struct psjf_avl_tree_deletion_result_inner result =
        psjf_avl_tree_delete_helper(
            // The tree
            tree,

            // The max node, the starting point of the delete algorithm
            tree->max,

            // Unencapsulates the data stored at max node, and passes it in
            unencapsulateData(tree->max->data),

            // Function pointer that compares encapsulated data in the node
            // with the unencapsulated data
            compareEncapsulatedNodeData,

            // Function pointer that returns 0 if the node should be deleted,
            // or a pointer to the data that should be stored in that node
            onNodeFound);

    // If a node was removed
    if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
        tree->numNodes = tree->numNodes - 1;

        struct psjf_avl_tree_min_and_max minAndMax =
            psjf_avl_tree_update_min_and_max_if_necessary_after_deletion(tree,
                result.nodeRemoved);

        tree->min = minAndMax.min;
        tree->max = minAndMax.max;
    }

    return psjf_avl_tree_make_deletion_result_final(result);
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

    // If there is no min node, or if a new min node was found, update it
    if (tree->min == 0 ||
        compareEncapsulatedNodeData(tree->min->data, data) > 0)
    {
        tree->min = insertionResult.addedNode;
    }

    // If there is no max node, or if a new max node was found, update it
    if (tree->max == 0 ||
        compareEncapsulatedNodeData(tree->max->data, data) < 0)
    {
        tree->max = insertionResult.addedNode;
    }

    // If the node that was added has no parent, then that node is the root.
    // Therefore, update the root.
    if (insertionResult.addedNode->parent == 0) {
        tree->root = insertionResult.addedNode;
    }

    // If a new node was created, increment the number of nodes
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

    // If the tree at node is left-heavy, rotate right, and update the height as
    // necessary in one step.
    if (balanceFactor < -1) {
        psjf_avl_tree_rotate_right(tree, node);
    }

    // If the tree at node is right-heavy, rotate left, and update the height as
    // necessary in one step.
    else if (balanceFactor > 1) {
        psjf_avl_tree_rotate_left(tree, node);
    }

    // If the tree at node is balanced, simply update the height of the node
    else {
        psjf_avl_tree_update_height(node);
    }
}

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node) {

    // If there is no left child, then the height of the left subtree is zero
    if (node->left == 0) {
        return 0;
    }

    // If there is a left child, then return the height of the left subtree
    else {
        return node->left->height;
    }
}

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node) {

    // If there is no right child, then the height of the right subtree is zero
    if (node->right == 0) {
        return 0;
    }

    // If there is a right child, then return the height of the right subtree
    else {
        return node->right->height;
    }
}

struct psjf_avl_tree_deletion_result_inner
    psjf_avl_tree_make_deletion_result_inner(
        struct psjf_avl_tree_deletion_result result,
        struct psjf_avl_tree_node * nodeRemoved)
{
    struct psjf_avl_tree_deletion_result_inner returnVal;

    // Copy the data items that struct psjf_avl_tree_deletion_result and
    // struct psjf_avl_tree_deletion_result_inner share
    returnVal.dataRemoved = result.dataRemoved;
    returnVal.dataToStoreInNode = result.dataToStoreInNode;

    // If there was no data that was removed, but the data in the node was
    // altered, then the status is TREE_DELETION_STATUS_NODE_ALTERED
    if (result.dataRemoved == 0 && result.dataToStoreInNode != 0) {
        returnVal.status = TREE_DELETION_STATUS_NODE_ALTERED;
    }

    // If there was data that was removed, and the data in the node was altered,
    // then the status is TREE_DELETION_STATUS_NODE_REMOVED
    else if (result.dataRemoved != 0 && result.dataToStoreInNode == 0) {
        returnVal.status = TREE_DELETION_STATUS_NODE_REMOVED;
    }

    // If there is any other combination of values, then user did not return a
    // valid set of values, thereby making the status
    // TREE_DELETION_STATUS_FAILURE
    else {
        returnVal.status = TREE_DELETION_STATUS_FAILURE;
    }

    // If there was a node that was removed, store it away
    if (returnVal.status == TREE_DELETION_STATUS_NODE_REMOVED) {
        returnVal.nodeRemoved = nodeRemoved;
    }

    // If there was not a node that was removed, set it to null
    else {
        returnVal.nodeRemoved = 0;
    }

    return returnVal;
}

struct psjf_avl_tree_deletion_result_final
    psjf_avl_tree_make_deletion_result_final(
        struct psjf_avl_tree_deletion_result_inner result)
{
    struct psjf_avl_tree_deletion_result_final returnVal = {
        .dataRemoved = result.dataRemoved,
        .dataToStoreInNode = result.dataToStoreInNode,
        .status = result.status
    };

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
    // If we were not able to find the node to return, return a
    // struct psjf_avl_tree_deletion_result_inner that indicates this failure
    if (node == 0) {
        return psjf_avl_tree_make_deletion_result_inner(
            psjf_avl_tree_make_deletion_result(0, 0), 0);
    }

    // If data is less than the data encapsulated in node, go to the left,
    // update the height node, and return
    else if (compareEncapsulatedNodeData(node->data, data) > 0) {
        struct psjf_avl_tree_deletion_result_inner returnVal =
            psjf_avl_tree_delete_helper(tree, node->left, data,
                compareEncapsulatedNodeData, onNodeFound);

        psjf_avl_tree_update_height(node);

        return returnVal;
    }

    // If data is greater than the data encapsulated in node, go to the left,
    // update the height node, and return
    else if (compareEncapsulatedNodeData(node->data, data) < 0) {
        struct psjf_avl_tree_deletion_result_inner returnVal =
            psjf_avl_tree_delete_helper(tree, node->right, data,
                compareEncapsulatedNodeData, onNodeFound);

        psjf_avl_tree_update_height(node);

        return returnVal;
    }

    // If data is equal to the data encapsulated in node
    else {

        // Use the function pointer onNodeFound in order to consult with the
        // user to determine what should be done, then store the result of this
        // consultation.
        struct psjf_avl_tree_deletion_result_inner result =
            psjf_avl_tree_make_deletion_result_inner(onNodeFound(node->data),
                node);

        // If, according to the consultation, the node should be removed,
        // remove it
        if (result.status == TREE_DELETION_STATUS_NODE_REMOVED) {
            psjf_avl_tree_perform_deletion(tree, node, data,
                compareEncapsulatedNodeData, onNodeFound);
        }

        // If, according to the consultation, the node's contents should be
        // changed, change the contents of the node
        else if (result.status == TREE_DELETION_STATUS_NODE_ALTERED) {
            node->data = result.dataToStoreInNode;
        }

        // Return the result of the consultation
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
    // If an empty spot was found in the tree, make a new node, and link
    // this node up
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

    // If data is equal to the data encapsulated in node, then consult the
    // user to see how the node's contents should be updated
    else if (compareEncapsulatedNodeData(node->data, data) == 0) {
        node->data = onPreviousNodeFound(node->data, data);
        return psjf_avl_tree_make_insertion_result(
            TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND, node);
    }

    // If data is less than the data encapsulated in the node, OR
    // if data is greater than the data encapsulated in the node
    else {
        // The node to go to next
        struct psjf_avl_tree_node * nextNode;

        // If data is less than the data encapsulated in the node,
        // set nextNode to node->left
        if (compareEncapsulatedNodeData(node->data, data) > 0) {
            nextNode = node->left;
        }

        // If data is greater than the data encapsulated in the node,
        // set nextNode to node->right
        else {
            nextNode = node->right;
        }

        // Recursively call psjf_avl_tree_insert_helper, and store the result
        // of the insertion
        struct psjf_avl_tree_insertion_result insertionResult =
            psjf_avl_tree_insert_helper(tree, nextNode, node, data,
                compareEncapsulatedNodeData, onNewNodeCreated,
                onPreviousNodeFound);

        // If a new node was created, then balance the tree at node if
        // necessary, and update the height of the nodes if necessary
        if (insertionResult.status == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
            psjf_avl_tree_balance_tree_if_necessary_at(tree, node);
        }

        // Return the result of the insertion
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
    // If the node to be deleted has no children
    if (node->left == 0 && node->right == 0) {

        // If the node to be deleted is the root, then clear the root out
        if (node == tree->root) {
            tree->root = 0;
        }

        // Remove parent's references to the child that will be deleted
        else if (node->parent->left == node) {
            node->parent->left = 0;
        }
        else {
            node->parent->right = 0;
        }
    }

    // If the node to be deleted has two children
    else if (node->left != 0 && node->right != 0) {

        // If there is no in-order successor
        if (node->left->right == 0 && node->right->left == 0) {

            // If the node to be deleted is the root
            if (node == tree->root) {
                tree->root = node->left;

                // Unnecessary
                //psjf_avl_tree_update_height(tree->root);
            }

            // If the parent's left child is the node to be deleted,
            else if (node->parent->left == node) {
                node->parent->left = node->left;
                node->left->parent = node->parent;
                psjf_avl_tree_update_height(node->parent->left);
            }

            // If the parent's right child is the node to be deleted
            else {
                node->parent->right = node->left;
                node->left->parent = node->parent;
                psjf_avl_tree_update_height(node->parent->right);
            }
        }

        // If the leftmost in-order successor exists, swap the data,
        // recursively call the psjf_avl_tree_delete_helper function, and
        // update the height of node
        else if (node->left->right != 0) {
            node->data = node->left->right->data;
            psjf_avl_tree_delete_helper(tree, node->left->right, data,
                compareEncapsulatedNodeData, onNodeFound);
            psjf_avl_tree_update_height(node);
        }

        // If the rightmost in-order successor exists, swap the data,
        // recursively call the psjf_avl_tree_delete_helper function, and
        // update the height of node
        else {
            node->data = node->right->left->data;
            psjf_avl_tree_delete_helper(tree, node->right->left, data,
                compareEncapsulatedNodeData, onNodeFound);
            psjf_avl_tree_update_height(node);
        }
    }

    // If the node to be deleted has one child on the left
    else if (node->right == 0) {
        // If the node to be deleted is the root, swap the root with its left
        // child, and set the left child's parent to null
        if (node == tree->root) {
            tree->root = node->left;
            node->left->parent = 0;
        }

        // If the left child of node's parent is the node to be deleted
        else if (node->parent->left == node) {
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }

        // If the right child of the node's parent is the node to be deleted
        else {
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }

        // Unnecessary
        // psjf_avl_tree_update_height(node->left);
    }

    // If the node to be deleted has one child on the right
    else {
        // If the node to be deleted is the root, swap the root with its right
        // child, and set the right child's parent to null
        if (node == tree->root) {
            tree->root = node->right;
            node->right->parent = 0;
        }

        // If the left child of the node's parent is the node to be deleted
        else if (node->parent->left == node) {
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }

        // If the right child of the node's parent is the node to be deleted
        else {
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }

        // Unnecessary
        // psjf_avl_tree_update_height(node->right);
    }
}

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node)
{
    struct psjf_avl_tree_node * temp = node->right;

    node->right = node->right->left;
    temp->left = node;

    node->right->parent = node;
    temp->left->parent = temp;

    psjf_avl_tree_update_height(node);
    psjf_avl_tree_update_height(temp);

    if (node == tree->root) {
        tree->root = temp;
        temp->parent = 0; // set the parent of the new root to null
    }
}

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node)
{
    struct psjf_avl_tree_node * temp = node->left;

    node->left = node->left->right;
    temp->right = node;

    node->left->parent = node;
    temp->right->parent = temp;

    psjf_avl_tree_update_height(node);
    psjf_avl_tree_update_height(temp);

    if (node == tree->root) {
        tree->root = temp;
        temp->parent = 0; // set the parent of the new root to null
    }
}

void * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    void * data, int (*compareEncapsulatedNodeData)(void * encapsulatedData,
        void * unencapsulatedData))
{
    // If node is null, return null; we did not find a match
    if (node == 0) {
        return 0;
    }

    // If the data is less than the data encapsulated in node, go to the left
    else if (compareEncapsulatedNodeData(node->data, data) > 0) {
        return psjf_avl_tree_search_helper(node->left, data,
            compareEncapsulatedNodeData);
    }

    // If the data is greater than the data encapsulated in the node, go to
    // the right
    else if (compareEncapsulatedNodeData(node->data, data) < 0) {
        return psjf_avl_tree_search_helper(node->right, data,
            compareEncapsulatedNodeData);
    }

    // If the data is equal to the data encapsulated inside the node
    else {
        return node->data;
    }
}

void psjf_avl_tree_update_height(struct psjf_avl_tree_node * node) {
    node->height = psjf_avl_tree_max(
        psjf_avl_tree_calculate_left_height(node) + 1,
        psjf_avl_tree_calculate_right_height(node) + 1);
}

struct psjf_avl_tree_min_and_max
    psjf_avl_tree_update_min_and_max_if_necessary_after_deletion(
        struct psjf_avl_tree * tree, struct psjf_avl_tree_node * nodeDeleted)
{
    struct psjf_avl_tree_min_and_max returnVal;

    // If the node that was deleted was the smallest value, then we need to
    // update min
    if (nodeDeleted == tree->min) {

        // If the node that was deleted had no children, then the next smallest
        // node is its parent
        if (tree->min->left == 0 && tree->min->right == 0) {
            returnVal.min = tree->min->parent;
        }

        // If the node that was deleted has one child on the right, then the
        // next smallest value its right child
        else {
            returnVal.min = tree->min->right;
        }
    }

    // If the node that was deleted was not the smallest value, then we do not
    // need to update min
    else {
        returnVal.min = tree->min;
    }

    // If the node that was deleted was the largest value, then we need to
    // update max
    if (nodeDeleted == tree->max) {

        // If the node that was deleted has no children, then the next largest
        // node is its parent
        if (tree->max->left == 0 && tree->max->right == 0) {
            returnVal.max = tree->max->parent;
        }

        // If the node that was deleted has one child on the left, then the
        // next largest value is its left child
        else {
            returnVal.max = tree->max->left;
        }
    }

    // If the node that was deleted was not the largest value, then we do not
    // need to update max
    else {
        returnVal.max = tree->max;
    }

    return returnVal;
}
