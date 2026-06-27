#include <stdio.h>

int main()
{
    int c;
    while (c = getchar() != EOF) {
        putchar(c);
    } /* the while gets a character, assigns it to c, and then tests whether the char was the end-of-file signal.
    If it wasn't, the body of the while is executed, printing the character. The while then repeats.
    When the end of the input is finally reached, the while terminates and so does main.*/
    
    return 0;
}