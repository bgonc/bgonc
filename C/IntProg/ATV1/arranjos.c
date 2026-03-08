#include <stdio.h>

int main()
{
    int n;
    int r;
    int i;
    int aux;

    printf("Calculo dos arranjos de N, R a R:\n");
    printf("Indique N: ");
    scanf("%d", &n);
    printf("Indique R: ");
    scanf("%d", &r);

    if (r < n && r > 0)
    {

        i = n - r + 1;
        aux = 1;

        while (i <= n)
        {

            aux *= i;
            printf("i=%d; arranjos=%d\n", i, aux);
            i++;
        }
        printf("Resultado=%d\n", aux);
    }
    else

        printf("Erro: N tem de ser maior que R e este maior que 0.");
}