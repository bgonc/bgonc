#include <stdio.h>

int main() {
    int num;
    int i1, i2, i3;
    int total = 0;

    scanf("%d", &num);

    i1 = num;
    while (i1 >= 1) 
    {
        i2 = i1;

        while (i2 >= 1) 
        {
            i3 = num - i1 - i2;
            if (i3 >= 1 && i3 <= i2) 
            {
                total++;
            }
            i2--;
        }
        i1--;
    }

    printf("%d\n", total);

}