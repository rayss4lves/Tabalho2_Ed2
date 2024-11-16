#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

/*-----------------------------------------------------------------------------------------------------*/

/* (pré-itens) funções necessárias para que os itens i, ii, iii e iv possam ocorrer */
Info criaInfo(const char *palavraPortugues, const int unidade) {
    Info info;
    info.palavraIngles = NULL;
    info.palavraPortugues = strdup(palavraPortugues);
    info.unidade = unidade;
    info.palavraIngles = (Inglesbin*)malloc(sizeof(Inglesbin));
    info.palavraIngles->esq = NULL;
    info.palavraIngles->dir = NULL;
    return info;
}
Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen){
    Portugues23 *no = (Portugues23*)malloc(sizeof(Portugues23));

    no->info1 = *informacao;
    no->esq = filhoesq;
    no->cent = filhocen;
    no->dir = NULL;
    no->nInfos = 1;

    return no;
}

Portugues23 *adicionaChave(Portugues23 *no, const Info *informacao, Portugues23 *filho){
    if(strcmp(informacao->palavraPortugues, no->info1.palavraPortugues) > 0){
        no->info2 = *informacao;
        no->dir = filho;
    }
    else{
        no->info2 = no->info1;
        no->dir = no->cent;
        no->info1 = *informacao;
        no->cent = filho;
    }
    no->nInfos = 2;

    return no;
}

Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 **filho){
    Portugues23 *maior;

    if(strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) > 0){
        *promove = (*no)->info2;
        
        maior = criaNo(informacao, (*no)->dir, (filho ? *filho : NULL));
    }
    else if(strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) > 0){
        *promove = *informacao;
        maior = criaNo(&(*no)->info2, (filho ? *filho : NULL), (*no)->dir);
    }
    else{
        *promove = (*no)->info1;
        // If filho is NULL, handle it by using NULL explicitly
        maior = criaNo(&(*no)->info2, (*no)->cent, (*no)->cent);
        (*no)->info1 = *informacao;
        (*no)->cent = (filho ? *filho : NULL);
    }
    (*no)->nInfos = 1;

    return maior;
}

int ehFolha(const Portugues23 *no){
    return (no->esq == NULL);
}

// Função de inserção na árvore 2-3
Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai) {
    Info promove1;
    Portugues23 *maiorNo = NULL;

    if (*no == NULL) {
        // Cria um novo nó se ele não existir
        *no = criaNo(informacao, NULL, NULL);
    } else {
        if (ehFolha(*no)) {
            // Se o nó for folha, tenta inserir a chave
            if ((*no)->nInfos == 1) {
                *no = adicionaChave(*no, informacao, NULL);
            } else {
                // Se o nó estiver cheio, realiza a quebra
                maiorNo = quebraNo(no, informacao, promove, NULL);
                if (*pai == NULL) {
                    // Se não tiver pai, cria um novo nó raiz
                    *no = criaNo(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        } else {
            // Se o nó não for folha, navega para o próximo nó
            if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0) {
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no);
            } else if ((*no)->nInfos == 1 || (strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)) {
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no);
            } else {
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no);
            }
        }

        if (maiorNo) {
            if ((*no)->nInfos == 1) {
                // Adiciona a chave no nó se houver espaço
                *no = adicionaChave(*no, promove, maiorNo);
                maiorNo = NULL;
            } else {
                // Realiza a quebra do nó se necessário
                maiorNo = quebraNo(no, promove, &promove1, &maiorNo);
                if (*pai != NULL) {
                    *no = criaNo(&promove1, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
    }

    return maiorNo;
}


void freeTree(Portugues23 *no){
    if(no != NULL){
        freeTree(no->esq);
        freeTree(no->cent);
        freeTree(no->dir);
        free(no);
    }
}


void adicionarTraducaoEmIngles(Info *info, const char *palavraIng) {
    if(info->palavraIngles == NULL){
        info->palavraIngles = (Inglesbin*)malloc(sizeof(Inglesbin));
        info->palavraIngles->esq = NULL;
        info->palavraIngles->dir = NULL;
    }

    info->palavraIngles->palavraIngles = strdup(palavraIng);
}

void exibir_tree23(const Portugues23 *raiz){
    if(raiz != NULL){
        exibir_tree23(raiz->esq);
        printf("Palavra (PT): %s, Unidade: %d", raiz->info1.palavraPortugues, raiz->info1.unidade);

        if(raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL){
            printf(", Traducao (EN): %s", raiz->info1.palavraIngles->palavraIngles);
            printf("\n");
        }

        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if(raiz->nInfos == 2){
            printf("Palavra (PT): %s, Unidade: %d", raiz->info2.palavraPortugues, raiz->info2.unidade);

            // Exibir a tradução em inglês, se houver
            if(raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
                printf(", Traducao (EN): %s", raiz->info2.palavraIngles->palavraIngles);
            printf("\n");
        }
        exibir_tree23(raiz->cent);
        if(raiz->nInfos == 2) {
            exibir_tree23(raiz->dir);
        }
    }
}

Portugues23 **buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao) {
    Portugues23 **nodo = NULL;

    if (*arvore) {
        nodo = arvore;

        // Loop para procurar o valor até encontrar o nó ou um nó folha
        while (*nodo && 
               strcmp(valor, (*nodo)->info1.palavraPortugues) != 0 && 
               ((*nodo)->nInfos == 1 || strcmp(valor, (*nodo)->info2.palavraPortugues) != 0)) {
            
            *irmao = (*nodo)->cent;  // Supondo que `irmao` inicialmente seja o centro

            if ((*nodo)->nInfos == 2 && strcmp(valor, (*nodo)->info2.palavraPortugues) > 0) {
                // Se `valor` é maior que `info2`, vá para o nó direito
                nodo = &((*nodo)->dir);
            } else if (strcmp(valor, (*nodo)->info1.palavraPortugues) < 0) {
                // Se `valor` é menor que `info1`, vá para o nó esquerdo
                nodo = &((*nodo)->esq);
            } else {
                // Caso contrário, vá para o nó do centro e ajuste `irmao`
                *irmao = (*nodo)->esq;
                nodo = &((*nodo)->cent);
            }
        }
    }

    return nodo;
}

/*-----------------------------------------------------------------------------------------------------*/

/* (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das
equivalentes em inglês */

/*-----------------------------------------------------------------------------------------------------*/

/* (ii) informar uma palavraPortugues em português e então imprima todas as palavras em inglês equivalente à palavraPortugues em
português dada, independente da unidade */

/*-----------------------------------------------------------------------------------------------------*/

/* (iii) informar uma palavraPortugues em inglês e a unidade a qual a mesma pertence removÊ-la das árvores binárias
das quais ela pertence. Caso ela seja a única palavraPortugues em uma das árvores binárias, remover também da
árvore 2-3 */

/*-----------------------------------------------------------------------------------------------------*/

/* (iv)informar uma palavraPortugues em português e a unidade a qual a mesma pertence e então removê-la, para isto
deve remover a palavraPortugues em inglês da árvore binária correspondente a palavraPortugues em português da mesma
unidade. Caso ela seja a única palavraPortugues na árvore binária, a palavraPortugues em português deve ser removida da
árvore 2-3 */