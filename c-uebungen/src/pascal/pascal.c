#include <stdio.h>
#include <stdlib.h>
#include "include/macros.h"

int main(int argc, char const *argv[])
{
   
   int depth = atoi(argv[1]);

   int **table;
   ALLOC_ARRAY2D(table, int, 12, 12, (0x00000000));

    table[0][0] = 1;


    for (int r = 1; r<depth; r++) {
        for (int c = 0; c < r; c++) {
            int first = table[r-1][c];
            int second = (c > 0) ? table[r-1][c-1] : 0;
            int new = first + second;

            table[r][c] = new;
            printf("%d ", new);
        }
        printf("\n");
    }


    return 0;
}




