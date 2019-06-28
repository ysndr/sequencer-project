#include <stdlib.h>
#include <stdio.h>
#include "difference.h"
#include "faparser.h"
#include "parallel.h"
#include "sequence.h"

int main(int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: sequencer <*.fa file> <*.fa file> <output_file> <nthreads>");
        return 1;
    }

    char *first_file_content = read_file(argv[1]);
    char *second_file_content = read_file(argv[2]);
    FILE *output_file = fopen(argv[3], "w+");
    size_t nthreads = atoi(argv[4]);

    printf("will create %ld threads\n", nthreads);


    Sequence seqA = parse_sequence(first_file_content);
    Sequence seqB = parse_sequence(second_file_content);

    printf("first sequence: \t%ld elements\n", seqA.length);
    printf("second sequence: \t%ld elements\n", seqB.length);

    if (seqA.length < seqB.length) {
        fprintf(stderr, "WARNING: First sequence should be the larger of both");
        return 2;
    }

    DifferenceList result = parallel_compare(seqA, seqB, nthreads);

    print_diff_list(result);

    printf("Writing results..");
    fprintf(output_file, "a\t b\t distance\n");
    for (int i = 0; i < result.length; i++) {
        fprintf(output_file, "%ld\t %ld\t %ld\n",
         result.differences[i].index_a,
         result.differences[i].index_b,
         result.differences[i].difference);
    };
    printf("\rWrote results!\n");

    fclose(output_file);
    drop_diff_list(result);
    drop_sequence(seqA);
    drop_sequence(seqB);

}
