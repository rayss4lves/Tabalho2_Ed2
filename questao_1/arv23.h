#ifndef arv23_H
#define arv23_H

#include "arvbin.h"

/*-----------------------------------------------------------------------------------------------------*/

/* ESTRUTAS DAS ÁRVORES E DEMAIS */



typedef struct info {
    char *palavraPortugues;
    Inglesbin *palavraIngles;
    int unidade;
} Info;

typedef struct Portugues23{
    Info info1;
    Info info2;
    struct Portugues23 *cent;
    struct Portugues23 *dir;
    struct Portugues23 *esq;
    int nInfos;
} Portugues23;

/*-----------------------------------------------------------------------------------------------------*/

/* FUNÇÕES ADICIONAIS */

Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen);
Portugues23 *adicionaChave(Portugues23 *no, const Info *informacao, Portugues23 *filho);
Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 **filho);
int ehFolha(const Portugues23 *no);
Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai);
void freeTree(Portugues23 *no);
void exibir_tree23(const Portugues23 *raiz);
void adicionarTraducaoEmIngles(Info *info, const char *palavraIng);
Info criaInfo(const char *palavra, const int unidade);
Portugues23 **buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao);




#endif