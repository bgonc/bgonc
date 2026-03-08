#include <stdio.h>

int main() {
    double m;
    int c;
    int moeda;

    printf("Introduza um montante em euros, podendo ter centimos: ");
    scanf("%lf", &m);

    c = (int)(m * 100 + 0.5);

    moeda = c / 200;

    if (moeda > 0) {
        printf("2 euros: %d\n", moeda);
    }
    c = c % 200;

    moeda = c / 100;

    if (moeda > 0) {
        printf("1 euro: %d\n", moeda);
    }
    c = c % 100;

    moeda = c / 50;

    if (moeda > 0) {
        printf("50 centimos: %d\n", moeda);
    }
    c = c % 50;

    moeda = c / 20;

    if (moeda > 0) {
        printf("20 centimos: %d\n", moeda);
    }
    c = c % 20;

    moeda = c / 10;

    if (moeda > 0) {
        printf("10 centimos: %d\n", moeda);
    }
    c = c % 10;

    moeda = c / 5;

    if (moeda > 0) {
        printf("5 centimos: %d\n", moeda);
    }
    c = c % 5;

    moeda = c / 2;

    if (moeda > 0) {
        printf("2 centimos: %d\n", moeda);
    }
    c = c % 2;

    moeda = c / 1;

    if (moeda > 0) {
        printf("1 centimo: %d\n", moeda);
    }
}
