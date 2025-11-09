#ifndef BANCO_H
#define BANCO_H

#include "lista.h"

typedef struct {
    Lista clientes;
    char arquivo_clientes[128];
    char arquivo_mov[128];
} Banco;

void banco_init(Banco* B, const char* arq_clientes, const char* arq_mov);
void banco_free(Banco* B);
int banco_carregar(Banco* B);
int banco_salvar(const Banco* B);

int banco_buscar_idx_por_conta(const Banco* B, const char* conta);

int banco_abrir_conta(Banco* B, const Cliente* novo);
int banco_encerrar_conta(Banco* B, const char* conta, const char* senha);
int banco_depositar(Banco* B, const char* conta, double valor);
int banco_sacar(Banco* B, const char* conta, const char* senha, double valor);
int banco_consultar_saldo(const Banco* B, const char* conta, const char* senha);
void banco_listar_ordenado_por_nome(Banco* B);
void banco_listar_ordenado_por_conta(Banco* B);

#endif