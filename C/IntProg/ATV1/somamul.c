#include <stdio.h>

int main()

{
    int A, B, K, i;
	
    printf("Introduza um numero inteiro: ");
    scanf("%d", &K);
	
    i=0;
    A=1;
    while(A<=K)
	{
		B=1;
		while(B<=K)
		{
		if(A+B<=K && A*B<=K)	
			i++;
			B++;
		}
	A++;
	}
	printf("Total %d", i);
}
