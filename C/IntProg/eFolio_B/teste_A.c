#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    FAX,
    NUT,
    CON,
    GUA,
    FOR,
    ZAN,
    RAI
} TipoAbelha;

typedef struct
{
    TipoAbelha tipo;
    int tempoNascimento;
} Abelha;

typedef struct
{
    Abelha *abelhas;
    int numeroAbelhas;
} Colmeia;

void inicializarAbelhas(Colmeia *colmeia, int N)
{
    int abelhasPorDia = N / 40;
    if (abelhasPorDia < 5)
    {
        colmeia->numeroAbelhas = 0;
        colmeia->abelhas = NULL;
        return;
    }

    colmeia->numeroAbelhas = abelhasPorDia * 40 + 41;
    colmeia->abelhas = malloc(sizeof(Abelha) * colmeia->numeroAbelhas);

    int indiceAbelha = 0;

    // Inicializa operárias
    for (int dia = 1; dia <= 40; dia++)
    {
        for (int b = 0; b < abelhasPorDia; b++)
        {
            TipoAbelha tipo = b % 5; // FAX, NUT, CON, GUA, FOR
            colmeia->abelhas[indiceAbelha].tipo = tipo;
            colmeia->abelhas[indiceAbelha].tempoNascimento = -(dia * 1440 - b);
            indiceAbelha++;
        }
    }

    // Inicializa zangões
    for (int i = 0; i < 40; i++)
    {
        colmeia->abelhas[indiceAbelha].tipo = ZAN;
        colmeia->abelhas[indiceAbelha].tempoNascimento = -(i + 1) * 1440;
        indiceAbelha++;
    }

    // Inicializa rainha
    colmeia->abelhas[indiceAbelha].tipo = RAI;
    colmeia->abelhas[indiceAbelha].tempoNascimento = 0;
}

void imprimirRelatorio(Colmeia *colmeia)
{
    int contadorAbelhas[7] = {0};

    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1)
        {
            contadorAbelhas[colmeia->abelhas[i].tipo]++;
        }
    }

    printf("Relatorio colmeia:\n");
    printf(" fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");

    for (int i = 0; i < 7; i++)
    {
        if (contadorAbelhas[i] > 0)
            printf("%3d ", contadorAbelhas[i]);
        else
            printf("    ");
    }
    printf("\n");
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

    Colmeia colmeia;
    inicializarAbelhas(&colmeia, nAbelhas);
    imprimirRelatorio(&colmeia);

    if (colmeia.abelhas)
        free(colmeia.abelhas);

    return 0;
}