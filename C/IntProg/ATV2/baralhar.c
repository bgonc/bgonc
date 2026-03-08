#include <stdio.h>

unsigned int randaux()
{
    static long seed = 1;
    return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

void baralhar(int identidade[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int j = (randaux() % (n - i)) + i;
        int temp = identidade[i];
        identidade[i] = identidade[j];
        identidade[j] = temp;
    }
}

int main()
{
    int identidade[1000];
    int n;

    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        identidade[i] = i;
    }

    baralhar(identidade, n);

    for (int i = 0; i < 10; i++)
    {
        printf("%d ", identidade[i]);
    }
    printf("\n");

    return 0;
}
