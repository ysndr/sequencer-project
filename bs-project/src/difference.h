#ifndef DIFFERENCE_H
#define DIFFERENCE_H

#include <stdlib.h>

typedef struct {
    size_t index_a;
    size_t index_b;
    size_t difference;
} Difference;



typedef struct {
    Difference *differences;
    size_t length;
} DifferenceList;

/**
 * concatenates and thereby cleans the two lists `first` and `other`
 */
extern DifferenceList concat_diff_lists(DifferenceList first, DifferenceList other);

extern DifferenceList empty_diff_list();

extern void drop_diff_list(DifferenceList list);
extern void drop_diff_list_ref(DifferenceList *list);


extern void print_diff(Difference diff);
extern void print_diff_list(DifferenceList diff);



#endif
