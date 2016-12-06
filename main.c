#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "PSJF_Dictionary.h"
#include "PSJF_Shared.h"

#define MIN 1
#define MAX 1000000
#define N 100000
#define RANDOM(min, max) ((rand() % (max - min + 1)) + min)

long comparelongs(long * ptr1, long * ptr2);
void printlong(void * ptr);

void printItem(struct psjf_dictionary_item * item) {
    printlong(item->key);
}

int treeWalkHelper(struct psjf_avl_tree_node * node, long * target) {
    if (node == 0) {
        return FALSE;
    }
    else if (comparelongs(node->data->key, target) == 0) {
        return TRUE;
    }
    else if (treeWalkHelper(node->left, target) == TRUE) {
        return TRUE;
    }
    else if (treeWalkHelper(node->right, target) == TRUE) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int treeWalkHelper2(struct psjf_avl_tree_node * node, long * target) {
    if (node == 0) {
        return FALSE;
    }
    else if (comparelongs(node->data->key, target) == 0) {
        return TRUE;
    }
    else if (comparelongs(node->data->key, target) > 0 && treeWalkHelper2(node->left, target) == TRUE) {
        return TRUE;
    }
    else if (comparelongs(node->data->key, target) < 0 && treeWalkHelper2(node->right, target) == TRUE) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

void treeWalk(struct psjf_avl_tree * tree, int i, long * array) {
    
    psjf_avl_tree_in_order_traversal(tree, &printItem);
    printf("\n");
    int j;
    for (j = i; j < N; j++) {
        if (treeWalkHelper(tree->root, &array[j]) == TRUE && treeWalkHelper2(tree->root, &array[j]) == FALSE) {
            printf("%ld\n", array[i]);
            printf("%ld\n", array[j]);
            printf("We have a problem!\n");
        }
    }
}

long linearSearch(long size, long * array, long target) {
    long i;
    
    for (i = 0; i < size; i++) {
        if (array[i] == target) {
            return TRUE;
        }
        if (array[i] == 0) {
            return FALSE;
        }
    }
    
    return FALSE;
}

void clearOutArray(long size, long * array) {
    long i;
    
    for (i = 0; i < size; i++) {
        array[i] = 0;
    }
}

int main(int argc, char const *argv[]) {
    srand(time(0));
    struct psjf_dictionary * dict = psjf_dictionary_make_dictionary();
    long randomNumbers[N];
    clearOutArray(N, randomNumbers);
    int i;

    for (i = 0; i < N; i++) {
        long * value = (long *) myalloc(sizeof(long));
        long * key = (long *) myalloc(sizeof(long));
        
        do {
            *key = RANDOM(MIN, MAX);
        } while (linearSearch(N, randomNumbers, *key) == TRUE);
        
        randomNumbers[i] = *key;
        *value = randomNumbers[i] * 2;

        psjf_dictionary_set(dict, key, value, &comparelongs);
        
        printf("1. Added (%ld, %ld) to the dictionary\n", *key, *value);
    }
    
    long * key = (long *) myalloc(sizeof(long));

    for (i = 0; i < N; i++) {
        *key = randomNumbers[i];
        long * value = (long *) psjf_dictionary_get(dict, key, &comparelongs);

        if (value == 0) {
            printf("2. %ld failed; null polonger...\n", randomNumbers[i]);
            return -1;
        }
        else if (randomNumbers[i] * 2 == *value) {
            //printf("2. %ld passed with a value of %ld\n", randomNumbers[i], *value);
        }
        else {
            printf("2. %ld failed with a value of %ld\n", randomNumbers[i], *value);
            return -1;
        }

        value = (long *) myalloc(sizeof(long));
        *value = randomNumbers[i] * 3;

        psjf_dictionary_set(dict, key, value, &comparelongs);
    }
    
    for (i = 0; i < N; i++) {
        *key = randomNumbers[i];
        long * value = (long *) psjf_dictionary_get(dict, key, &comparelongs);
        
        if (value == 0) {
            printf("3. %ld failed; null polonger...\n", randomNumbers[i]);
            return -1;
        }
        else if (randomNumbers[i] * 3 == *value) {
            //printf("3. %ld passed with a value of %ld\n", randomNumbers[i], *value);
        }
        else {
            printf("3. %ld failed with a value of %ld\n", randomNumbers[i], *value);
            return -1;
        }
        
        value = (long *) psjf_dictionary_remove(dict, key, &comparelongs);
        
        if (value == 0) {
            printf("4. %ld failed; null polonger...\n", randomNumbers[i]);
            return -1;
        }
        else if (randomNumbers[i] * 3 == *value) {
            //printf("4. %ld passed with a value of %ld\n", randomNumbers[i], *value);
        }
        else {
            printf("4. %ld failed with a value of %ld\n", randomNumbers[i], *value);
            return -1;
        }
    }
    
    for (i = 0; i < N; i++) {
        
        *key = randomNumbers[i];
        
        if (psjf_dictionary_get(dict, key, &comparelongs) == 0) {
            printf("5. %ld passed!\n", *key);
        }
        else {
            printf("5. %ld failed...\n", *key);
            return -1;
        }
    }

    psjf_dictionary_clear(dict);
    free(dict);
    return 0;
}

long comparelongs(long * ptr1, long * ptr2) {
    return *ptr1 - *ptr2;
}

void printlong(void * ptr) {
    printf("%10ld", *((long *) ptr));
}

//void printItem(long ptr) {
//    struct psjf_dictionary_item * item = (struct psjf_dictionary_item *) ptr;
//    printf("(");
//    printlong(item->key);
//    printf(", ");
//    printlong(item->value);
//    printf(")");
//}
