#ifndef CONTEUDOS_H
#define CONTEUDOS_H

// Estrutura que representa um conteúdo do catálogo
typedef struct {
    char titulo[100];              // Título do conteúdo
    char categoria[50];           // Categoria (ex: Ação, Drama)
    int duracao;                  // Duração em minutos
    int classificacaoEtaria;      // Classificação etária (ex: 12, 16, 18)
    int visualizacoes;            // Número de vezes que o conteúdo foi visualizado
} Conteudo;

// Lista de funções disponíveis para manipular os conteúdos

// Lê os conteúdos do ficheiro CSV e carrega-os para a memória
Conteudo* lerConteudosCSV(const char *nomeFicheiro, int *num);

// Grava os conteúdos da memória de volta para o ficheiro CSV
void gravarConteudosCSV(const char *nomeFicheiro, Conteudo *lista, int num);

// Lista todos os conteúdos atualmente carregados
void listarConteudos(Conteudo *lista, int num);

// Adiciona um novo conteúdo à lista (usando realloc)
Conteudo* adicionarConteudo(Conteudo *lista, int *num);

// Edita um conteúdo existente com base na sua posição
void editarConteudo(Conteudo *lista, int num);

// Remove um conteúdo da lista e ajusta o array
Conteudo* removerConteudo(Conteudo *lista, int *num);

// Pesquisa um conteúdo pelo título
void pesquisarPorTitulo(Conteudo *lista, int num);

// NOVAS FUNÇÕES:
// Pesquisa conteúdos com base na categoria
void pesquisarPorCategoria(Conteudo *lista, int num);

// Pesquisa conteúdos com base na classificação etária
void pesquisarPorClassEtaria(Conteudo *lista, int num);

// Mostra o conteúdo com mais visualizações
void mostrarMaisVisualizado(Conteudo *lista, int num);

#endif
