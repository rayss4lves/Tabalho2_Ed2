#include "arvbin.h"
#include "lista_encadeada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Inglesbin *createNode(const char *palavraIngles, char *unidade)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidades = NULL;
        inserir_lista_encadeada_unidade(&(novoNo->unidades), unidade);
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
int insertpalavraIngles(Inglesbin **root, const char *palavraIngles, char *unidade)
{
    int result = 0;
    if ((*root) == NULL)
    {
        Inglesbin *new = createNode(palavraIngles, unidade);
        *root = new;
    }
    else
    {
        if (strcmp(palavraIngles, (*root)->palavraIngles) < 0)
            insertpalavraIngles(&(*root)->esq, palavraIngles, unidade);
        else if (strcmp(palavraIngles, (*root)->palavraIngles) > 0)
            insertpalavraIngles(&(*root)->dir, palavraIngles, unidade);
        else
            inserir_lista_encadeada_unidade(&((*root)->unidades), unidade);
    }
    return result;
}

void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, char *unidade)
{
    insertpalavraIngles(&(info->palavraIngles), palavraIng, unidade);
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); // Percorre a árvore à esquerda
        printf("\n");
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s \n", root->palavraIngles);
        show_lista_encadeada_unidade(root->unidades);
        printBinaryTree(root->dir); // Percorre a árvore à direita
    }
}

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
                (*raiz)->unidades = endFilho->unidades;

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

void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, char *unidade)
{
    int removeu;
    if (*raiz != NULL)
    {
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade);

        if ((*raiz)->info1.palavraIngles != NULL && strcmp((*raiz)->info1.palavraIngles->palavraIngles, palavraIngles) == 0)
        {
            int achou = contar_lista_encadeada_unidade((*raiz)->info1.palavraIngles->unidades);
            // Verifica se há apenas uma unidade para essa palavra inglesa
            if ( achou > 1)
            {
                removeu = remover_lista_encadeada_unidade(&((*raiz)->info1.palavraIngles->unidades), unidade);
                if (removeu)
                    printf("Unidade %s removida para a palavra %s\n\n", unidade, palavraIngles);
                
            }
            else
            {
                // Se ainda houver mais unidades para a palavra inglesa, só remove a unidade
                printf("Entrou aqui!\n");
                removeu = removerPalavraIngles(&(*raiz)->info1.palavraIngles, palavraIngles);
                if (removeu)
                    printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);

                // Se a palavra inglesa foi removida e não há mais traduções, removemos a palavra do nó
                if ((*raiz)->info1.palavraIngles == NULL)
                {
                    removeu = arvore_2_3_remover(raiz, (*raiz)->info1.palavraPortugues);
                    if (removeu)
                        printf("Palavra portuguesa removida\n\n");
                }
            }
        }

        BuscarPalavraIngles(&(*raiz)->cent, palavraIngles, unidade);

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL && strcmp((*raiz)->info2.palavraIngles->palavraIngles, palavraIngles) == 0)
        {
            if (contar_lista_encadeada_unidade((*raiz)->info2.palavraIngles->unidades) == 1)
            {
                removeu = removerPalavraIngles(&(*raiz)->info2.palavraIngles, palavraIngles);
                if (removeu)
                    printf("A palavra %s foi removida com sucesso!\n\n", palavraIngles);
                if ((*raiz)->info2.palavraIngles == NULL)
                {
                    removeu = arvore_2_3_remover(raiz, (*raiz)->info2.palavraPortugues);
                    if (removeu)
                        printf("Removido\n\n");
                }
            }
            else
            {
                removeu = remover_lista_encadeada_unidade(&((*raiz)->info2.palavraIngles->unidades), unidade);
                if (removeu)
                    printf("Removido\n\n");
            }
        }

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL)
        {
            BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade);
        }
    }
}

void free_arvore_binaria(Inglesbin *raiz)
{
    if (raiz)
    {
        free_arvore_binaria(raiz->esq);
        free_arvore_binaria(raiz->dir);
        free(raiz->palavraIngles);
        free(raiz);
    }
}