#include "PSJF_Dictionary.h"
#include "PSJF_Shared.h"

long (*comparator)(long * ptr1, long * ptr2);

long compareItemAndKey(struct psjf_dictionary_item * item, long * key) {
    return comparator(item->key, key);
}

struct psjf_dictionary_item * psjf_dictionary_search_helper(
    struct psjf_dictionary * dict, long * key,
    long (*compareKeys)(long * ptr1, long * ptr2))
{
    comparator = compareKeys;
    struct psjf_dictionary_item * ptr = psjf_avl_tree_search(dict->tree, key, &compareItemAndKey);

    if (ptr == 0) {
        return 0;
    }
    else {
        return (struct psjf_dictionary_item *) ptr;
    }
}

struct psjf_dictionary_item * psjf_dictionary_make_item(long * key, void * value)
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

struct psjf_dictionary_item * psjf_dictionary_get(struct psjf_dictionary * dict, long * key,
    long (*compareKeys)(long * ptr1, long * ptr2))
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

struct psjf_dictionary_item * psjf_dictionary_set(struct psjf_dictionary * dict, long * key,
    void * value, long (*compareKeys)(long * ptr1, long * ptr2))
{
    struct psjf_dictionary_item * item =
        psjf_dictionary_search_helper(dict, key, compareKeys);

    if (item == 0) {
        psjf_avl_tree_insert(dict->tree, psjf_dictionary_make_item(key, value), &compareItemAndKey);
        return value;
    }
    else {
        struct psjf_dictionary_item * previous = item->value;
        item->value = value;
        return previous;
    }
}

struct psjf_dictionary_item * psjf_dictionary_remove(struct psjf_dictionary * dict, long * key,
    long (*compareKeys)(long * ptr1, long * ptr2))
{
    comparator = compareKeys;
    struct psjf_dictionary_item * ptr = psjf_avl_tree_delete(dict->tree, key, &compareItemAndKey);

    if (ptr == 0) {
        return 0;
    }
    else {
        struct psjf_dictionary_item * item =
            (struct psjf_dictionary_item *) ptr;

        struct psjf_dictionary_item * value = item->value;
        free(item);
        return value;
    }
}

void psjf_dictionary_clear(struct psjf_dictionary * dict) {
    psjf_avl_tree_delete_all(dict->tree);
}
