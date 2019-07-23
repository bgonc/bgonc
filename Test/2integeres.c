#include <stdio.h>
int main()
{
	int num1, num2, soma;
	printf("Escolhe 2 integers\n");
	scanf("%d %d", &num1, &num2);
	soma = num1 + num2;
	printf("a soma é %d + %d = %d\n", num1, num2, soma);
	return 0;
}
