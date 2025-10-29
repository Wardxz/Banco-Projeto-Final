#include "banco.h"
#include "io.h"
#include "conta.h"
#include "quicksort.h"
#include <string.h>

static void copia_str(char* destino, const char* origem, size_t tamanho) {
    strncpy(destino, origem, tamanho);
    destino[tamanho - 1] = '\0';
}
void banco_init(Banco* B, const char* arq_clientes, const char* arq_mov) {
    lista_init(&B->clientes);
    copia_str(B->arquivo_clientes, arq_clientes, sizeof(B->arquivo_clientes));
    copia_str(B->arquivo_mov, arq_mov, sizeof(B->arquivo_mov));
}

void banco_free(Banco* B) {
    lista_free(&B->clientes);
}

int banco_carregar(Banco* B) {
    return carregar_clientes(B->arquivo_clientes, &B->clientes);
}

int banco_salvar(const Banco* B) {
    return salvar_clientes(B->arquivo_clientes, &B->clientes);
}

int banco_buscar_idx_por_conta(const Banco* B, const char* conta) {
    return lista_find_conta(&B->clientes, conta);
}

int banco_consultar_cliente(const Banco* B, const char* conta, Cliente* out) {
    int idx = banco_buscar_idx_por_conta(B, conta);
    if (idx < 0) return 0;

    if (out) {
        *out = B->clientes.dados[idx];
    }
    return 1;
}

int banco_abrir_conta(Banco* B, const Cliente* novo) {
    int ok = abrir_conta(&B->clientes, novo);
    if (!ok) return 0;

    return banco_salvar(B);
}

int banco_encerrar_conta(Banco* B, const char* conta) {
    int ok = encerrar_conta(&B->clientes, conta);
    if (!ok) return 0;

    return banco_salvar(B);
}

int banco_alterar_dados(Banco* B, const char* conta, const Cliente* novos) {
    int idx = banco_buscar_idx_por_conta(B, conta);
    if (idx < 0) return 0;

    Cliente* c = &B->clientes.dados[idx];
    Cliente novo = *c;

    copia_str(novo.nome,        novos->nome,        sizeof(novo.nome));
    copia_str(novo.cpf,         novos->cpf,         sizeof(novo.cpf));
    copia_str(novo.data_nasc,   novos->data_nasc,   sizeof(novo.data_nasc));
    copia_str(novo.telefone,    novos->telefone,    sizeof(novo.telefone));
    copia_str(novo.endereco,    novos->endereco,    sizeof(novo.endereco));
    copia_str(novo.cep,         novos->cep,         sizeof(novo.cep));
    copia_str(novo.local,       novos->local,       sizeof(novo.local));
    copia_str(novo.numero_casa, novos->numero_casa, sizeof(novo.numero_casa));
    copia_str(novo.bairro,      novos->bairro,      sizeof(novo.bairro));
    copia_str(novo.cidade,      novos->cidade,      sizeof(novo.cidade));
    copia_str(novo.estado,      novos->estado,      sizeof(novo.estado));
    copia_str(novo.senha,       novos->senha,       sizeof(novo.senha));

    *c = novo;
    return banco_salvar(B);
}
int banco_depositar(Banco* B, const char* conta, double valor) {
    int idx = banco_buscar_idx_por_conta(B, conta);
    if (idx < 0) return 0;

    if (!depositar(&B->clientes.dados[idx], valor))
        return 0;

    if (!registrar_mov(B->arquivo_mov, conta, "DEPOSITO", valor, B->clientes.dados[idx].saldo))
        return 0;

    return banco_salvar(B);
}

int banco_sacar(Banco* B, const char* conta, double valor) {
    int idx = banco_buscar_idx_por_conta(B, conta);
    if (idx < 0) return 0;

    if (!sacar(&B->clientes.dados[idx], valor))
        return 0;

    if (!registrar_mov(B->arquivo_mov, conta, "SAQUE", valor, B->clientes.dados[idx].saldo))
        return 0;

    return banco_salvar(B);
}

void banco_listar_ordenado_por_nome(Banco* B) {
    if (B->clientes.tam == 0) return;
    quicksort_nome(B->clientes.dados, 0, (int)B->clientes.tam - 1);
}

void banco_listar_ordenado_por_conta(Banco* B) {
    if (B->clientes.tam == 0) return;
    quicksort_conta(B->clientes.dados, 0, (int)B->clientes.tam - 1);
}