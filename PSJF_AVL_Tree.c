#include <stdlib.h>
#include <stdio.h>

#include "PSJF_AVL_Tree.h"
#include "PSJF_Linked_List.h"
#include "PSJF_Shared.h"

//////////////////////////////////////////////////////////////////////////////
// Structs
//////////////////////////////////////////////////////////////////////////////

struct psjf_avl_tree_find_min_results {
    struct psjf_avl_tree_node * min;
    struct psjf_avl_tree_node * parent;
};

//////////////////////////////////////////////////////////////////////////////
// Function Headers
//////////////////////////////////////////////////////////////////////////////

int psjf_avl_tree_balance_tree_if_necessary_at(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

long psjf_avl_tree_calculate_left_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_calculate_right_height(struct psjf_avl_tree_node * node);

struct psjf_avl_tree_node * psjf_avl_tree_make_tree_node(
    struct psjf_avl_tree_node * left,
    struct psjf_avl_tree_node * right, long height, struct psjf_dictionary_item * item);

long psjf_avl_tree_max(long num1, long num2);

struct psjf_dictionary_item * psjf_avl_tree_delete_helper(
                                                          struct psjf_avl_tree * tree,
                                                          struct psjf_avl_tree_node * node,
                                                          struct psjf_avl_tree_node * parent,
                                                          long * key,
                                                          long (*compareEncapsulatedNodeData)(struct psjf_dictionary_item * dictionaryItem, long * key));

void psjf_avl_tree_delete_all_helper(struct psjf_avl_tree_node * node);

void psjf_avl_tree_in_order_traversal_helper(struct psjf_avl_tree_node * node,
    void (*onNodeTouched)(struct psjf_dictionary_item * item));

int psjf_avl_tree_insert_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    struct psjf_dictionary_item * item,
    long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key));

void psjf_avl_tree_rotate_left(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

void psjf_avl_tree_rotate_right(struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent);

void psjf_avl_tree_swap_data(struct psjf_avl_tree_node * node1, struct psjf_avl_tree_node * node2);

struct psjf_dictionary_item * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    long * key, long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key));

void psjf_avl_tree_update_height(struct psjf_avl_tree_node * node);

long psjf_avl_tree_update_height_down(struct psjf_avl_tree_node * node);

//////////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////////

struct psjf_dictionary_item * psjf_avl_tree_delete(
    struct psjf_avl_tree * tree,
    long * key,
    long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key))
{
    struct psjf_dictionary_item * returnVal = psjf_avl_tree_delete_helper(tree, tree->root, 0, key, comparator);

    if (returnVal != 0) {
        tree->numNodes = tree->numNodes - 1;
    }

    return returnVal;
}

void psjf_avl_tree_delete_all(struct psjf_avl_tree * tree) {
    psjf_avl_tree_delete_all_helper(tree->root);
    tree->root = 0;
    tree->numNodes = 0;
}

long psjf_avl_tree_get_num_nodes(struct psjf_avl_tree * tree) {
    return tree->numNodes;
}

void psjf_avl_tree_in_order_traversal(struct psjf_avl_tree * tree,
    void (*onNodeTouched)(struct psjf_dictionary_item * item))
{
    psjf_avl_tree_in_order_traversal_helper(tree->root, onNodeTouched);
}

int psjf_avl_tree_insert(struct psjf_avl_tree * tree, struct psjf_dictionary_item * item,
                         long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key))
{
    int returnVal = psjf_avl_tree_insert_helper(tree, tree->root, 0, item, comparator);

    if (returnVal == TREE_INSERT_STATUS_NEW_NODE_CREATED) {
        tree->numNodes = tree->numNodes + 1;
    }

    return returnVal;
}

struct psjf_avl_tree * psjf_avl_tree_make_empty_tree(void) {
    struct psjf_avl_tree * emptyTree =
        (struct psjf_avl_tree *) myalloc(sizeof(struct psjf_avl_tree));

    emptyTree->numNodes = 0;
    emptyTree->root = 0;

    return emptyTree;
}

