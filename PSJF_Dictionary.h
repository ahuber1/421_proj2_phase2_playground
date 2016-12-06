#ifndef PSJF_DICTIONARY_H
#define PSJF_DICTIONARY_H

#include <stdlib.h>
#include "PSJF_AVL_Tree.h"

struct psjf_dictionary {
    struct psjf_avl_tree * tree;
};

struct psjf_dictionary_item {
    void * key;
    void * value;
};

long psjf_dictionary_get_length(struct psjf_dictionary * dict);

void * psjf_dictionary_get(struct psjf_dictionary * dict, void * key,
    int (*compareKeys)(void * keyInDict, void * key));

struct psjf_dictionary * psjf_dictionary_make_dictionary(void);

void * psjf_dictionary_set(struct psjf_dictionary * dict, void * key,
    void * value, int (*compareKeys)(void * keyInDict, void * key));

void * psjf_dictionary_remove(struct psjf_dictionary * dict, void * key,
    int (*compareKeys)(void * keyInDict, void * key));

void psjf_dictionary_clear(struct psjf_dictionary * dict);

#endif
