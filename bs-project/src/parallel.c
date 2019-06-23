#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "difference.h"
#include "parallel.h"
#include "sequence.h"


void *compare_fn(void *arg) {

    ThreadContext *context = (ThreadContext*) arg;




    DifferenceList *list = malloc(sizeof(DifferenceList));

    *list = compare_all_to_all(
        context->hyperA,
        context->hyperB,
        context->start,
        CHUNK_SIZE
    );

    printf("chunk with start %d, returns with %ld\n", context->start, list->length);

    return list;

}
extern DifferenceList parallel_compare(
    Sequence seqA, Sequence SeqB, size_t nthreads) {
    
    pthread_t threads[nthreads];
    ThreadContext contexts[nthreads];

    DifferenceList returnList = empty_diff_list();

    size_t hyper_a_size = seqA.length / nthreads;
    
    for (size_t slot = 0; slot < nthreads; slot++) {
        


	printf("creating thread %d\n", slot);
        size_t start = slot*hyper_a_size;

        // expand last one by so much that have been ignored before
        // due to integer rounding
        if (slot == nthreads -1 && nthreads * hyper_a_size < seqA.length) {
            hyper_a_size += (seqA.length - nthreads * hyper_a_size);
        }
        
        // create chunk
        Sequence hyper_a = get_subsequence(
            start,
            hyper_a_size,
            seqA);

        ThreadContext context;

        context.hyperA = hyper_a;
        context.hyperB = SeqB;
        context.start = start;

        contexts[slot] = context;

        pthread_create(&threads[slot], NULL, compare_fn, &contexts[slot]);
    }

    DifferenceList* thread_results[nthreads];

    for (int i = 0; i < nthreads; i++) {
        DifferenceList l;
        
	printf("waiting dor thread %d to end\n", i);

        pthread_join(threads[i], (void**) &thread_results[i]);
        printf("thread %d joined with length %ld\n", i, thread_results[i]->length);
        print_diff_list(*thread_results[i]);
    }

    for (int i = 0; /*i < nthreads*/ 0; i++) {

        DifferenceList list = concat_diff_lists(returnList, *thread_results[i]);
        
        drop_diff_list(returnList);
        drop_diff_list(*thread_results[i]);

        returnList = list;
    }

    return returnList;

}
