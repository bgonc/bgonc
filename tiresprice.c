#include <stdio.h>
#define SALESTAX .07

int main()
{

	int numTires;
	float tirePrice, beforeTax, netSales;

	printf("How many tires did you purchase?");
	scanf(" %d", &numTires);
	printf("What was the cost per tire (enter in €XX.XX format)? ");
	scanf(" $%f", &tirePrice);

	beforeTax = tirePrice * numTires;
	netSales = beforeTax + (beforeTax * SALESTAX);

	printf("You spent $%.2f on your tires\n\n\n", netSales);

	return 0;
	}

