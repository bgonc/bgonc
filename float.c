#include <stdio.h>
int main ()
{
	float num1;
	double num2;

	printf("Enter a number: \n");
	scanf("%f", &num1);
	printf("Enter another number: \n");
	scanf("%lf", &num2);

	printf("num1 = %.2f\n", num1);
	printf("num2 = %.2lf\n", num2);
	
	return 0;
}
