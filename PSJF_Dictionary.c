#include "PSJF_Dictionary.h"
#include "PSJF_Shared.h"

int (*comparator)(void * keyInDict, void * key);

int compareItemAndKey(void * item, void * key) {
    struct psjf_dictionary_item * itm = (struct psjf_dictionary_item *) item;
    return comparator(itm->key, key);
}

int compareItemAndItem(void * item1, void * item2) {
    struct psjf_dictionary_item * itm1 = (struct psjf_dictionary_item *) item1;
    struct psjf_dictionary_item * itm2 = (struct psjf_dictionary_item *) item2;
    return comparator(itm1->key, itm2->key);
}

struct psjf_dictionary_item * psjf_dictionary_search_helper(
    struct psjf_dictionary * dict, void * key,
    int (*compareKeys)(void * keyInDict, void * key))
{
    comparator = compareKeys;
    void * ptr = psjf_avl_tree_search(dict->tree, key,
        &compareItemAndKey);

    if (ptr == 0) {
        return 0;
    }
    else {
        return (struct psjf_dictionary_item *) ptr;
    }
}

struct psjf_dictionary_item * psjf_dictionary_make_item(void * key,
    void * value)
{
    struct psjf_dictionary_item * item =
        (struct psjf_dictionary_item *)
            myalloc(sizeof(struct psjf_dictionary_item));

    item->key = key;
    item->value = value;

    return item;
}

long psjf_dictionary_get_length(struct psjf_dictionary * dict) {
    return psjf_avl_tree_get_num_nodes(dict->tree);
}

void * psjf_dictionary_get(struct psjf_dictionary * dict, void * key,
    int (*compareKeys)(void * keyInDict, void * key))
{
    struct psjf_dictionary_item * item =
        psjf_dictionary_search_helper(dict, key, compareKeys);

    if (item == 0) {
        return 0;
    }
    else {
        return item->value;
    }
}

struct psjf_dictionary * psjf_dictionary_make_dictionary() {
    struct psjf_dictionary * dict =
        (struct psjf_dictionary *) myalloc(sizeof(struct psjf_dictionary));

    dict->tree = psjf_avl_tree_make_empty_tree();

    return dict;
}

void * psjf_dictionary_set(struct psjf_dictionary * dict, void * key,
    void * value, int (*compareKeys)(void * keyInDict, void * key))
{
    struct psjf_dictionary_item * item =
        psjf_dictionary_search_helper(dict, key, compareKeys);

    if (item == 0) {
        psjf_avl_tree_insert(dict->tree,
            psjf_dictionary_make_item(key, value), &compareItemAndItem);
        return value;
    }
    else {
        void * previous = item->value;
        item->value = value;
        return previous;
    }
}

void * psjf_dictionary_remove(struct psjf_dictionary * dict, void * key,
    int (*compareKeys)(void * keyInDict, void * key))
{
    comparator = compareKeys;
    void * ptr = psjf_avl_tree_delete(dict->tree, key, &compareItemAndKey);

    if (ptr == 0) {
        return 0;
    }
    else {
        struct psjf_dictionary_item * item =
            (struct psjf_dictionary_item *) ptr;

        void * value = item->value;
        free(item);
        return value;
    }
}

void psjf_dictionary_clear(struct psjf_dictionary * dict) {
    psjf_avl_tree_delete_all(dict->tree);
}
