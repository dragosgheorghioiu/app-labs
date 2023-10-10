#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

int main()
{
    int a[SIZE], i;
    long sum = 0;

    #pragma omp parallel private(i) shared(a)
    {
        #pragma omp for
        for (i = 0; i < SIZE; i++)
        {
            a[i] = i;
        }
    }
    
    #pragma omp parallel for private(i) shared(a) reduction(+ : sum)
    for (i = 0; i < SIZE; i++)
    {
        sum += a[i];
    }

    printf("Total sum = %ld\n", sum);

    return 0;
}