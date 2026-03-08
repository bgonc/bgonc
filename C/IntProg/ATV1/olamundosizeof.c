#include <stdio.h>

int main()

{
    printf("Olá Mundo!");
    printf("\nsizeof(char): %d", sizeof(char));
    printf("\nsizeof(short): %d", sizeof(short));
    printf("\nsizeof(int): %d", sizeof(int));
    printf("\nsizeof(long): %d", sizeof(long));
    printf("\nsizeof(long long): %d", sizeof(long long));
    printf("\nsizeof(float): %d", sizeof(float));
    printf("\nsizeof(double): %d", sizeof(double));
    printf("\nsizeof(double long): %d", sizeof(long double));

    printf("\nO total é %d", sizeof(short)+sizeof(int)+sizeof(long)+sizeof(long long)+sizeof(float)+sizeof(double)+sizeof(long double));
}