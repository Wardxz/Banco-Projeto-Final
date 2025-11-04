#include "cliente.h"
#include <string.h>
#include <stdio.h>

void cliente_padrao(Cliente* c) {
    memset(c, 0, sizeof *c);
    strcpy(c->estado, "DF");
    c->saldo = 0.0;
    c->ativo = 1;
}
int cliente_cmp_nome(const Cliente* a, const Cliente* b) {
    return strcmp(a->nome, b->nome);
}
int cliente_cmp_conta(const Cliente* a, const Cliente* b) {
    return strcmp(a->conta, b->conta);
}
void imprimir_dados_cliente(const Cliente* c) {
    if (!c || !c->ativo) return;
    printf("\n--- DADOS DO CLIENTE ---\n");
    printf("Conta: %s-%s | Nome: %\n", c->agencia, c->conta, c->nome);
    printf("CPF: %s | Saldo: R$%.2f\n", c->cpf, c->saldo);
    printf("Endereco: %s, %s - %s/%s\n", c->endereco, c->numero_casa, c->cidade, c->estado);
}
