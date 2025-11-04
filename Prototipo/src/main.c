#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"
#include "io.h"
#include "cliente.h"

#define ARQ_CLIENTES "data/clientes.txt"
#define ARQ_MOV "data/movimentos.txt"
#define MAX_OP_LEN 4

static void limpar_entrada(void) {
    int ch; // descarta o caracter quando encontrar \n ou EOF
    while((ch = getchar()) != '\n' && ch != EOF) {}
}

static void ler_dado(const char* prompt, char* buffer, size_t tam) {
    printf("%s", prompt);
    ler_linha(buffer, tam);
}

static void ler_cliente_base(Cliente* c) {
    cliente_padrao(c);

    printf("---- DADOS CADASTRAIS ----\n");
    ler_dado("Agencia (7 digitos): ", c -> agencia, sizeof(c -> agencia));
    ler_dado("Conta (15 digitos): ", c -> conta, sizeof(c -> conta));
    ler_dado("Nome Completo: ", c -> nome, sizeof(c -> nome));
    ler_dado("CPF: ", c -> cpf, sizeof(c -> cpf));
    ler_dado("Senha: ", c -> senha, sizeof(c -> senha));
    ler_dado("Data de Nascimento (DD/MM/AAAA): ", c -> data_de_nasc, sizeof(c -> data_de_nasc));
    ler_dado("Telefone: ", c -> telefone, sizeof(c -> telefone));

    printf("\n---- ENDERECO ----\n");
    ler_dado("CEP: ", c -> cep, sizeof(c -> cep));
    ler_dado("Logradouro/Rua: ", c -> endereco, sizeof(c -> endereco));
    ler_dado("Numero: ", c -> numero_casa, sizeof(c -> numero_casa));
    ler_dado("Complemento/Local (Ex: Apto 101): ", c -> local, sizeof(c -> local));
    ler_dado("Bairro: ", c -> bairro, sizeof(c -> bairro));
    ler_dado("Cidade: ", c -> idade, sizeof(c -> idade));
    ler_dado("Estado (UF, 2 letras. Ex: DF): ", c -> estado, sizeof(c -> estado));
}

static int menu() {
    int op;
    char buffer[MAX_OP_LEN];

    printf("================================================================================\n");
    printf("  ____  _           _                        ____                      _         \n");
    printf(" / ___|| |_   __ _ | |_   ___   _ __ ___    | __ )   __ _  _ __    ___ | |__    ___  \n");
    printf(" \\___ \\| __| / _` || __| / _ \\ | '_ ` _ \\   |  _ \\  / _` || '_ \\  / __|| '_ \\  / _ \\ \n");
    printf("  ___) | |_ | (_| || |_ | (_) || | | | | |  | |_) || (_| || | | || (__ | | | ||  __/ \n");
    printf(" |____/ \\__| \\__,_| \\__| \\___/ |_| |_| |_|  |____/  \\__,_||_| |_| \\___||_| |_| \\___| \n");
    printf("================================================================================\n");
    printf("                               PROJETO BANCO                                     \n");
    printf("================================================================================\n");

    printf("\n---- OPÇÕES PRINCIPAIS ----\n");
    printf("1. Abrir conta\n");
    printf("2. Encerrar conta (Requer senha e Saldo Zero)\n");
    printf("3. Consultar dados de cliente\n");
    printf("4. Alterar dados cadastrais\n");
    printf("5. Depositar\n");
    printf("6. Sacar\n");
    printf("7. Listar por Nome (Quick Sort)\n");
    printf("8. Listar por Conta (Quick Sort)\n");
    printf("9. Consultar Saldo (Requer senha)\n");
    printf("0. Sair e Salvar\n");

    ler_dado("Escolha a opcao: ", buffer, sizeof(buffer));

    if (sscanf(buffer, "%d", &op) != 1) { op = -1; }
    return op;
}

int main() {
    Banco B;
    banco_init(&B, ARQ_CLIENTES, ARQ_MOV);
    banco_carregar(&B);

    int op;

    do {
        op = menu();
        char conta[16], senha[20], valor_str[20];
        double valor;
        Cliente novo = {0};

        switch (op) {
            case 1:
                ler_cliente_base(&novo);
                if (banco_abrir_conta(&B, &novo)) {
                    printf("Conta aberta com sucesso! Conta: %s\n", novo.conta);
                    banco_salvar(&B);
                } else {
                    printf("Erro ao abrir conta. Conta ja existe ou falha de memoria.\n");
                }
                break;

            case 2:
                ler_dado("Conta para encerrar: ", conta, sizeof(conta));
                ler_dado("Senha: ", senha, sizeof(senha));
                if (!banco_encerrar_conta(&B, conta, senha)) {
                    printf("Falha no encerramento (verifique senha ou saldo).\n");
                }
                break;

            // ... (Case 3 e 4 para consultar e alterar dados, a serem implementados)

            case 5: // Depositar
                ler_dado("Conta para deposito: ", conta, sizeof(conta));
                ler_dado("Valor: R$ ", valor_str, sizeof(valor_str));
                if (sscanf(valor_str, "%lf", &valor) == 1 && valor > 0) {
                    if (banco_depositar(&B, conta, valor)) {
                        printf("Deposito realizado com sucesso.\n");
                    } else {
                        printf("Falha no deposito. Conta nao encontrada.\n");
                    }
                } else {
                    printf("Valor de deposito invalido.\n");
                }
                break;

            case 6:
                ler_dado("Conta para saque: ", conta, sizeof(conta));
                ler_dado("Senha: ", senha, sizeof(senha));
                ler_dado("Valor: R$ ", valor_str, sizeof(valor_str));

                if (sscanf(valor_str, "%lf", &valor) == 1 && valor > 0) {
                    if (banco_sacar(&B, conta, valor, senha)) {
                        printf("Saque realizado com sucesso.\n");
                    } else {
                        printf("Falha no saque. (Verifique conta, senha ou saldo).\n");
                    }
                } else {
                    printf("Valor de saque invalido.\n");
                }
                break;

            case 7:
                banco_listar_ordenado_por_nome(&B);
                break;

            case 8:
                banco_listar_ordenado_por_conta(&B);
                break;

            case 9:
                ler_dado("Conta para consulta: ", conta, sizeof(conta));
                ler_dado("Senha: ", senha, sizeof(senha));
                banco_consultar_saldo(&B, conta, senha);
                break;

            case 0:
                printf("Encerrando e salvando dados...\n");
                banco_salvar(&B);
                break;

            default:
                printf("Opcao invalida. Digite 0 para sair.\n");
                break;
        }
    } while (op != 0);

    banco_free(&B);
    return 0;
}