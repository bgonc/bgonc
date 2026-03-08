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

void MergeSort(int v[], int n)
{
    void MergeSortHelper(int v[], int inicio, int fim) // Função auxiliar para recursividade
    {
        if (inicio < fim)
        {
            int meio = (inicio + fim) / 2;
            MergeSortHelper(v, inicio, meio);
            MergeSortHelper(v, meio + 1, fim);
            Merge(v, inicio, meio, fim);
        }
    }

    MergeSortHelper(v, 0, n - 1);
}

int main()
{
    int i, n;
    /* keyword static força a que esta variável seja na realidade global,
    e mantem os valores entre chamadas da função. Neste caso é a função
    main, que não será chamada segunda vez, mas a keyword static permite
    que o compilador utilize outra zona de memória que não o stack. */
    static int v[MAXVECTOR];

    scanf("%d", &n);
    n++;

    for (i = 0; i < MAXVECTOR; i++)
        v[i] = randaux() % n;
    MergeSort(v, MAXVECTOR);
    printf("%d %d %d", v[MAXVECTOR / 4], v[MAXVECTOR / 2], v[3 * MAXVECTOR / 4]);
}