#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int year;
    scanf("%d", &year);

    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) {
        printf("not a leap year\n");
        return 1;
    }
    printf("a leap year\n");
    return 0;

}
