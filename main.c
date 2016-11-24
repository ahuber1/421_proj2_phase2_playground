#include <stdio.h>
#include <stdlib.h>

#include "PSJF_Linked_List.h"
#include "PSJF_Shared.h"

int compareInts(void * ptr1, void * ptr2);
void printInt(void * ptr);

int main(int argc, char const *argv[]) {
    return 0;
}

int compareInts(void * ptr1, void * ptr2) {
    return *((int*) ptr1) - *((int*) ptr2);
}

void printInt(void * ptr) {
    printf("%d", *((int*) ptr));
}
