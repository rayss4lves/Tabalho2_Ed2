#ifndef ARVBIN_H
#define ARVBIN_H

#include "structs.h"
#include "arv23.h"

Inglesbin* createNode(const char* word, char *unidade);

// Função para inserir uma palavra em inglês na arvore binaria de busca
int insertpalavraIngles(Inglesbin **root, const char *palavraIngles, char *unidade);

void adicionarTraducao(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, char *unidade);

int ehFolhas(Inglesbin *raiz);

Inglesbin *soUmFilho(Inglesbin *raiz);

Inglesbin *menorFilho(Inglesbin *raiz);

void printBinaryTree(Inglesbin *root);

int removerPalavraIngles(Inglesbin **raiz, char *palavra);
void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, char *unidade);

void free_arvore_binaria(Inglesbin *raiz);

#endif