#include <stdio.h>
int main ()
{
	int a;
	float b;

	printf("Enter integer and then a float: ");
	scanf("%d\n%f", &a, &b);

	printf("you entered %d and %.2f\n", a, b);
	return 0;
}
