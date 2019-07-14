#include <stdio.h>

int main ()
{
	char firstInitial;
	char lastInitial;
	int age;
	int favorite_number;

	printf("What letter does your first name begin with?\n");
	scanf(" %c", &firstInitial);

	printf("What letter does your last name begin with?\n");
	scanf(" %c", &lastInitial);
	
	
	printf("How old are you?\n");
	scanf(" %d", &age);

	printf("what is your favorite number (integer only)?\n");
	scanf(" %d", &favorite_number);

	printf("\nYour initials are %c.%c. and you are %d years old", firstInitial, lastInitial, age);
	printf("\nYour favorite number is %d.\n\n", favorite_number);

	return 0;
	}
