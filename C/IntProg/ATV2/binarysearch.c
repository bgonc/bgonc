#include <stdio.h>
#define MAXVECTOR 1000000

unsigned int randaux()
{
    static long seed = 1;
    return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

void Merge(int v[], int inicio, int meio, int fim)
{
    int i = inicio, j = meio + 1, k = 0;
    int temp[fim - inicio + 1];

    while (i <= meio && j <= fim)
    {
        if (v[i] <= v[j])
            temp[k++] = v[i++];
        else
            temp[k++] = v[j++];
    }

    while (i <= meio)
        temp[k++] = v[i++];

    while (j <= fim)
        temp[k++] = v[j++];

    for (i = inicio, k = 0; i <= fim; i++, k++)
        v[i] = temp[k];
}

void MergeSort(int v[], int inicio, int fim)
{
    if (inicio < fim)
    {
        int meio = (inicio + fim) / 2;
        MergeSort(v, inicio, meio);
        MergeSort(v, meio + 1, fim);
        Merge(v, inicio, meio, fim);
    }
}

int binarySearch(int v[], int inicio, int fim, int elemento)
{
    if (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;

        if (v[meio] == elemento)
            return meio;

        if (v[meio] > elemento)
            return binarySearch(v, inicio, meio - 1, elemento);

        return binarySearch(v, meio + 1, fim, elemento);
    }

    return -1;
}

int main()
{
    static int v[MAXVECTOR];
    int n, elemento, posicao;

    // Lê o número aleatório máximo
    scanf("%d", &n);
    n++;

    // Preenche o vetor com números aleatórios
    for (int i = 0; i < MAXVECTOR; i++)
        v[i] = randaux() % n;

    // Ordena o vetor
    MergeSort(v, 0, MAXVECTOR - 1);

    // Lê o elemento a procurar
    scanf("%d", &elemento);

    // Procura o elemento usando busca binária
    posicao = binarySearch(v, 0, MAXVECTOR - 1, elemento);

    // Imprime a posição ou -1 se não encontrado
    printf("%d\n", posicao);

    return 0;
}
