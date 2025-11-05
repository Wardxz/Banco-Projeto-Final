#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dao.h"

static Usuario banco[100];
static int contador = 0;

int salvar_usuario(Usuario u) {
    if (contador >= 100) return 0;
    banco[contador++] = u;
    return 1;
}

Usuario* buscar_usuario_por_id(int id) {
    for (int i = 0; i < contador; i++) {
        if (banco[i].id == id)
            return &banco[i];
    }
    return NULL;
}
