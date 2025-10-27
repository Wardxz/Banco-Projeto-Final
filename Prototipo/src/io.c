#include <stdio.h>
#include <string.h>
#include "io.h"

void ler_linha(char* buf, size_t tam) {
    if (fgets(buf, tam, stdin)) buf[strspn(buf, "\n")] = 0;
}