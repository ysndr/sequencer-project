#ifndef PARALLEL_H
#define PARALLEL_H

#define CHUNK_SIZE 50 

#include "sequence.h"


typedef struct ThreadContextTag {
    Sequence hyperA;
    Sequence hyperB;
    size_t start;   
} ThreadContext;

extern DifferenceList parallel_compare(
    Sequence seqA, Sequence SeqB, size_t nthreads);
#endif
