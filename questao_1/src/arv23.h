#ifndef arv23_H
#define arv23_H

#include "structs.h"
#include "arvbin.h"


Portugues23 *BuscarPalavra(Portugues23 **no, const char *palavraPortugues);
int inserirPalavraPortugues(Portugues23 **arvore, char *palavraPortugues, char *palavraIngles, int unidade);
void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade);

Info criaInfo(char *palavra, char *palavraIngles, int unidade);
Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen);
void adicionaChave(Portugues23 **no, const Info *informacao, Portugues23 *filho);
Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 *filho);
int ehFolha(const Portugues23 *no);
Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai);


void freeInfo2_3(Info *info);
void freeTree(Portugues23 *no);

void exibir_tree23(const Portugues23 *raiz);
void imprimirPalavrasUnidade(Portugues23 *arvore, int unidade);
void exibir_traducao_Portugues(Portugues23 **raiz, const char *palavraPortugues);
void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortuguês);

void no23_desalocar(Portugues23 **no);
int arvore23_rebalancear(Portugues23 **raiz, char *info, Portugues23 **maior);
Portugues23 *no23_juntar(Portugues23 *filho1, Info info, Portugues23 *maior, Portugues23 **raiz);
int possivel_remover(Portugues23 *raiz);
int arvore23_remover2(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior);
int arvore23_remover1(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior);
int arvore23_remover_nao_folha2(Portugues23 **origem, Portugues23* raiz, Info *info, Portugues23 *filho1, Portugues23 *filho2, Portugues23 **maior);
int arvore23_remover_nao_folha1(Portugues23 **origem, Portugues23* raiz, Info *info, Portugues23 *filho1, Portugues23 *filho2, Portugues23 **maior);

int arvore_2_3_remover(Portugues23 **raiz, char *info);
int ehInfo1(Portugues23 *raiz, char *valor);
int ehInfo2(Portugues23 *raiz, char *valor);


#endif