#ifndef SERVICE_H
#define SERVICE_H

#include "dao.h"

int cadastrar_usuario(char* nome, int idade);
Usuario* obter_usuario(int id);

#endif
