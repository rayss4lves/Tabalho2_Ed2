#ifndef arv23_H
#define arv23_H

#include "structs.h"

/*-----------------------------------------------------------------------------------------------------*/

/* ESTRUTAS DAS ÁRVORES E DEMAIS */



/*-----------------------------------------------------------------------------------------------------*/

/* FUNÇÕES ADICIONAIS */

Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen);
Portugues23 *adicionaChave(Portugues23 *no, const Info *informacao, Portugues23 *filho);
Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 **filho);
int ehFolha(const Portugues23 *no);
Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai);
void freeTree(Portugues23 *no);
void exibir_tree23(const Portugues23 *raiz);
void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade);
Info criaInfo(char *palavra, char *palavraIngles, int unidade);


void imprimirInfoUnidade(Portugues23 *arvore, int unidade);
void exibir_traducao_Portugues(Portugues23 **raiz, const char *palavraPortugues);
Portugues23 *BuscarPalavra(Portugues23 **no, const char *palavraPortugues);

Inglesbin* createNode(const char* word, int unit);

// Função para inserir uma palavra em inglês na árvore binária de busca
Inglesbin* insertpalavraIngles(Inglesbin* root, const char* word, int unit);
void printBinaryTree(Inglesbin* root);

void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortuguês);
void adicionarTraducao(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, int unidade);

int removerPalavraIngles(Inglesbin *raiz, char *palavra);
// Inglesbin* removeInglesbin(Inglesbin* root, const char* word, int unit);
void BuscarPalavraIngles(Portugues23 **no, char *palavraIngles);

int ehFolhas(Inglesbin *raiz);

Inglesbin *soUmFilho(Inglesbin *raiz);

Inglesbin *menorFilho(Inglesbin *raiz);


#endif