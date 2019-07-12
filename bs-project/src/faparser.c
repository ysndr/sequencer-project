#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sequence.h"




extern char *read_file(char *path) {
    FILE *f;
    if ((f = fopen(path, "r")) == NULL) {
        printf("%s could not be opened!", path);
        exit(1);
    }
    
    fseek(f, 0, SEEK_END);
    long items = ftell(f); // last index + 1 for this index' content
    rewind(f);
      
    char *buf = malloc(items * sizeof(char) +1);
    
    fread(buf, sizeof(char), items, f);

    fclose(f);
    
    return buf;
}


extern Sequence parse_sequence(char *content) {

    
    size_t good_len = 0;
    size_t bad_len = 0;
    
    size_t dest = 0;
    size_t orig = 0;

    // jump over first line unconditionally
    orig = strcspn(content, "\n") +1;



    do {
        good_len = strcspn(content + orig, "\nagtcN");
        bad_len = strspn(content + orig + good_len, "\nagtcN");

        
        memmove(content+dest, content + orig, good_len);

        orig = orig + good_len + bad_len;
        dest = dest + good_len;

    } while (good_len != 0 || bad_len != 0);

    Sequence seq;
    seq.content = content;
    seq.length = dest;
  

    return seq;
    
}
