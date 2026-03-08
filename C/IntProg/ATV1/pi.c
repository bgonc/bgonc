#include <stdio.h> 
#include <math.h>

int main() 
{
    int K, k = 0;
    double soma = 0.0;
    double fator = (2 * sqrt(2)) / 9801;
    double PI;

    scanf("%d", &K);

    while (k <= K) 
    {
        double fatorial_4k = 1, fatorial_k = 1;
        int i = 1;

        while (i <= 4 * k) 
        {
            fatorial_4k *= i;
            i++;
        }

        i = 1;
        while (i <= k) 
        {
            fatorial_k *= i;
            i++;
        }

        double numerador = fatorial_4k * (1103 + 26390 * k);
        double denominador = pow(fatorial_k, 4) * pow(396, 4 * k);
        soma += numerador / denominador;

        k++;
    }

    PI = 1 / (fator * soma);

    printf("%.16f\n", PI);
}
