#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"


/*-----------------------------------------------------------------------------------------------------*/
Inglesbin* createNode(const char* word, int unidade) {
    Inglesbin* newNode = (Inglesbin*)malloc(sizeof(Inglesbin));
    if (newNode != NULL) {
        strcpy(newNode->palavraIngles, word);
        newNode->unidade = unidade;
        newNode->esq = newNode->dir = NULL;
    }
    return newNode;
}

// Função para inserir uma palavra em inglês na árvore binária de busca
Inglesbin* insertpalavraIngles(Inglesbin* root, const char* word, int unidade) {
    Inglesbin *result;
    if (root == NULL) {
        result = createNode(word, unidade);
    } else {
        if (strcmp(word, root->palavraIngles) < 0) {
            root->esq = insertpalavraIngles(root->esq, word, unidade);
        } else if (strcmp(word, root->palavraIngles) > 0) {
            root->dir = insertpalavraIngles(root->dir, word, unidade);
        }
        result = root;
    }
    return result;
}
/* (pré-itens) funções necessárias para que os itens i, ii, iii e iv possam ocorrer */
Info criaInfo(char *palavra, char *palavraIngles, int unidade) {
    Info info;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    info.palavraPortugues = strdup(palavra);
    info.unidade = unidade;
    info.palavraIngles->unidade = unidade;
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


void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade) {    
    info->palavraIngles = insertpalavraIngles(info->palavraIngles, palavraIng, unidade);
}


void exibir_tree23(const Portugues23 *raiz){
    if(raiz != NULL){
        exibir_tree23(raiz->esq);
        printf("Palavra (PT): %s, Unidade: %d", raiz->info1.palavraPortugues, raiz->info1.unidade);
        if( raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL){
            printf(", Traducao (EN): %s", raiz->info1.palavraIngles->palavraIngles);
            printf("\n");
        }
        exibir_tree23(raiz->cent);
        // Se houver o segundo elemento (ninfos == 2), exibe o segundo filho
        if(raiz->nInfos == 2){
            printf("Palavra (PT): %s, Unidade: %d", raiz->info2.palavraPortugues, raiz->info2.unidade);

            // Exibir a tradução em inglês, se houver
            if(raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
                printf(", Traducao (EN): %s", raiz->info2.palavraIngles->palavraIngles);
            printf("\n");
        }
        
        if(raiz->nInfos == 2) {
            exibir_tree23(raiz->dir);
        }
    }
}


//#########################################TENTAR CORRIGIR ESSA BOMBA######################################







// Portugues23 **buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao) {
//     Portugues23 **nodo = NULL;

//     if (*arvore) {
//         nodo = arvore;

//         // Loop para procurar o valor até encontrar o nó ou um nó folha
//         while (*nodo && 
//                strcmp(valor, (*nodo)->info1.palavraPortugues) != 0 && 
//                ((*nodo)->nInfos == 1 || strcmp(valor, (*nodo)->info2.palavraPortugues) != 0)) {
            
//             *irmao = (*nodo)->cent;  // Supondo que `irmao` inicialmente seja o centro

//             if ((*nodo)->nInfos == 2 && strcmp(valor, (*nodo)->info2.palavraPortugues) > 0) {
//                 // Se `valor` é maior que `info2`, vá para o nó direito
//                 nodo = &((*nodo)->dir);
//             } else if (strcmp(valor, (*nodo)->info1.palavraPortugues) < 0) {
//                 // Se `valor` é menor que `info1`, vá para o nó esquerdo
//                 nodo = &((*nodo)->esq);
//             } else {
//                 // Caso contrário, vá para o nó do centro e ajuste `irmao`
//                 *irmao = (*nodo)->esq;
//                 nodo = &((*nodo)->cent);
//             }
//         }
//     }

//     return nodo;
// }

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

// void imprimirNodo(Portugues23 *no, int value, int nivel) {
//     int i;
//     if (no) {
//         // Indentação para representar o nível do nó na árvore
//         for (i = 0; i < nivel; i++) {
//             printf("\t");
//         }

//         if (value == 0) {
//             printf("Palavra (PT): %s\n", no->info1.palavraPortugues);
//             printf("Traduções em inglês:\n");
//             printBinaryTree(no->info1.palavraIngles); // Imprime a árvore binária de traduções de `info1`
//         } else {
//             printf("Palavra (PT): %s\n", no->info2.palavraPortugues);
//             printf("Traduções em inglês:\n");
//             printBinaryTree(no->info2.palavraIngles); // Imprime a árvore binária de traduções de `info2`
//         }
//     }
// }

// void imprimirArvore(Portugues23 *arvore, int nivel) {
//     if (arvore) {
//         // Imprime a subárvore à esquerda
//         imprimirArvore(arvore->esq, nivel + 1);

//         // Imprime `info1` e suas traduções
//         imprimirNodo(arvore, 0, nivel);

//         // Imprime a subárvore do meio
//         imprimirArvore(arvore->cent, nivel + 1);

//         // Se o nó possui `info2`, imprime `info2` e a subárvore à direita
//         if (arvore->nInfos == 2) {
//             imprimirNodo(arvore, 1, nivel); // Imprime `info2`
//             imprimirArvore(arvore->dir, nivel + 1);
//         }
//     }
// }
// void imprimirInfoUnidade(Portugues23 *arvore, int unidade) {
//     if (arvore) {
//         // Verifica se `info1` pertence à unidade
//         if (arvore->info1.unidade == unidade) {
//             int primeira = 1;
//             imprimirTraducoes(arvore->info1.palavraIngles, arvore->info1.palavraPortugues, &primeira);
//         }
//         // Verifica se `info2` pertence à unidade
//         if (arvore->nInfos == 2 && arvore->info2.unidade == unidade) {
//             int primeira = 1;
//             imprimirTraducoes(arvore->info2.palavraIngles, arvore->info2.palavraPortugues, &primeira);
//         }

//         // Percorre recursivamente as subárvores
//         imprimirInfoUnidade(arvore->esq, unidade);
//         imprimirInfoUnidade(arvore->cent, unidade);
//         if (arvore->nInfos == 2) {
//             imprimirInfoUnidade(arvore->dir, unidade);
//         }
//     }
// }

// void printBinaryTree(Inglesbin *root) {
//     if (root != NULL) {
//         printBinaryTree(root->esq);
//         printf("- %s (Unidade: %d)\n", root->palavraIngles, root->unidade);
//         printBinaryTree(root->dir);
//     }
// }

// void imprimirTraducoes(Inglesbin *arvore, const char *palavraPortugues, int *primeira) {
//     if (arvore) {
//         imprimirTraducoes(arvore->esq, palavraPortugues, primeira);

//         // Imprime o cabeçalho apenas na primeira vez
//         if (*primeira) {
//             printf("Palavra em Português: %s\n", palavraPortugues);
//             printf("Traduções em Inglês:\n");
//             *primeira = 0;
//         }

//         printf("- %s (Unidade: %d)\n", arvore->palavraIngles, arvore->unidade);

//         imprimirTraducoes(arvore->dir, palavraPortugues, primeira);
//     }
// }
