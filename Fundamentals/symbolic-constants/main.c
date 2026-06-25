#include <stdio.h>

/* 
- %3d is used to format the Fahrenheit value as a 3-digit integer, and 
- %6.1f is used to format the Celsius value as a floating-point number with 1 decimal place and a total width of 6 characters.
- \t is used to insert a tab space between the two values for better readability.
- \n is used to move to the next line after printing each row of the table.
*/

#define LOWER 0 // Define a symbolic constant for the lower limit of table (0deg Fahrenheit)
#define UPPER 300 // Define a symbolic constant for the upper limit of table (300deg Fahrenheit)
#define STEP 20 // Define a symbolic constant for the step size (increments by 20 steps)

int main()
{
    int fahr, celsius;
    printf("Fahrenheit to Celsius conversion table:\n");

    for (fahr = LOWER; fahr <= UPPER; fahr += STEP){
        celsius = (5.0 / 9.0) * (fahr - 32);
        printf("%3d\t%6.1f\n", fahr, celsius); // Print the Fahrenheit and Celsius values in a formatted table
    }
    
    return 0;
}