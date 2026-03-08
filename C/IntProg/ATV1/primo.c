#include <stdio.h>

int main() 
{
    int n, i = 2, menor_divisor = 0;

    printf("Indique N:\n");
    scanf("%d", &n);

    while (i * i <= n) 
    {
        if (n % i == 0) 
        {
            menor_divisor = i;  
            i = n;
        }
        i++;
    }

    if (menor_divisor == 0 && n > 1) 
    {
        menor_divisor = 0;
    }

    printf("%d\n", menor_divisor);

}
