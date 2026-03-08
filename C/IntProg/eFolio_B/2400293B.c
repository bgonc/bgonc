#include <stdio.h>
#include <stdlib.h>

// Função para imprimir o cabeçalho do relatório.
void imprimirCabecalho(void)
{
    printf("Relatorio colmeia:\n");
    printf(" fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");
}

// Função para distribuir as abelhas operárias.
void distribuirOperarias(int totalAbelhas,
                         int *fax, int *nut, int *con, int *gua, int *forr)
{
    int abelhasPorDia = totalAbelhas / 40;
    int cicloCompleto = abelhasPorDia / 5;
    int resto = abelhasPorDia % 5;

    *fax = (cicloCompleto + (resto > 0 ? 1 : 0)) * 40;
    *nut = (cicloCompleto + (resto > 1 ? 1 : 0)) * 40;
    *con = (cicloCompleto + (resto > 2 ? 1 : 0)) * 40;
    *gua = (cicloCompleto + (resto > 3 ? 1 : 0)) * 40;
    *forr = (cicloCompleto + (resto > 4 ? 1 : 0)) * 40;
}

// Função para imprimir a distribuição das abelhas.
void imprimirDistribuicao(int fax, int nut, int con, int gua, int forr,
                          int zangoes, int rainha)
{
    printf("  %d  %d  %d  %d  %d  %d   %d\n",
           fax, nut, con, gua, forr, zangoes, rainha);
}

// Função para verificar se há abelhas suficientes (>= 200).
// Se houver, distribui e imprime. Se não, imprime apenas uma linha em branco.
void verificaNumAbelhas(int nAbelhas)
{
    if (nAbelhas < 200)
    {
        printf("\n");
    }
    else
    {
        int fax = 0, nut = 0, con = 0, gua = 0, forr = 0;
        distribuirOperarias(nAbelhas, &fax, &nut, &con, &gua, &forr);
        imprimirDistribuicao(fax, nut, con, gua, forr, 40, 1);
    }
}

// Calcula quantos favos (ceiling(nCelulas/3000))
int calcularFavos(int nCelulas)
{
    int nFavos = nCelulas / 3000;
    if (nCelulas % 3000 != 0)
    {
        nFavos++;
    }
    return nFavos;
}

// Distribui as células de um favo:
void distribuirCelulasFavo(int totalCelulasFavo,
                           int *mel, int *pol, int *nec, int *cri, int *zan)
{
    *mel = 0;
    *pol = 0;
    *nec = 0;
    *cri = 0;
    *zan = 1; // 1 célula zangão

    for (int i = 0; i < totalCelulasFavo; i++)
    {
        switch (i % 4)
        {
        case 0:
            (*mel)++;
            break;
        case 1:
            (*pol)++;
            break;
        case 2:
            (*nec)++;
            break;
        default:
            (*cri)++;
            break;
        }
    }
}

// Imprime a informação de um único favo
void imprimirFavo(int indice, int mel, int pol, int nec, int cri, int zan)
{
    printf("    Favo   %d:            celulas vazias:", indice);
    if (cri > 0)
    {
        printf("             %d", cri);
    }
    if (zan > 0)
    {
        printf("   %d", zan);
    }
    printf("\n");

    printf("                                 usadas:");
    if (mel > 0 || pol > 0 || nec > 0)
    {
        printf("  %d  %d  %d", mel, pol, nec);
    }
    printf("\n");
}

/// Imprime todos os favos necessários
void imprimirFavos(int nCelulas)
{
    if (nCelulas <= 0)
    {
        return;
    }

    int nFavos = calcularFavos(nCelulas);
    int celulasPorFavo = nCelulas / nFavos;

    for (int i = 0; i < nFavos; i++)
    {
        int mel = 0, pol = 0, nec = 0, cri = 0, zan = 0;
        distribuirCelulasFavo(celulasPorFavo, &mel, &pol, &nec, &cri, &zan);
        imprimirFavo(i, mel, pol, nec, cri, zan);
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

    imprimirCabecalho();

    verificaNumAbelhas(nAbelhas);

    imprimirFavos(nCelulas);

    return 0;
}
