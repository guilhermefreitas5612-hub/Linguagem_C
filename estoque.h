#ifndef ESTOQUE_H
#define ESTOQUE_H

#define MAX_CATEGORIAS 100
#define MAX_PRODUTOS 500
#define TAM_NOME 100
#define ARQ_CATEGORIAS "categorias.txt"
#define ARQ_PRODUTOS "produtos.txt"

typedef struct {
    int id;
    char nome[TAM_NOME];
} Categoria;

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    float preco;
    int quantidade;
    int idCategoria;
    char prateleira;
    int ativo;
} Produto;

extern Categoria categorias[MAX_CATEGORIAS];
extern Produto produtos[MAX_PRODUTOS];
extern int totalCategorias;
extern int totalProdutos;

#endif
