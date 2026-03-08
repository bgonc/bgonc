#include <stdio.h>

void trocos(double m)
{
    int v[8] = {200, 100, 50, 20, 10, 5, 2, 1};

    const char *descricao[8] =
        {
            "2 euros", "1 euro", "50 centimos", "20 centimos",
            "10 centimos", "5 centimos", "2 centimos", "1 centimo"};

    int c = (int)(m * 100 + 0.5);

    for (int i = 0; i < 8; i++)
    {
        int moeda = c / v[i];
        if (moeda > 0)
        {
            printf("%s: %d\n", descricao[i], moeda);
            c = c % v[i];
        }
    }
}

int main()
{
    double m;

    scanf("%lf", &m);

    trocos(m);
}
