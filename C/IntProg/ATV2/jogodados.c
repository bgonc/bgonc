#include <stdio.h>

unsigned int randaux()
{
    static long seed = 1;
    return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

int jogar(int num_lancamentos)
{
    int pontos = 0, dado_anterior = -1, dado_atual;

    for (int i = 0; i < num_lancamentos; i++)
    {
        dado_atual = (randaux() % 6) + 1;
        printf("%d ", dado_atual);

        if (dado_atual == dado_anterior)
        {
            pontos += dado_atual;
            printf("Pontos: %d\n", -pontos);
            return -pontos;
        }

        pontos += dado_atual;
        dado_anterior = dado_atual;
    }

    printf("Pontos: %d\n", pontos);
    return pontos;
}

int main()
{
    int num_jogos, num_lancamentos, total_pontos = 0;

    scanf("%d", &num_jogos);
    scanf("%d", &num_lancamentos);

    for (int i = 0; i < num_jogos; i++)
    {
        total_pontos += jogar(num_lancamentos);
    }

    printf("%d\n", total_pontos);
    return 0;
}
