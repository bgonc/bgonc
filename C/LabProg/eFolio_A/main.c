#include <stdio.h>
#include <stdlib.h>
#include "conteudos.h"
#include "interacoes.h"

#define FICHEIRO_CSV "conteudos.csv"

int main()
{
    Conteudo *lista = NULL;
    int numConteudos = 0;
    int opcao;

    // Leitura inicial do ficheiro CSV
    lista = lerConteudosCSV(FICHEIRO_CSV, &numConteudos);

    do
    {
        printf("\n=== STREAMFLIX ===\n");
        printf("1 - Listar conteudos\n");
        printf("2 - Adicionar conteudo\n");
        printf("3 - Editar conteudo\n");
        printf("4 - Remover conteudo\n");
        printf("5 - Pesquisar por titulo\n");
        printf("6 - Registar interacao\n");
        printf("7 - Pesquisar por categoria\n");
        printf("8 - Pesquisar por classificacao etaria\n");
        printf("9 - Ver conteudo mais visualizado\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar o \n do buffer

        switch (opcao)
        {
        case 1:
            listarConteudos(lista, numConteudos);
            break;
        case 2:
            lista = adicionarConteudo(lista, &numConteudos);
            break;
        case 3:
            editarConteudo(lista, numConteudos);
            break;
        case 4:
            lista = removerConteudo(lista, &numConteudos);
            break;
        case 5:
            pesquisarPorTitulo(lista, numConteudos);
            break;
        case 6:
            registarInteracao(lista, numConteudos);
            break;
        case 7:
            pesquisarPorCategoria(lista, numConteudos);
            break;
        case 8:
            pesquisarPorClassEtaria(lista, numConteudos);
            break;
        case 9:
            mostrarMaisVisualizado(lista, numConteudos);
            break;
        case 0:
            printf("A guardar dados e a sair...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    // Gravação final dos conteudos no ficheiro CSV
    gravarConteudosCSV(FICHEIRO_CSV, lista, numConteudos);

    // Libertar memória alocada dinamicamente
    free(lista);

    return 0;
}