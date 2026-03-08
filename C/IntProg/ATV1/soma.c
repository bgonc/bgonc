#include <stdio.h>

int main()

{
    int n;
    int soma;
    int contagem=1;

    printf("Indique um numero: ");
    scanf("%d", &n);

    while(contagem<=n)
    {
        soma+=contagem;
        printf("adicionar %d, parcial %d\n",contagem, soma);
        contagem++;
    }
}