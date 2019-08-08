#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#include "difference.h"
#include "parallel.h"
#include "sequence.h"


void *compare_fn(void *arg) {

    ThreadContext *context = (ThreadContext*) arg;



    #ifdef NO_POSTPROC

    DifferenceList *list = calloc(1, sizeof(DifferenceList));

    *list = compare_all_to_all(
        context->hyperA,
        context->hyperB,
        context->start,
        CHUNK_SIZE
    );
    printf("[thread %ld] chunk with start %ld, returns with %ld\n",
        context->thread_id,
        context->start,
        list->length);
    return list;

    #else
    int chunks = context->hyperA.length - (CHUNK_SIZE-1);

    int count = 0;

    for (int start = 0; start < chunks; start++) {

        count++;


        DifferenceList differences = compare_one_to_all(
            get_subsequence(start, CHUNK_SIZE, context->hyperA),
            context->hyperB,
            context->start + start
        );

        if (differences.length == 0) {
            drop_diff_list(differences);
            continue;
        }
        pthread_mutex_lock( context->lock );
        // printf("locked: %ld", context->thread_id);

        context->result_sink->type = THREAD_DATA;
        context->result_sink->differences = differences;
        context->result_sink->thread_id = context->thread_id;

        //printf( "\33[2K\r[thread %ld] processed %d/%d chunks! Signalling...", context->thread_id, start+1, chunks);
        // printf("signal: %ld", context->thread_id);

        pthread_cond_signal(context->signal);
        pthread_mutex_unlock(context->lock);
    }


    pthread_mutex_lock( context->lock );

    context->result_sink->type = THREAD_END;
    context->result_sink->thread_id = context->thread_id;

    //printf( "\33[2K\r[thread %ld] ending", context->thread_id);

    pthread_cond_signal(context->signal);
    pthread_mutex_unlock(context->lock);
    pthread_exit(NULL);
    #endif
}
extern DifferenceList parallel_compare(
    Sequence s_frame, Sequence s_compare, size_t nthreads) {

    pthread_t threads[nthreads];
    ThreadContext contexts[nthreads];

    DifferenceList returnList = empty_diff_list();
    Result result_sink;
    result_sink.type = IDLE;

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t signal = PTHREAD_COND_INITIALIZER;



    pthread_mutex_lock( &lock );

    for (size_t slot = 0; slot < nthreads; slot++) {
        size_t hyper_a_size = s_frame.length / nthreads;

        size_t start = slot*hyper_a_size;
	    printf("[compare] creating thread %ld starting at %ld\n", slot, start);

        // expand last one by so much that have been ignored before
        // due to integer rounding
        if (slot == nthreads -1) {
            hyper_a_size += (s_frame.length - nthreads * hyper_a_size);
        }
        else if (slot < nthreads -1) {
            hyper_a_size += (CHUNK_SIZE -1);
        }

        // create chunk with extended size from global frame sequence
        Sequence hyper_a = get_subsequence(
            start,
            hyper_a_size,
            s_frame);

        ThreadContext context;

        context.hyperA = hyper_a;
        context.hyperB = s_compare;
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
	printf("\33[2K\r[compare] waiting for thread %d to end\n", i);

        pthread_join(threads[i], (void**) &thread_results[i]);
        printf("\33[2K\r[compare] thread %d joined with length %ld\n", i, thread_results[i]->length);
    }

    for (int i = 0; i < nthreads; i++) {

        DifferenceList list = concat_diff_lists(returnList, *thread_results[i]);

        drop_diff_list(returnList);
        drop_diff_list(*(thread_results[i]));

        returnList = list;
    }
    #else

    // for (int i = 0; i< nthreads; i++) {
    //     pthread_detach(threads[i]);
    // }

    size_t running = nthreads;

    size_t received = 0;

    while(running > 0) {
        
        Result result;

        char *spinner = "\\|/-";

        printf("\n\033[A\33[2K\r[compare] (%ld/%ld) %c",
            running, nthreads, spinner[received % 4]);
        fflush(stdout);

        while(result_sink.type == IDLE) {
            pthread_cond_wait(&signal, &lock);
            result = result_sink;
        }
        
        result_sink.type = IDLE;

        // result changed
        if(result.type == THREAD_END) {
            printf("Thread %ld ended", result.thread_id);
            running--;
            continue;
        }

        received++;

       
        

        Difference max;
        max.difference = 0;

        for (int d = 0; d < result.differences.length; d++) {

            // ouch :D
            if(result.differences.differences[d].difference > max.difference) {
                max = result.differences.differences[d];
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
