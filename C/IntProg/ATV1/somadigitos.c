#include <stdio.h>

int main()

{
    int n;
    int i=0;
    int r;

    printf("Calculo da soma do quadrado dos digitos de um numero: \n");
    printf("Numero: ");
    scanf("%d", &n);
    

    while(n > 0)
    {
        r = n%10;

        i+=r*r;
        
        printf("n=%d; soma=%d\n", n, i);
        
        n = n/10;

    }

    /*    minha solução:
    
    int n;
    int i;
    int soma;

    printf("Calculo da soma do quadrado dos digitos de um numero: \n");
    printf("Numero: ");
    scanf("%d", &n);

    while (0<n)

    {
        i=(n%10)*(n%10);

        soma+=i;

        printf("n=%d; soma=%d\n", n, soma);

        n=n/10;

    }
    printf("Resultado: %d\n", soma);*/

}