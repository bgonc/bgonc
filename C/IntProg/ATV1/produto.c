#include <stdio.h>

int main()

{
    int n;
    int contador=1;
    int produto=1;

    printf("Calculo do produto dos primeiros N numeros.\n Indique N: ");
    scanf("%d", &n);

    while(contador<=n)
    {
        produto*=contador;
        printf("Factorial(%d)=%d\n", contador, produto);
    contador++;
    }
    printf("Resultado: %d\n", produto);
}