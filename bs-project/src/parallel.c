#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "difference.h"
#include "parallel.h"
#include "sequence.h"


void *compare_fn(void *arg) {

    ThreadContext *context = (ThreadContext*) arg;



    #ifdef NO_POSTPROC

    DifferenceList *list = malloc(sizeof(DifferenceList));

    *list = compare_all_to_all(
        context->hyperA,
        context->hyperB,
        context->start,
        CHUNK_SIZE
    );

    printf("chunk with start %d, returns with %ld\n", context->start, list->length);

    return list;

    #else
    int chunks = context->hyperA.length - CHUNK_SIZE;

    for (int start = 0; start < chunks; start++) {
        DifferenceList differences = compare_one_to_all(
            get_subsequence(start, CHUNK_SIZE, context->hyperA),
            context->hyperB,
            context->start + start
        );

        pthread_mutex_lock( context->lock );

        context->result_sink->type = THREAD_DATA;
        context->result_sink->differences = differences;
        context->result_sink->thread_id = context->thread_id;

        printf( "[thread %ld] processed %d/%d chunks! Signalling...\n", context->thread_id, start+1, chunks);

        pthread_cond_signal(context->signal);
        pthread_mutex_unlock(context->lock);
    }


        pthread_mutex_lock( context->lock );

        context->result_sink->type = THREAD_END;
        context->result_sink->thread_id = context->thread_id;

        printf( "[thread %ld] ending\n", context->thread_id);

        pthread_cond_signal(context->signal);
        pthread_mutex_unlock(context->lock);


    #endif



}
extern DifferenceList parallel_compare(
    Sequence seqA, Sequence SeqB, size_t nthreads) {

    pthread_t threads[nthreads];
    ThreadContext contexts[nthreads];

    DifferenceList returnList = empty_diff_list();

    size_t hyper_a_size = seqA.length / nthreads;

    Result result_sink;
    result_sink.type = IDLE;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t signal = PTHREAD_COND_INITIALIZER;



    for (size_t slot = 0; slot < nthreads; slot++) {

        size_t start = slot*hyper_a_size;
	    printf("[compare] creating thread %ld starting at %ld\n", slot, start);

        // expand last one by so much that have been ignored before
        // due to integer rounding
        if (slot == nthreads -1 && nthreads * hyper_a_size < seqA.length) {
            hyper_a_size += (seqA.length - nthreads * hyper_a_size);
        }
        else {
            hyper_a_size += 49;
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
        context.thread_id = slot;
        context.lock = &lock;
        context.signal = &signal;
        context.result_sink = &result_sink;

        contexts[slot] = context;

        pthread_create(&threads[slot], NULL, compare_fn, &contexts[slot]);
    }


    #ifdef NO_POSTPROC

    DifferenceList* thread_results[nthreads];

    for (int i = 0; i < nthreads; i++) {
        DifferenceList l;

	printf("[compare] waiting for thread %d to end\n", i);

        pthread_join(threads[i], (void**) &thread_results[i]);
        printf("[compare] thread %d joined with length %ld\n", i, thread_results[i]->length);
        //print_diff_list(*thread_results[i]);
    }

    for (int i = 0; i < nthreads; i++) {

        DifferenceList list = concat_diff_lists(returnList, *thread_results[i]);

        drop_diff_list(returnList);
        drop_diff_list(*thread_results[i]);

        returnList = list;
    }
    #else

    pthread_mutex_lock( &lock );
    int running = nthreads;

    while(running > 0) {
        result_sink.type = IDLE;

        while(result_sink.type == IDLE) {
            pthread_cond_wait(&signal, &lock);
        }

        // result changed
        if(result_sink.type == THREAD_END) {
            printf("[compare] Thread %ld ended\n", result_sink.thread_id);
            running--;
            continue;
        }

        if(result_sink.type == THREAD_DATA) {
            printf("[compare] Receiving data from thread %ld\n", result_sink.thread_id);
        }

        if(result_sink.differences.length == 0) {
            printf("[compare] No significant differences, skipping post-processing\n");
            continue;
        }

        printf("[compare] post-processing\n");

        Difference max;
        max.difference = 0;

        for (int d = 0; d < result_sink.differences.length; d++) {

            // ouch :D
            if(result_sink.differences.differences[d].difference > max.difference) {
                max = result_sink.differences.differences[d];
            }
        }

        DifferenceList list;
        DifferenceList element = one_element_list(max);
        list = concat_diff_lists(returnList, element);

        drop_diff_list(returnList);
        drop_diff_list(element);
        returnList = list;
    }

     pthread_mutex_unlock(&lock);

    #endif



    return returnList;

}
