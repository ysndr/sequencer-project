#include <stdlib.h>
#include <stdio.h>
#include "faparser.h"
#include "sequence.h"

int main(int argc, char *argv[]) {

    char delim[] = " ";
    int numargs;


    if (argc != 3) {
        fprintf(stderr, "Usage: sequencer <*.fa file> <*.fa file>");
        return 1;
    }

    char *first_file_content = read_file(argv[1]);
    char *second_file_content = read_file(argv[2]);


    Sequence seqA = parse_sequence(second_file_content);
    Sequence seqB = parse_sequence(first_file_content);

    size_t difference = get_difference(seqA, seqB);

    printf("difference: %ld", difference);

    drop_sequence(seqA);
    drop_sequence(seqB);
}
