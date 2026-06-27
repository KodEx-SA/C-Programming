#include <stdio.h>

/* getchar -> reads the next input character from a text stream and returns that as it's value
Happens after => c = getchar();
c variable contains the next character of input.
The function putchar - prints a character each time it's called: putchar(c);

File copying
read a char
    while (char is not end-of-file indicator)
        output the char just read
        read a character*/

int main()
{
    int c;
    c = getchar();
    while(c != EOF) {
        putchar(c);
        c = getchar();
    }
    
    return 0;
}