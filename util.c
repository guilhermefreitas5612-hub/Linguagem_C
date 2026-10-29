#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void lerTexto(const char *mensagem, char *destino, size_t tamanho) {
    if (tamanho == 0) return;
    printf("%s", mensagem);
    if (!fgets(destino, (int)tamanho, stdin)) { destino[0] = '\0'; return; }
    size_t n = strlen(destino);
    if (n && destino[n-1] == '\n') destino[n-1] = '\0';
    else { int c; while ((c=getchar())!='\n' && c!=EOF) {} }
}
int textoVazio(const char *texto) {
    while (*texto) { if (!isspace((unsigned char)*texto)) return 0; texto++; }
    return 1;
}
int lerInteiro(const char *mensagem) {
    char linha[128], *fim;
    long valor;
    for (;;) {
        lerTexto(mensagem, linha, sizeof linha);
        errno=0; valor=strtol(linha,&fim,10);
        while (isspace((unsigned char)*fim)) fim++;
        if (linha[0] && *fim=='\0' && errno==0 && valor>=-2147483647L-1 && valor<=2147483647L)
            return (int)valor;
        puts("Entrada invalida. Digite um numero inteiro.");
    }
}
float lerPreco(const char *mensagem) {
    char linha[128], *fim;
    float valor;
    for (;;) {
        lerTexto(mensagem, linha, sizeof linha);
        for (size_t i=0; linha[i]; i++) if (linha[i]==',') linha[i]='.';
        errno=0; valor=strtof(linha,&fim);
        while (isspace((unsigned char)*fim)) fim++;
        if (linha[0] && *fim=='\0' && errno==0 && valor>0.0f) return valor;
        puts("Preco invalido. Digite um valor maior que zero.");
    }
}
