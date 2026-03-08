#include <stdio.h>
#include <math.h>

int main() {
    int K, a = -K, b = -K, c = -K;
    double delta;
    int total = 0;

    scanf("%d", &K);

    a = -K;
    while (a <= K) 
    {
        if (a != 0) 
        {
            b = -K;
            while (b <= K) 
            {
                if (b != 0) 
                {
                    c = -K;
                    while (c <= K) 
                    {
                        if (c != 0) 
                        {
                            delta = b * b - 4 * a * c;

                            if (delta >= 0) 
                            {
                                double raiz_delta = sqrt(delta);
                                if (raiz_delta == (int)raiz_delta) 
                                {

                                    double x1 = (-b + raiz_delta) / (2 * a);
                                    double x2 = (-b - raiz_delta) / (2 * a);


                                    if (x1 == (int)x1 && x2 == (int)x2) 
                                    {

                                        printf("%d %d %d\n", a, b, c);
                                        total++;
                                    }
                                }
                            }
                        }
                        c++;
                    }
                }
                b++;
            }
        }
        a++;
    }

}