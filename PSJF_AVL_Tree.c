#include <stdlib.h>
#include <stdio.h>

#include "PSJF_AVL_Tree.h"
#include "PSJF_Linked_List.h"
#include "PSJF_Shared.h"

//////////////////////////////////////////////////////////////////////////////
// Function Headers
//////////////////////////////////////////////////////////////////////////////

int psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node);

struct psjf_avl_tree_node * psjf_avl_tree_make_tree_node(
    struct psjf_avl_tree_node * left,
    struct psjf_avl_tree_node * right, long height, void * data);

long psjf_avl_tree_max(long num1, long num2);

void * psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToDelete));

void psjf_avl_tree_in_order_traversal_helper(struct psjf_avl_tree_node * node,
    void (*onNodeTouched)(void *));

int psjf_avl_tree_insert_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToInsert));

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

void psjf_avl_tree_swap_data(struct psjf_avl_tree_node * node1, struct psjf_avl_tree_node * node2);

void * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    void * data, int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToSearchFor));

void psjf_avl_tree_update_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_update_height_down(struct psjf_avl_tree_node * node);

//////////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////////

void * psjf_avl_tree_delete(
    struct psjf_avl_tree * tree,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToDelete))
{
    void * returnVal = psjf_avl_tree_delete_helper(tree, tree->root, 0, data, compareEncapsulatedNodeData);
    
    if (returnVal != 0) {
        tree->numNodes = tree->numNodes - 1;
    }
    
    return returnVal;
}

long psjf_avl_tree_get_num_nodes(struct psjf_avl_tree * tree) {
    return tree->numNodes;
}

void psjf_avl_tree_in_order_traversal(struct psjf_avl_tree * tree,
    void (*onNodeTouched)(void *))
{
    psjf_avl_tree_in_order_traversal_helper(tree->root, onNodeTouched);
}

int psjf_avl_tree_insert(struct psjf_avl_tree * tree, void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData, void * unencapsulatedData))
{
    int returnVal = psjf_avl_tree_insert_helper(tree, tree->root, 0, data, compareEncapsulatedNodeData);
    
    if (returnVal == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
        tree->numNodes = tree->numNodes + 1;
    }
    
    return returnVal;
}

struct psjf_avl_tree * psjf_avl_tree_make_empty_tree(void) {
    struct psjf_avl_tree * emptyTree =
        (struct psjf_avl_tree *) malloc(sizeof(struct psjf_avl_tree));

    emptyTree->numNodes = 0;
    emptyTree->root = 0;

