#include "produto.h"
#include "categoria.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

int indiceProdutoPorCodigo(int codigo, int incluirInativos) {
    for (int i=0; i<totalProdutos; i++)
        if (produtos[i].codigo==codigo &&
            (incluirInativos || produtos[i].ativo))
            return i;
    return -1;
}

static int categoriaExiste(int id) {
    return indiceCategoriaPorId(id)>=0;
}

static int prateleiraValida(char prateleira) {
    prateleira=(char)toupper((unsigned char)prateleira);
    return prateleira=='A' || prateleira=='B' ||
           prateleira=='C' || prateleira=='D';
}

static void exibirProduto(const Produto *p) {
    int c=indiceCategoriaPorId(p->idCategoria);
    printf("Codigo: %d\nNome: %s\nPrateleira: %c\nPreco: R$ %.2f\nQuantidade: %d\nDisponibilidade: %s\nCategoria: %s\n",
        p->codigo, p->nome, p->prateleira, p->preco, p->quantidade,
        p->quantidade>0 ? "Disponivel" : "Indisponivel",
        c>=0 ? categorias[c].nome : "Sem categoria");
}

void salvarProdutos(void) {
    FILE *f=fopen(ARQ_PRODUTOS,"w");
    if (!f) {
        puts("Aviso: nao foi possivel salvar produtos.");
        return;
    }
    for (int i=0; i<totalProdutos; i++)
        fprintf(f,"%d|%s|%.2f|%d|%d|%c|%d\n",
            produtos[i].codigo, produtos[i].nome, produtos[i].preco,
            produtos[i].quantidade, produtos[i].idCategoria,
            produtos[i].prateleira, produtos[i].ativo);
    fclose(f);
}

void carregarProdutos(void) {
    FILE *f=fopen(ARQ_PRODUTOS,"r");
    char linha[512];
    if (!f) return;

    while (fgets(linha,sizeof linha,f) && totalProdutos<MAX_PRODUTOS) {
        Produto p;
        char *parte=strtok(linha,"|");
        if (!parte) continue;
        p.codigo=atoi(parte);

        parte=strtok(NULL,"|");
        if (!parte) continue;
        snprintf(p.nome,sizeof p.nome,"%s",parte);

        parte=strtok(NULL,"|");
        if (!parte) continue;
        p.preco=(float)atof(parte);

        parte=strtok(NULL,"|");
        if (!parte) continue;
        p.quantidade=atoi(parte);

        parte=strtok(NULL,"|");
        if (!parte) continue;
        p.idCategoria=atoi(parte);

        /*
         * Formato atual: ...|idCategoria|prateleira|ativo
         * Compatibilidade: registros antigos ...|idCategoria|ativo
         * recebem prateleira A como valor inicial.
         */
        parte=strtok(NULL,"|");
        if (!parte) continue;
        if (parte[1]=='\0' && prateleiraValida(parte[0])) {
            p.prateleira=(char)toupper((unsigned char)parte[0]);
            parte=strtok(NULL,"|");
            if (!parte) continue;
            p.ativo=atoi(parte);
        } else {
            p.prateleira='A';
            p.ativo=atoi(parte);
        }

        p.nome[strcspn(p.nome,"\r\n")]='\0';
        if (p.codigo>0 && !textoVazio(p.nome) && p.preco>0 &&
            p.quantidade>=0 && prateleiraValida(p.prateleira) &&
            (p.ativo==0 || p.ativo==1) &&
            indiceProdutoPorCodigo(p.codigo,1)<0)
            produtos[totalProdutos++]=p;
    }
    fclose(f);
}

