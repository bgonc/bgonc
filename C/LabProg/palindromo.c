#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Função para limpar a string (remover espaços, pontuação e converter para minúsculas)
void limparString(char *original, char *limpa)
{
    int j = 0;
    for (int i = 0; original[i] != '\0'; i++)
    {
        if (isalpha(original[i]))
        {                                    // Verifica se é uma letra
            limpa[j] = tolower(original[i]); // Converte para minúsculas
            j++;
        }
    }
    limpa[j] = '\0'; // Adiciona o terminador de string
}

// Função para verificar se uma string é palíndromo
int ehPalindromo(char str[])
{
    int len = strlen(str);
    
    // Verificar se a string é válida
    if (len <= 0)
    {
        return 1; // String vazia é considerada palíndromo
    }
    
    int inicio = 0;
    int fim = len - 1;

    while (inicio < fim)
    {
        if (str[inicio] != str[fim])
        {
            return 0; // Não é palíndromo
        }
        inicio++;
        fim--;
    }
    return 1; // É palíndromo
}

int main()
{
    FILE *file;
    char linha[200];      // Buffer para armazenar cada linha do ficheiro
    char linhaLimpa[200]; // Buffer para armazenar a linha sem espaços/pontuação

    // Abrir o ficheiro para leitura
    file = fopen("frases.txt", "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o ficheiro.\n");
        return 1;
    }

    printf("Palíndromos encontrados:\n");

    // Ler cada linha do ficheiro
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        // Remover a quebra de linha do fgets()
        linha[strcspn(linha, "\n")] = '\0';

        // Limpar a string (remover espaços, pontuação e converter para minúsculas)
        limparString(linha, linhaLimpa);

        // Verificar se é um palíndromo
        if (ehPalindromo(linhaLimpa))
        {
            printf("'%s'\n", linha); // Imprime a frase original
        }
    }

    fclose(file); // Fecha o ficheiro
    return 0;
}
