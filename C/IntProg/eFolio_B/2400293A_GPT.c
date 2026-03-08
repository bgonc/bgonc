#include <stdio.h>
#include <stdlib.h>

// Define os diferentes tipos de abelhas na colmeia
typedef enum
{
    FAX, // 0 Faxineiras
    NUT, // 1 Nutrizes
    CON, // 2 Construtoras
    GUA, // 3 Guardiãs
    FOR, // 4 Forrageiras
    ZAN, // 5 Zangões
    RAI  // 6 Rainha
} TipoAbelha;

// Estrutura para armazenar informações de cada abelha
typedef struct
{
    TipoAbelha tipo;
    int tempoNascimento; // Em minutos, negativo indica passado
} Abelha;

// Estrutura principal que representa a colmeia
typedef struct
{
    Abelha *abelhas;
    int numeroAbelhas;
} Colmeia;

//Inicializa a população de abelhas na colmeia
int inicializarAbelhas(Colmeia *colmeia, int N)
{
    // Calcula quantas abelhas operárias por dia de idade
    int abelhasPorDia = N / 40;

    // Se não houver abelhas suficientes para ter 1 de cada tipo por dia
    if (abelhasPorDia < 5)
    {
        colmeia->numeroAbelhas = 0;
        colmeia->abelhas = NULL;
        return 1; // Sucesso, caso válido de colmeia vazia
    }

    // Total = (operárias por dia * 40 dias) + (40 zangões + 1 rainha)
    colmeia->numeroAbelhas = abelhasPorDia * 40 + 41;

    // Tenta alocar memória para as abelhas
    colmeia->abelhas = malloc(sizeof(Abelha) * colmeia->numeroAbelhas);
    if (colmeia->abelhas == NULL)
    {
        colmeia->numeroAbelhas = 0;
        return 0; // Falha na alocação
    }

    int indiceAbelha = 0;

    // Inicializa as abelhas operárias para cada dia de idade
    for (int dia = 1; dia <= 40; dia++)
    {
        // Para cada dia, distribui as abelhas entre os 5 tipos
        for (int b = 0; b < abelhasPorDia; b++)
        {
            // Ciclo entre os tipos: FAX -> NUT -> CON -> GUA -> FOR
            TipoAbelha tipo = b % 5;
            colmeia->abelhas[indiceAbelha].tipo = tipo;
            // Calcula o tempo de nascimento: dias convertidos em minutos (1440 = 24*60)
            colmeia->abelhas[indiceAbelha].tempoNascimento = -(dia * 1440 - b);
            indiceAbelha++;
        }
    }

    // Inicializa os 40 zangões, um para cada dia de idade
    for (int i = 0; i < 40; i++)
    {
        colmeia->abelhas[indiceAbelha].tipo = ZAN;
        colmeia->abelhas[indiceAbelha].tempoNascimento = -(i + 1) * 1440;
        indiceAbelha++;
    }

    // Inicializa a única rainha, com idade 0
    colmeia->abelhas[indiceAbelha].tipo = RAI;
    colmeia->abelhas[indiceAbelha].tempoNascimento = 0;

    return 1;
}

void imprimirRelatorio(Colmeia *colmeia)
{
    int contadorAbelhas[7] = {0};

    // Conta as abelhas vivas (tipo != -1) de cada tipo
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1)
        {
            contadorAbelhas[colmeia->abelhas[i].tipo]++;
        }
    }

    // Imprime o cabeçalho do relatório
    printf("Relatorio colmeia:\n");
    printf(" fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");

    // Imprime a quantidade de cada tipo de abelha
    for (int i = 0; i < 7; i++)
    {
        if (contadorAbelhas[i] > 0)
            printf("%3d ", contadorAbelhas[i]);
        else
            printf("    ");
    }
    printf("\n");

    // Imprime informações sobre o favo (vazio nesta alínea)
    printf("    Favo   0:            celulas vazias:\n");
    printf("                                 usadas:\n");
}

int main()
{
    int nAbelhas, nCelulas, minutos, temporadas;

    // printf("Quantidade de abelhas: ");
    scanf("%d", &nAbelhas);
    // printf("Quantidade de celulas: ");
    scanf("%d", &nCelulas);
    // printf("Minutos por temporada: ");
    scanf("%d", &minutos);
    // printf("Temporadas: ");
    scanf("%d", &temporadas);

    // Inicializa e processa a colmeia
    Colmeia colmeia;
    if (!inicializarAbelhas(&colmeia, nAbelhas))
    {
        return 1; // Erro na inicialização
    }

    imprimirRelatorio(&colmeia);

    // Liberta a memória alocada
    if (colmeia.abelhas)
    {
        free(colmeia.abelhas);
    }

    return 0;
}