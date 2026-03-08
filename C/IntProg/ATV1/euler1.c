#include <stdio.h>

int main()

{
    int i = 1;
    int k;
    double n = 1;
    double f = 1;
    

    scanf("%d", &k);

    while(i <= k)
    {

        f*=i;
        n+=1/f;
        
        i++;
    }
    printf("%.16g\n", n);
}