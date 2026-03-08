#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    FAX, // 0
    NUT, // 1
    CON, // 2
    GUA, // 3
    FOR, // 4
    ZAN, // 5
    RAI  // 6
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
    int conteudo; // Para células de alimento: quantidade. Para células de cria: tempo do ovo
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
            colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = -(dia * 36);
            indiceAbelha++;
        }
    }

    // Inicializa zangões
    for (int i = 0; i < 40; i++)
    {
        colmeia->abelhas[indiceAbelha].tipo = ZAN;
        colmeia->abelhas[indiceAbelha].tempoNascimento = -(i + 1) * 1440;
        colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = -((i + 1) * 36);
        indiceAbelha++;
    }

    // Inicializa rainha
    colmeia->abelhas[indiceAbelha].tipo = RAI;
    colmeia->abelhas[indiceAbelha].tempoNascimento = 0;
    colmeia->abelhas[indiceAbelha].tempoUltimaRefeicao = 0;
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
            limparColmeia(colmeia);
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

Celula *encontrarCelulaAlimento(Colmeia *colmeia, TipoCelula tipoAlimento)
{
    // Primeiro encontra o menor valor não nulo
    int conteudoMinimo = 999999;
    
    // Do favo mais novo (ID maior) para o mais velho
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        // Das células mais novas (últimas) para mais velhas
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == tipoAlimento && celula->estado == USADA && celula->conteudo > 0)
            {
                if (celula->conteudo < conteudoMinimo)
                {
                    conteudoMinimo = celula->conteudo;
                }
            }
        }
    }

    // Agora retorna a primeira célula com esse valor mínimo
    // Do favo mais novo para o mais velho
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        // Das células mais novas para mais velhas
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
Celula *encontrarCelulaVazia(Colmeia *colmeia, TipoCelula tipo)
{
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == tipo && celula->estado == VAZIA)
            {
                return celula;
            }
        }
    }
    return NULL;
}

int devemorrerPorIdade(Abelha *abelha, int tempoAtual)
{
    if (abelha->tipo == RAI)
        return 0;

    int idadeEmMinutos = tempoAtual - abelha->tempoNascimento;
    return idadeEmMinutos >= 57600; // 40 dias
}

void promoverAbelhas(Colmeia *colmeia)
{
    int totalOperarias = 0;
    int qtdPorTipo[5] = {0}; // FAX, NUT, CON, GUA, FOR

    // Conta operárias por tipo
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo >= FAX && colmeia->abelhas[i].tipo <= FOR)
        {
            qtdPorTipo[colmeia->abelhas[i].tipo]++;
            totalOperarias++;
        }
    }

    if (totalOperarias == 0)
        return;

    // Calcula percentagens reais e ideais
    float percentagens[5];
    float ideais[5] = {0.10, 0.30, 0.20, 0.10, 0.30}; // fax, nut, con, gua, for

    for (int i = 0; i < 5; i++)
    {
        percentagens[i] = (float)qtdPorTipo[i] / totalOperarias;
    }

    // Do FOR até NUT, promove se percentagem abaixo do ideal
    for (int tipo = FOR; tipo > FAX; tipo--)
    {
        if (percentagens[tipo] < ideais[tipo])
        {
            // Procura abelha mais antiga do nível anterior para promover
            int abelhaParaPromover = -1;
            int tempoMaisAntigo = 0;

            for (int i = 0; i < colmeia->numeroAbelhas; i++)
            {
                if (colmeia->abelhas[i].tipo == tipo - 1)
                {
                    if (colmeia->abelhas[i].tempoNascimento < tempoMaisAntigo ||
                        abelhaParaPromover == -1)
                    {
                        tempoMaisAntigo = colmeia->abelhas[i].tempoNascimento;
                        abelhaParaPromover = i;
                    }
                }
            }

            if (abelhaParaPromover != -1)
            {
                colmeia->abelhas[abelhaParaPromover].tipo++;
            }
        }
    }
}

