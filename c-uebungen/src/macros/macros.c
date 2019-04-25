#include "include/macros.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


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
    puts("success");
    return 0;
}
