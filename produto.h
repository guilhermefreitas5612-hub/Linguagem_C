#ifndef PRODUTO_H
#define PRODUTO_H
#include "estoque.h"
int indiceProdutoPorCodigo(int codigo, int incluirInativos);
void carregarProdutos(void);
void salvarProdutos(void);
void cadastrarProduto(void);
void listarProdutos(void);
void pesquisarProduto(void);
void alterarEstoque(void);
void removerProduto(void);
#endif
