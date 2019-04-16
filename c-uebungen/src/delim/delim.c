#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    char input;
    while (scanf("%c", &input) && input != '\0')
    {

        switch (input)
        {
        case '\t':
        case '\n':
        case ' ':
            break; // ignore char
        default:
            printf("%c", input);
        }
    }
    printf("\n");
    return 0;
}
