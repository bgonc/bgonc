#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conteudos.h"

// Lê os conteudos a partir do ficheiro CSV
Conteudo *lerConteudosCSV(const char *nomeFicheiro, int *num)
{
    FILE *f = fopen(nomeFicheiro, "r");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro %s.\n", nomeFicheiro);
        *num = 0;
        return NULL;
    }

    Conteudo *lista = NULL;
    *num = 0;
    char linha[256];

    // Lê cada linha e extrai os campos
    while (fgets(linha, sizeof(linha), f) != NULL)
    {
        lista = realloc(lista, (*num + 1) * sizeof(Conteudo));
        sscanf(linha, "%99[^,],%49[^,],%d,%d,%d",
               lista[*num].titulo,
               lista[*num].categoria,
               &lista[*num].duracao,
               &lista[*num].classificacaoEtaria,
               &lista[*num].visualizacoes);
        (*num)++;
    }

    fclose(f);
    return lista;
}

// Grava os conteudos no ficheiro CSV
void gravarConteudosCSV(const char *nomeFicheiro, Conteudo *lista, int num)
{
    FILE *f = fopen(nomeFicheiro, "w");
    if (f == NULL)
    {
        printf("Erro ao abrir o ficheiro para escrita.\n");
        return;
    }

    for (int i = 0; i < num; i++)
    {
        fprintf(f, "%s,%s,%d,%d,%d\n",
                lista[i].titulo,
                lista[i].categoria,
                lista[i].duracao,
                lista[i].classificacaoEtaria,
                lista[i].visualizacoes);
    }

    fclose(f);
}

// Lista todos os conteudos carregados na memória
void listarConteudos(Conteudo *lista, int num)
{
    if (num == 0)
    {
        printf("Nenhum conteudo encontrado.\n");
        return;
    }

    printf("\n--- Lista de Conteudos ---\n");
    for (int i = 0; i < num; i++)
    {
        printf("%d - %s (%s) - %d min - M%d - %d visualizacoes\n",
               i + 1,
               lista[i].titulo,
               lista[i].categoria,
               lista[i].duracao,
               lista[i].classificacaoEtaria,
               lista[i].visualizacoes);
    }
}

// Adiciona um novo conteudo
Conteudo *adicionarConteudo(Conteudo *lista, int *num)
{
    lista = realloc(lista, (*num + 1) * sizeof(Conteudo));

    printf("Titulo: ");
    fgets(lista[*num].titulo, sizeof(lista[*num].titulo), stdin);
    lista[*num].titulo[strcspn(lista[*num].titulo, "\n")] = 0;

    printf("Categoria: ");
    fgets(lista[*num].categoria, sizeof(lista[*num].categoria), stdin);
    lista[*num].categoria[strcspn(lista[*num].categoria, "\n")] = 0;

    printf("Duracao (min): ");
    scanf("%d", &lista[*num].duracao);
    getchar();

    printf("Classificacao etaria: ");
    scanf("%d", &lista[*num].classificacaoEtaria);
    getchar();

    lista[*num].visualizacoes = 0; // novo conteudo começa com 0 visualizacoes

    (*num)++;
    printf("Conteudo adicionado com sucesso.\n");
    return lista;
}

// Edita um conteudo
void editarConteudo(Conteudo *lista, int num)
{
    int idx;
    listarConteudos(lista, num);

    printf("Numero do conteudo a editar: ");
    scanf("%d", &idx);
    getchar();

    if (idx < 1 || idx > num)
    {
        printf("Indice invalido.\n");
        return;
    }

    idx--; // converter para Indice de array

    printf("Novo titulo: ");
    fgets(lista[idx].titulo, sizeof(lista[idx].titulo), stdin);
    lista[idx].titulo[strcspn(lista[idx].titulo, "\n")] = 0;

    printf("Nova categoria: ");
    fgets(lista[idx].categoria, sizeof(lista[idx].categoria), stdin);
    lista[idx].categoria[strcspn(lista[idx].categoria, "\n")] = 0;

    printf("Nova duracao (min): ");
    scanf("%d", &lista[idx].duracao);
    getchar();

    printf("Nova classificacao etaria: ");
    scanf("%d", &lista[idx].classificacaoEtaria);
    getchar();

    printf("Conteudo editado com sucesso.\n");
}

// Remove um conteudo da lista
Conteudo *removerConteudo(Conteudo *lista, int *num)
{
    int idx;
    listarConteudos(lista, *num);

    printf("Numero do conteudo a remover: ");
    scanf("%d", &idx);
    getchar();

    if (idx < 1 || idx > *num)
    {
        printf("Indice invalido.\n");
        return lista;
    }

    idx--; // converter para Indice de array

    // mover os conteudos seguintes para trás
    for (int i = idx; i < *num - 1; i++)
    {
        lista[i] = lista[i + 1];
    }

    (*num)--;
    lista = realloc(lista, (*num) * sizeof(Conteudo));
    printf("Conteudo removido com sucesso.\n");
    return lista;
}

// Pesquisa por titulo
void pesquisarPorTitulo(Conteudo *lista, int num)
{
    char termo[100];
    printf("Introduza o titulo a pesquisar: ");
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = 0;

    printf("\n--- Resultados da pesquisa ---\n");
    for (int i = 0; i < num; i++)
    {
        if (strstr(lista[i].titulo, termo) != NULL)
        {
            printf("%d - %s (%s) - %d min - M%d - %d visualizacoes\n",
                   i + 1, lista[i].titulo, lista[i].categoria,
                   lista[i].duracao, lista[i].classificacaoEtaria,
                   lista[i].visualizacoes);
        }
    }
}

// Pesquisa conteudos por categoria
void pesquisarPorCategoria(Conteudo *lista, int num)
{
    char categoria[50];
    printf("Introduza a categoria a pesquisar: ");
    fgets(categoria, sizeof(categoria), stdin);
    categoria[strcspn(categoria, "\n")] = 0;

    printf("\n--- Resultados da pesquisa por categoria ---\n");
    for (int i = 0; i < num; i++)
    {
        if (strcmp(lista[i].categoria, categoria) == 0)
        {
            printf("%d - %s (%d min, M%d, %d visualizacoes)\n",
                   i + 1, lista[i].titulo, lista[i].duracao,
                   lista[i].classificacaoEtaria, lista[i].visualizacoes);
        }
    }
}

// Pesquisa conteudos por classificacao etaria
void pesquisarPorClassEtaria(Conteudo *lista, int num)
{
    int idade;
    printf("Introduza a classificacao etaria (apenas o numero, ex: 6, 12, 16): ");
    scanf("%d", &idade);
    getchar();

    printf("\n--- Resultados da pesquisa por classificacao etaria ---\n");
    for (int i = 0; i < num; i++)
    {
        if (lista[i].classificacaoEtaria == idade)
        {
            printf("%d - %s (%s, %d min, %d visualizacoes)\n",
                   i + 1, lista[i].titulo, lista[i].categoria,
                   lista[i].duracao, lista[i].visualizacoes);
        }
    }
}

// Mostra o conteudo mais visualizado
void mostrarMaisVisualizado(Conteudo *lista, int num)
{
    if (num == 0)
    {
        printf("Lista vazia.\n");
        return;
    }

    int max = 0;
    for (int i = 1; i < num; i++)
    {
        if (lista[i].visualizacoes > lista[max].visualizacoes)
        {
            max = i;
        }
    }

    printf("\n--- Conteudo mais visualizado ---\n");
    printf("%s (%s) - %d visualizacoes\n",
           lista[max].titulo,
           lista[max].categoria,
           lista[max].visualizacoes);
}