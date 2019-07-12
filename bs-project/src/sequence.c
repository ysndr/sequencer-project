#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "difference.h"
#include "sequence.h"


extern Sequence get_subsequence(size_t start, size_t size, Sequence seq) {

    Sequence subseq;
    subseq.length = 0;
    subseq.content = NULL;

    if (start > seq.length) return subseq;

    subseq.content = seq.content + start;
    subseq.length = size;

    ssize_t overflow = start + size - seq.length;

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
            // early return if its not possible to get a difference above Threshold
            size_t max_diag = (second.length - j < first.length -i) ? (first.length -i) : (second.length -j);
            if ((max + W_MATCH * ( max_diag ) ) < MIN_DIFF) {
               return 0;
            }

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
/**
 * slides a window `s_window` of bases over another global series `s_global`
 * compares the window to an equally sized chunk on the global series
 * the results of all comparisons are written to a DifferenceList and returned
 */
extern DifferenceList compare_one_to_all(
    Sequence s_window,
    Sequence s_global,
    size_t index_window) {

    int total_comparisons = s_global.length - s_window.length;

    DifferenceList list;
    list.differences = malloc(sizeof(Difference) * total_comparisons);
    list.length = 0;

    // compare to all frames of seqB
    for (int comparison = 0;
        comparison < total_comparisons;
        comparison++) {
        size_t difference = get_difference(
            s_window,
            get_subsequence(comparison, s_window.length, s_global));

        // add to list if difference above threshold
        if (difference >= MIN_DIFF) {
            Difference diff_s;
            diff_s.difference = difference;
            diff_s.index_b = comparison;
            diff_s.index_a = index_window;
            list.differences[list.length] = diff_s;
            list.length++;
        }
    }

    return list;
}


/**
 * slides a window across a frame thereby using each window
 * to compare to the other global series base by base
 */
extern DifferenceList compare_all_to_all(
    Sequence s_frame,
    Sequence s_global,
    size_t start_frame_global,
    size_t chunk_size) {

    DifferenceList list;
    list.length = 0;


    for (int start = 0; start < s_frame.length - chunk_size; start++) {
        DifferenceList differences = compare_one_to_all(
            get_subsequence(start, chunk_size, s_frame),
            s_global,
            start_frame_global + start
        );

        DifferenceList new_list = concat_diff_lists(list, differences);
        drop_diff_list(list);
        drop_diff_list(differences);
        list = new_list;
    }

    return list;
}



extern void drop_sequence (Sequence seq) {
    free(seq.content);
}
