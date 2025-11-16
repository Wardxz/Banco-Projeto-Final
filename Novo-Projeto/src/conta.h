#ifndef CONTA_H
#define CONTA_H

#include <stdio.h>
#include <string.h>

#define MAX_CONTAS 100
#define TAM_NOME 100
#define TAM_CPF 15
#define TAM_AGENCIA 10
#define TAM_TELEFONE 20

#define ATIVA 1               // ativa
#define ENCERRADA 0           // encerrada


typedef struct {
    int numero;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char agencia[TAM_AGENCIA];
    char telefone[TAM_TELEFONE];
    double saldo;
    int status;                       // ATIVA ou ENCERRADA
} Conta;


extern Conta contas[MAX_CONTAS];
extern int qtd_contas;                // quantidade de contas válidas
extern int proximo_numero;            // número sequencial para novas contas

/* ======= Protótipos de funções ======= */

/* Funções de busca e exibição */
int encontrar_conta_por_numero(const Conta contas[], int qtd, int numero);
int encontrar_conta_ativa_por_cpf(const Conta contas[], int qtd, const char* cpf);
void mostrar_dados(const Conta* c);

/* Operações obrigatórias */
int abrir_conta(const char* nome, const char* cpf, const char* agencia, const char* telefone);
int depositar(int numero, double valor);
int sacar(int numero, double valor);
int transferir(int origem, int destino, double valor);
int atualizar_contato(int numero, const char* novo_telefone, const char* nova_agencia);
int encerrar_conta(int numero);
void listar_contas(int filtro_status); // ATIVA, ENCERRADA ou -1 para todas

#endif