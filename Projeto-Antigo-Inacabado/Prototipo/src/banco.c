#include "banco.h"
#include "io.h"
#include "conta.h"
#include "quicksort.h"
#include <stdio.h>

void banco_init(Banco* B, const char* ac, const char* am) {
    lista_init(&B -> clientes);

    strncpy(B -> arquivo_clientes, ac, sizeof(B -> arquivo_clientes));
    B -> arquivo_clientes[sizeof(B -> arquivo_clientes) - 1] = '\0';
    strncpy(B -> arquivo_mov, am, sizeof(B -> arquivo_mov));
    B -> arquivo_mov[sizeof(B -> arquivo_mov) - 1] = '\0';
}

void banco_free(Banco* B) { lista_free(&B -> clientes); }

int banco_carregar(Banco* B) { return carregar_clientes(B -> arquivo_clientes, &B -> clientes); }

int banco_salvar(const Banco* B) { return salvar_clientes(B -> arquivo_clientes, &B -> clientes); }

int banco_depositar(Banco* B, const char* conta, double valor) {
    int idx = lista_find_conta(&B -> clientes, conta);
    if (idx < 0) return 0;

    Cliente* c = &B -> clientes.dados[idx];
    if (!depositar(c, valor)) return 0;
    registrar_mov(B -> arquivo_mov, conta, "DEPOSITO", valor, c -> saldo);

    return banco_salvar(B);
}

void banco_listar_ordenado_por_conta(Banco* B) {
    if (B -> clientes.tam == 0) { printf("Lista vazia.\n"); return; }

    quicksort_conta(B -> clientes.dados, 0, (int)B -> clientes.tam - 1);

    printf("\n==== CLIENTES (Ordenado por Conta) ====\n");
    for (size_t i = 0; i < B -> clientes.tam; i++) {
        imprimir_dados_cliente(&B -> clientes.dados[i]);
    }
}

void banco_listar_ordenado_por_nome(Banco* B) {
    if (B -> clientes.tam == 0) { printf("Nenhum cliente cadastrado.\n"); return; }

    quicksort_nome(B -> clientes.dados, 0, (int)B -> clientes.tam - 1);

    printf("\n==== CLIENTES (Ordenado por Nome) ====\n");
    for (size_t i = 0; i < B -> clientes.tam; i++) {
        if (B -> clientes.dados[i].ativo) {
            imprimir_dados_cliente(&B -> clientes.dados[i]);
        }
    }
}

int banco_buscar_idx_por_conta(const Banco* B, const char* conta) { return lista_find_conta(&B -> clientes, conta); }

int banco_encerrar_conta(Banco* B, const char* conta, const char* senha) {
    int idx = banco_buscar_idx_por_conta(B, conta);
    if (idx < 0) { printf("Erro: Conta nao encotrada ou ja encerrada.\n"); return 0; }

    Cliente* c = &B -> clientes.dados[idx];
    if (strcmp(c -> senha, senha) != 0) { printf("Erro: Senha incorreta.\n"); return 0; }

    if (!encerrar_conta(&B -> clientes, conta)) {
        printf("Erro: O saldo da conta deve ser zero para o encerramento. Saldo atual: R$ %.2f\n", c -> saldo);
        return 0;
    }

    printf("Conta %s encerrada com sucesso.\n", conta);
    return banco_salvar(B);
}

int banco_consultar_saldo(const Banco* B, const char* conta, const char* senha) {
    int idx = banco_buscar_idx_por_conta(B, conta);

    if (idx < 0) {
        printf("Erro: Conta nao encontrada ou inativa.\n");
        return 0;
    }

    const Cliente* c = &B->clientes.dados[idx];

    if (strcmp(c->senha, senha) != 0) {
        printf("Erro: Senha incorreta.\n");
        return 0;
    }

    printf("\n--- CONSULTA DE SALDO ---\n");
    printf("Titular: %s\n", c->nome);
    printf("Conta: %s\n", c->conta);
    printf("Saldo Atual: R$ %.2f\n", c->saldo);

    return 1;
}