#include <stdio.h>

int main() {
    int x, y, step;
    x = 12;
    y = 10;
    step = 0;
    printf("Result of x + y = %d\n", x + y);
    printf("Result of x - y = %d\n", x - y);
    printf("Result of x * y = %d\n", x * y);
    printf("Result of x / y = %d\n", x / y);
    printf("Result of x %% y = %d\n", x % y);
    printf("Step starts from %d\n", ++step);
    printf("Multiple calc: %d\n", x / y + 6 * 5 - 1);
    
    return 0;
}
