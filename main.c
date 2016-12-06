#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "PSJF_Dictionary.h"
#include "PSJF_Shared.h"

#define MIN 1
#define MAX 100000
#define N 10
#define RANDOM(min, max) ((rand() % (max - min + 1)) + min)

int compareInts(void * ptr1, void * ptr2);
void printInt(void * ptr);

void printItem(void * ptr) {
    struct psjf_dictionary_item * item = (struct psjf_dictionary_item *) ptr;
    printf("(");
    printInt(item->key);
    printf(", ");
    printInt(item->value);
    printf(")");
}

int linearSearch(int size, int * array, int target) {
    int i;
    
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

void clearOutArray(int size, int * array) {
    int i;
    
    for (i = 0; i < size; i++) {
        array[i] = 0;
    }
}

int main(int argc, char const *argv[]) {
    srand(time(0));
    struct psjf_dictionary * dict = psjf_dictionary_make_dictionary();
    int randomNumbers[N];
    clearOutArray(N, randomNumbers);
    int i;

    for (i = 0; i < N; i++) {
        int * value = (int *) myalloc(sizeof(int));
        int * key = (int *) myalloc(sizeof(int));
        
        do {
            *key = RANDOM(MIN, MAX);
        } while (linearSearch(N, randomNumbers, *key) == TRUE);
        
        randomNumbers[i] = *key;
        *value = randomNumbers[i] * 2;

        psjf_dictionary_set(dict, key, value, &compareInts);
        printf("1. Added (%d, %d)\n", randomNumbers[i], *value);
        printf("\n");
        psjf_avl_tree_in_order_traversal(dict->tree, &printItem);
        printf("\n");
    }
    
    int * key = (int *) myalloc(sizeof(int));

    for (i = 0; i < N; i++) {
        printf("%d\n", randomNumbers[i]);
        *key = randomNumbers[i];
        int * value = (int *) psjf_dictionary_get(dict, key, &compareInts);
        printf("\n");
        //psjf_avl_tree_in_order_traversal(dict->tree, &printItem);

        if (value == 0) {
            fprintf(stderr, "2. Failure; null pointer...\n");
            return -1;
        }
        else if (randomNumbers[i] * 2 == *value) {
            printf("2. %d passed with a value of %d\n", i, *value);
        }
        else {
            fprintf(stderr, "2. %d failed with a value of %d\n", i, *value);
            return -1;
        }

        value = (int *) myalloc(sizeof(int));
        *value = randomNumbers[i] * 3;

        psjf_dictionary_set(dict, &randomNumbers[i], value, &compareInts);
        printf("\n");
        //psjf_avl_tree_in_order_traversal(dict->tree, &printItem);
    }

    for (i = 0; i < N; i++) {
        
        int * value = (int *) psjf_dictionary_get(dict, &randomNumbers[i], &compareInts);
        printf("\n");
        //psjf_avl_tree_in_order_traversal(dict->tree, &printItem);

        if (value == 0) {
            fprintf(stderr, "3. Failure; null pointer...\n");
            return -1;
        }
        else if (randomNumbers[i] * 3 == *value) {
            printf("3. %d passed with a value of %d\n", i, *value);
        }
        else {
            fprintf(stderr, "3. %d failed with a value of %d\n", i, *value);
            return -1;
        }

        psjf_dictionary_remove(dict, &randomNumbers[i], &compareInts);
        printf("\n");
        //psjf_avl_tree_in_order_traversal(dict->tree, &printItem);
    }

    for (i = 0; i < N; i++) {
        if (psjf_dictionary_get(dict, &randomNumbers[i], &compareInts) == 0) {
            printf("4. %d passed\n", randomNumbers[i]);
        }
        else {
            fprintf(stderr, "4. %d failed\n", randomNumbers[i]);
            return -1;
        }
    }

    psjf_dictionary_clear(dict);
    free(dict);
}

int compareInts(void * ptr1, void * ptr2) {
    int num1 = *((int *) ptr1);
    int num2 = *((int *) ptr2);
    printf("num1 = %d\tnum2 = %d\n", num1, num2);
    return num1 - num2;
}

void printInt(void * ptr) {
    printf("%d", *((int*) ptr));
}

//void printItem(void * ptr) {
//    struct psjf_dictionary_item * item = (struct psjf_dictionary_item *) ptr;
//    printf("(");
//    printInt(item->key);
//    printf(", ");
//    printInt(item->value);
//    printf(")");
//}
