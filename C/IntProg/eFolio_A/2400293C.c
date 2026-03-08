#include <stdio.h>
#include <stdlib.h>

/* Valida se K está entre 2 e 100 */
int validar_K(int k)
{
    if (k < 2 || k > 100)
    {
        printf("K tem de ser entre 2 e 100.\n");
        return 0;
    }
    return 1;
}

/* Verifica se a sequência é válida */
int verificar_sequencia(int k, int vetor[], int tamanho)
{
    int soma = 0;
    unsigned long long produto = 1; /* long long para prevenir overflow */
    for (int i = 0; i < tamanho; i++)
    {
        soma += vetor[i];
        produto *= vetor[i];
        if (produto > k)
        { /* Evita overflow */
            produto = k + 1;
        }
    }
    return soma <= k && produto >= k;
}

/* Verifica se a sequência é uma sequência de vitória */
int verificar_vitoria(int k, int vetor[], int tamanho)
{
    int soma_diferencas = 0;
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            soma_diferencas += abs(vetor[i] - vetor[j]);
        }
    }
    return soma_diferencas == k;
}

/* Imprime a sequência atual */
void imprimir_sequencia(int vetor[], int tamanho, char jogador, int ultima)
{
    printf("Sequencia: ");
    for (int i = 0; i < tamanho; i++)
    {
        printf("%d ", vetor[i]);
    }
    if (!ultima)
    {
        printf("[Joga %c]\n", jogador);
    }
    else
    {
        printf("\n");
    }
}

/* Processa uma jogada */
int processar_jogada(int vetor[], int *tamanho, int indice, int valor)
{
    if (indice < 0)
    {
        indice = 0; /* Trata índices negativos como 0 */
    }
    if (indice >= *tamanho)
    {
        if (valor > 0)
        {
            /* Adiciona um valor no final da sequência */
            vetor[*tamanho] = valor;
            (*tamanho)++;
        }
        else if (valor < 0)
        {
            /* Adiciona o valor absoluto no final */
            vetor[*tamanho] = -valor;
            (*tamanho)++;
        }
    }
    else if (valor == 0)
    {
        /* Remove o valor no índice especificado */
        for (int i = indice; i < *tamanho - 1; i++)
        {
            vetor[i] = vetor[i + 1];
        }
        (*tamanho)--;
    }
    else if (valor < 0)
    {
        for (int i = *tamanho; i > indice; i--)
        {
            vetor[i] = vetor[i - 1];
        }
        vetor[indice] = -valor;
        (*tamanho)++;
    }
    else
    {
        vetor[indice] = valor;
    }
    return 1;
}

int main()
{
    int k, vetor[100], tamanho = 2, jogadas = 0, max_jogadas;
    char jogador = 'A';
    int indice, valor, inserir_remover_contador = 0;

    printf(" Indique K:\n");
    scanf("%d", &k);

    if (!validar_K(k))
    {
        return 1;
    }

    vetor[0] = k / 2;
    vetor[1] = k / 2;
    max_jogadas = k;

    imprimir_sequencia(vetor, tamanho, jogador, 0);

    while (jogadas < max_jogadas)
    {
        printf("Jogada (indice valor):\n");
        scanf("%d %d", &indice, &valor);

        if (!processar_jogada(vetor, &tamanho, indice, valor))
        {
            imprimir_sequencia(vetor, tamanho, jogador, 1);
            printf("Jogador %c perdeu.\n", jogador);
            return 0;
        }

        /* Verificar sequência inválida primeiro */
        if (!verificar_sequencia(k, vetor, tamanho))
        {
            imprimir_sequencia(vetor, tamanho, jogador, 1);
            printf("Jogador %c perdeu.\n", jogador);
            return 0;
        }

        /* Verificar sequência de vitória apenas se for válida */
        if (verificar_vitoria(k, vetor, tamanho))
        {
            imprimir_sequencia(vetor, tamanho, jogador, 1);
            printf("Jogador %c ganhou.\n", jogador);
            return 0;
        }

        if (jogadas == max_jogadas - 1)
        {
            imprimir_sequencia(vetor, tamanho, jogador, 1);
            printf("Empate.\n");
            return 0;
        }

        /* Regras de alternância */
        if (valor == 0 || valor < 0)
        {
            if (inserir_remover_contador < 1)
            {
                inserir_remover_contador++;
            }
            else
            {
                jogador = (jogador == 'A') ? 'B' : 'A';
                inserir_remover_contador = 0;
            }
        }
        else if (indice >= tamanho)
        {
            jogador = (jogador == 'A') ? 'B' : 'A';
        }
        else
        {
            jogador = (jogador == 'A') ? 'B' : 'A';
            inserir_remover_contador = 0;
        }

        imprimir_sequencia(vetor, tamanho, jogador, 0);
        jogadas++;
    }
}