#ifndef ESTOQUE_H
#define ESTOQUE_H
#define MAX_CATEGORIAS 100
#define MAX_PRODUTOS 500
#define TAM_NOME 100
#define ARQ_CATEGORIAS "categorias.txt"
#define ARQ_PRODUTOS "produtos.txt"

typedef struct { int id; char nome[TAM_NOME]; } Categoria;
typedef struct {
    int codigo;
    char nome[TAM_NOME];
    float preco;
    int quantidade;
    int idCategoria;
    int ativo;
} Produto;

extern Categoria categorias[MAX_CATEGORIAS];
extern Produto produtos[MAX_PRODUTOS];
extern int totalCategorias, totalProdutos;

int indiceCategoriaPorId(int id);
int indiceProdutoPorCodigo(int codigo, int incluirInativos);
void carregarCategorias(void);
void salvarCategorias(void);
void cadastrarCategoria(void);
void listarCategorias(void);
void carregarProdutos(void);
void salvarProdutos(void);
void cadastrarProduto(void);
void listarProdutos(void);
void pesquisarProduto(void);
void alterarEstoque(void);
void removerProduto(void);
#endif
