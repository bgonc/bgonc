#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HORA_REFEICAO 1440
#define MAX_CEL_FAVO 3000
#define MAX_IDADE -57600
#define N_ZANGOES 40

typedef struct
{
    char funcao[4]; // (fax, nut, con, gua, for, zan, rai)
    int idade;
    int ultima_refeicao;
} Abelha;

typedef struct
{
    char tipo[4];   // "mel", "pol", "nec", "cri", "zan"
    int quantidade; // miligramas
} Celula;

typedef struct
{
    int id;
    int total_celulas;
    Celula *celulas;
} Favo;

void imprimir_detalhes(const Abelha *abelhas, int abelhas_totais, const Favo *favos, int nFavos);

void atribuir_abelhas(Abelha *abelhas, int nAbelhas, int *contador_abelhas);
int contar_abelhas(const Abelha *abelhas, int abelhas_totais, int *contador_abelhas);
void simular_temporadas(Abelha *abelhas, int abelhas_totais, Favo *favos, int nFavos, int nTemporadas, int minutos_temporada, int *contador_vazias, int *contador_usadas, int *contador_abelhas);
void atribuir_celulas(Favo *favos, int nFavos, int nCelulas, int *contador_vazias, int *contador_usadas);
void gerar_relatorio(Favo *favos, int nAbelhas, int nFavos, int minutos_atual, int *contador_abelhas, int *contador_vazias, int *contador_usadas);
void conversao_tempo(int minutos, int *dias, int *horas, int *minutos_restantes);
Celula *encontrar_celula(Favo *favos, int nFavos, const char *tipo, int quantidade_necessaria);

void imprimir_detalhes(const Abelha *abelhas, int abelhas_totais, const Favo *favos, int nFavos);

int main()
{
    int nAbelhas, nCelulas, nFavos, nTemporadas, minutos_temporada;
    int contador_vazias[5] = {0};  // [0]=mel, [1]=pol, [2]=nec, [3]=cri, [4]=zan
    int contador_usadas[4] = {0};  // [0]=mel, [1]=pol, [2]=nec, [3]=cri
    int contador_abelhas[7] = {0}; // [0]=fax, [1]=nut, [2]=con, [3]=gua, [4]=for, [5]=zan, [6]=rai
    int abelhas_totais;

    scanf("%d", &nAbelhas);
    scanf("%d", &nCelulas);
    scanf("%d", &minutos_temporada);
    scanf("%d", &nTemporadas);

    nTemporadas = (nTemporadas < 0) ? -nTemporadas : nTemporadas;                 // tornar valor absoluto
    nFavos = (nCelulas > 0) ? ((nCelulas + MAX_CEL_FAVO - 1) / MAX_CEL_FAVO) : 0; // arredondamento
    abelhas_totais = nAbelhas + N_ZANGOES + 1;
    Abelha *abelhas = malloc(abelhas_totais * sizeof(Abelha));
    Favo *favos = malloc(nFavos * sizeof(Favo));

    if (abelhas != NULL)
    {
        atribuir_abelhas(abelhas, nAbelhas, contador_abelhas);
    }
    else
    {
        printf("Erro ao alocar memória p/ abelhas.\n");
        return 1;
    }

    if (favos != NULL)
    {
        atribuir_celulas(favos, nFavos, nCelulas, contador_vazias, contador_usadas);
    }
    else
    {
        free(abelhas);
        printf("Erro ao alocar memória p/ favos.\n");
        return 1;
    }

    gerar_relatorio(favos, nAbelhas, nFavos, 0, contador_abelhas, contador_vazias, contador_usadas);
    //imprimir_detalhes(abelhas, abelhas_totais, favos, nFavos);
    simular_temporadas(abelhas, abelhas_totais, favos, nFavos, nTemporadas, minutos_temporada, contador_vazias, contador_usadas, contador_abelhas);

    free(abelhas);

    for (int f = 0; f < nFavos; f++)
    {
        free(favos[f].celulas);
    }
    free(favos);

    return 0;
}

