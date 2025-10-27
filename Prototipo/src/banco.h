#ifndef BANCO_H
#define BANCO_H

#include <stddef.h>
#include "cliente.h"

typedef struct {
    Cliente* dados;
    size_t tam;
    size_t cap;
}   Banco;

void banco_init(Banco* B);
void banco_free(Banco* B);
int banco_salvar(Banco* B);
int banco_abrir_conta(Banco* B, Cliente novo);
int banco_depositar(Banco* B, const char* conta, double valor);
int banco_sacar(Banco* B, const char* conta, const char* senha, double valor);
int banco_encerrar_conta(Banco* B, const char* conta);
void banco_consultar_cliente(Banco* B, const char* conta);
void banco_listar_ordenado_por_nome(Banco* B);

#endif