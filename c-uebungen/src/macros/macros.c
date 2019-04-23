#include "./include/macros.h"

#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ARRAY2D(ARRAY,TYPE,DIM1,DIM2,INIT) \
({\
    int *#ARRAY;\
    #ARRAY = malloc(sizeof(TYPE*)*DIM1 + DIM1*DIM2*sizeof(TYPE));\
})
    // for (int p = 0; p < DIM1; p++) { \
    //     ARRAY[p] = ARRAY + DIM1 * sizeof(TYPE*) + p * (DIM2) * sizeof(TYPE);\
    //     for (int e = 0; e < DIM2; e++) {\
    //         *ARRAY[p][e] = INIT;\
    //     }\
    // }\


int main(int argc, char const *argv[])
{
    
    // ALLOC_ARRAY2D(test,int,3,4,100);

    int *feld = malloc(sizeof(int*)*4 + 4*3*sizeof(int));

    return 0;
}