void atribuir_abelhas(Abelha *abelhas, int nAbelhas, int *contador_abelhas)
{
    const char *funcoes[] = {"fax", "nut", "con", "gua", "for"};
    int nAbelhasDia = nAbelhas / 40;
    nAbelhas = nAbelhasDia * 40;

    for (int dia = 1; dia <= 40; dia++) // abelhas operárias
    {
        int i = (dia - 1) * nAbelhasDia;
        int idade = dia * -1440;

        for (int j = 0; j < nAbelhasDia; j++)
        {
            strcpy(abelhas[i + j].funcao, funcoes[j % 5]);
            abelhas[i + j].idade = idade + j;
            abelhas[i + j].ultima_refeicao = (dia * -1440) / 40;
            contador_abelhas[j % 5]++;
        }
    }

    for (int dia = 1; dia <= 40; dia++) // Zangões
    {
        int i = nAbelhas + (dia - 1);
        strcpy(abelhas[i].funcao, "zan");
        abelhas[i].idade = dia * -1440;
        abelhas[i].ultima_refeicao = abelhas[i].idade / 40;
        contador_abelhas[5]++;
    }

    strcpy(abelhas[nAbelhas + N_ZANGOES].funcao, "rai"); // Rainha
    abelhas[nAbelhas + N_ZANGOES].idade = 0;
    abelhas[nAbelhas + N_ZANGOES].ultima_refeicao = 0;
    contador_abelhas[6]++;

    for (int i = 0; i < nAbelhas + N_ZANGOES + 1; i++)
    {
        for (int j = 0; j < nAbelhas + N_ZANGOES - i; j++)
        {
            if (abelhas[j].idade > abelhas[j + 1].idade)
            {
                Abelha temp = abelhas[j];
                abelhas[j] = abelhas[j + 1];
                abelhas[j + 1] = temp;
            }
        }
    }
}

int contar_abelhas(const Abelha *abelhas, int abelhas_totais, int *contador_abelhas)
{
    const char *funcoes[] = {"fax", "nut", "con", "gua", "for", "zan", "rai"};
    int abelhas_vivas = 0;

    for (int i = 0; i < 7; i++)
    {
        contador_abelhas[i] = 0;
    }

    for (int i = 0; i < abelhas_totais; i++)
    {
        if (abelhas[i].idade > MAX_IDADE)
        {
            abelhas_vivas++;
            for (int j = 0; j < 7; j++) // Procura no array de funções
            {
                if (strcmp(abelhas[i].funcao, funcoes[j]) == 0)
                {
                    contador_abelhas[j]++;
                    break;
                }
            }
        }
    }
    return abelhas_vivas;
}

void atribuir_celulas(Favo *favos, int nFavos, int nCelulas, int *contador_vazias, int *contador_usadas)
{
    const char *tipos[] = {"mel", "pol", "nec", "cri"};
    const int quantidades[] = {500, 20, 40, 0}; // quantidades padrão para cada tipo
    int cel_por_favo = (nCelulas / nFavos);

    for (int f = 0; f < nFavos; f++)
    {
        for (int i = 0; i < 4; i++) // reiniciar contadores
            contador_usadas[i] = 0;

        for (int i = 0; i < 5; i++)
            contador_vazias[i] = 0;

        favos[f].id = f;
        favos[f].total_celulas = cel_por_favo;
        favos[f].celulas = malloc((cel_por_favo + 1) * sizeof(Celula));

        if (favos[f].celulas != NULL)
        {
            strcpy(favos[f].celulas[cel_por_favo].tipo, "zan");
            favos[f].celulas[cel_por_favo].quantidade = 0;
            contador_vazias[4]++;

            for (int i = 0; i < cel_por_favo; i++)
            {
                int tipo = i % 4;
                strcpy(favos[f].celulas[i].tipo, tipos[tipo]);
                favos[f].celulas[i].quantidade = quantidades[tipo];

                if (quantidades[tipo] > 0) // Células usadas
                    contador_usadas[tipo]++;
                else // Células vazias
                    contador_vazias[tipo]++;
            }
        }
        else
        {
            printf("Erro ao alocar memória p/ celulas.\n");
            exit(1);
        }
    }
}

