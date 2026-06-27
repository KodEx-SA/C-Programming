#include <stdio.h>

/* 
    int; // <- 4 bytes - Integer values
    double; // <- 8 bytes - store numbers with more precision(precidence)
    float; // <- 4 bytes
    char; // <- 1 byte
*/

int main()
{
    int _integer = 12;
    printf("Integer value = %d\n", _integer);
    printf("Size of int: %d bytes\n", sizeof(int));

    double _double = 12.4523;
    printf("Double value = %.2lf\n", _double);

    // double can also be used to store exponential numbers
    double _dblExpo_num = 2e3; // 2 to the exponent of 3
    printf("Exponent double value = %lf\n", _dblExpo_num);

    float _float = 10.9f; // f -> syntax used in c for float variables
    printf("Float value = %.1f\n", _float);
    printf("Float value = %.2f\n", _float);

    char _char = 'A';
    printf("Value for char is: %c\n", _char);
    
    return 0;
}