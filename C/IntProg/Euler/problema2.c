#include <stdio.h>

void main()
{
    int K;
    int soma = 0;
    int fib1 = 0;
    int fib2 = 1;
    int i = 0;

    scanf("%d", &K);

    while(fib2 < K)
    {
        
        i = fib1 + fib2;

        if(fib2 % 2 == 0)
        {
            soma+=fib2;
        }

        fib1 = fib2;
        fib2 = i;

    }

    printf("%d",soma);
}