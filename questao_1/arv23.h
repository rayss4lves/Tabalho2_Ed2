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
Info criaInfo(char *palavra, int unidade);
// Portugues23 **buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao);

//void imprimirArvore(Portugues23 *arvore);

//funções ingles
Inglesbin* createNode(const char* word, int unit);

// Função para inserir uma palavra em inglês na árvore binária de busca
Inglesbin* insertpalavraIngles(Inglesbin* root, const char* word, int unit);
void printBinaryTree(Inglesbin* root);
void imprimirTraducoes(Inglesbin *arvore, const char *palavraPortugues, int *primeira);
void imprimirInfoUnidade(Portugues23 *arvore, int unidade);
void imprimirArvore(Portugues23 *arvore, int nivel);
// void imprimirNodo(Portugues23 *no, int value, int nivel);




#endif