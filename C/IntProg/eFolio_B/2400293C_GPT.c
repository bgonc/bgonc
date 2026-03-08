#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Define os diferentes tipos de células no favo
typedef enum
{
    MEL,      // Armazena mel (até 500mg)
    POL,      // Armazena pólen (até 20mg)
    NEC,      // Armazena néctar (até 40mg)
    CRI,      // Célula de criação de abelhas operárias
    ZAN_CELL, // Célula de criação de zangões
    REA       // Célula real para criar rainhas
} TipoCelula;

// Estrutura para armazenar informações de cada abelha
typedef struct
{
    TipoAbelha tipo;
    int tempoNascimento;     // Em minutos, negativo indica passado
    int tempoUltimaRefeicao; // Tempo da última alimentação
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

void limparColmeia(Colmeia *colmeia)
{
    if (!colmeia)
        return;

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

    if (colmeia->abelhas)
    {
        free(colmeia->abelhas);
        colmeia->abelhas = NULL;
    }

    colmeia->numeroAbelhas = 0;
    colmeia->numeroFavos = 0;
}

// Inicializa a população de abelhas na colmeia
int inicializarAbelhas(Colmeia *colmeia, int N)
{
    // Calcula quantas abelhas operárias por dia de idade
    int abelhasPorDia = N / 40;
    if (abelhasPorDia < 5)
    {
        colmeia->numeroAbelhas = 0;
        colmeia->abelhas = NULL;
        return 1;
    }

    colmeia->numeroAbelhas = abelhasPorDia * 40 + 41;
    colmeia->abelhas = malloc(sizeof(Abelha) * colmeia->numeroAbelhas);
    if (!colmeia->abelhas)
    {
        colmeia->numeroAbelhas = 0;
        return 0;
    }

    int indiceAbelha = 0;

    // Inicializa operárias para cada dia
    for (int dia = 1; dia <= 40; dia++)
    {
        for (int b = 0; b < abelhasPorDia; b++)
        {
            colmeia->abelhas[indiceAbelha].tipo = b % 5;
            colmeia->abelhas[indiceAbelha].tempoNascimento = -(dia * 1440 - b);
            colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = -(dia * 36);
            indiceAbelha++;
        }
    }

    // Inicializa os 40 zangões
    for (int i = 0; i < 40; i++)
    {
        colmeia->abelhas[indiceAbelha].tipo = ZAN;
        colmeia->abelhas[indiceAbelha].tempoNascimento = -(i + 1) * 1440;
        colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = -((i + 1) * 36);
        indiceAbelha++;
    }

    // Inicializa a rainha
    colmeia->abelhas[indiceAbelha].tipo = RAI;
    colmeia->abelhas[indiceAbelha].tempoNascimento = 0;
    colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = 0;

    return 1;
}

// Inicializa as células e favos da colmeia
int inicializarCelulas(Colmeia *colmeia, int W)
{
    if (W <= 0)
    {
        colmeia->numeroFavos = 0;
        colmeia->favos = NULL;
        return 1;
    }

    colmeia->numeroFavos = (W + 2999) / 3000;
    int celulasPorFavo = W / colmeia->numeroFavos;

    colmeia->favos = calloc(colmeia->numeroFavos, sizeof(Favo));
    if (!colmeia->favos)
    {
        colmeia->numeroFavos = 0;
        return 0;
    }

    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        colmeia->favos[i].numeroCelulas = celulasPorFavo + 1;
        colmeia->favos[i].celulas = calloc(celulasPorFavo + 1, sizeof(Celula));
        if (!colmeia->favos[i].celulas)
        {
            limparColmeia(colmeia);
            return 0;
        }

        // Primeira célula é sempre ZAN
        colmeia->favos[i].celulas[0].tipo = ZAN_CELL;
        colmeia->favos[i].celulas[0].estado = VAZIA;

        int celulaBase = celulasPorFavo / 4;
        int celulaExtra = celulasPorFavo % 4;
        int indice = 1;

        // Distribui células de MEL
        int celulasMel = celulaBase + (celulaExtra > 0 ? 1 : 0);
        for (int j = 0; j < celulasMel; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = MEL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 500;
            indice++;
        }

        // Distribui células de POL
        int celulasPol = celulaBase + (celulaExtra > 1 ? 1 : 0);
        for (int j = 0; j < celulasPol; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = POL;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 20;
            indice++;
        }

        // Distribui células de NEC
        int celulasNec = celulaBase + (celulaExtra > 2 ? 1 : 0);
        for (int j = 0; j < celulasNec; j++)
        {
            colmeia->favos[i].celulas[indice].tipo = NEC;
            colmeia->favos[i].celulas[indice].estado = USADA;
            colmeia->favos[i].celulas[indice].conteudo = 40;
            indice++;
        }

        // Restante são células de CRI
        while (indice < colmeia->favos[i].numeroCelulas)
        {
            colmeia->favos[i].celulas[indice].tipo = CRI;
            colmeia->favos[i].celulas[indice].estado = VAZIA;
            indice++;
        }
    }
    return 1;
}

