#ifndef SEQUENCE_H
#define SEQUENCE_H

#define W_MATCH 2
#define W_MISMATCH -1
#define W_GAP -1

#include <stdlib.h>




struct SequenceTag {
    char *content; 
    size_t length;
};

typedef struct SequenceTag Sequence;

extern Sequence get_subsequence(size_t start, size_t size, Sequence seq);
extern size_t get_difference(Sequence first, Sequence second);
extern void drop_sequence (Sequence seq);


#endif
