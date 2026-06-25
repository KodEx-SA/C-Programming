#include <stdio.h>

int main() {
    short a; // var a of type short, which is a data type that can store small integers
    long b; // var b of type long, which is a data type that can store large integers
    long long c; // var c of type long long, which is a data type that can store very large integers
    long double d; // var d of type long double, which is a data type that can store very large floating-point numbers

    // check sizes of the above variables
    printf("Size of short = %zu bytes\n", sizeof(a)); // %zu is the format specifier for size_t type, which is the return type of sizeof operator
    printf("Size of long = %zu bytes\n", sizeof(b));
    printf("Size of long long = %zu bytes\n", sizeof(c));
    printf("Size of long double = %zu bytes\n", sizeof(d));

    int age; // var age of type int, which is a data type that can store integers
    printf("\nSize of int = %d bytes\n", sizeof(age)); // print the size of int data type, which is typically 4 bytes on most systems

    double num; // var num of type double, which is a data type that can store floating-point numbers
    printf("Size of double = %zu bytes\n", sizeof(num)); // print the size of double data type, which is typically 8 bytes on most systems

    float num1; // var num1 of type float, which is a data type that can store floating-point numbers with less precision than double
    printf("Size of float = %zu bytes\n", sizeof(num1)); // print the size of float data type, which is typically 4 bytes on most systems

    return 0;
}