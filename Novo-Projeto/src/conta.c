#include "conta.h"

Conta contas[MAX_CONTAS];
int qtd_contas = 0;
int proximo_numero = 1;  // começa em 1 para facilitar a identificação

/* ============================================================
 * FUNÇÕES AUXILIARES
 * ============================================================ */

/*
 * Busca uma conta pelo número.
 * Retorna o índice no vetor ou -1 se não encontrada.
 */
int encontrar_conta_por_numero(const Conta contas[], int qtd, int numero) {
    for (int i = 0; i < qtd; i++) {
        if (contas[i].numero == numero) {
            return i;
        }
    }
    return -1;
}

/*
 * Busca uma conta pelo CPF.
 * Retorna o índice no vetor ou -1 se não encontrada.
 */
int encontrar_conta_ativa_por_cpf(const Conta contas[], int qtd, const char* cpf) {
    for (int i = 0; i < qtd; i++) {
        if (strcmp(contas[i].cpf, cpf) == 0 && contas[i].status == ATIVA) { // Verifica se é ATIVA
            return i;
        }
    }
    return -1;
}

/*
 * Exibe todos os dados de uma conta específica.
 */
void mostrar_dados(const Conta* c) {
    if (c == NULL) {
        printf("Conta inexistente.\n");
        return;
    }

    printf("\n========================================\n");
    printf("Número da conta: %d\n", c->numero);
    printf("Nome: %s\n", c->nome);
    printf("CPF: %s\n", c->cpf);
    printf("Agência: %s\n", c->agencia);
    printf("Telefone: %s\n", c->telefone);
    printf("Status: %s\n", (c->status == ATIVA) ? "ATIVA" : "ENCERRADA");
    printf("Saldo: R$ %.2f\n", c->saldo);
    printf("========================================\n");
}

/* ============================================================
 * FUNÇÕES PRINCIPAIS DO SISTEMA
 * ============================================================ */


int abrir_conta(const char* nome, const char* cpf, const char* agencia, const char* telefone) {
    if (qtd_contas >= MAX_CONTAS) {
        printf("Erro: limite máximo de contas atingido.\n");
        return 0;
    }

    if (encontrar_conta_ativa_por_cpf(contas, qtd_contas, cpf) != -1) { // Usa a nova função
        printf("Erro: já existe uma conta ativa com este CPF.\n");
        return 0;
    }

    Conta nova;
    nova.numero = proximo_numero++;
    strncpy(nova.nome, nome, TAM_NOME - 1);
    strncpy(nova.cpf, cpf, TAM_CPF - 1);
    strncpy(nova.agencia, agencia, TAM_AGENCIA - 1);
    strncpy(nova.telefone, telefone, TAM_TELEFONE - 1);
    nova.saldo = 0.0;
    nova.status = ATIVA;

    contas[qtd_contas++] = nova;

    printf("Conta criada com sucesso! Número: %d\n", nova.numero);
    return 1;
}

int depositar(int numero, double valor) {
    int i = encontrar_conta_por_numero(contas, qtd_contas, numero);

    if (i == -1 || contas[i].status == ENCERRADA) {
        printf("Conta inválida ou encerrada.\n");
        return 0;
    }

    if (valor <= 0) {
        printf("Valor inválido.\n");
        return 0;
    }

    contas[i].saldo += valor;
    printf("Depósito realizado! Novo saldo: R$ %.2f\n", contas[i].saldo);
    return 1;
}


int sacar(int numero, double valor) {
    int i = encontrar_conta_por_numero(contas, qtd_contas, numero);

    if (i == -1 || contas[i].status == ENCERRADA) {
        printf("Conta inválida ou encerrada.\n");
        return 0;
    }

    if (valor <= 0 || valor > contas[i].saldo) {
        printf("Valor inválido ou saldo insuficiente.\n");
        return 0;
    }

    contas[i].saldo -= valor;
    printf("Saque realizado! Novo saldo: R$ %.2f\n", contas[i].saldo);
    return 1;
}


int transferir(int origem, int destino, double valor) {
    int i_origem = encontrar_conta_por_numero(contas, qtd_contas, origem);
    int i_destino = encontrar_conta_por_numero(contas, qtd_contas, destino);

    if (i_origem == -1 || i_destino == -1) {
        printf("Conta de origem ou destino não encontrada.\n");
        return 0;
    }

    if (contas[i_origem].status == ENCERRADA || contas[i_destino].status == ENCERRADA) {
        printf("Ambas as contas devem estar ativas.\n");
        return 0;
    }

    if (valor <= 0 || contas[i_origem].saldo < valor) {
        printf("Valor inválido ou saldo insuficiente.\n");
        return 0;
    }

    contas[i_origem].saldo -= valor;
    contas[i_destino].saldo += valor;

    printf("Transferência concluída!\n");
    printf("Novo saldo da conta %d: R$ %.2f\n", origem, contas[i_origem].saldo);
    printf("Novo saldo da conta %d: R$ %.2f\n", destino, contas[i_destino].saldo);
    return 1;
}

int atualizar_contato(int numero, const char* novo_telefone, const char* nova_agencia) {
    int i = encontrar_conta_por_numero(contas, qtd_contas, numero);

    if (i == -1) {
        printf("Conta não encontrada.\n");
        return 0;
    }

    if (contas[i].status == ENCERRADA) {
        printf("Conta encerrada, não é possível atualizar.\n");
        return 0;
    }

    strncpy(contas[i].telefone, novo_telefone, TAM_TELEFONE - 1);
    strncpy(contas[i].agencia, nova_agencia, TAM_AGENCIA - 1);

    printf("Dados atualizados com sucesso!\n");
    return 1;
}

int encerrar_conta(int numero) {
    int i = encontrar_conta_por_numero(contas, qtd_contas, numero);

    if (i == -1) {
        printf("Conta não encontrada.\n");
        return 0;
    }

    if (contas[i].saldo != 0) {
        printf("Erro: o saldo deve ser igual a zero para encerrar a conta.\n");
        return 0;
    }

    contas[i].status = ENCERRADA;
    printf("Conta %d encerrada com sucesso.\n", numero);
    return 1;
}

void listar_contas(int filtro_status) {
    printf("\n======= LISTAGEM DE CONTAS =======\n");

    for (int i = 0; i < qtd_contas; i++) {
        if (filtro_status == -1 || contas[i].status == filtro_status) {
            mostrar_dados(&contas[i]);
        }
    }
}
