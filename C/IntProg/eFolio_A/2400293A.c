#include <stdio.h>

/* Valida se K está entre 2 e 100 */
int validar_K(int k)
{
    if (k < 2 || k > 100)
    {
        printf("K tem de ser entre 2 e 100.\n");
        return 0;
    }
    return 1;
}

int main()
{
    int k;

    printf("Indique K: ");
    scanf("%d", &k);

    if (validar_K(k)) /* Apenas gera a sequência se K for válido */
    {
        printf("Sequencia: %d %d\n", k / 2, k / 2);
    }
}