void cadastrarProduto(void) {
    Produto p;
    char nome[TAM_NOME];
    char prateleira[16];

    puts("\n=== CADASTRAR PRODUTO ===");
    if (!totalCategorias) {
        puts("Cadastre uma categoria antes de cadastrar produtos.");
        return;
    }
    if (totalProdutos>=MAX_PRODUTOS) {
        puts("Limite de produtos atingido.");
        return;
    }

    for (;;) {
        p.codigo=lerInteiroPositivo("Codigo do produto (positivo): ");
        if (indiceProdutoPorCodigo(p.codigo,1)>=0) {
            puts("Esse codigo ja foi usado, inclusive por produto removido.");
            continue;
        }
        break;
    }

    for (;;) {
        lerTexto("Nome do produto: ",nome,sizeof nome);
        if (textoVazio(nome)) {
            puts("O nome nao pode ficar em branco.");
            continue;
        }
        break;
    }
    snprintf(p.nome,sizeof p.nome,"%s",nome);

    do {
        lerTexto("Prateleira (A, B, C ou D): ",prateleira,sizeof prateleira);
        if (strlen(prateleira)!=1 ||
            !prateleiraValida(prateleira[0]))
            puts("Prateleira invalida. Informe somente A, B, C ou D.");
    } while (strlen(prateleira)!=1 ||
             !prateleiraValida(prateleira[0]));
    p.prateleira=(char)toupper((unsigned char)prateleira[0]);

    p.preco=lerPreco("Preco unitario (maior que zero): ");
    p.quantidade=lerInteiroNaoNegativo("Quantidade inicial (zero ou maior): ");

    listarCategorias();
    do {
        p.idCategoria=lerInteiroPositivo("Codigo da categoria: ");
        if (!categoriaExiste(p.idCategoria))
            puts("Categoria inexistente. Escolha uma cadastrada.");
    } while (!categoriaExiste(p.idCategoria));

    p.ativo=1;
    produtos[totalProdutos++]=p;
    salvarProdutos();
    puts("Produto cadastrado com sucesso:");
    exibirProduto(&p);
}

void listarProdutos(void) {
    int encontrados=0;
    puts("\n=== LISTA DE PRODUTOS ===");
    for (int i=0; i<totalProdutos; i++) {
        if (produtos[i].ativo) {
            puts("------------------------------");
            exibirProduto(&produtos[i]);
            encontrados++;
        }
    }
    if (!encontrados) puts("Nenhum produto ativo cadastrado.");
}

void pesquisarProduto(void) {
    int opcao=lerInteiro("\nPesquisar por: 1 - Codigo | 2 - Nome: ");
    if (opcao==1) {
        int codigo=lerInteiroPositivo("Codigo do produto: ");
        int i=indiceProdutoPorCodigo(codigo,0);
        if (i<0) puts("Produto nao encontrado.");
        else exibirProduto(&produtos[i]);
    } else if (opcao==2) {
        char nome[TAM_NOME];
        int achou=0;
        lerTexto("Nome do produto: ",nome,sizeof nome);
        if (textoVazio(nome)) {
            puts("Nome de pesquisa nao pode ficar em branco.");
            return;
        }
        for (int i=0; i<totalProdutos; i++) {
            if (produtos[i].ativo && strcmp(produtos[i].nome,nome)==0) {
                exibirProduto(&produtos[i]);
                achou=1;
            }
        }
        if (!achou) puts("Produto nao encontrado.");
    } else {
        puts("Opcao de pesquisa invalida.");
    }
}

void alterarEstoque(void) {
    int codigo=lerInteiroPositivo("Codigo do produto: ");
    int i=indiceProdutoPorCodigo(codigo,0);
    if (i<0) {
        puts("Produto ativo nao encontrado.");
        return;
    }

    int tipo=lerInteiro("1 - Entrada | 2 - Saida: ");
    if (tipo!=1 && tipo!=2) {
        puts("Operacao invalida.");
        return;
    }

    int qtd=lerInteiroPositivo("Quantidade (maior que zero): ");
    if (tipo==1) {
        if (qtd>INT_MAX-produtos[i].quantidade) {
            puts("Quantidade excede o limite permitido.");
            return;
        }
        produtos[i].quantidade+=qtd;
    } else {
        if (qtd>produtos[i].quantidade) {
            puts("Estoque insuficiente. Quantidade nao alterada.");
            return;
        }
        produtos[i].quantidade-=qtd;
    }

    salvarProdutos();
    puts("Estoque atualizado:");
    exibirProduto(&produtos[i]);
}

void removerProduto(void) {
    int codigo=lerInteiroPositivo("Codigo do produto a remover: ");
    int i=indiceProdutoPorCodigo(codigo,1);
    if (i<0) {
        puts("Codigo nunca cadastrado. Nenhuma alteracao realizada.");
        return;
    }
    if (!produtos[i].ativo) {
        puts("Esse produto ja esta removido.");
        return;
    }

    produtos[i].ativo=0;
    salvarProdutos();
    puts("Produto removido. O registro foi mantido para impedir reutilizacao do codigo.");
}
