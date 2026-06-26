#include <stdio.h>

/* print fahrenheit - celsius table (temperature conversion)
    for fahr = 0, 20, ....., 300 */

/* for the while loop
Formula: c = (f - 32) * 32
Reason for multiplying by 5 & dividing by 9 instead of multiplying by 5/9:::
In C, Integer division truncates: any fractional part is discarded.
since 5 & 9 are integers, 5/9 would be truncated by 0 resulting in all temps reporting as 0 */

int main()
{
    double fahr, celsius;
    int lower, upper, step;

    lower = 0;   // start at 0deg Fahr
    upper = 300; // upper limit - stop at 300deg Fahr
    step = 20;   // step size

    fahr = lower; // fahr starts at 0
    while (fahr <= upper){
        // celsius = 5 * (fahr - 32) / 9;
        // printf("%d\t%d\n", fahr, celsius);
        
        celsius = (5.0 / 9.0) * (fahr - 32.0);
        printf("%3.0f\t%6.1f\n", fahr, celsius);
        
        fahr += step;
    }

    return 0;
}