// Encontra a célula com menor quantidade do tipo especificado que não esteja vazia
Celula *encontrarCelulaAlimento(Colmeia *colmeia, TipoCelula tipoAlimento)
{
    int conteudoMinimo = 999999;

    // Primeiro encontra o menor valor não nulo
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == tipoAlimento &&
                celula->estado == USADA &&
                celula->conteudo > 0)
            {
                if (celula->conteudo < conteudoMinimo)
                {
                    conteudoMinimo = celula->conteudo;
                }
            }
        }
    }

    // Retorna a primeira célula com o valor mínimo
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == tipoAlimento &&
                celula->estado == USADA &&
                celula->conteudo > 0 &&
                celula->conteudo == conteudoMinimo)
            {
                return celula;
            }
        }
    }
    return NULL;
}

// Verifica se uma abelha deve morrer por idade
int devemorrerPorIdade(Abelha *abelha, int tempoAtual)
{
    if (abelha->tipo == RAI)
        return 0;

    int idadeEmMinutos = tempoAtual - abelha->tempoNascimento;
    return idadeEmMinutos >= 57600; // 40 dias em minutos
}

// Tenta alimentar uma abelha
int alimentarAbelha(Colmeia *colmeia, Abelha *abelha, int tempoAtual)
{
    // Se não precisa comer ainda, retorna OK
    if (tempoAtual - abelha->tempoUltimaRefeicao < 1440)
        return 1;

    // Verifica se tem abelhas mais prioritárias para comer
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1 &&
            tempoAtual - colmeia->abelhas[i].tempoUltimaRefeicao >= 1440)
        {
            // Abelhas mais novas têm prioridade
            if (colmeia->abelhas[i].tempoNascimento < abelha->tempoNascimento)
            {
                return 1;
            }

            // Se mesma idade, ordem por tipo
            if (colmeia->abelhas[i].tempoNascimento == abelha->tempoNascimento &&
                colmeia->abelhas[i].tipo < abelha->tipo)
            {
                return 1;
            }
        }
    }

    Celula *celulaPolena = encontrarCelulaAlimento(colmeia, POL);
    Celula *celulaMel = encontrarCelulaAlimento(colmeia, MEL);

    if (!celulaPolena || !celulaMel ||
        celulaPolena->conteudo < 4 ||
        celulaMel->conteudo < 10)
        return 0;

    celulaPolena->conteudo -= 4;
    if (celulaPolena->conteudo == 0)
        celulaPolena->estado = VAZIA;

    celulaMel->conteudo -= 10;
    if (celulaMel->conteudo == 0)
        celulaMel->estado = VAZIA;

    abelha->tempoUltimaRefeicao = tempoAtual;
    return 1;
}

// Imprime o estado atual da colmeia com o tempo decorrido
void imprimirRelatorio(Colmeia *colmeia, int tempoAtual)
{
    int contadorAbelhas[7] = {0};

    // Conta as abelhas vivas por tipo
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1)
        {
            contadorAbelhas[colmeia->abelhas[i].tipo]++;
        }
    }

    // Imprime cabeçalho com tempo decorrido
    printf("Relatorio colmeia:");
    if (tempoAtual > 0)
    {
        int dias = tempoAtual / 1440;
        int horas = (tempoAtual % 1440) / 60;
        int minutos = tempoAtual % 60;

        if (dias > 0 && horas > 0)
            printf(" %d dia(s) %d hora(s)", dias, horas);
        else if (dias > 0)
            printf(" %d dia(s)", dias);
        else if (horas > 0)
            printf(" %d hora(s)", horas);

        if (minutos > 0 || (dias == 0 && horas == 0))
        {
            if (dias > 0 || horas > 0)
                printf(" %d minuto(s)", minutos);
            else
                printf(" %d minuto(s)", minutos);
        }
    }
    printf("\n fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");

    // Imprime quantidade de cada tipo de abelha
    for (int i = 0; i < 7; i++)
    {
        if (contadorAbelhas[i] > 0)
            printf("%3d ", contadorAbelhas[i]);
        else
            printf("    ");
    }
    printf("\n");

    // Imprime informações dos favos
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
        printf("\n");

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

