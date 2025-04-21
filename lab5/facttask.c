#include <stdio.h>

// Recursive function to calculate factorial
unsigned long long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

int main() {
    int number;
    printf("Enter a non-negative integer: ");
    scanf("%d", &number);

    if (number < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        unsigned long long result = 0;
        #pragma omp parallel
        {
            #pragma omp task
             result = factorial(number);
            #pragma omp barrier
            #pragma omp single
            {
                #pragma omp taskwait
                printf("Factorial of %d is %llu\n", number, result);
            }
        }
    }

    return 0;
}
