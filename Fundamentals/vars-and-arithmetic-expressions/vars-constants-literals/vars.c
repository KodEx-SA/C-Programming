#include <stdio.h>

    /* print fahrenheit - celsius table
        for fahr = 0, 20, 30, ........*/

main() {
    int fahr, celsius;
    int lower, upper, step;

    lower = 0; // lower limit of temp table
    upper = 300; // upper limit
    step = 20; // step size

    fahr = lower; // initialize fahr to lower limit
    
    while (fahr <= upper) { // loop until fahr is less than or equal upper limit
        celsius = 5 * (fahr - 32) / 9; // calculate celsius from fahr
        printf("%d\t%d\n", fahr, celsius); // print fahr and celsius with a tab and new line
        fahr = fahr + step; // increment fahr by step size
    }
}