// Simula a colmeia pelo tempo especificado
void simularColmeia(Colmeia *colmeia, int minutos, int temporadas)
{
    int tempoAtual = 0;
    imprimirRelatorio(colmeia, tempoAtual);

    int relatorioPendente = abs(temporadas);

    while (relatorioPendente > 0 && tempoAtual < 100000)
    {
        int mortesOcorreram = 0;

        for (int m = tempoAtual + 1; m <= tempoAtual + minutos; m++)
        {
            // Primeiro verificamos mortes por idade
            for (int i = 0; i < colmeia->numeroAbelhas; i++)
            {
                if (colmeia->abelhas[i].tipo == -1)
                    continue;

                if (devemorrerPorIdade(&colmeia->abelhas[i], m))
                {
                    colmeia->abelhas[i].tipo = -1;
                    mortesOcorreram = 1;
                }
            }

            // Lista de abelhas que precisam comer
            int precisamComer[5000] = {0}; // indice das abelhas que precisam comer
            int numPrecisamComer = 0;

            for (int i = 0; i < colmeia->numeroAbelhas; i++)
            {
                if (colmeia->abelhas[i].tipo != -1 &&
                    m - colmeia->abelhas[i].tempoUltimaRefeicao >= 1440)
                {
                    precisamComer[numPrecisamComer++] = i;
                }
            }

            // Ordena por prioridade (mais novas primeiro)
            for (int i = 0; i < numPrecisamComer - 1; i++)
            {
                for (int j = 0; j < numPrecisamComer - i - 1; j++)
                {
                    Abelha *a1 = &colmeia->abelhas[precisamComer[j]];
                    Abelha *a2 = &colmeia->abelhas[precisamComer[j + 1]];

                    if (a1->tempoNascimento < a2->tempoNascimento ||
                        (a1->tempoNascimento == a2->tempoNascimento &&
                         a1->tipo > a2->tipo))
                    {
                        int temp = precisamComer[j];
                        precisamComer[j] = precisamComer[j + 1];
                        precisamComer[j + 1] = temp;
                    }
                }
            }

            // Alimenta na ordem correta
            for (int i = 0; i < numPrecisamComer; i++)
            {
                Abelha *abelha = &colmeia->abelhas[precisamComer[i]];

                Celula *celulaPolena = encontrarCelulaAlimento(colmeia, POL);
                Celula *celulaMel = encontrarCelulaAlimento(colmeia, MEL);

                if (!celulaPolena || !celulaMel ||
                    celulaPolena->conteudo < 4 ||
                    celulaMel->conteudo < 10)
                {
                    // Sem comida, morre
                    abelha->tipo = -1;
                    mortesOcorreram = 1;
                }
                else
                {
                    // Come
                    celulaPolena->conteudo -= 4;
                    if (celulaPolena->conteudo == 0)
                        celulaPolena->estado = VAZIA;

                    celulaMel->conteudo -= 10;
                    if (celulaMel->conteudo == 0)
                        celulaMel->estado = VAZIA;

                    abelha->tempoUltimaRefeicao = m;
                }
            }

            // Se todas morreram, termina
            int abelhasVivas = 0;
            for (int i = 0; i < colmeia->numeroAbelhas; i++)
            {
                if (colmeia->abelhas[i].tipo != -1)
                    abelhasVivas++;
            }

            if (abelhasVivas == 0)
            {
                imprimirRelatorio(colmeia, m);
                return;
            }
        }

        tempoAtual += minutos;
        imprimirRelatorio(colmeia, tempoAtual);
        relatorioPendente--;
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

    simularColmeia(&colmeia, minutos, temporadas);

    limparColmeia(&colmeia);

    return 0;
}