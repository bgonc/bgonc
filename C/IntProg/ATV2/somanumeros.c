#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char str[255];
    char *token;
    const char delimiters[] = " ";
    double soma = 0;

    fgets(str, sizeof(str), stdin);

    token = strtok(str, delimiters);

    while (token != NULL)
    {
        soma += atof(token);

        token = strtok(NULL, delimiters);
    }

    printf("%.15g", soma);
}