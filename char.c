#include <stdio.h>
int main ()
{
	char chr;
	printf("Enter a character: \n");
	scanf("%c", &chr);
	printf("You entered %c.\n", chr);
	printf("ASCII value is %d.\n", chr);
	return 0;
}
