#include <stdio.h>

#define RAND_A 231533
#define RAND_B 82571
#define RAND_M 428573

unsigned int rand(unsigned int seed)
{
    return (RAND_A * seed + RAND_B) % RAND_M;
}

int main()
{
    unsigned int seed, n;

    scanf("%u", &seed);
    scanf("%u", &n);

    for (int i = 0; i < 10; i++)
    {
        seed = rand(seed);
        printf("%u ", seed % (n + 1));
    }
}