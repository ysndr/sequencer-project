#include <stdlib.h>
#include <stdio.h>
#include "sequence.h"


extern Sequence get_subsequence(size_t start, size_t size, Sequence seq) {

    Sequence subseq;
    subseq.length = 0;
    subseq.content = NULL;

    if (start > seq.length) return subseq;

    subseq.content = seq.content + start;
    subseq.length = size;

    size_t overflow = start + size - seq.length; 

    if (overflow > 0) {
        subseq.length = size - overflow;
    }

    return subseq;

}

extern size_t get_difference(Sequence first, Sequence second) {

    size_t ancestors_top[second.length +1];
    size_t values[second.length+1];
    values[0] = 0;
    size_t max = 0;

    for (int j = 0; j < second.length +1; j++) {
        ancestors_top[j] = 0;
    }

    
    for (int i = 0; i < first.length; i++) {
        
        for (int j = 0; j < second.length; j++) {

            int value = 0;

            int matching = ancestors_top[j]
                + ((first.content[i] == second.content[j]) 
                ? W_MATCH 
                : W_MISMATCH);
            
            int insertion = values[j] + W_GAP;
            int deletion = ancestors_top[j+1] + W_GAP;

            value = (matching > value) ? matching : value;
            value = (insertion > value) ? insertion : value;
            value = (deletion > value) ? deletion : value;

            max = (value > max) ? value : max;

            values[j+1] = value;
        }

        for (int j = 0; j < second.length +1; j++) {
            ancestors_top[j] = values[j];
        }
    }

    return max;
}

extern void drop_sequence (Sequence seq) {
    free(seq.content);
} 