    return emptyTree;
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

int psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent)
{
    long leftHeight = psjf_avl_tree_calculate_left_height(node);
    long rightHeight = psjf_avl_tree_calculate_right_height(node);
    long balanceFactor = -leftHeight + rightHeight;
    long originalHeight = node->height;

    // If the tree at node is left-heavy, rotate right, and update the height as
    // necessary in one step.
    if (balanceFactor < -1) {
        psjf_avl_tree_rotate_right(tree, node, parent);
    }

    // If the tree at node is right-heavy, rotate left, and update the height as
    // necessary in one step.
    else if (balanceFactor > 1) {
        psjf_avl_tree_rotate_left(tree, node, parent);
    }

    // If the tree at node is balanced, simply update the height of the node
    else {
        psjf_avl_tree_update_height(node);
    }
    
    if (node->height == originalHeight) {
        return FALSE;
    }
    else {
        return TRUE;
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

struct psjf_avl_tree_node * psjf_avl_tree_make_tree_node(
    struct psjf_avl_tree_node * left,
    struct psjf_avl_tree_node * right, long height, void * data)
{
    struct psjf_avl_tree_node * newNode =
        (struct psjf_avl_tree_node*) malloc(sizeof(
            struct psjf_avl_tree_node));
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


void * psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * dataInNode, void * dataToDelete))
{
    // If we were not able to find the node to return, return a
    // struct psjf_avl_tree_deletion_result_inner that indicates this failure
    if (node == 0) {
        return 0;
    }

    // If data is less than the data encapsulated in node, go to the left,
    // update the height node, and return
    else if (compareEncapsulatedNodeData(node->data, data) > 0)
    {
        void * returnVal = psjf_avl_tree_delete_helper(tree, node->left, node, data, compareEncapsulatedNodeData);
        psjf_avl_tree_update_height(node);
        return returnVal;
    }

    // If data is greater than the data encapsulated in node, go to the right,
    // update the height node, and return
    else if (compareEncapsulatedNodeData(node->data, data) < 0)
    {
        void * returnVal = psjf_avl_tree_delete_helper(tree, node->right, node, data, compareEncapsulatedNodeData);
        psjf_avl_tree_update_height(node);
        return returnVal;
    }

    // If data is equal to the data encapsulated in node
    else {
        // If there are no children
        if (node->left == 0 && node->right == 0) {
            // If the node that was just deleted the root
            if (node == tree->root) {
                tree->root = 0;
            }
            // If the left child of the parent is the node to delete
            else if (parent->left == node) {
                parent->left = 0;
            }
            // If the right child of the parent is the node to delete
            else {
                parent->right = 0;
            }
            
            void * returnVal = node->data;
            free(node);
            return returnVal;
        }
        // If there are two children
        else if (node->left != 0 && node->right != 0) {
            
            // If the leftmost inorder successor exists, swap data and recursively call
            if (node->left->right != 0) {
                psjf_avl_tree_swap_data(node, node->left->right);
                void * returnVal = psjf_avl_tree_delete_helper(tree, node->left->right, node->left, data, compareEncapsulatedNodeData);
                psjf_avl_tree_update_height(node);
                return returnVal;
            }
            
            // If the rightmost inorder successor exists
            else if (node->right->left != 0) {
                psjf_avl_tree_swap_data(node, node->right->left);
                void * returnVal =
                    psjf_avl_tree_delete_helper(tree, node->right->left, node->right, data, compareEncapsulatedNodeData);
                psjf_avl_tree_update_height(node);
                return returnVal;
            }
            // If there is no inorder successor
            else {
                psjf_avl_tree_swap_data(node, node->left);
                void * returnVal = psjf_avl_tree_delete_helper(tree, node->left, node, data, compareEncapsulatedNodeData);
                psjf_avl_tree_update_height(node);
                return returnVal;
            }
        }
        // If there is only one child on the left, swap data, and recursively call
        else if (node->right == 0) {
            psjf_avl_tree_swap_data(node, node->left);
            void * returnVal = psjf_avl_tree_delete_helper(tree, node->left, node, data, compareEncapsulatedNodeData);
            psjf_avl_tree_update_height(node);
            return returnVal;
        }
        // If there is only one child on the right, swap data and recursively call
        else {
            psjf_avl_tree_swap_data(node, node->right);
            void * returnVal = psjf_avl_tree_delete_helper(tree, node->right, node, data, compareEncapsulatedNodeData);
            psjf_avl_tree_update_height(node);
            return returnVal;
        }
    }
}

void psjf_avl_tree_in_order_traversal_helper(struct psjf_avl_tree_node * node,
    void (*onNodeTouched)(void *))
{
    if (node != 0) {
        printf("  NODE: ");
        onNodeTouched(node->data);
        printf(" (height = %ld)\n", node->height);

        printf("  LEFT: ");
        if (node->left == 0){
            printf("null\n");
        }
        else {
            onNodeTouched(node->left->data);
            printf("\n");
        }

        printf(" RIGHT: ");
        if (node->right == 0) {
            printf("null\n");
        }
        else {
            onNodeTouched(node->right->data);
            printf("\n");
        }

        printf("\n");

        psjf_avl_tree_in_order_traversal_helper(node->left, onNodeTouched);
        psjf_avl_tree_in_order_traversal_helper(node->right, onNodeTouched);
    }
}

int psjf_avl_tree_insert_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    void * data,
    int (*compareEncapsulatedNodeData)(void * encapsulatedData, void * unencapsulatedData))
{
    // If an empty spot was found in the tree, make a new node, and link
    // this node up
    if (node == 0) {
        struct psjf_avl_tree_node * addedNode =
            psjf_avl_tree_make_tree_node(
                0,      // left node
                0,      // right node
                0,      // height
                data    // node data
            );

        // Link the parent (if there is one) to the child we just created
        if (parent != 0) {
            if (compareEncapsulatedNodeData(parent->data, data) > 0) {
                parent->left = addedNode;
            }
            else {
                parent->right = addedNode;
            }
        }
        
        if (tree->root == 0) {
            tree->root = addedNode;
        }

        return TREE_INSERT_STATUS_NEW_NODE_CREATED;
    }

    // If data is equal to the data encapsulated in node, then consult the
    // user to see how the node's contents should be updated
    else if (compareEncapsulatedNodeData(node->data, data) == 0) {
        return TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND;
    }

    // If data is less than the data encapsulated in the node, OR
    // if data is greater than the data encapsulated in the node
    else {
        int insertionResult;

        // If data is less than the data encapsulated in the node,
        // set nextNode to node->left
        if (compareEncapsulatedNodeData(node->data, data) > 0) {
            insertionResult = psjf_avl_tree_insert_helper(tree, node->left, node, data, compareEncapsulatedNodeData);
        }

        // If data is greater than the data encapsulated in the node,
        // set nextNode to node->right
        else {
            insertionResult = psjf_avl_tree_insert_helper(tree, node->right, node, data, compareEncapsulatedNodeData);
        }

        // If a new node was created, then balance the tree at node if
        // necessary, and update the height of the nodes if necessary
        if (insertionResult == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
            psjf_avl_tree_balance_tree_if_necessary_at(tree, node, parent);
        }

        // Return the result of the insertion
        return insertionResult;
    }
}

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent)
{
    struct psjf_avl_tree_node * pivot = node->right;
    
    // Update references
    if (parent != 0 && parent->left == node) {
        parent->left = pivot;
    }
    else if (parent != 0) {
        parent->right = pivot;
    }
    
    node->right = pivot->left;
    pivot->left = node;
    
    // Update height
    psjf_avl_tree_update_height_down(node->left);
    psjf_avl_tree_update_height_down(pivot->right);
    psjf_avl_tree_update_height(node);
    psjf_avl_tree_update_height(pivot);
    
    if (node == tree->root) {
        tree->root = pivot;
    }
}

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent)
{
    struct psjf_avl_tree_node * pivot = node->left;
    
    // Update pointers
    
    if (parent != 0 && parent->left == node) {
        parent->left = pivot;
    }
    else if (parent != 0) {
        parent->right = pivot;
    }
    
    node->left = pivot->right;
    pivot->right = node;
    
    // Update height
    psjf_avl_tree_update_height_down(pivot->left);
    psjf_avl_tree_update_height_down(node->right);
    psjf_avl_tree_update_height_down(node);
    psjf_avl_tree_update_height(pivot);
    
    if (node == tree->root) {
        tree->root = pivot;
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
    if (node->left == 0 && node->right == 0) {
        node->height = 0;
    }
    else {
        node->height = psjf_avl_tree_max(
            psjf_avl_tree_calculate_left_height(node) + 1,
            psjf_avl_tree_calculate_right_height(node) + 1);
    }
}

void psjf_avl_tree_swap_data(struct psjf_avl_tree_node * node1, struct psjf_avl_tree_node * node2) {
    void * temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

long psjf_avl_tree_update_height_down(struct psjf_avl_tree_node * node) {
    if (node == 0) {
        return 0;
    }
    else {
        node->height = psjf_avl_tree_max(psjf_avl_tree_update_height_down(node->left), psjf_avl_tree_update_height_down(node->right));
        return node->height + 1;
    }
}
