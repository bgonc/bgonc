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

typedef enum
{
    VAZIA,
    USADA
} EstadoCelula;

typedef enum
{
    MEL,
    POL,
    NEC,
    CRI,
    ZAN_CELL,
    REA
} TipoCelula;

typedef struct
{
    TipoAbelha tipo;
    int tempoNascimento;
    int tempoUltimaRefeicao;
} Abelha;

typedef struct
{
    TipoCelula tipo;
    EstadoCelula estado;
    int conteudo;
} Celula;

typedef struct
{
    Celula *celulas;
    int numeroCelulas;
} Favo;

typedef struct
{
    Abelha *abelhas;
    int numeroAbelhas;
    Favo *favos;
    int numeroFavos;
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

void inicializarCelulas(Colmeia *colmeia, int W)
{
    if (W <= 0)
        return;

    colmeia->numeroFavos = (W + 2999) / 3000;
    int celulasPorFavo = W / colmeia->numeroFavos;

    colmeia->favos = calloc(colmeia->numeroFavos, sizeof(Favo));
    if (!colmeia->favos)
        return;

    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        colmeia->favos[i].numeroCelulas = celulasPorFavo + 1; // +1 para ZAN
        colmeia->favos[i].celulas = calloc(celulasPorFavo + 1, sizeof(Celula));
        if (!colmeia->favos[i].celulas)
        {
            // Liberar memória já alocada
            for (int j = 0; j < i; j++)
            {
                free(colmeia->favos[j].celulas);
            }
            free(colmeia->favos);
            return;
        }

        // Primeira célula é sempre ZAN
        colmeia->favos[i].celulas[0].tipo = ZAN_CELL;
        colmeia->favos[i].celulas[0].estado = VAZIA;

        // Calcula quantidade base por tipo (excluindo ZAN)
        int celulaBase = celulasPorFavo / 4;
        int celulaExtra = celulasPorFavo % 4;
        int indice = 1;

        // MEL
        int celulasMel = celulaBase + (celulaExtra > 0 ? 1 : 0);
        for (int j = 0; j < celulasMel; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = MEL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 500;
            indice++;
        }

        // POL
        int celulasPol = celulaBase + (celulaExtra > 1 ? 1 : 0);
        for (int j = 0; j < celulasPol; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = POL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 20;
            indice++;
        }

        // NEC
        int celulasnec = celulaBase + (celulaExtra > 2 ? 1 : 0);
        for (int j = 0; j < celulasnec; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = NEC;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 40;
            indice++;
        }

        // CRI
        while (indice < colmeia->favos[i].numeroCelulas)
        {
            colmeia->favos[i].celulas[indice].tipo = CRI;
            colmeia->favos[i].celulas[indice].estado = VAZIA;
            indice++;
        }
    }
}

void limparColmeia(Colmeia *colmeia)
{
    if (colmeia)
    {
        if (colmeia->favos)
        {
            for (int i = 0; i < colmeia->numeroFavos; i++)
            {
                if (colmeia->favos[i].celulas)
                {
                    free(colmeia->favos[i].celulas);
                }
            }
            free(colmeia->favos);
        }
        if (colmeia->abelhas)
        {
            free(colmeia->abelhas);
        }
    }
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

    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        int vazias[6] = {0}, usadas[6] = {0};

        for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->estado == VAZIA)
                vazias[celula->tipo]++;
            else
                usadas[celula->tipo]++;
        }

        printf("    Favo   %d:            celulas vazias:", i);

        if (vazias[MEL] > 0)
            printf("%4d", vazias[MEL]);
        else
            printf("    ");
        if (vazias[POL] > 0)
            printf("%4d", vazias[POL]);
        else
            printf("    ");
        if (vazias[NEC] > 0)
            printf("%4d", vazias[NEC]);
        else
            printf("    ");
        if (vazias[CRI] > 0)
            printf("%4d", vazias[CRI]);
        if (vazias[ZAN_CELL] > 0)
            printf("%4d", vazias[ZAN_CELL]);
        printf("\n                                 usadas:");

        if (usadas[MEL] > 0)
            printf("%4d", usadas[MEL]);
        else
            printf("    ");
        if (usadas[POL] > 0)
            printf("%4d", usadas[POL]);
        else
            printf("    ");
        if (usadas[NEC] > 0)
            printf("%4d", usadas[NEC]);
        else
            printf("    ");
        printf("\n");
    }
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
    inicializarCelulas(&colmeia, nCelulas);

    imprimirRelatorio(&colmeia);

    limparColmeia(&colmeia);

    return 0;
}