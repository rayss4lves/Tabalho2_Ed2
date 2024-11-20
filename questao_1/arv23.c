#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

/*-----------------------------------------------------------------------------------------------------*/
Inglesbin *createNode(const char *palavraIngles, int unidade)
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
Inglesbin *insertpalavraIngles(Inglesbin *root, const char *palavraIngles, int unidade)
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
/* (pré-itens) funções necessarias para que os itens i, ii, iii e iv possam ocorrer */
Info criaInfo(char *palavra, char *palavraIngles, int unidade)
{
    Info info;

    info.palavraPortugues = malloc(strlen(palavra) + 1);
    strcpy(info.palavraPortugues, palavra);

    info.palavraIngles = NULL;
    info.palavraIngles = insertpalavraIngles(info.palavraIngles, palavraIngles, unidade);
    info.unidade = unidade;

    if (info.palavraIngles != NULL)
    {
        info.palavraIngles->unidade = unidade;
    }
    return info;
}

Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen)
{
    Portugues23 *no = (Portugues23 *)malloc(sizeof(Portugues23));
    if (!no)
    {
        printf("Erro ao alocar mem0ria para no");
    }
    else
    {
        no->info1 = *informacao;
        memset(&(no->info2), 0, sizeof(Info)); // Inicializa info2 com zeros
        no->esq = filhoesq;
        no->cent = filhocen;
        no->dir = NULL;
        no->nInfos = 1;
    }

    return no;
}

Portugues23 *adicionaChave(Portugues23 *no, const Info *informacao, Portugues23 *filho)
{
    if (strcmp(informacao->palavraPortugues, no->info1.palavraPortugues) > 0)
    {
        no->info2 = *informacao;
        no->dir = filho;
    }
    else
    {
        no->info2 = no->info1;
        no->dir = no->cent;
        no->info1 = *informacao;
        no->cent = filho;
    }
    no->nInfos = 2;
    return no;
}

Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 **filho)
{
    Portugues23 *maior;

    if (strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) > 0)
    {
        *promove = (*no)->info2;
        maior = criaNo(informacao, (*no)->dir, (filho ? *filho : NULL));
    }
    else if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) > 0)
    {
        *promove = *informacao;
        maior = criaNo(&(*no)->info2, (filho ? *filho : NULL), (*no)->dir);
    }
    else
    {
        *promove = (*no)->info1;
        maior = criaNo(&(*no)->info2, (*no)->cent, (*no)->dir);
        (*no)->info1 = *informacao;
        (*no)->cent = (filho ? *filho : NULL);
    }

    (*no)->nInfos = 1;
    return maior;
}

int ehFolha(const Portugues23 *no)
{
    return (no->esq == NULL);
}

// Portugues23 *adicionarTraducaoPalavra_existente(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, int unidade) {
//     Portugues23 *resultado = NULL; // Inicializa o retorno como NULL

//     if (no != NULL) {
//         if (strcmp(palavraPortugues, no->info1.palavraPortugues) == 0) {
//             // Se a palavra em português já existe, adicione a tradução em inglês
//             adicionarTraducaoEmIngles(&no->info1, palavraIngles, unidade);
//             resultado = no; // Palavra encontrada, retorna o nó
//         } else if (no->nInfos == 2 && strcmp(palavraPortugues, no->info2.palavraPortugues) == 0) {
//             // Se a palavra em português já existe na segunda posição, adicione a tradução em inglês
//             adicionarTraducaoEmIngles(&no->info2, palavraIngles, unidade);
//             resultado = no; // Palavra encontrada, retorna o nó
//         } else {
//             // Continua a busca nos filhos
//             if (strcmp(palavraPortugues, no->info1.palavraPortugues) < 0) {
//                 resultado = adicionarTraducaoPalavra_existente(no->esq, palavraPortugues, palavraIngles, unidade);
//             } else if (no->nInfos == 1 || strcmp(palavraPortugues, no->info2.palavraPortugues) < 0) {
//                 resultado = adicionarTraducaoPalavra_existente(no->cent, palavraPortugues, palavraIngles, unidade);
//             } else {
//                 resultado = adicionarTraducaoPalavra_existente(no->dir, palavraPortugues, palavraIngles, unidade);
//             }
//         }
//     }

