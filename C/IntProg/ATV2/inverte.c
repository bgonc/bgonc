#include <stdio.h>
#include <string.h>

/* definir o tamanho máximo de uma string em macros, para assim ser
fácil de alterar este valor sem ter de ver todo o código */
#define MAXSTR 255

void strinv(char *str)
{
    int tamanho = strlen(str);
    int i, j;
    char temp;
    

    for(i = 0, j = tamanho -1; i < j; i++, j--)
    {

        temp =  str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main()
{
    char str[MAXSTR];
    fgets(str,MAXSTR,stdin);
    str[strlen(str)-1]=0;
    strinv(str);
    printf("%s", str);
}