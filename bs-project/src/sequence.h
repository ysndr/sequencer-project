#ifndef SEQUENCE_H
#define SEQUENCE_H

#define W_MATCH 2
#define W_MISMATCH -1
#define W_GAP -1

#define MIN_DIFF 70

#include <pthread.h>
#include <stdlib.h>
#include "difference.h"



typedef struct {
    char *content; 
    size_t length;
} Sequence;

extern Sequence get_subsequence(size_t start, size_t size, Sequence seq);
extern void drop_sequence (Sequence seq);


extern size_t get_difference(Sequence first, Sequence second);

extern DifferenceList compare_one_to_all(
    Sequence seqA,
    Sequence hyperSeq,
    size_t index_a);

extern DifferenceList compare_all_to_all(
    Sequence hyperA,
    Sequence hyperB, 
    size_t start_a_global, 
    size_t chunkSize);



#endif
