#include <stdio.h>

void main()
{
    long long K, resultado = 0;
    int i = 1;
    int soma = 0;

    scanf("%lld", &K);


    while(soma <= K )
    {
        soma+= i*i;

        if(K % i == 0)
        {
            resultado = i;
        
        }
        
        
        i++;
    }

    printf("%lld", resultado);
}