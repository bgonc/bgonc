#include <stdio.h>

int main()

{
    int n;
    int soma;
    int i=3;
    int fib1=1;
    int fib2=2;

    printf("Calculo do valor da funcao Fibonacci:\nIndique N: ");
    scanf("%d", &n);

    if(n==1 || n==2)
        printf("Resultado: %d\n", n);

    else
    {

        while(i<=n)
        {
        soma=fib1+fib2;

        printf("Fib(%d)=%d\n", i, soma);

        fib1=fib2;
        fib2=soma;

        i++;
        
        }
    printf("Resultado: %d\n", soma);
    }
}