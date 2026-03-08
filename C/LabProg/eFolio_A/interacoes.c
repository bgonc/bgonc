#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conteudos.h"
#include "interacoes.h"

// Permite ao utilizador registar uma interacao com um conteudo
void registarInteracao(Conteudo *lista, int num)
{
    int idx;
    char tipo[20];

    if (num == 0)
    {
        printf("Nao existem conteudos disponiveis.\n");
        return;
    }

    listarConteudos(lista, num);

    printf("Escolha o numero do conteudo: ");
    scanf("%d", &idx);
    getchar(); // limpar o \n do buffer

    if (idx < 1 || idx > num)
    {
        printf("Conteudo invalido.\n");
        return;
    }

    idx--; // converter para índice do array

    printf("Tipo de interacao (Reproduzir, Pausa, Terminar): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = 0;

    // Se a interacao for "Terminar", aumenta o contador de visualizacoes
    if (strcmp(tipo, "Terminar") == 0)
    {
        lista[idx].visualizacoes++;
        printf("Visualizacao registada com sucesso.\n");
    }
    else
    {
        printf("Interacao '%s' registada (sem alterar visualizacoes).\n", tipo);
    }

    // Regista a interacao num ficheiro externo (append)
    FILE *f = fopen("interacoes.csv", "a");
    if (f != NULL)
    {
        fprintf(f, "%s,%s\n", lista[idx].titulo, tipo);
        fclose(f);
    }
    else
    {
        printf("Erro ao gravar no ficheiro de interacoes.\n");
    }
}