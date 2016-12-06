#ifndef PSJF_DICTIONARY_H
#define PSJF_DICTIONARY_H

#include <stdlib.h>
#include "PSJF_AVL_Tree.h"

struct psjf_dictionary {
    struct psjf_avl_tree * tree;
};

struct psjf_dictionary_item {
    long * key;
    void * value;
};

long psjf_dictionary_get_length(struct psjf_dictionary * dict);

struct psjf_dictionary_item * psjf_dictionary_get(struct psjf_dictionary * dict, long * key,
    long (*compareKeys)(long * ptr1, long * ptr2));

struct psjf_dictionary * psjf_dictionary_make_dictionary(void);

struct psjf_dictionary_item * psjf_dictionary_set(struct psjf_dictionary * dict, long * key,
    void * value, long (*compareKeys)(long * ptr1, long * ptr2));

struct psjf_dictionary_item * psjf_dictionary_remove(struct psjf_dictionary * dict, long * key,
    long (*compareKeys)(long * ptr1, long * ptr2));

void psjf_dictionary_clear(struct psjf_dictionary * dict);

#endif
