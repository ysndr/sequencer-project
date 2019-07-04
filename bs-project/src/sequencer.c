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


    Sequence frame = parse_sequence(first_file_content);
    Sequence compare = parse_sequence(second_file_content);

    printf("first sequence: \t%ld elements\n", frame.length);
    printf("second sequence: \t%ld elements\n", compare.length);

    if (frame.length < compare.length) {
        fprintf(stderr, "WARNING: First sequence should be the larger of both");
        return 2;
    }

    DifferenceList result = parallel_compare(frame, compare, nthreads);

    //print_diff_list(result);

    printf("Writing results..");
    fprintf(output_file, "a\t b\t distance\n");
    for (int i = 0; i < result.length; i++) {
        fprintf(output_file, "%ld\t %ld\t %ld\n",
         result.differences[i].index_a,
         result.differences[i].index_b,
         result.differences[i].difference);
    };
    printf("\33[2K\rdone!\n");

    fclose(output_file);
    drop_diff_list(result);
    drop_sequence(frame);
    drop_sequence(compare);

}