//     return resultado; // Retorna o resultado final
// }



Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai)
{
    Info promove1;
    Portugues23 *maiorNo = NULL;

    if (*no == NULL)
    {
        *no = criaNo(informacao, NULL, NULL);
    }
    else
    {
        // Portugues23 *noExistente = adicionarTraducaoPalavra_existente(*no, informacao->palavraPortugues, informacao->palavraIngles->palavraIngles, informacao->unidade);
        // if (noExistente)
        // {
        //     maiorNo = noExistente; // Atualiza o retorno caso a palavra ja exista
        // }
        if (ehFolha(*no))
        {
            if ((*no)->nInfos == 1)
            {
                *no = adicionaChave(*no, informacao, NULL);
            }
            else
            {
                maiorNo = quebraNo(no, informacao, promove, NULL);
                if (*pai == NULL)
                {
                    *no = criaNo(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        {
            if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no);
            }
            else if ((*no)->nInfos == 1 || strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no);
            }
            else
            {
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no);
            }
        }

        if (maiorNo)
        {
            if ((*no)->nInfos == 1)
            {
                *no = adicionaChave(*no, promove, maiorNo);
                maiorNo = NULL;
            }
            else
            {
                maiorNo = quebraNo(no, promove, &promove1, &maiorNo);
                if (*pai != NULL)
                {
                    *no = criaNo(&promove1, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
    }

    return maiorNo;
}

void freeTree(Portugues23 *no)
{
    if (no != NULL)
    {
        freeTree(no->esq);
        freeTree(no->cent);
        freeTree(no->dir);
        free(no);
    }
}

void adicionarTraducaoEmIngles(Info *info, const char *palavraIng, int unidade)
{
    info->palavraIngles = insertpalavraIngles(info->palavraIngles, palavraIng, unidade);
}

void exibir_tree23(const Portugues23 *raiz)
{
    if (raiz != NULL)
    {
        exibir_tree23(raiz->esq);
        printf("Palavra (PT): %s, Unidade: %d", raiz->info1.palavraPortugues, raiz->info1.unidade);
        if (raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL)
        {
            printBinaryTree(raiz->info1.palavraIngles);
            printf("\n");
        }
        exibir_tree23(raiz->cent);
        // Se houver o segundo elemento (ninfos == 2), exibe o segundo filho
        if (raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s, Unidade: %d", raiz->info2.palavraPortugues, raiz->info2.unidade);

            // Exibir a tradução em inglês, se houver
            if (raiz->info2.palavraIngles != NULL && raiz->info2.palavraIngles->palavraIngles != NULL)
                printBinaryTree(raiz->info2.palavraIngles);
            printf("\n");
        }

        if (raiz->nInfos == 2)
        {
            exibir_tree23(raiz->dir);
        }
    }
}

void imprimirInfoUnidade(Portugues23 *arvore, int unidade)
{
    if (arvore)
    {
        // Percorre a arvore e imprime apenas as palavras da unidade especificada
        if (arvore->info1.unidade == unidade)
        {
            //int primeira = 1;
            imprimirTraducoes(arvore->info1.palavraIngles, arvore->info1.palavraPortugues);
        }
        if (arvore->nInfos == 2 && arvore->info2.unidade == unidade)
        {
            int primeira = 1;
            imprimirTraducoes(arvore->info2.palavraIngles, arvore->info2.palavraPortugues);
        }

        // Percorre recursivamente as subarvores
        imprimirInfoUnidade(arvore->esq, unidade);
        imprimirInfoUnidade(arvore->cent, unidade);
        imprimirInfoUnidade(arvore->dir, unidade);
    }
}

void imprimirArvorePorUnidade(Portugues23 *arvore)
{
    int unidade = 1;
    while (unidade <= 2) // Verifica explicitamente até 2 unidades
    {
        printf("%% Unidade %d\n", unidade);
        imprimirInfoUnidade(arvore, unidade);
        printf("\n");
        unidade++; // Incrementa unidade após a impressão
    }
}


void imprimirTraducoes(Inglesbin *node, const char *palavraPortugues)
{
    if (node)
    {
        // Imprime o nó esquerdo
        imprimirTraducoes(node->esq, palavraPortugues);

        // Imprime a palavra em inglês e a tradução em português
        
        printf("%s: %s;", node->palavraIngles, palavraPortugues);
       // *primeira = 0;  // Após a primeira impressão, 'primeira' se torna 0

        // Imprime o nó direito
        imprimirTraducoes(node->dir, palavraPortugues);
    }
}


void exibir_traducoes_unidade(const Portugues23 *raiz, int unidade)
{
    if (raiz != NULL)
    {
        if (raiz->info1.unidade == unidade)
            printf("Palavra (PT): %s, Unidade: %d, Traducao (EN): %s\n", raiz->info1.palavraPortugues, raiz->info1.unidade, raiz->info1.palavraIngles->palavraIngles);
        if (raiz->info2.unidade == unidade)
            printf("Palavra (PT): %s, Unidade: %d, Traducao (EN): %s\n", raiz->info2.palavraPortugues, raiz->info2.unidade, raiz->info2.palavraIngles->palavraIngles);
        exibir_traducoes_unidade(raiz->esq, unidade);
        exibir_traducoes_unidade(raiz->cent, unidade); // Subarvore central para arvore 2-3
        exibir_traducoes_unidade(raiz->dir, unidade);
    }
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq);
        printf("%s \n", root->palavraIngles);
        printBinaryTree(root->dir);
    }
}

// #########################################TENTAR CORRIGIR ESSA BOMBA######################################

// void imprimirTraducoesEmIngles(Portugues23 *arvore, const char *palavraPortugues) {
//     Portugues23 **nodo = NULL;
//     Portugues23 *irmao = NULL;

//     nodo = buscarValorArvore(&arvore, palavraPortugues, &irmao);
//     if (*nodo) {
//         printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);
//         if ((*nodo)->nInfos == 2) {
//             printBinaryTree((*nodo)->info1.palavraIngles);
//             printBinaryTree((*nodo)->info2.palavraIngles);
//         } else {
//             printBinaryTree((*nodo)->info1.palavraIngles);
//         }
//     } else {
//         printf("Palavra '%s' não encontrada.\n", palavraPortugues);
//     }
// }

// Portugues23** buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao) {
//     Portugues23 **nodo = arvore;  // Inicializa a arvore de busca

//     // Verifica se a arvore e o primeiro no não são NULL
//     while (*nodo) {
//         // Verifica se o valor esta no primeiro ou segundo campo do no
//         if (strcmp(valor, (*nodo)->info1.palavraPortugues) == 0 ||
//             ((*nodo)->nInfos == 2 && strcmp(valor, (*nodo)->info2.palavraPortugues) == 0)) {
//             return nodo;  // Se encontrou o valor, retorna o ponteiro para o no
//         }

//         // Atualiza `irmao` para o filho do meio (cent) se o valor não foi encontrado
//         *irmao = (*nodo)->cent;

//         // Se o valor for menor que o primeiro campo, vai para o no esquerdo
//         if (strcmp(valor, (*nodo)->info1.palavraPortugues) < 0) {
//             nodo = &((*nodo)->esq);
//         }
//         // Se o valor for maior que o segundo campo, vai para o no direito
//         else if ((*nodo)->nInfos == 2 && strcmp(valor, (*nodo)->info2.palavraPortugues) > 0) {
//             nodo = &((*nodo)->dir);
//         }
//         // Caso contrario, vai para o no do meio
//         else {
//             nodo = &((*nodo)->cent);
//             *irmao = (*nodo)->esq;  // Atualiza o `irmao` para o filho esquerdo
//         }
//     }

//     // Se o valor não foi encontrado, retorna NULL
//     return NULL;
// }

/*-----------------------------------------------------------------------------------------------------*/

/* (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das
equivalentes em inglês */

/*-----------------------------------------------------------------------------------------------------*/

/* (ii) informar uma palavraPortugues em português e então imprima todas as palavras em inglês equivalente à palavraPortugues em
português dada, independente da unidade */

/*-----------------------------------------------------------------------------------------------------*/

/* (iii) informar uma palavraPortugues em inglês e a unidade a qual a mesma pertence removÊ-la das arvores binarias
das quais ela pertence. Caso ela seja a única palavraPortugues em uma das arvores binarias, remover também da
arvore 2-3 */

/*-----------------------------------------------------------------------------------------------------*/

/* (iv)informar uma palavraPortugues em português e a unidade a qual a mesma pertence e então removê-la, para isto
deve remover a palavraPortugues em inglês da arvore binaria correspondente a palavraPortugues em português da mesma
unidade. Caso ela seja a única palavraPortugues na arvore binaria, a palavraPortugues em português deve ser removida da
arvore 2-3 */
