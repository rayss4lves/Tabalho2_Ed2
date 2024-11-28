#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED 1
#define BLACK 0

// passar as funções para o arquivo . c
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

PortuguesRB *criaNo(Info *informacao)
{
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

void rotacao_direita(PortuguesRB **raiz)
{
    PortuguesRB *aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = RED;
    (*raiz) = aux;
}

void rotacao_esquerda(PortuguesRB **raiz)
{
    PortuguesRB *aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    aux->cor = (*raiz)->cor;
    (*raiz)->cor = RED;
    (*raiz) = aux;
}

void balancear(PortuguesRB **raiz)
{
    if (*raiz)
    {
        if (cor((*raiz)->dir) == RED && cor((*raiz)->esq) == BLACK)
            rotacao_esquerda(raiz);
        if (cor((*raiz)->esq) == RED && cor((*raiz)->esq->esq) == RED)
            rotacao_direita(raiz);
        if (cor((*raiz)->esq) == RED && cor((*raiz)->dir) == RED)
            troca_cor(raiz);
    }
}

int inserirRB(PortuguesRB **raiz, Info *informacao)
{

    int inseriu = 1;
    if (*raiz == NULL)
    {
        *raiz = criaNo(informacao);
    }
    else
    {
        if (strcmp(informacao->palavraPortugues, (*raiz)->info.palavraPortugues) < 0)
        {
            inseriu = inserirRB(&(*raiz)->esq, informacao);
        }
        else
        {
            inseriu = inserirRB(&(*raiz)->dir, informacao);
        }
        balancear(&(*raiz));
    }

    return inseriu;
}

int inserirArvRB(PortuguesRB **raiz, Info *informacao)
{
    int inseriu = inserirRB(raiz, informacao);
    if (inseriu)
    {
        (*raiz)->cor = BLACK;
    }
    return inseriu;
}

void moveEsqVermelha(PortuguesRB **raiz)
{
    troca_cor(raiz);

    if ((*raiz)->dir && cor((*raiz)->dir->esq) == RED)
    {
        rotacao_direita(&(*raiz)->dir);
        rotacao_esquerda((raiz));
        troca_cor(raiz);
    }
}

void moveDirVermelha(PortuguesRB **raiz)
{
    troca_cor(raiz);

    if ((*raiz)->esq && cor((*raiz)->esq->esq) == RED)
    {
        rotacao_direita(raiz);
        troca_cor(raiz);
    }
}

void removeMenor(PortuguesRB **raiz)
{
    if (!((*raiz)->esq))
    {
        free(*raiz);
        *raiz = NULL;
    }
    else
    {
        if (cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
            moveEsqVermelha(raiz);

        removeMenor(&(*raiz)->esq);
        balancear(raiz);
    }
}

PortuguesRB *procuraMenor(PortuguesRB *raiz)
{
    PortuguesRB *menor;
    menor = raiz;

    if (raiz)
        if (raiz->esq)
            menor = procuraMenor(raiz->esq);

    return menor;
}

int removerNoArvVP(PortuguesRB **raiz, char *valor)
{
    int existe = 0;
    printf("\nEntrou \n");

    if (*raiz)
    {
        if (strcmp(valor, (*raiz)->info.palavraPortugues) < 0)
        {
            if ((*raiz)->esq && cor((*raiz)->esq) == BLACK && cor((*raiz)->esq->esq) == BLACK)
                moveEsqVermelha(raiz);

            existe = removerNoArvVP(&(*raiz)->esq, valor);
        }
        else
        {
            if (cor((*raiz)->esq) == RED)
                rotacao_direita(raiz);

            if (strcmp(valor, (*raiz)->info.palavraPortugues) == 0 && (*raiz)->dir == NULL)
            {
                free(*raiz);
                *raiz = NULL;

                existe = 1;
            }else{
                if ((*raiz)->dir && cor((*raiz)->dir) == BLACK && cor((*raiz)->dir->esq) == BLACK)
                moveDirVermelha(raiz);

                if (strcmp(valor, (*raiz)->info.palavraPortugues) == 0)
                {
                    PortuguesRB *aux;
                    aux = procuraMenor((*raiz)->dir);
                    (*raiz)->info = aux->info;
                    removeMenor(&(*raiz)->dir);

                    existe = 1;
                }
                else
                {
                    existe = removerNoArvVP(&(*raiz)->dir, valor);
                }
            }
        }
    }
    balancear(raiz);
    return existe;
}

void adicionarTraducaoEmIngles(PortuguesRB *raiz, char *palavraIng, int unidade)
{
    raiz->info.palavraIngles = insertpalavraIngles(raiz->info.palavraIngles, palavraIng, unidade);
}

PortuguesRB *BuscarPalavra(PortuguesRB **arvore, char *palavraPortugues)
{
    PortuguesRB *atual = NULL;

    if (*arvore != NULL)
    {
        if (strcmp(palavraPortugues, (*arvore)->info.palavraPortugues) == 0)
        {
            atual = *arvore;
        }
        else if (strcmp(palavraPortugues, (*arvore)->info.palavraPortugues) < 0)
        {
            atual = BuscarPalavra(&(*arvore)->esq, palavraPortugues);
        }
        else
        {
            atual = BuscarPalavra(&(*arvore)->dir, palavraPortugues);
        }
    }
    return atual;
}

// void exibir_tree23(const Portugues23 *raiz)
// {
//     if (raiz != NULL)
//     {
//         exibir_tree23(raiz->esq);
//         printf("Palavra (PT): %s", raiz->info.palavraPortugues);
//         if (raiz->info.palavraIngles != NULL && raiz->info.palavraIngles->palavraIngles != NULL)
//         {
//             printBinaryTree(raiz->info.palavraIngles);
//             printf("\n");
//         }
//         exibir_tree23(raiz->cent);
//         // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
//         if (raiz->nInfos == 2)
//         {
//             printf("Palavra (PT): %s", raiz->info2.palavraPortugues);

//             // Exibir a tradução em inglês, se houver
//             if (raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
//                 printBinaryTree(raiz->info2.palavraIngles);
//             printf("\n");
//         }

//         if (raiz->nInfos == 2)
//         {
//             exibir_tree23(raiz->dir);
//         }
//     }
// }

// void imprimirInfoUnidade(Portugues23 *arvore, int unidade)
// {
//     if (arvore)
//     {
//         imprimirInfoUnidade(arvore->esq, unidade);
//         imprimirTraducoes(arvore->info.palavraIngles, unidade, arvore->info.palavraPortugues);

//         imprimirInfoUnidade(arvore->cent, unidade);
//         if (arvore->nInfos == 2)
//         {
//             imprimirTraducoes(arvore->info2.palavraIngles, unidade, arvore->info2.palavraPortugues);
//         }
//         imprimirInfoUnidade(arvore->dir, unidade);
//     }
// }

// void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortuguês)
// {
//     if (node)
//     {
//         if (node->unidade == unidade)
//         {
//             printf("Palavra em Português: %s\n", palavraPortuguês);
//             printf("Palavra em inglês: %s\n", node->palavraIngles);
//         }
//         imprimirTraducoes(node->esq, unidade, palavraPortuguês);
//         imprimirTraducoes(node->dir, unidade, palavraPortuguês);
//     }
// }

// void exibir_traducao_Portugues(Portugues23 **raiz, const char *palavraPortugues)
// {
//     Portugues23 *resultado = NULL;
//     if (raiz != NULL)
//     {
//         resultado = BuscarPalavra(raiz, palavraPortugues);
//         if (resultado)
//         {
//             printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);

//             if (strcmp(palavraPortugues, resultado->info.palavraPortugues) == 0)
//             {
//                 printBinaryTree(resultado->info.palavraIngles);
//             }
//             else
//             {
//                 printBinaryTree(resultado->info2.palavraIngles);
//             }

//         }
//         else
//         {
//             printf("A palavra '%s' não foi encontrada na árvore.\n", palavraPortugues);
//         }
//     }
// }

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); // Percorre a árvore à esquerda
        printf("\n");
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s = Unidade: %d\n", root->palavraIngles, root->unidade);
        printBinaryTree(root->dir); // Percorre a árvore à direita
    }
}

