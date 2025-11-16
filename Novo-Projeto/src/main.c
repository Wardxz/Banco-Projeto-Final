#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conta.c"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Menu principal
void exibir_menu() {
    printf("\n========================================\n");
    printf("        SISTEMA BANCÁRIO SIMPLES        \n");
    printf("========================================\n");
    printf("1. Abrir nova conta\n");
    printf("2. Depositar\n");
    printf("3. Sacar\n");
    printf("4. Transferir\n");
    printf("5. Consultar saldo e dados\n");
    printf("6. Atualizar telefone e agencia\n");
    printf("7. Encerrar conta\n");
    printf("8. Listar todas as contas\n");
    printf("9. Listar apenas contas ativas\n");
    printf("0. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opção: ");
}

int main() {
    int opcao;

    do {
        exibir_menu();
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: {
                char nome[TAM_NOME];
                char cpf[TAM_CPF];
                char agencia[TAM_AGENCIA];
                char telefone[TAM_TELEFONE];

                printf("Nome do titular: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("CPF: ");
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';

                printf("Agência: ");
                fgets(agencia, sizeof(agencia), stdin);
                agencia[strcspn(agencia, "\n")] = '\0';

                printf("Telefone: ");
                fgets(telefone, sizeof(telefone), stdin);
                telefone[strcspn(telefone, "\n")] = '\0';

                abrir_conta(nome, cpf, agencia, telefone);
                break;
            }

            case 2: {
                int numero;
                double valor;
                printf("Número da conta: ");
                scanf("%d", &numero);
                printf("Valor do depósito: R$ ");
                scanf("%lf", &valor);
                limpar_buffer();
                depositar(numero, valor);
                break;
            }

            case 3: {
                int numero;
                double valor;
                printf("Número da conta: ");
                scanf("%d", &numero);
                printf("Valor do saque: R$ ");
                scanf("%lf", &valor);
                limpar_buffer();
                sacar(numero, valor);
                break;
            }

            case 4: {
                int origem, destino;
                double valor;
                printf("Número da conta origem: ");
                scanf("%d", &origem);
                printf("Número da conta destino: ");
                scanf("%d", &destino);
                printf("Valor da transferência: R$ ");
                scanf("%lf", &valor);
                limpar_buffer();
                transferir(origem, destino, valor);
                break;
            }

            case 5: {
                int tipo_busca;
                printf("Consultar por:\n1. Número da conta\n2. CPF\nEscolha: ");
                scanf("%d", &tipo_busca);
                limpar_buffer();

                if (tipo_busca == 1) {
                    int numero;
                    printf("Número da conta: ");
                    scanf("%d", &numero);
                    limpar_buffer();
                    int i = encontrar_conta_por_numero(contas, qtd_contas, numero);
                    mostrar_dados((i != -1) ? &contas[i] : NULL);
                } else if (tipo_busca == 2) {
                    char cpf[TAM_CPF];
                    printf("CPF: ");
                    fgets(cpf, sizeof(cpf), stdin);
                    cpf[strcspn(cpf, "\n")] = '\0';
                    int i = encontrar_conta_ativa_por_cpf(contas, qtd_contas, cpf);
                    mostrar_dados((i != -1) ? &contas[i] : NULL);
                } else {
                    printf("Opção de busca inválida.\n");
                }
                break;
            }

            case 6: {
                int numero;
                char novo_tel[TAM_TELEFONE];
                char nova_ag[TAM_AGENCIA];

                printf("Número da conta: ");
                scanf("%d", &numero);
                limpar_buffer();

                printf("Novo telefone: ");
                fgets(novo_tel, sizeof(novo_tel), stdin);
                novo_tel[strcspn(novo_tel, "\n")] = '\0';

                printf("Nova agência: ");
                fgets(nova_ag, sizeof(nova_ag), stdin);
                nova_ag[strcspn(nova_ag, "\n")] = '\0';

                atualizar_contato(numero, novo_tel, nova_ag);
                break;
            }


            case 7: {
                int numero;
                printf("Número da conta a encerrar: ");
                scanf("%d", &numero);
                limpar_buffer();
                encerrar_conta(numero);
                break;
            }

            case 8:
                listar_contas(-1);  // todas
                break;

            case 9:
                listar_contas(ATIVA);
                break;

            case 0:
                printf("Encerrando o sistema...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);
    return 0;
}