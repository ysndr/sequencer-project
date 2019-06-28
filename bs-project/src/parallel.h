#ifndef PARALLEL_H
#define PARALLEL_H

#define CHUNK_SIZE 50

#include "sequence.h"
#include <pthread.h>


typedef enum  {
    IDLE,
    THREAD_DATA,
    THREAD_END
} ResultType;

typedef struct {
    ResultType type;
    size_t thread_id;
    DifferenceList differences;
} Result;


typedef struct {
    Sequence hyperA;
    Sequence hyperB;
    pthread_mutex_t *lock;
    pthread_cond_t *signal;
    Result *result_sink;
    size_t start;
    size_t thread_id;
} ThreadContext;


extern DifferenceList parallel_compare(
    Sequence seqA, Sequence SeqB, size_t nthreads);
#endif
