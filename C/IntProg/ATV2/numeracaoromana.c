#include <stdio.h>
#include <string.h>

typedef struct
{
    char *romano;
    int valor;
} SimboloRomano;

SimboloRomano simbolos[] = {
    {"M", 1000}, {"CM", 900}, {"D", 500}, {"CD", 400}, {"C", 100}, {"XC", 90}, {"L", 50}, {"XL", 40}, {"X", 10}, {"IX", 9}, {"V", 5}, {"IV", 4}, {"I", 1}};

#define TOTAL_SIMBOLOS 13

void converterParaRomano(int num, char resultado[])
{
    resultado[0] = '\0'; // Inicializa a string vazia

    for (int i = 0; i < TOTAL_SIMBOLOS; i++)
    {
        while (num >= simbolos[i].valor)
        {
            strcat(resultado, simbolos[i].romano);
            num -= simbolos[i].valor;
        }
    }
}

int converterParaArabe(char romano[])
{
    int total = 0, i = 0;

    while (romano[i] != '\0')
    {
        for (int j = 0; j < TOTAL_SIMBOLOS; j++)
        {
            int len = strlen(simbolos[j].romano);

            if (strncmp(&romano[i], simbolos[j].romano, len) == 0)
            {
                total += simbolos[j].valor;
                i += len;
                break;
            }
        }
    }

    return total;
}

int main()
{
    int numArabe;
    char numRomano[100];

    // Entrada e conversão de árabe para romano
    scanf("%d", &numArabe);
    converterParaRomano(numArabe, numRomano);
    printf("%s\n", numRomano);

    // Entrada e conversão de romano para árabe
    scanf("%s", numRomano);
    printf("%d\n", converterParaArabe(numRomano));

    return 0;
}