void contar_celulas(Favo *favos, int favo_idx, int *contador_vazias, int *contador_usadas)
{
    const char *tipos[] = {"mel", "pol", "nec", "cri", "zan"};
    const int usados_index[] = {0, 1, 2, 3}; // Índices para células usadas

    // Reiniciar os contadores
    for (int i = 0; i < 5; i++) // [0]=mel, [1]=pol, [2]=nec, [3]=cri, [4]=zan
        contador_vazias[i] = 0;
    for (int i = 0; i < 4; i++) // [0]=mel, [1]=pol, [2]=nec, [3]=cri
        contador_usadas[i] = 0;

    // Contar as células no favo especificado
    for (int c = 0; c < favos[favo_idx].total_celulas; c++)
    {
        for (int t = 0; t < 5; t++) // Verifica o tipo de célula
        {
            if (strcmp(favos[favo_idx].celulas[c].tipo, tipos[t]) == 0)
            {
                if (favos[favo_idx].celulas[c].quantidade == 0) // Célula vazia
                {
                    contador_vazias[t]++;
                }
                else if (t < 4) // Célula usada (somente "mel", "pol", "nec", "cri")
                {
                    contador_usadas[usados_index[t]]++;
                }
                break;
            }
        }
    }
}

void simular_temporadas(Abelha *abelhas, int abelhas_totais, Favo *favos, int nFavos, int nTemporadas, int minutos_temporada, int *contador_vazias, int *contador_usadas, int *contador_abelhas)
{
    int tempo_total = nTemporadas * minutos_temporada;
    int abelhasVivas;
    int diferenca;

    for (int minuto = 1; minuto <= tempo_total; minuto++) // simulação por minuto
    {
        for (int i = 0; i < abelhas_totais; i++) // ajuste da idade
        {
            if (abelhas[i].idade > MAX_IDADE)
            {
                if (strcmp(abelhas[i].funcao, "rai") != 0) // excepto a rainha
                {
                    if ((-abelhas[i].idade) >= 57600) // 40 dias
                    {
                        abelhas[i].idade = MAX_IDADE - 1; // abelha morre
                    }
                    else
                    {
                        abelhas[i].idade -= 1; // envelhecimento
                    }
                }
            }
        }

        for (int i = abelhas_totais - 1; i >= 0; i--) // alimentação
        {
            if (abelhas[i].idade > MAX_IDADE)
            {
                diferenca = minuto - abelhas[i].ultima_refeicao;

                if (diferenca >= HORA_REFEICAO)
                {
                    Celula *cPol = encontrar_celula(favos, nFavos, "pol", 4); // procura de celulas
                    Celula *cMel = encontrar_celula(favos, nFavos, "mel", 10);

                    if (cMel == NULL)
                    {
                        abelhas[i].idade = MAX_IDADE - 1;
                        continue;
                    }
                    if (cPol == NULL)
                    {
                        abelhas[i].idade = MAX_IDADE - 1; // morre de fome
                        continue;
                    }

                    cMel->quantidade -= 10;
                    cPol->quantidade -= 4;

                    if (cMel->quantidade == 0)
                    {
                        contador_usadas[0]--;
                        contador_vazias[0]++;
                    }
                    if (cPol->quantidade == 0)
                    {
                        contador_usadas[1]--;
                        contador_vazias[1]++;
                    }

                    abelhas[i].ultima_refeicao = minuto;
                }
            }
        }

        abelhasVivas = contar_abelhas(abelhas, abelhas_totais, contador_abelhas);

        if (abelhasVivas == 0) // relatório caso não existam abelhas
        {
            gerar_relatorio(favos, 0, nFavos, minuto, contador_abelhas, contador_vazias, contador_usadas);
            break;
        }

        if (minuto % minutos_temporada == 0) // relatório periódico
        {
            gerar_relatorio(favos, abelhas_totais, nFavos, minuto, contador_abelhas, contador_vazias, contador_usadas);
            //imprimir_detalhes(abelhas, abelhas_totais, favos, nFavos);
        }
    }
}

Celula *encontrar_celula(Favo *favos, int nFavos, const char *tipo, int quantidade_necessaria)
{
    int quantidade;
    Celula *escolhida = NULL;

    for (int f = nFavos - 1; f >= 0; f--)
    {
        for (int c = favos[f].total_celulas - 1; c >= 0; c--)
        {
            if (strcmp(favos[f].celulas[c].tipo, tipo) == 0)
            {
                quantidade = favos[f].celulas[c].quantidade;
                if (quantidade >= quantidade_necessaria)
                {
                    // se ainda nao temos escolhida, ou quantidade < escolhida->quantidade
                    if (escolhida == NULL || quantidade < escolhida->quantidade)
                    {
                        escolhida = &favos[f].celulas[c];
                    }
                }
            }
        }
    }
    return escolhida;
}

