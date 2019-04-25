#include <stdio.h>
#include <stdlib.h>
#include "include/macros.h"

int main(int argc, char const *argv[])
{
   
    int depth = atoi(argv[1]);

    for (int n = 0; n < depth; n++) {
        for (int k = 0; k <= n; k++) {
            printf("%d ", over(n,k));
        }
        printf("\n");
    }

    return 0;
}


int over(int n, int k) {
    return fak(n)/(fak(k)*fak(n-k));
}

int fak(int k) {
    int result = 1;
    for (int i = 1; i <= k; i++) {
        result = result*i;
    }
    return result;
}
