#include <stdio.h>

int main()
{
    int a, b;

    printf("Enter the first number: ");
    scanf("%d", &a); // accept input from the user

    printf("Enter the second number: ");
    scanf("%d", &b);

    printf("================== Outputs ==================\n");
    printf("Result of a / b is: %d\n", a / b);
    printf("Result of a %% b is: %d", a % b);
    
    return 0;
}