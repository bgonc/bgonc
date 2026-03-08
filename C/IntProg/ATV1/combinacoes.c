#include <stdio.h>

int main()

{
    int n, r;
    int a;
    int c;
    int d;
    int i;
    int j;

    printf("Calculo das combinacoes de N, R a R:\n");
    printf("Indique N:");
    scanf("%d", &n);
    printf("Indique R:");
    scanf("%d", &r);

    i=n-r+1;
    j=1;
    a=1;

    while(i<=n)
    {
        c=a*i;
        d=c/j;
        
        printf("%d*%d=%d, %d/%d=%d\n", a, i, c, c, j, d );

        a=d;

        j++;
        i++;
    }
    printf("Resultado %d", d);
}