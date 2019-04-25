#include "./include/macros.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ALLOC_ARRAY2D(ARRAY,TYPE,DIM1,DIM2,INIT) \
({\
    ARRAY = (TYPE **) malloc(sizeof(TYPE*)*DIM1 + DIM1*DIM2*sizeof(TYPE)); \
    for (int p = 0; p < DIM1; p++) { \
         ARRAY[p] = (TYPE*) (ARRAY + DIM1 + p * DIM2);\
         for (int e = 0; e < DIM2; e++) {\
                ARRAY[p][e] = INIT;\
         }\
    }\
})

#define DEL_BLOCK2D(ARRAY) (free(ARRAY))


int main(int argc, char const *argv[])
{
    int **feld;

    int dim1 = 4;
    int dim2 = 5;
    ALLOC_ARRAY2D(feld, int, dim1, dim2, (0xabcdef12));


    for (int i = 0; i < dim1; i++)
        for (int j = 0; j < dim1; j++)
            assert((feld[i][j] == 0xabcdef12));

    DEL_BLOCK2D(feld);
    return 0;
}
