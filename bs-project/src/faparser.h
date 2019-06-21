#ifndef FAPARSER_H
#define FAPARSER_H

#include "sequence.h"

extern char *read_file(char *path);
extern Sequence parse_sequence(char *content);

#endif
