#include <stdio.h>

/* highest precedence vs lowest precedence */

int main()
{
    int a = 2, b = 3, c = 4, d = 5;

    printf("Result of a * b / c = %d\n", a*b/c); // left to right
    printf("Result of a + b - c = %d\n", a+b-c); // left to right
    printf("Result of a + b * d - c %% a = %d", a+b*d-c%a); // right to left
    
    return 0;
}