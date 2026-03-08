#include <stdio.h>
// tabuada
int main() {
  int num;
  int i;
  int produto;

  printf("Insira um  número: \n");
  scanf("%d", &num);

  for (i = 1; i <= 10; i++) {
    produto = i * num;
    printf("%d x %d = %d\n", num, i, produto);
  }
}
