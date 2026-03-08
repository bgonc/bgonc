#include <stdio.h>
#include <stdlib.h>

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

/* Verifica se a sequência é válida */
int verificar_sequencia(int k, int vetor[], int tamanho)
{
    int soma = 0;
    unsigned long long produto = 1; /* long long para prevenir overflow */
    for (int i = 0; i < tamanho; i++)
    {
        soma += vetor[i];
        produto *= vetor[i];
        if (produto > k)
        { /* Evita overflow */
            produto = k + 1;
        }
    }
    return soma <= k && produto >= k;
}

/* Verifica se a sequência é uma sequência de vitória */
int verificar_vitoria(int k, int vetor[], int tamanho)
{
    int soma_diferencas = 0;
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            soma_diferencas += abs(vetor[i] - vetor[j]);
        }
    }
    return soma_diferencas == k;
}

/* Imprime a sequência atual */
void imprimir_sequencia(int vetor[], int tamanho, char jogador, int ultima)
{
    printf("Sequencia: ");
    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", vetor[i]);
    }
    if (!ultima)
    {
        printf("[Joga %c]\n", jogador);
    }
    else
    {
        printf("\n");
    }
}

int main()
{
    int k, vetor[100], tamanho = 0, num = -1;

    printf("Indique K: ");
    scanf("%d", &k);

    if (validar_K(k))
    {
        printf("Indique uma sequencia de numeros inteiros positivos, terminando com 0: ");
        while (num != 0)
        {
            /* Lê cada número da sequência até encontrar 0 */
            scanf("%d", &num);

            if (num != 0 && tamanho < 100)
            {
                vetor[tamanho++] = num;
            }
        }

        /* Verificar a sequência fornecida */
        if (!verificar_sequencia(k, vetor, tamanho))
        {
            printf("Sequencia invalida\n");
        }
        else if (verificar_vitoria(k, vetor, tamanho))
        {
            printf("Sequencia vitoria\n");
        }
        else
        {
            printf("Sequencia valida\n");
        }
    }
}