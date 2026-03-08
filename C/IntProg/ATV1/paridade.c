#include <stdio.h>

int main()

{
	int numero;

	printf("Indique um numero: ");
	scanf("%d", &numero);

	if(numero%2 == 0)
		printf("%d e Par\n", numero);
	else
		printf("%d e Impar\n", numero);
}
