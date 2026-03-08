#include <stdio.h>

void trocos(double m)
{
    // Vetor com os valores das moedas em centimos
    int v[8] = {200, 100, 50, 20, 10, 5, 2, 1};

    // Vetor com as descrições das moedas
    const char *descricao[8] =
        {
            "2 euros", "1 euro", "50 centimos", "20 centimos",
            "10 centimos", "5 centimos", "2 centimos", "1 centimo"};

    // Converte o montante para centimos
    int c = (int)(m * 100 + 0.5);

    // Itera pelas moedas
    for (int i = 0; i < 8; i++)
    {
        int moeda = c / v[i]; // Calcula a quantidade de moedas
        if (moeda > 0)
        {
            // Imprime a descrição da moeda e a quantidade
            printf("%s: %d\n", descricao[i], moeda);
            c = c % v[i]; // Atualiza o montante restante
        }
    }
}

int main()
{
    double m;

    // Lê o montante
    printf("Introduza um montante em euros, podendo ter centimos: ");
    scanf("%lf", &m);

    // Chama a função para calcular os trocos
    trocos(m);

    return 0;
}