void promoverNaoAdultas(Colmeia *colmeia, int tempoAtual)
{
    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == CRI && celula->estado == USADA)
            {
                int tempoCriacao = tempoAtual - celula->conteudo;
                
                if (tempoCriacao > 21 * 1440)
                {
                    // Criar nova abelha faxineira
                    int novoIndice = colmeia->numeroAbelhas++;
                    colmeia->abelhas = realloc(colmeia->abelhas, 
                                             sizeof(Abelha) * colmeia->numeroAbelhas);

                    colmeia->abelhas[novoIndice].tipo = FAX;
                    colmeia->abelhas[novoIndice].tempoNascimento = tempoAtual;
                    colmeia->abelhas[novoIndice].tempoUltimaRefeicao = tempoAtual;

                    // Libera a célula
                    celula->estado = VAZIA;
                    celula->conteudo = 0;
                }
            }
        }
    }
}
int porOvos(Colmeia *colmeia, int tempoAtual, int *trabalhoFaxineiras)
{
    static int ultimoOvoColocado = -999999; // guarda último tempo que colocou ovo
    if (tempoAtual == ultimoOvoColocado)
        return 0; // só um ovo por minuto

    // Verifica se tem rainha
    int temRainha = 0;
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo == RAI)
        {
            temRainha = 1;
            break;
        }
    }
    if (!temRainha)
        return 0;

    // Verifica se tem faxineiras suficientes
    if (*trabalhoFaxineiras < 10)
        return 0;

    // Procura célula de cria vazia
    Celula *celulaVazia = NULL;
    for (int i = colmeia->numeroFavos - 1; i >= 0 && !celulaVazia; i--)
    {
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            if (colmeia->favos[i].celulas[j].tipo == CRI &&
                colmeia->favos[i].celulas[j].estado == VAZIA)
            {
                celulaVazia = &colmeia->favos[i].celulas[j];
                break;
            }
        }
    }

    if (!celulaVazia)
        return 0;

    // Coloca ovo
    celulaVazia->estado = USADA;
    celulaVazia->conteudo = tempoAtual; // marca tempo inicial
    *trabalhoFaxineiras -= 10;
    ultimoOvoColocado = tempoAtual;

    return 1;
}

int recolherPolen(Colmeia *colmeia, int *trabalhoForrageiras, int *trabalhoFaxineiras)
{
    if (*trabalhoForrageiras < 60 || *trabalhoFaxineiras < 10)
        return 0;

    // Procura célula de pólen vazia
    Celula *celulaVazia = NULL;
    for (int i = colmeia->numeroFavos - 1; i >= 0; i--)
    {
        for (int j = colmeia->favos[i].numeroCelulas - 1; j >= 0; j--)
        {
            if (colmeia->favos[i].celulas[j].tipo == POL &&
                colmeia->favos[i].celulas[j].estado == VAZIA)
            {
                celulaVazia = &colmeia->favos[i].celulas[j];
                break;
            }
        }
        if (celulaVazia)
            break;
    }

    // Se não há células vazias, zera o trabalho das forrageiras
    if (!celulaVazia)
    {
        *trabalhoForrageiras = 0;
        return 0;
    }

    // Coloca pólen
    celulaVazia->estado = USADA;
    celulaVazia->conteudo = 20;
    *trabalhoForrageiras -= 60;
    *trabalhoFaxineiras -= 10;

    return 1;
}