struct psjf_dictionary_item * psjf_avl_tree_search(struct psjf_avl_tree * tree, long * key,
                                                   long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key))
{
    return psjf_avl_tree_search_helper(tree->root, key, comparator);
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
    struct psjf_avl_tree_node * right, long height, struct psjf_dictionary_item * item)
{
    struct psjf_avl_tree_node * newNode =
        (struct psjf_avl_tree_node*) myalloc(sizeof(
            struct psjf_avl_tree_node));
    newNode->left   = left;
    newNode->right  = right;
    newNode->height = height;
    newNode->data   = item;

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

struct psjf_avl_tree_find_min_results psjf_avl_tree_find_min(struct psjf_avl_tree_node * node, struct psjf_avl_tree_node * parent) {
    if (node->left != 0) {
        return psjf_avl_tree_find_min(node->left, node);
    }
    else {
        struct psjf_avl_tree_find_min_results results = {
            .min = node,
            .parent = parent,
        };
        return results;
    }
}

struct psjf_dictionary_item * psjf_avl_tree_delete_helper(
    struct psjf_avl_tree * tree,
    struct psjf_avl_tree_node * node,
    struct psjf_avl_tree_node * parent,
    long * key,
    long (*comparator)(struct psjf_dictionary_item * dictionaryItem, long * key))
{
    // If we were not able to find the node to return, return a
    // struct psjf_avl_tree_deletion_result_inner that indicates this failure
    if (node == 0) {
        return 0;
    }

    // If data is less than the data encapsulated in node, go to the left,
    // update the height node, and return
    else if (comparator(node->data, key) > 0)
    {
        void * returnVal = psjf_avl_tree_delete_helper(tree, node->left, node, key, comparator);
        psjf_avl_tree_update_height(node);
        return returnVal;
    }

    // If data is greater than the data encapsulated in node, go to the right,
    // update the height node, and return
    else if (comparator(node->data, key) < 0)
    {
        struct psjf_dictionary_item * returnVal = psjf_avl_tree_delete_helper(tree, node->right, node, key, comparator);
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
            struct psjf_avl_tree_find_min_results results = psjf_avl_tree_find_min(node->right, node);
            psjf_avl_tree_swap_data(node, results.min);
            void * returnVal = psjf_avl_tree_delete_helper(tree, results.min, results.parent, key, comparator);
            psjf_avl_tree_update_height_down(node->right);
            return returnVal;
        }
        // If there is only one child on the left
        else if (node->right == 0) {
            if (tree->root == node) {
                tree->root = node->left;
            }
            else if (parent->left == node) {
                parent->left = node->left;
            }
            else {
                parent->right = node->left;
            }
            
            psjf_avl_tree_update_height(node->left);
            return node->data;
        }
        // If there is only one child on the right
        else {
            if (tree->root == node) {
                tree->root = node->right;
            }
            else if (parent->left == node) {
                parent->left = node->right;
            }
            else {
                parent->right = node->right;
            }
            
            psjf_avl_tree_update_height(node->right);
            return node->data;
        }
    }
}

void psjf_avl_tree_in_order_traversal_helper(struct psjf_avl_tree_node * node,
    void (*onNodeTouched)(struct psjf_dictionary_item *))
{
    if (node != 0) {
        if (node->left != 0) {
            onNodeTouched(node->left->data);
        }
        else {
            printf("          ");
        }
        
        printf(" -> ");
        onNodeTouched(node->data);
        printf(" -> ");
        
        if (node->right != 0) {
            onNodeTouched(node->right->data);
        }
        else {
            printf("          ");
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
    struct psjf_dictionary_item * item,
    long (*comparator)(struct psjf_dictionary_item * item, long * key))
{
    // If an empty spot was found in the tree, make a new node, and link
    // this node up
    if (node == 0) {
        struct psjf_avl_tree_node * addedNode =
            psjf_avl_tree_make_tree_node(
                0,      // left node
                0,      // right node
                0,      // height
                item    // node data
            );

        // Link the parent (if there is one) to the child we just created
        if (parent != 0) {
            if (comparator(parent->data, item->key) > 0) {
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
    else if (comparator(node->data, item->key) == 0) {
        return TREE_INSERT_STATUS_PREVIOUS_NODE_FOUND;
    }

    // If data is less than the data encapsulated in the node, OR
    // if data is greater than the data encapsulated in the node
    else {
        int insertionResult;

        // If data is less than the data encapsulated in the node,
        // set nextNode to node->left
        if (comparator(node->data, item->key) > 0) {
            insertionResult = psjf_avl_tree_insert_helper(tree, node->left, node, item, comparator);
        }

        // If data is greater than the data encapsulated in the node,
        // set nextNode to node->right
        else {
            insertionResult = psjf_avl_tree_insert_helper(tree, node->right, node, item, comparator);
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

struct psjf_dictionary_item * psjf_avl_tree_search_helper(struct psjf_avl_tree_node * node,
    long * key, long (*comparator)(struct psjf_dictionary_item * item, long * key))
{
    // If node is null, return null; we did not find a match
    if (node == 0) {
        return 0;
    }

    // If the data is less than the data encapsulated in node, go to the left
    else if (comparator(node->data, key) > 0) {
        void * returnVal = psjf_avl_tree_search_helper(node->left, key, comparator);
        if (returnVal == 0) {
            //printf("Went L after looking at %ld\n", *node->data->key);
        }
        return returnVal;
    }

    // If the data is greater than the data encapsulated in the node, go to
    // the right
    else if (comparator(node->data, key) < 0) {
        void * returnVal = psjf_avl_tree_search_helper(node->right, key, comparator);
        if (returnVal == 0) {
            //printf("Went R after looking at %ld\n", *node->data->key);
        }
        return returnVal;
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

void psjf_avl_tree_delete_all_helper(struct psjf_avl_tree_node * node) {
    if (node != 0) {
        psjf_avl_tree_delete_all_helper(node->left);
        psjf_avl_tree_delete_all_helper(node->right);
        free(node->data);
        free(node);
    }
}
