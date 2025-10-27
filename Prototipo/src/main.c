#include <stdio.h>
#include <stdlib.h>
#include "banco.h"
#include "io.h"
#include "io.c"

void menu_conta(Banco* B, Cliente* c) {
    int op;
    do {
        printf("\n--- CONTA %s (%s) ---\n", c->conta, c->nome);
        printf("1. Consultar saldo\n");
        printf("2. Depósito\n");
        printf("3. Saque\n");
        printf("4. Ver extrato\n");
        printf("0. Sair da conta\n");
        printf("Opção: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) {
            printf("Saldo atual: R$ %.2f\n", c->saldo);
        }

        else if (op == 2) {
            double valor;
            printf("Valor do depósito: ");
            scanf("%lf", &valor);
            getchar();
            banco_depositar(B, c->conta, valor);
        }

        else if (op == 3) {
            double valor;
            printf("Valor do saque: ");
            scanf("%lf", &valor);
            getchar();
            banco_sacar(B, c->conta, c->senha, valor);
        }

        else if (op == 4) {
            char path[64];
            sprintf(path, "data/movimentos.txt");
            FILE* f = fopen(path, "r");
            if (!f) { printf("Sem extratos registrados.\n"); continue; }
            char linha[256];
            printf("\n=== EXTRATO - Conta %s ===\n", c->conta);
            while (fgets(linha, sizeof(linha), f)) {
                if (strstr(linha, c->conta)) printf("%s", linha);
            }
            fclose(f);
        }

    } while (op != 0);
}

void menu_principal(Banco* B) {
    int op;
    do {
        printf("\n=== BANCO MALVADER ===\n");
        printf("1. Abrir Conta\n");
        printf("2. Listar Clientes\n");
        printf("3. Acessar Conta\n");
        printf("4. Encerrar Conta\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) {
            Cliente novo = {0};
            printf("Nome: "); ler_linha(novo.nome, sizeof(novo.nome));
            printf("Conta (ex: 1234-5): "); ler_linha(novo.conta, sizeof(novo.conta));
            printf("Agência: "); ler_linha(novo.agencia, sizeof(novo.agencia));
            printf("CPF: "); ler_linha(novo.cpf, sizeof(novo.cpf));
            printf("Senha: "); ler_linha(novo.senha, sizeof(novo.senha));
            banco_abrir_conta(B, novo);
        }

        else if (op == 2) {
            banco_listar_ordenado_por_nome(B);
        }

        else if (op == 3) {
            char conta[16], senha[20];
            printf("Conta: "); ler_linha(conta, sizeof(conta));
            printf("Senha: "); ler_linha(senha, sizeof(senha));

            int found = 0;
            for (size_t i = 0; i < B->tam; i++) {
                if (strcmp(B->dados[i].conta, conta) == 0 &&
                    strcmp(B->dados[i].senha, senha) == 0 &&
                    B->dados[i].ativo) {
                    printf("\nAcesso permitido! Bem-vindo(a), %s.\n", B->dados[i].nome);
                    menu_conta(B, &B->dados[i]);
                    found = 1;
                    break;
                }
            }
            if (!found)
                printf("Erro: Conta inexistente ou senha incorreta.\n");
        }

        else if (op == 4) {
            char conta[16];
            printf("Conta: "); ler_linha(conta, sizeof(conta));
            banco_encerrar_conta(B, conta);
        }

    } while (op != 0);

    printf("\nEncerrando o Banco Malvader...\n");
}

int main(void) {
    Banco B;
    banco_init(&B);

    menu_principal(&B);

    banco_free(&B);
    return 0;
}
