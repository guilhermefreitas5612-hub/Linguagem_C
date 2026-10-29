#include "categoria.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int indiceCategoriaPorId(int id) {
    for (int i=0; i<totalCategorias; i++)
        if (categorias[i].id==id) return i;
    return -1;
}

void salvarCategorias(void) {
    FILE *f=fopen(ARQ_CATEGORIAS,"w");
    if (!f) { puts("Aviso: nao foi possivel salvar categorias."); return; }
    for (int i=0;i<totalCategorias;i++)
        fprintf(f,"%d|%s\n",categorias[i].id,categorias[i].nome);
    fclose(f);
}

void carregarCategorias(void) {
    FILE *f=fopen(ARQ_CATEGORIAS,"r");
    char linha[256];
    if (!f) return;
    while (fgets(linha,sizeof linha,f) && totalCategorias<MAX_CATEGORIAS) {
        char *sep=strchr(linha,'|');
        if (!sep) continue;
        *sep++='\0';
        Categoria c;
        c.id=atoi(linha);
        sep[strcspn(sep,"\r\n")]='\0';
        snprintf(c.nome,sizeof c.nome,"%s",sep);
        if (c.id>0 && !textoVazio(c.nome) && indiceCategoriaPorId(c.id)<0)
            categorias[totalCategorias++]=c;
    }
    fclose(f);
}

void cadastrarCategoria(void) {
    Categoria nova;
    char nome[TAM_NOME];
    puts("\n=== CADASTRAR CATEGORIA ===");
    if (totalCategorias>=MAX_CATEGORIAS) {
        puts("Limite de categorias atingido.");
        return;
    }
    for (;;) {
        nova.id=lerInteiroPositivo("Codigo da categoria (positivo): ");
        if (indiceCategoriaPorId(nova.id)>=0) {
            puts("Esse codigo ja existe. Informe outro.");
            continue;
        }
        break;
    }
    for (;;) {
        lerTexto("Nome da categoria: ",nome,sizeof nome);
        if (textoVazio(nome)) {
            puts("O nome nao pode ficar em branco.");
            continue;
        }
        break;
    }
    snprintf(nova.nome,sizeof nova.nome,"%s",nome);
    categorias[totalCategorias++]=nova;
    salvarCategorias();
    printf("Categoria cadastrada: codigo %d - %s\n",nova.id,nova.nome);
}

void listarCategorias(void) {
    puts("\n=== LISTA DE CATEGORIAS ===");
    if (!totalCategorias) {
        puts("Nenhuma categoria cadastrada.");
        return;
    }
    printf("%-8s | %s\n","CODIGO","NOME");
    for (int i=0;i<totalCategorias;i++)
        printf("%-8d | %s\n",categorias[i].id,categorias[i].nome);
}
