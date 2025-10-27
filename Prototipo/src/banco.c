#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

void banco_init(Banco* B) {
    B -> dados = NULL;
    B -> tam = 0;
    B -> cap = 0;

    FILE* f = fopen("data/clientes.txt", "r");
    if (!f) return;

    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        Cliente c = {0};
        linha[strcspn(linha, "\n")] = 0;
        
        sscanf(linha,
            "%7[^;];%15[^;];%99[^;];%14[^;];%10[^;];%19[^;];%119[^;];%9[^;];"
            "%59[^;];%9[^;];%59[^;];%59[^;];%2[^;];%19[^;];%lf;%d", 
            c.agencia, c.conta, c.nome, c.cpf, c.data_nasc, c.telefone,
            c.endereco, c.cep, c.local, c.numero_casa, c.bairro,
            c.cidade, c.estado, c.senha, &c.saldo, &c.ativo);
    
        if (B -> tam == B -> cap) {
            size_t nova = B -> cap ? B -> cap * 2 : 4;
            Cliente* tmp = realloc(B -> dados, nova * sizeof * B -> dados);
            if (!tmp) break;
            B -> dados = tmp;
            B -> cap = nova;
        }
        B -> dados[B -> tam++] = c;
    }
    fclose(f);
}

int banco_salvar(Banco* B) {
    FILE* f = fopen("data/clientes.txt", "w");
    if (!f) return 0;

    for (size_t i = 0; i < B -> tam; i++) {
        Cliente* c = &B -> dados[i];
        fprintf(f, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%.2f;%d\n",
        c -> agencia, c -> conta, c -> nome, c -> cpf, c -> data_nasc, c -> telefone, c -> endereco, c -> cep, c -> local, c -> numero_casa, c -> bairro, c -> cidade, c -> estado, c -> senha, c -> saldo, c -> ativo);
    }

    fclose(f);
    return 1;
}

int banco_abrir_conta(Banco* B, Cliente novo) {
    for(size_t i = 0; i < B -> tam; i++) {
        if (strcmp(B -> dados[i].cpf, novo.cpf) == 0 ||
        strcmp(B -> dados[i].conta, novo.conta) == 0) {
            printf("Erro: conta ou CPF já existente.\n");
            return 0;
        }
    }

    if (B -> tam == B -> cap) {
        size_t nova = B -> cap ? B -> cap * 2 : 4;
        Cliente* tmp = realloc(B -> dados, nova * sizeof * B -> dados);
        if (!tmp) return 0;
        B -> dados = tmp;
        B -> cap = nova;
    }

    novo.saldo = 0.0;
    novo.ativo = 1;
    B -> dados[B -> tam++] = novo;
    banco_salvar(B);

    printf("Conta criada com sucesso!\n");
    return 1;
}

int banco_depositar(Banco* B, const char* conta, double valor) {
    if (valor <= 0) return 0;
    for(size_t i = 0; i < B -> tam; i++) {
        if (strcmp(B -> dados[i].conta, conta) == 0 && B -> dados[i].ativo) {
            B -> dados[i].saldo += valor;

            FILE* f = fopen("data/movimentos.txt", "a");
            if (f) {
                fprintf(f, "2025-10-26;%s;DEPOSITO;%.2f;saldo=%.2f\n",
                conta, valor, B -> dados[i].saldo);
                fclose(f);
            }

            banco_salvar(B);
            return 1;
        }
    }
    printf("Conta nao encontrada.\n");
    return 0;
}

int banco_sacar(Banco* B, const char* conta, const char* senha, double valor) {
    if (valor <= 0) return 0;

    for (size_t i = 0; i < B -> tam; i++) {
        Cliente* c = &B -> dados[i];
        if (strcmp(c -> conta, conta) == 0 && c -> ativo) {
            if (strcmp(c -> senha, senha) != 0) {
                printf("Senha incorreta.\n");
                return 0;
            }
            if(c -> saldo < valor) {
                printf("Saldo insuficiente.\n");
                return 0;
            }
            c -> saldo -= valor;

            FILE* f = fopen("data/movimentos.txt", "a");
            if (f) {
                fprintf(f, "2025-10-26;%s;SAQUE;%.2f;saldo=%.2f\n",
                    conta, valor, c -> saldo);
                fclose(f);
            }

            banco_salvar(B);
            return 1;
        }
    }
    printf("Conta nao encontrada.\n");
    return 0;
}

int banco_encerrar_conta(Banco* B, const char* conta) {
    for (size_t i = 0; i < B -> tam; i++) {
        Cliente* c = &B -> dados[i];
        if (strcmp(c -> conta, conta) == 0) {
            if (c -> saldo != 0) {
                printf("Erro: saldo diferente de zero.\n");
                return 0;
            }
            c -> ativo = 0;
            banco_salvar(B);
            printf("Conta encerrada com sucessso.\n");
            return 1;
        }
    }
    printf("Conta nao encontrada.\n");
    return 0;
}

void banco_consultar_cliente(Banco* B, const char* conta) {
    for (size_t i = 0; i < B -> tam; i++) {
        Cliente* c = &B -> dados[i];
        if (strcmp(c -> conta, conta) == 0) {
            printf("=== CLIENTE ===\nNome: %s\nCPF: %s\nSaldo: %.2f\n", c -> nome, c -> cpf, c -> saldo);
            return;
        }
    }
    printf("Conta nao encerrada.\n");
}

void banco_free(Banco* B) {
    free(B -> dados);
    B -> dados = NULL;
    B -> tam = B -> cap = 0;
}