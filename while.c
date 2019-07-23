#include <stdio.h>

main()

{
	int ctr = 0;

	while (ctr < 5)
	{
		printf("Counter is at %d.\n", ++ctr);
	}

	while (ctr > 1)
	{
		printf("Counter is at %d.\n", --ctr);
	}

	return 0;
}
