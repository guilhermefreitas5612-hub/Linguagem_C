#ifndef UTIL_H
#define UTIL_H
#include <stddef.h>
void lerTexto(const char *mensagem, char *destino, size_t tamanho);
int lerInteiro(const char *mensagem);
float lerPreco(const char *mensagem);
int textoVazio(const char *texto);
#endif
