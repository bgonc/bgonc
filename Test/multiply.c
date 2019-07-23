#include <stdio.h>
int main()
{
	double num1, num2, total;
	printf("escolhe 2 numeros: ");
	scanf("%lf %lf", &num1, &num2);
	
	total = num1 * num2;
	printf("resultado de %.2lf * %.2lf = %.2lf\n", num1, num2, total);

	return 0;
}
