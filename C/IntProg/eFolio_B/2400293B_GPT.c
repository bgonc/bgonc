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

// Define os estados possíveis de uma célula
typedef enum
{
    VAZIA,
    USADA
} EstadoCelula;

// Tipos de células e suas funções
typedef enum
{
    MEL,      // Armazena mel (500mg)
    POL,      // Armazena pólen (20mg)
    NEC,      // Armazena néctar (40mg)
    CRI,      // Criação de operárias
    ZAN_CELL, // Criação de zangões
    REA       // Criação de rainhas
} TipoCelula;

// Estrutura para armazenar informações de cada abelha
typedef struct
{
    TipoAbelha tipo;
    int tempoNascimento; // Em minutos, negativo indica passado
    int tempoUltimaRefeicao;
} Abelha;

// Estrutura para representar uma célula do favo
typedef struct
{
    TipoCelula tipo;
    EstadoCelula estado;
    int conteudo; // Quantidade em miligramas para células de armazenamento
} Celula;

// Estrutura para representar um favo
typedef struct
{
    Celula *celulas;
    int numeroCelulas;
} Favo;

// Estrutura principal que representa a colmeia
typedef struct
{
    Abelha *abelhas;
    int numeroAbelhas;
    Favo *favos;
    int numeroFavos;
} Colmeia;

// Inicializa a população de abelhas na colmeia
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
        for (int b = 0; b < abelhasPorDia; b++)
        {
            TipoAbelha tipo = b % 5;
            colmeia->abelhas[indiceAbelha].tipo = tipo;
            colmeia->abelhas[indiceAbelha].tempoNascimento = -(dia * 1440 - b);
            colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao =
                -(dia * 24 * 60) / 40; // Última refeição baseada na idade
            indiceAbelha++;
        }
    }

    // Inicializa os 40 zangões, um para cada dia de idade
    for (int i = 0; i < 40; i++)
    {
        colmeia->abelhas[indiceAbelha].tipo = ZAN;
        colmeia->abelhas[indiceAbelha].tempoNascimento = -(i + 1) * 1440;
        colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao =
            -((i + 1) * 24 * 60) / 40;
        indiceAbelha++;
    }

    // Inicializa a única rainha
    colmeia->abelhas[indiceAbelha].tipo = RAI;
    colmeia->abelhas[indiceAbelha].tempoNascimento = 0;
    colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = 0;

    return 1; // Sucesso
}

// Inicializa as células e favos da colmeia
int inicializarCelulas(Colmeia *colmeia, int W)
{
    if (W <= 0)
    {
        colmeia->numeroFavos = 0;
        colmeia->favos = NULL;
        return 1; // Sucesso - caso válido sem células
    }

    // Calcula número de favos necessários (arredondamento para cima)
    colmeia->numeroFavos = (W + 2999) / 3000;

    // Distribui células igualmente entre os favos
    int celulasPorFavo = W / colmeia->numeroFavos;

    // Aloca array de favos
    colmeia->favos = calloc(colmeia->numeroFavos, sizeof(Favo));
    if (!colmeia->favos)
    {
        colmeia->numeroFavos = 0;
        return 0;
    }

    // Inicializa cada favo
    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        // +1 para a célula de zangão
        colmeia->favos[i].numeroCelulas = celulasPorFavo + 1;
        colmeia->favos[i].celulas = calloc(celulasPorFavo + 1, sizeof(Celula));

        if (!colmeia->favos[i].celulas)
        {
            // Liberta memória já alocada em caso de falha
            for (int j = 0; j < i; j++)
            {
                free(colmeia->favos[j].celulas);
            }
            free(colmeia->favos);
            colmeia->favos = NULL;
            colmeia->numeroFavos = 0;
            return 0;
        }

        // Primeira célula é sempre ZAN
        colmeia->favos[i].celulas[0].tipo = ZAN_CELL;
        colmeia->favos[i].celulas[0].estado = VAZIA;
        colmeia->favos[i].celulas[0].conteudo = 0;

        // Distribui os outros tipos de células
        int celulaBase = celulasPorFavo / 4;
        int celulaExtra = celulasPorFavo % 4;
        int indice = 1;

        // Inicializa células de mel
        int celulasMel = celulaBase + (celulaExtra > 0 ? 1 : 0);
        for (int j = 0; j < celulasMel; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = MEL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 500;
            indice++;
        }

        // Inicializa células de pólen
        int celulasPol = celulaBase + (celulaExtra > 1 ? 1 : 0);
        for (int j = 0; j < celulasPol; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = POL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 20;
            indice++;
        }

        // Inicializa células de néctar
        int celulasNec = celulaBase + (celulaExtra > 2 ? 1 : 0);
        for (int j = 0; j < celulasNec; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = NEC;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 40;
            indice++;
        }

        // Restante são células de cria
        while (indice < colmeia->favos[i].numeroCelulas)
        {
            colmeia->favos[i].celulas[indice].tipo = CRI;
            colmeia->favos[i].celulas[indice].estado = VAZIA;
            colmeia->favos[i].celulas[indice].conteudo = 0;
            indice++;
        }
    }

    return 1;
}

// Libera toda a memória alocada para a colmeia
void limparColmeia(Colmeia *colmeia)
{
    if (!colmeia)
        return;

    // Libera a memória dos favos
    if (colmeia->favos)
    {
        for (int i = 0; i < colmeia->numeroFavos; i++)
        {
            free(colmeia->favos[i].celulas);
            colmeia->favos[i].celulas = NULL;
        }
        free(colmeia->favos);
        colmeia->favos = NULL;
    }

    // Liberta a memória das abelhas
    free(colmeia->abelhas);
    colmeia->abelhas = NULL;

    // Zera os contadores
    colmeia->numeroAbelhas = 0;
    colmeia->numeroFavos = 0;
}

void imprimirRelatorio(Colmeia *colmeia)
{
    int contadorAbelhas[7] = {0};

    // Conta as abelhas vivas de cada tipo
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1)
        {
            contadorAbelhas[colmeia->abelhas[i].tipo]++;
        }
    }

    // Imprime cabeçalho do relatório
    printf("Relatorio colmeia:\n");
    printf(" fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");

    // Imprime contagem de abelhas
    for (int i = 0; i < 7; i++)
    {
        if (contadorAbelhas[i] > 0)
            printf("%3d ", contadorAbelhas[i]);
        else
            printf("    ");
    }
    printf("\n");

    // Imprime informações de cada favo
    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        int vazias[6] = {0}, usadas[6] = {0};

        // Conta células vazias e usadas por tipo
        for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->estado == VAZIA)
                vazias[celula->tipo]++;
            else
                usadas[celula->tipo]++;
        }

        // Imprime células vazias
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
        printf("\n");

        // Imprime células usadas
        printf("                                 usadas:");
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

    // Inicializa a colmeia
    Colmeia colmeia;
    if (!inicializarAbelhas(&colmeia, nAbelhas))
    {
        return 1; // Erro na inicialização
    }

    if (!inicializarCelulas(&colmeia, nCelulas))
    {
        free(colmeia.abelhas); // Limpa memória já alocada
        return 1;
    }

    imprimirRelatorio(&colmeia);

    // Liberta toda a memória alocada
    limparColmeia(&colmeia);

    return 0;
}