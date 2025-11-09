#ifndef IO_H
#define IO_H

#include "lista.h"
#include <stddef.h>

void ler_linha(char* buf, size_t tam);

int salvar_clientes(const char* arq, const Lista* L);
int carregar_clientes(const char* arq, const Lista* L);

int registrar_mov(const char* arq, const char* conta, const char* tipo, double valor, double saldo);

#endif