void exibirArvore(PortuguesRB *raiz)
{
    if (raiz)
    {
        exibirArvore(raiz->esq);
        printf("Cor - %d\n", raiz->cor);
        printf("Valor - %s\n", raiz->info.palavraPortugues);
        printBinaryTree(raiz->info.palavraIngles);
        printf("\n");
        exibirArvore(raiz->dir);
    }
}

// ############################################# rEMOÇÃO ############################################

int ehFolhas(Inglesbin *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = NULL;

    if (raiz->dir == NULL)
    {
        aux = raiz->esq;
    }
    else if (raiz->esq == NULL)
    {
        aux = raiz->dir;
    }

    return aux;
}

Inglesbin *menorFilho(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, char *palavra)
{
    Inglesbin *endFilho;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0)
        {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            Inglesbin *aux = *raiz;
            if (ehFolhas(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(*raiz)) != NULL)
            {
                free(aux);
                *raiz = endFilho;
            }
            else
            {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidade = endFilho->unidade;

                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        }
        else if (strcmp(palavra, (*raiz)->palavraIngles) < 0)
        {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        }
        else
        {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}

void BuscarPalavraIngles(PortuguesRB **raiz, char *palavraIngles, int unidade)
{
    if (*raiz != NULL)
    {
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade);

        if ((*raiz)->info.palavraIngles != NULL && (*raiz)->info.palavraIngles->unidade == unidade)
            removerPalavraIngles(&(*raiz)->info.palavraIngles, palavraIngles);
        BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade);
    }
}