/*
 * Projeto Final - Sistema de Estoque
 * Linguagem: C
 *
 * Funcionalidades:
 * 1. Cadastrar produto
 * 2. Listar produtos
 * 3. Pesquisar produto pelo codigo
 * 4. Cadastrar categoria
 * 5. Listar categorias
 *
 * Os dados ficam armazenados em arquivos texto na pasta do programa:
 * categorias.txt e produtos.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <errno.h>

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
} Produto;

Categoria categorias[MAX_CATEGORIAS];
Produto produtos[MAX_PRODUTOS];
int totalCategorias = 0;
int totalProdutos = 0;
int proximoIdCategoria = 1;
int proximoCodigoProduto = 1;

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerTexto(const char *mensagem, char *destino, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        } else {
            limparEntrada();
        }
    } else {
        destino[0] = '\0';
    }
}

int lerInteiro(const char *mensagem) {
    char linha[100];
    char *fim;
    long valor;

    for (;;) {
        lerTexto(mensagem, linha, sizeof(linha));
        errno = 0;
        valor = strtol(linha, &fim, 10);
        while (*fim == ' ' || *fim == '\t') fim++;

        if (linha[0] != '\0' && *fim == '\0' && errno == 0 &&
            valor >= -2147483647L - 1 && valor <= 2147483647L) {
            return (int)valor;
        }
        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
}

float lerPreco(const char *mensagem) {
    char linha[100];
    char *fim;
    float valor;
    size_t i;

    for (;;) {
        lerTexto(mensagem, linha, sizeof(linha));
        for (i = 0; linha[i] != '\0'; i++) {
            if (linha[i] == ',') linha[i] = '.';
        }
        errno = 0;
        valor = strtof(linha, &fim);
        while (*fim == ' ' || *fim == '\t') fim++;

        if (linha[0] != '\0' && *fim == '\0' && errno == 0 &&
            valor >= 0.0f) {
            return valor;
        }
        printf("Preco invalido. Digite um valor igual ou maior que zero.\n");
    }
}

void salvarCategorias(void) {
    FILE *arquivo = fopen(ARQ_CATEGORIAS, "w");
    int i;
    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel salvar as categorias.\n");
        return;
    }
    for (i = 0; i < totalCategorias; i++) {
        fprintf(arquivo, "%d|%s\n", categorias[i].id, categorias[i].nome);
    }
    fclose(arquivo);
}

void salvarProdutos(void) {
    FILE *arquivo = fopen(ARQ_PRODUTOS, "w");
    int i;
    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel salvar os produtos.\n");
        return;
    }
    for (i = 0; i < totalProdutos; i++) {
        fprintf(arquivo, "%d|%s|%.2f|%d|%d\n",
                produtos[i].codigo, produtos[i].nome, produtos[i].preco,
                produtos[i].quantidade, produtos[i].idCategoria);
    }
    fclose(arquivo);
}

void carregarCategorias(void) {
    FILE *arquivo = fopen(ARQ_CATEGORIAS, "r");
    char linha[256];
    if (arquivo == NULL) return;

    while (fgets(linha, sizeof(linha), arquivo) != NULL &&
           totalCategorias < MAX_CATEGORIAS) {
        Categoria c;
        char *separador = strchr(linha, '|');
        if (separador == NULL) continue;
        *separador = '\0';
        c.id = atoi(linha);
        separador++;
        separador[strcspn(separador, "\r\n")] = '\0';
        snprintf(c.nome, sizeof(c.nome), "%s", separador);
        if (c.id > 0 && c.nome[0] != '\0') {
            categorias[totalCategorias++] = c;
            if (c.id >= proximoIdCategoria) proximoIdCategoria = c.id + 1;
        }
    }
    fclose(arquivo);
}

void carregarProdutos(void) {
    FILE *arquivo = fopen(ARQ_PRODUTOS, "r");
    char linha[512];
    if (arquivo == NULL) return;

    while (fgets(linha, sizeof(linha), arquivo) != NULL &&
           totalProdutos < MAX_PRODUTOS) {
        Produto p;
        char *parte;
        parte = strtok(linha, "|");
        if (!parte) continue;
        p.codigo = atoi(parte);

        parte = strtok(NULL, "|");
        if (!parte) continue;
        snprintf(p.nome, sizeof(p.nome), "%s", parte);

        parte = strtok(NULL, "|");
        if (!parte) continue;
        p.preco = (float)atof(parte);

        parte = strtok(NULL, "|");
        if (!parte) continue;
        p.quantidade = atoi(parte);

        parte = strtok(NULL, "|\r\n");
        if (!parte) continue;
        p.idCategoria = atoi(parte);

        if (p.codigo > 0 && p.nome[0] != '\0') {
            produtos[totalProdutos++] = p;
            if (p.codigo >= proximoCodigoProduto)
                proximoCodigoProduto = p.codigo + 1;
        }
    }
    fclose(arquivo);
}

int indiceCategoriaPorId(int id) {
    int i;
    for (i = 0; i < totalCategorias; i++) {
        if (categorias[i].id == id) return i;
    }
    return -1;
}

int indiceProdutoPorCodigo(int codigo) {
    int i;
    for (i = 0; i < totalProdutos; i++) {
        if (produtos[i].codigo == codigo) return i;
    }
    return -1;
}

void cadastrarCategoria(void) {
    Categoria nova;
    int i;

    printf("\n=== CADASTRAR CATEGORIA ===\n");
    if (totalCategorias >= MAX_CATEGORIAS) {
        printf("Limite de categorias atingido.\n");
        return;
    }

    lerTexto("Nome da categoria: ", nova.nome, sizeof(nova.nome));
    if (nova.nome[0] == '\0') {
        printf("O nome da categoria nao pode ficar vazio.\n");
        return;
    }

    for (i = 0; i < totalCategorias; i++) {
        if (strcmp(categorias[i].nome, nova.nome) == 0) {
            printf("Essa categoria ja esta cadastrada.\n");
            return;
        }
    }

    nova.id = proximoIdCategoria++;
    categorias[totalCategorias++] = nova;
    salvarCategorias();
    printf("Categoria cadastrada com sucesso! ID: %d | Nome: %s\n",
           nova.id, nova.nome);
}

void listarCategorias(void) {
    int i;
    printf("\n=== LISTA DE CATEGORIAS ===\n");
    if (totalCategorias == 0) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }

    printf("%-8s | %s\n", "ID", "NOME");
    printf("-------------------------------\n");
    for (i = 0; i < totalCategorias; i++) {
        printf("%-8d | %s\n", categorias[i].id, categorias[i].nome);
    }
}

void cadastrarProduto(void) {
    Produto novo;
    int idCategoria;

    printf("\n=== CADASTRAR PRODUTO ===\n");
    if (totalCategorias == 0) {
        printf("Cadastre uma categoria antes de cadastrar produtos.\n");
        return;
    }
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido.\n");
        return;
    }

    lerTexto("Nome do produto: ", novo.nome, sizeof(novo.nome));
    if (novo.nome[0] == '\0') {
        printf("O nome do produto nao pode ficar vazio.\n");
        return;
    }

    novo.preco = lerPreco("Preco (ex.: 59,90): ");
    do {
        novo.quantidade = lerInteiro("Quantidade em estoque: ");
        if (novo.quantidade < 0)
            printf("A quantidade nao pode ser negativa.\n");
    } while (novo.quantidade < 0);

    listarCategorias();
    idCategoria = lerInteiro("Digite o ID da categoria do produto: ");
    if (indiceCategoriaPorId(idCategoria) == -1) {
        printf("Categoria nao encontrada. Produto nao cadastrado.\n");
        return;
    }

    novo.codigo = proximoCodigoProduto++;
    novo.idCategoria = idCategoria;
    produtos[totalProdutos++] = novo;
    salvarProdutos();

    printf("Produto cadastrado com sucesso!\n");
    printf("Codigo: %d | Nome: %s\n", novo.codigo, novo.nome);
}

void listarProdutos(void) {
    int i, indiceCat;
    printf("\n=== LISTA DE PRODUTOS ===\n");
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("%-8s | %-24s | %-10s | %-10s | %s\n",
           "CODIGO", "PRODUTO", "PRECO (R$)", "QUANTIDADE", "CATEGORIA");
    printf("-------------------------------------------------------------------------------\n");
    for (i = 0; i < totalProdutos; i++) {
        indiceCat = indiceCategoriaPorId(produtos[i].idCategoria);
        printf("%-8d | %-24.24s | %10.2f | %-10d | %s\n",
               produtos[i].codigo, produtos[i].nome, produtos[i].preco,
               produtos[i].quantidade,
               indiceCat >= 0 ? categorias[indiceCat].nome : "Sem categoria");
    }
}

void pesquisarProduto(void) {
    int codigo, indice, indiceCat;
    printf("\n=== PESQUISAR PRODUTO ===\n");
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado para pesquisar.\n");
        return;
    }

    codigo = lerInteiro("Digite o codigo do produto: ");
    indice = indiceProdutoPorCodigo(codigo);
    if (indice == -1) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        return;
    }

    indiceCat = indiceCategoriaPorId(produtos[indice].idCategoria);
    printf("\nProduto encontrado:\n");
    printf("Codigo: %d\n", produtos[indice].codigo);
    printf("Nome: %s\n", produtos[indice].nome);
    printf("Preco: R$ %.2f\n", produtos[indice].preco);
    printf("Quantidade em estoque: %d\n", produtos[indice].quantidade);
    printf("Categoria: %s\n",
           indiceCat >= 0 ? categorias[indiceCat].nome : "Sem categoria");
}

void exibirMenu(void) {
    printf("\n====================================\n");
    printf("       SISTEMA DE ESTOQUE - C\n");
    printf("====================================\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Pesquisar produto\n");
    printf("4 - Cadastrar categoria\n");
    printf("5 - Listar categorias\n");
    printf("0 - Sair\n");
    printf("------------------------------------\n");
}

int main(void) {
    int opcao;
    setlocale(LC_ALL, "");

    carregarCategorias();
    carregarProdutos();

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                cadastrarProduto();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                pesquisarProduto();
                break;
            case 4:
                cadastrarCategoria();
                break;
            case 5:
                listarCategorias();
                break;
            case 0:
                printf("Programa encerrado. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida. Escolha uma opcao de 0 a 5.\n");
        }
    } while (opcao != 0);

    return 0;
}
