/*
 * Projeto Final - Sistema de Estoque
 * Compilar: gcc main.c produto.c categoria.c util.c -o sistema_estoque.exe
 */
#include <stdio.h>
#include <locale.h>
#include "estoque.h"
#include "util.h"

Categoria categorias[MAX_CATEGORIAS];
Produto produtos[MAX_PRODUTOS];
int totalCategorias=0, totalProdutos=0;

static void exibirMenu(void) {
    puts("\n====================================");
    puts("       SISTEMA DE ESTOQUE - C");
    puts("====================================");
    puts("1 - Cadastrar Produto");
    puts("2 - Listar Produtos");
    puts("3 - Pesquisar Produto");
    puts("4 - Alterar Estoque de um Produto");
    puts("5 - Remover Produto");
    puts("6 - Categorias");
    puts("7 - Sair");
    puts("------------------------------------");
}
static void menuCategorias(void) {
    int opcao;
    do {
        puts("\n=== CATEGORIAS ===");
        puts("1 - Cadastrar Categoria");
        puts("2 - Listar Categorias");
        puts("3 - Voltar");
        opcao=lerInteiro("Escolha uma opcao: ");
        switch(opcao) {
            case 1: cadastrarCategoria(); break;
            case 2: listarCategorias(); break;
            case 3: break;
            default: puts("Opcao invalida. Escolha de 1 a 3.");
        }
    } while(opcao!=3);
}
int main(void) {
    int opcao;
    setlocale(LC_ALL,"");
    carregarCategorias();
    carregarProdutos();
    do {
        exibirMenu();
        opcao=lerInteiro("Escolha uma opcao: ");
        switch(opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: pesquisarProduto(); break;
            case 4: alterarEstoque(); break;
            case 5: removerProduto(); break;
            case 6: menuCategorias(); break;
            case 7: salvarCategorias(); salvarProdutos(); puts("Programa encerrado. Ate mais!"); break;
            default: puts("Opcao invalida. Escolha de 1 a 7.");
        }
    } while(opcao!=7);
    return 0;
}
