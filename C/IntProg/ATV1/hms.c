#include <stdio.h>

int main()

{
    int hora;
    int min;
    int seg;

    printf("Hora: ");
    scanf("%d", &hora);
    printf("Minuto: ");
    scanf("%d", &min);
    printf("Segundos: ");
    scanf("%d", &seg);

    int total=((hora*60)*60)+(min*60)+seg;

    printf("Numero de segundos desde o inicio do dia: %d", total);
}
