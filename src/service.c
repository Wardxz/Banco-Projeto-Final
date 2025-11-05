#include <stdio.h>
#include <string.h>
#include "service.h"

// Função que contém a lógica de negócio
int cadastrar_usuario(char* nome, int idade) {
    if (idade < 0) {
        printf("Idade inválida!\n");
        return 0;
    }

    Usuario u;
    u.id = idade + (rand() % 1000);  // Simulação de ID
    strcpy(u.nome, nome);
    u.idade = idade;

    return salvar_usuario(u);
}

Usuario* obter_usuario(int id) {
    return buscar_usuario_por_id(id);
}
