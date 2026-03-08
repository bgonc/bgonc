#include <stdio.h>
#include <math.h>

unsigned int randaux()
{
    static long seed = 1;
    return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

float Uniforme()
{
    return (randaux() % 10000 + 1) / 10000.0;
}

int main()
{
    float v[1000], max;
    int i, n;

    scanf("%d", &n);

    for (i = 0; i < 1000; i++)

        v[i] = -log(Uniforme());

    for (i = 0; i < 10; i++)

        printf("%.2f ", v[i]);

    max = v[0];

    for (i = 0; i < n; i++)

    {

        if (v[i] > max)
        {
            max = v[i];
        }
    }
    printf("\n%.2f", max);
}