void imprimirRelatorio(Colmeia *colmeia, int tempoAtual)
{
    int contadorAbelhas[7] = {0};
    int contadorOvos = 0, contadorLarvas = 0, contadorPupas = 0;

    // Conta abelhas por tipo
    for (int i = 0; i < colmeia->numeroAbelhas; i++)
    {
        if (colmeia->abelhas[i].tipo != -1)
            contadorAbelhas[colmeia->abelhas[i].tipo]++;
    }

    // Conta ovos, larvas e pupas
    for (int i = 0; i < colmeia->numeroFavos; i++)
    {
        for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++)
        {
            Celula *celula = &colmeia->favos[i].celulas[j];
            if (celula->tipo == CRI && celula->estado == USADA)
            {
                int tempoCriacao = tempoAtual - celula->conteudo;
                if (tempoCriacao < 3 * 1440)
                    contadorOvos++;
                else if (tempoCriacao < 9 * 1440)
                    contadorLarvas++;
                else if (tempoCriacao < 21 * 1440)
                    contadorPupas++;
            }
        }
    }

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

    // Imprime contador de abelhas
    for (int i = 0; i < 7; i++)
    {
        if (contadorAbelhas[i] > 0)
            printf("%3d ", contadorAbelhas[i]);
        else
            printf("    ");
    }

    // Imprime ovos, larvas e pupas
    if (contadorOvos > 0)
        printf("%3d ", contadorOvos);
    else
        printf("    ");
    if (contadorLarvas > 0)
        printf("%3d ", contadorLarvas);
    else
        printf("    ");
    if (contadorPupas > 0)
        printf("%3d ", contadorPupas);
    else
        printf("    ");
    printf("\n");

    // Imprime favos
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
        if (usadas[CRI] > 0)
            printf("%4d", usadas[CRI]);
        printf("\n");
    }
}
void simularColmeia(Colmeia *colmeia, int minutos, int temporadas)
{
   int tempoAtual = 0;
   imprimirRelatorio(colmeia, tempoAtual);

   int relatorioPendente = abs(temporadas);
   int trabalhoForrageiras = 0;
   int trabalhoFaxineiras = 0;
   int terminado = 0;
   
   while (relatorioPendente > 0 && !terminado)
   {
       int tempoAlvoRelatorio = tempoAtual + minutos;
       
       for (int m = tempoAtual + 1; m <= tempoAlvoRelatorio && !terminado; m++)
       {
           // Verifica mortes por idade
           for (int i = 0; i < colmeia->numeroAbelhas; i++)
           {
               if (colmeia->abelhas[i].tipo == -1)
                   continue;

               if (devemorrerPorIdade(&colmeia->abelhas[i], m))
               {
                   colmeia->abelhas[i].tipo = -1;
               }
           }

           // Se temporadas positivo
           if (temporadas > 0)
           {
               // Promove abelhas operárias
               promoverAbelhas(colmeia);
               
               // Acumula trabalho
               for (int i = 0; i < colmeia->numeroAbelhas; i++)
               {
                   if (colmeia->abelhas[i].tipo != -1)
                   {
                       if (colmeia->abelhas[i].tipo == FAX && 
                           m - colmeia->abelhas[i].tempoUltimaRefeicao < 1440)
                           trabalhoFaxineiras++;
                       else if (colmeia->abelhas[i].tipo == FOR && 
                                m - colmeia->abelhas[i].tempoUltimaRefeicao < 1440)
                           trabalhoForrageiras++;
                   }
               }

               // Tenta por ovos e recolher pólen
               porOvos(colmeia, m, &trabalhoFaxineiras);
               recolherPolen(colmeia, &trabalhoForrageiras, &trabalhoFaxineiras);
           }

           // Evolui ovos/larvas/pupas
           promoverNaoAdultas(colmeia, m);

           // Alimentação
           int precisamComer[5000] = {0};
           int numPrecisamComer = 0;

           for (int i = 0; i < colmeia->numeroAbelhas; i++)
           {
               if (colmeia->abelhas[i].tipo != -1 &&
                   (colmeia->abelhas[i].tempoNascimento == m ||
                    m - colmeia->abelhas[i].tempoUltimaRefeicao >= 1440))
               {
                   precisamComer[numPrecisamComer++] = i;
               }
           }

           // Se há abelhas para alimentar
           if (numPrecisamComer > 0)
           {
               Celula *celulaMel = encontrarCelulaAlimento(colmeia, MEL);
               Celula *celulaPolena = encontrarCelulaAlimento(colmeia, POL);

               // Se não tem mel ou pólen suficiente
               if (!celulaMel || !celulaPolena || 
                   celulaMel->conteudo < 10 || celulaPolena->conteudo < 4)
               {
                   // Mata todas as abelhas que precisam comer
                   for (int i = 0; i < numPrecisamComer; i++)
                   {
                       colmeia->abelhas[precisamComer[i]].tipo = -1;
                   }

                   // Todo mel fica vazio
                   for (int i = 0; i < colmeia->numeroFavos; i++)
                   {
                       for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++)
                       {
                           if (colmeia->favos[i].celulas[j].tipo == MEL)
                           {
                               colmeia->favos[i].celulas[j].estado = VAZIA;
                               colmeia->favos[i].celulas[j].conteudo = 0;
                           }
                       }
                   }
               }
               else // Tem comida suficiente
               {
                   // Alimenta a abelha
                   celulaMel->conteudo -= 10;
                   celulaPolena->conteudo -= 4;

                   if (celulaMel->conteudo == 0)
                       celulaMel->estado = VAZIA;
                   if (celulaPolena->conteudo == 0)
                       celulaPolena->estado = VAZIA;

                   colmeia->abelhas[precisamComer[0]].tempoUltimaRefeicao = m;
               }
           }

           // Zera trabalho ao fim do minuto
           trabalhoFaxineiras = 0;
           trabalhoForrageiras = 0;

           // Verifica se todas morreram E não há ovos/larvas/pupas
           int temAbelhasOuCrias = 0;
           for (int i = 0; i < colmeia->numeroAbelhas; i++) {
               if (colmeia->abelhas[i].tipo != -1)
                   temAbelhasOuCrias = 1;
           }
           if (!temAbelhasOuCrias) {
               for (int i = 0; i < colmeia->numeroFavos; i++) {
                   for (int j = 0; j < colmeia->favos[i].numeroCelulas; j++) {
                       if (colmeia->favos[i].celulas[j].tipo == CRI && 
                           colmeia->favos[i].celulas[j].estado == USADA) {
                           int tempoCriacao = m - colmeia->favos[i].celulas[j].conteudo;
                           if (tempoCriacao <= 21 * 1440) {
                               temAbelhasOuCrias = 1;
                               break;
                           }
                       }
                   }
                   if (temAbelhasOuCrias) break;
               }
           }
           
           if (!temAbelhasOuCrias) {
               imprimirRelatorio(colmeia, m);
               terminado = 1;
               break;
           }
       }

       if (!terminado)
       {
           tempoAtual = tempoAlvoRelatorio;
           imprimirRelatorio(colmeia, tempoAtual);
           relatorioPendente--;
       }
   }
}
int main()
{
    int nAbelhas, nCelulas, minutos, temporadas;

    scanf("%d", &nAbelhas);
    scanf("%d", &nCelulas);
    scanf("%d", &minutos);
    scanf("%d", &temporadas);

    Colmeia colmeia;
    inicializarAbelhas(&colmeia, nAbelhas);
    inicializarCelulas(&colmeia, nCelulas);

    simularColmeia(&colmeia, minutos, temporadas);

    limparColmeia(&colmeia);

    return 0;
}