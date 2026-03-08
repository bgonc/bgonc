#include <stdio.h>

void main()
{
    int K, soma=0, i = 1;
    scanf("%d", &K);

    while(i < K)
    {
        if( i % 3 == 0 || i % 5 ==0)
        {     
        soma+=i;
        }
        
        i++;

    }
    printf("%d",soma);
}