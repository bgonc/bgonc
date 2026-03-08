#include <stdio.h>
#include <string.h>

#define MAX_PESSOAS 5

typedef struct {
    char nome[50];
    int idade;
} Pessoa;

void adicionarPessoa(Pessoa p[]) {

    for(int i = 0; i < MAX_PESSOAS; i++) {
        fgets(p[i].nome, sizeof(p[i].nome), stdin);
        p[i].nome[strcspn(p[i].nome, "\n")] = '\0';

        scanf("%d", &p[i].idade);

        while (getchar() != '\n');

    }
}

void atualizarIdade() {
}

void procurarNome() {
}

void listarDados(Pessoa p[]) {

    for(int i = 0; i < MAX_PESSOAS; i++) {
        printf("%s tem %d anos\n", p[i].nome, p[i].idade);
    }
}

int main(void) {
    Pessoa pess[MAX_PESSOAS];
    int opcao;

    do {
        printf("1. Adicionar pessoa\n");
        printf("2. Atualizar idade\n");
        printf("3. Procurar por nome\n");
        printf("4. Listar todos\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
        case 1:
            adicionarPessoa(pess);
            break;
        case 2:
            atualizarIdade();
            break;
        case 3:
            procurarNome();
            break;
        case 4:
            listarDados(pess);
            break;
        case 5:
            printf("A guardar dados e a sair...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        }

    } while (opcao != 5);

    return 0;
}
