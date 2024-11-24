#include "structs.h"
#define RED 1
#define BLACK 0
Info criaInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.palavraPortugues = malloc(strlen(palavra) + 1);
    strcpy(info.palavraPortugues, palavra);

    info.palavraIngles = NULL;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    return info;
}

PortuguesRB *criaNo(Info *informacao){
    PortuguesRB *novo = (PortuguesRB *)malloc(sizeof(PortuguesRB));
    novo->info = *informacao;
    novo->cor = 1;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}
int cor(PortuguesRB *raiz)
{
    int cor;

    if (raiz == NULL)
        cor = BLACK;
    else
        cor = raiz->cor;

    return cor;
}

void troca_cor(PortuguesRB **raiz)
{
  (*raiz)->cor = !(*raiz)->cor;
  if ((*raiz)->esq)
    (*raiz)->esq->cor = !(*raiz)->esq->cor;
  if ((*raiz)->dir)
    (*raiz)->dir->cor = !(*raiz)->dir->cor;
}

void balancear(PortuguesRB **raiz){
    if(cor((*raiz)->esq) == BLACK && cor((*raiz)->dir) == RED)
        rotacionarEsq(raiz);
    if(cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED)
        rotacionarDir(raiz);
    if(cor((*raiz)->esq) == RED && cor((*raiz)->esq) == RED)
        trocaCor(raiz);
}

int inserirRB(PortuguesRB **raiz, Info *informacao){
    
    int inseriu = 1;
    if(*raiz == NULL){
        *raiz = criaNo(informacao);
    }else{
        if (strcmp(informacao->palavraPortugues, (*raiz)->info.palavraPortugues) < 0)
        {
           inseriu = inserirRB((*raiz)->esq, informacao);
        }else{
            inseriu = inserirRB((*raiz)->dir, informacao);
        }
        balancear(raiz);
    }

}

int inserirArvRB(PortuguesRB **raiz, Info *informacao){
    int inseriu = inserirRB(raiz, informacao);
    if(inseriu){
        (*raiz)->cor = BLACK;
    }
    return inseriu;
}
