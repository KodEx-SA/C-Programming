#include <stdio.h>

int main()
{
    int fahr, celsius;
    celsius = (5.0 / 9.0) * (fahr - 32);

    for (fahr = 0; fahr <= 300; fahr += 20) {
        printf("%3d %6.1f\n", fahr, (celsius));
    }
    
    return 0;
}