void gerar_relatorio(Favo *favos, int nAbelhas, int nFavos, int minutos_atual, int *contador_abelhas, int *contador_vazias, int *contador_usadas)
{
    int dias = 0, horas = 0, mins = 0;

    if (minutos_atual > 0)
    {
        conversao_tempo(minutos_atual, &dias, &horas, &mins);
        printf("Relatorio colmeia:");
        if (dias > 0)
            printf(" %d dia(s)", dias);
        if (horas > 0)
            printf(" %d hora(s)", horas);
        if (mins > 0 || (dias == 0 && horas == 0))
            printf(" %d minuto(s)", mins);
        printf("\n");
    }
    else
    {
        printf("Relatorio colmeia:\n");
    }

    printf(" fax nut con gua for zan rai ovo lar pup mel pol nec cri zan rea\n");

    if (nAbelhas < 200) // colmeia vazia
    {
        printf("\n");
    }
    else
    {
        printf((contador_abelhas[0] > 0) ? "%4d" : "    ", contador_abelhas[0]);
        printf((contador_abelhas[1] > 0) ? "%4d" : "    ", contador_abelhas[1]);
        printf((contador_abelhas[2] > 0) ? "%4d" : "    ", contador_abelhas[2]);
        printf((contador_abelhas[3] > 0) ? "%4d" : "    ", contador_abelhas[3]);
        printf((contador_abelhas[4] > 0) ? "%4d" : "    ", contador_abelhas[4]);
        printf((contador_abelhas[5] > 0) ? "%4d" : "    ", contador_abelhas[5]);
        printf((contador_abelhas[6] > 0) ? "%4d" : "    ", contador_abelhas[6]);
        printf("\n");
    }

    for (int f = 0; f < nFavos; f++)
    {
        contar_celulas(favos, f, contador_vazias, contador_usadas);
        printf("    Favo   %d:            celulas vazias:", f);
        // [0]=mel, [1]=pol, [2]=nec, [3]=cri, [4]=zan
        printf((contador_vazias[0] > 0) ? "%4d" : "    ", contador_vazias[0]);
        printf((contador_vazias[1] > 0) ? "%4d" : "    ", contador_vazias[1]);
        printf((contador_vazias[2] > 0) ? "%4d" : "    ", contador_vazias[2]);
        printf((contador_vazias[3] > 0) ? "%4d" : "    ", contador_vazias[3]);
        printf("   1\n");
        // mel=usadas[0], pol=usadas[1], nec=usadas[2], cri=usadas[3]
        printf("                                 usadas:");
        printf((contador_usadas[0] > 0) ? "%4d" : "    ", contador_usadas[0]);
        printf((contador_usadas[1] > 0) ? "%4d" : "    ", contador_usadas[1]);
        printf((contador_usadas[2] > 0) ? "%4d" : "    ", contador_usadas[2]);
        printf("\n");
    }
}

void conversao_tempo(int minutos, int *dias, int *horas, int *minutos_restantes)
{
    *dias = minutos / 1440;
    *horas = (minutos % 1440) / 60;
    *minutos_restantes = minutos % 60;
}

void imprimir_detalhes(const Abelha *abelhas, int abelhas_totais, const Favo *favos, int nFavos) {
    // Imprime os detalhes das abelhas
    printf("Detalhes das Abelhas:\n");
    printf("  ID    Função    Idade   Última Refeição\n");
    for (int i = 0; i < abelhas_totais; i++) {
        printf("%4d   %6s   %6d   %6d\n",
               i, abelhas[i].funcao, abelhas[i].idade, abelhas[i].ultima_refeicao);
    }

    // Imprime os detalhes das células em cada favo
    printf("\nDetalhes das Células:\n");
    for (int f = 0; f < nFavos; f++) {
        printf("Favo %d:\n", favos[f].id);
        printf("  ID    Tipo   Quantidade\n");
        for (int c = 0; c < favos[f].total_celulas + 1; c++) { // Inclui célula "zan"
            printf("%4d   %4s   %6d\n",
                   c, favos[f].celulas[c].tipo, favos[f].celulas[c].quantidade);
        }
    }
}
