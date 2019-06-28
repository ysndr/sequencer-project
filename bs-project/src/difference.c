#include <stdio.h>
#include "difference.h"




extern DifferenceList concat_diff_lists(DifferenceList first, DifferenceList second) {

    size_t length = first.length + second.length;

    Difference *differences = malloc(sizeof(Difference) * length);

    for (size_t i = 0; i < first.length; i++) {
        differences[i] = first.differences[i];
    }

    for (size_t j = 0; j < second.length; j++) {
        differences[first.length + j] = second.differences[j];
    }

    DifferenceList list;
    list.differences = differences;
    list.length = length;

    return list;
}

extern DifferenceList empty_diff_list() {
    DifferenceList list;
    list.differences = NULL;
    list.length = 0;
    return list;
}

extern DifferenceList one_element_list(Difference difference) {
    DifferenceList list;
    list.differences = malloc(sizeof(Difference) *1);
    list.differences[0] = difference;
    list.length = 1;
    return list;
}

extern void drop_diff_list(DifferenceList list) {
    if (list.differences != NULL) free(list.differences);
}

extern void drop_diff_list_ref(DifferenceList *list) {
     if (list->differences != NULL) free(list->differences);
    free(list);
}


extern void print_diff(Difference diff) {

    printf("{diff: %ld, idx_a: %ld, idx_b: %ld}", diff.difference, diff.index_a, diff.index_b);
}

extern void print_diff_list(DifferenceList list) {
    printf("DiffList: [\n");
    for (size_t i = 0; i < list.length; i++) {
        print_diff(list.differences[i]);
        puts(",");
    }
    printf("]\n");
}
