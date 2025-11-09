#include "conta.h"
#include <string.h>

int depositar(Cliente* c, double valor) {
    if (!c || valor <= 0) return 0;
    c -> saldo += valor;
    return 1;
}

int sacar(Cliente* c, double valor, const char* senha_digitada) {
    if (!c || valor <= 0) return 0;
    if (strcmp(c -> senha, senha_digitada) != 0) return 0;
    if (valor > c-> saldo) return 0;
    c -> saldo -= valor;
    return 1;
}

int abrir_conta(Lista* L, const Cliente* novo) {
    if (lista_find_conta(L, novo -> conta) != -1) return 0;
    return lista_add(L, *novo);
}

int encerrar_conta(Lista* L, const char* conta) {
    int idx = lista_find_conta(L, conta);
    if (idx < 0) return 0;

    Cliente* c = &L -> dados[idx];
    if (c -> saldo != 0.0) return 0;

    c -> ativo = 0;
    return 1;
}