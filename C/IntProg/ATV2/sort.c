#include <stdio.h>

unsigned int randaux()
{
  static long seed = 1;
  return (((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

void PrintInts(int v[], int n)
{
  for (int i = 0; i < n; i++)
    printf("%d ", v[i]);
  printf("\n");
}

void Sort(int v[], int n)
{
  for (int i = 0; i < n - 1; i++)
  {

    for (int j = i + 1; j < n; j++)
    {
      if (v[i] > v[j])
      {

        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;
      }
    }
  }
}

int main()
{
  int v[1000], i, n;

  scanf("%d", &n);
  n++;

  for (i = 0; i < 1000; i++)
    v[i] = randaux() % n;

  PrintInts(v, 10);

  Sort(v, 1000);

  PrintInts(v, 10);
}