#ifndef DAO_H
#define DAO_H

typedef struct {
    int id;
    char nome[50];
    int idade;
} Usuario;

int salvar_usuario(Usuario u);
Usuario* buscar_usuario_por_id(int id);

#endif
