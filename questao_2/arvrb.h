#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED 1
#define BLACK 0

//passar as funções para o arquivo .c
Inglesbin *createNode(char *palavraIngles, int unidade)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidade = unidade;
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
Inglesbin *insertpalavraIngles(Inglesbin *root, char *palavraIngles, int unidade)
{
    Inglesbin *result;
    if (root == NULL)
    {
        result = createNode(palavraIngles, unidade);
    }
    else
    {
        if (strcmp(palavraIngles, root->palavraIngles) < 0)
        {
            root->esq = insertpalavraIngles(root->esq, palavraIngles, unidade);
        }
        else if (strcmp(palavraIngles, root->palavraIngles) > 0)
        {
            root->dir = insertpalavraIngles(root->dir, palavraIngles, unidade);
        }
        result = root;
    }
    return result;
}

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



PortuguesRB *rotacao_direita(PortuguesRB *raiz){
    PortuguesRB *aux = raiz->esq;
    raiz->esq = aux->dir;
    aux->dir = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

PortuguesRB *rotacao_esquerda(PortuguesRB *raiz){
    PortuguesRB *aux = raiz->dir;
    raiz->dir = aux->esq;
    aux->esq = raiz;
    aux->cor = raiz->cor;
    raiz->cor = RED;
    return aux;
}

PortuguesRB *balancear(PortuguesRB **raiz){
    if(cor((*raiz)->esq) == BLACK && cor((*raiz)->dir) == RED)
        *raiz = rotacao_esquerda(*raiz);
    if(cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED)
        *raiz = rotacao_direita(*raiz);
    if(cor((*raiz)->esq) == RED && cor((*raiz)->dir) == RED)
        troca_cor(raiz);
    return *raiz;
}

int inserirRB(PortuguesRB **raiz, Info *informacao){
    
    int inseriu = 1;
    if(*raiz == NULL){
        *raiz = criaNo(informacao);
    }else{
        if (strcmp(informacao->palavraPortugues, (*raiz)->info.palavraPortugues) < 0)
        {
           inseriu = inserirRB(&(*raiz)->esq, informacao);
        }else{
            inseriu = inserirRB(&(*raiz)->dir, informacao);
        }
        balancear(&(*raiz));
    }
    
    return inseriu;
}

int inserirArvRB(PortuguesRB **raiz, Info *informacao){
    int inseriu = inserirRB(raiz, informacao);
    if(inseriu){
        (*raiz)->cor = BLACK;
    }
    return inseriu;
}

void exibirArvore(PortuguesRB *raiz){
	if(raiz){
		exibirArvore(raiz->esq);
		printf("Cor - %d\n", raiz->cor);
		printf("Valor - %s\n", raiz->info.palavraPortugues);
		exibirArvore(raiz->dir);
	}
}

void moveEsqVermelha(PortuguesRB **raiz){
	troca_cor(raiz);

	if((*raiz)->dir && cor((*raiz)->dir->esq) == RED){
		*raiz = rotacao_direita((*raiz)->dir);
		*raiz = rotacao_esquerda((*raiz));
		troca_cor(raiz);
	}
}

void moveDirVermelha(PortuguesRB **raiz){
	troca_cor(raiz);

	if((*raiz)->esq && cor((*raiz)->esq->esq) == RED){
		*raiz = rotacao_direita((*raiz)->dir);
		troca_cor(raiz);
	}
}

void removeMenor(PortuguesRB **raiz){
	if(!((*raiz)->esq)){
		free(*raiz);
		*raiz = NULL;
	}else{
		if(cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
			moveEsqVermelha(raiz);
		
		removeMenor(&(*raiz)->esq);
		balancear(raiz);
	}
}

PortuguesRB *procuraMenor(PortuguesRB *raiz){
	PortuguesRB *menor;
	menor = raiz;

	if(raiz)
		if(raiz->esq)
			menor = procuraMenor(raiz->esq);
	

	return menor;
}

int removerNoArvVP(PortuguesRB **raiz, char *valor){
	int existe = 0;

	if(*raiz){
		if(strcmp(valor, (*raiz)->info.palavraPortugues) < 0){
			if((*raiz)->esq && cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
				moveEsqVermelha(raiz);

			existe = removerNoArvVP(&(*raiz)->esq,valor);
		}else{
			if(cor((*raiz)->esq) == RED)
				*raiz = rotacao_direita((*raiz));
			
			if(strcmp(valor, (*raiz)->info.palavraPortugues) == 0 && (*raiz)->dir == NULL){
				free(*raiz);
				*raiz = NULL;
				
				existe = 1;
			}else{
				if((*raiz)->dir && cor((*raiz)->dir) == BLACK && cor((*raiz)->dir->esq) == BLACK)
					moveDirVermelha(raiz);

				if(strcmp(valor, (*raiz)->info.palavraPortugues) == 0){
					PortuguesRB *aux;
					aux = procuraMenor((*raiz)->dir);
					(*raiz)->info = aux->info;
					removeMenor(&(*raiz)->dir);

					existe = 1;
				}else{
					existe = removerNoArvVP(&(*raiz)->dir, valor);
				}
			}
		}
	}

	return existe;
}
