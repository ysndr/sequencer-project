#include <stdlib.h>
#include <stdio.h>
#include "difference.h"
#include "faparser.h"
#include "parallel.h"
#include "sequence.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: sequencer <*.fa file> <*.fa file>");
        return 1;
    }

    char *first_file_content = read_file(argv[1]);
    char *second_file_content = read_file(argv[2]);
    size_t nthreads = atoi(argv[3]);

    printf("will create %ld threads", nthreads);


    Sequence seqA = parse_sequence(second_file_content);
    Sequence seqB = parse_sequence(first_file_content);


    Sequence test = get_subsequence(0, 3, seqA);


    if (seqA.length > seqB.length) {
        fprintf(stderr, "WARNING: First sequence should be the larger of both");
    }
    
    DifferenceList result = parallel_compare(seqA, seqB, nthreads);

    print_diff_list(result);

    drop_sequence(seqA);
    drop_sequence(seqB);

}
