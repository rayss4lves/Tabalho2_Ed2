#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

Portugues23 *BuscarPalavra(Portugues23 **no, const char *palavraPortugues)
{
    Portugues23 *inserida = NULL; // Inicializa o retorno como NULL

    if (no != NULL && *no != NULL)
    {
        if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) == 0)
        {
            inserida = (*no); // Palavra encontrada, retorna o nó
        }
        else if ((*no)->nInfos == 2 && strcmp(palavraPortugues, (*no)->info2.palavraPortugues) == 0)
        {
            inserida = (*no);
        }
        else
        {
            // Continua a busca nos filhos
            if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                inserida = BuscarPalavra(&(*no)->esq, palavraPortugues);
            }
            else if ((*no)->nInfos == 1 || strcmp(palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                inserida = BuscarPalavra(&(*no)->cent, palavraPortugues);
            }
            else
            {
                inserida = BuscarPalavra(&(*no)->dir, palavraPortugues);
            }
        }
    }

    return inserida;
}

void adicionarTraducao(Portugues23 *no, const char *palavraPortugues, const char *palavraIngles, int unidade)
{
    if (strcmp(palavraPortugues, (no)->info1.palavraPortugues) == 0)
    {
        adicionarTraducaoEmIngles(&(no)->info1, palavraIngles, unidade);
    }
    else if (no->nInfos == 2 && strcmp(palavraPortugues, no->info2.palavraPortugues) == 0)
    {
        adicionarTraducaoEmIngles(&(no)->info2, palavraIngles, unidade);
    }
}

int inserirPalavraPortugues(Portugues23 **arvore, char *palavraPortugues, char *palavraIngles, int unidade)
{
    Info promove;
    Portugues23 *Pai = NULL;
    int inseriu;

    // Busca a palavra na árvore
    Portugues23 *noExistente = BuscarPalavra(arvore, palavraPortugues);

    if (noExistente != NULL)
    {
        printf("A palavra %s já existe. Adicionando tradução...\n", palavraPortugues);
        adicionarTraducao(noExistente, palavraPortugues, palavraIngles, unidade);
        inseriu = 0;
    }
    else
    {
        printf("Inserindo a palavra %s \n", palavraPortugues);
        Info novoInfo = criaInfo(palavraPortugues, palavraIngles, unidade);
        inserirArv23(arvore, &novoInfo, &promove, &Pai);
        inseriu = 1;
    }
    return inseriu;
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

Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen)
{
    Portugues23 *no;
    no = (Portugues23 *)malloc(sizeof(Portugues23)); // allocate memory

    // Preenche info1 com os dados da nova disciplina
    no->info1 = *informacao;
    no->esq = filhoesq;
    no->cent = filhocen;
    no->nInfos = 1;

    // Inicializa info2 com nulo
    no->info2.palavraIngles = NULL;
    no->info2.palavraPortugues = NULL;
    no->dir = NULL;

    return no;
}

void adicionaChave(Portugues23 **no, const Info *informacao, Portugues23 *filho)
{
    if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) > 0)
    {
        // Adiciona a nova informação a info2
        (*no)->info2 = *informacao;
        (*no)->dir = filho; // Ajusta o ponteiro do filho direito
    }
    else
    {
        // Move info1 para info2 e coloca a nova informação em info1
        (*no)->info2 = (*no)->info1;
        (*no)->info1 = *informacao;
        (*no)->dir = (*no)->cent;
        (*no)->cent = filho;
    }
    (*no)->nInfos = 2; // Atualiza o número de informações
}

Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 *filho)
{
    Portugues23 *maior;

    if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
    {
        // A nova informação é menor que info1, então info1 será promovida
        *promove = (*no)->info1;
        maior = criaNo(&(*no)->info2, (*no)->cent, (*no)->dir);

        // Atualiza info1 com a nova informação
        (*no)->info1 = *informacao;
        (*no)->cent = filho;
    }
    else if (strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
    {
        // A nova informação é maior que info1 e menor que info2, então ela será promovida
        *promove = *informacao;
        maior = criaNo(&(*no)->info2, filho, (*no)->dir);
    }
    else
    {
        // A nova informação é maior que info1 e info2, então info2 será promovido
        *promove = (*no)->info2;
        maior = criaNo(informacao, (*no)->dir, filho);
    }

    // Limpa info2
    (*no)->info2.palavraIngles = NULL;
    (*no)->info2.palavraPortugues = NULL;

    (*no)->nInfos = 1; // Atualizando a quantidade de informação no nó
    (*no)->dir = NULL; // Ajusta o filho direito

    return maior;
}

int ehFolha(const Portugues23 *no)
{
    int achou = 0;

    if (no->esq == NULL)
    {
        achou = 1; // Se não tem filho esquerdo, é uma folha
    }

    return achou;
}

Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **Pai)
{
    Info promove1;
    Portugues23 *maiorNo = NULL;

    if (*no == NULL)
    {
        // Cria um novo nó caso seja nulo
        *no = criaNo(informacao, NULL, NULL);
    }
    else
    {
        if (ehFolha(*no))
        { // Verifica se é folha
            if ((*no)->nInfos == 1)
            {
                // O nó tem espaço para a nova chave
                adicionaChave(no, informacao, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                Portugues23 *novo;
                novo = quebraNo(no, informacao, promove, NULL); // quebra no e sobe a informação
                if (*Pai == NULL)
                {
                    Portugues23 *novaRaiz;
                    novaRaiz = criaNo(promove, *no, novo); // Cria nova raiz se necessário
                    *no = novaRaiz;
                }
                else
                {
                    maiorNo = novo; // Ajusta o novo maior nó
                }
            }
        }
        else
        { // Nó não é folha
            // Navega para o filho apropriado
            if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no); // Insere na subárvore à esquerda
            }
            else if ((*no)->nInfos == 1 || strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no); // Insere na subárvore do centro
            }
            else
            {
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no); // Insere na subárvore à direita
            }

            // Após inserir, verifica se houve promoção
            if (maiorNo != NULL)
            {
                if ((*no)->nInfos == 1)
                {
                    adicionaChave(no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                { // Quando não tem espaço
                    // O nó precisa ser quebrado
                    Portugues23 *novo;
                    novo = quebraNo(no, promove, &promove1, maiorNo); // Quebra o nó e sobe a informação
                    if (*Pai == NULL)
                    {
                        Portugues23 *novaRaiz;
                        novaRaiz = criaNo(&promove1, *no, novo); // Cria nova raiz se necessário
                        *no = novaRaiz;
                        maiorNo = NULL;
                    }
                    else
                    {
                        maiorNo = novo; // Ajusta o novo maior nó
                        *promove = promove1;
                    }
                }
            }
        }
    }

    return maiorNo;
}

// ############################################## FUNÇOES PARA EXIBIR ##############################################

void exibir_tree23(const Portugues23 *Raiz)
{
    if (Raiz != NULL)
    {
        exibir_tree23(Raiz->esq);
        printf("Palavra (PT): %s", Raiz->info1.palavraPortugues);
        if (Raiz->info1.palavraIngles != NULL && Raiz->info1.palavraIngles->palavraIngles != NULL)
        {
            printBinaryTree(Raiz->info1.palavraIngles);
            printf("\n");
        }

        exibir_tree23(Raiz->cent);
        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if (Raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s", Raiz->info2.palavraPortugues);

            // Exibir a tradução em inglês, se houver
            if (Raiz->info2.palavraIngles != NULL && Raiz->info2.palavraIngles->palavraIngles != NULL)
                printBinaryTree(Raiz->info2.palavraIngles);
            printf("\n");
            exibir_tree23(Raiz->dir);
        }
    }
}

void imprimirPalavrasUnidade(Portugues23 *arvore, int unidade)
{
    if (arvore)
    {
        imprimirPalavrasUnidade(arvore->esq, unidade);
        imprimirTraducoes(arvore->info1.palavraIngles, unidade, arvore->info1.palavraPortugues);

        imprimirPalavrasUnidade(arvore->cent, unidade);
        if (arvore->nInfos == 2)
        {
            imprimirTraducoes(arvore->info2.palavraIngles, unidade, arvore->info2.palavraPortugues);
        }
        imprimirPalavrasUnidade(arvore->dir, unidade);
    }
}

void imprimirTraducoes(Inglesbin *node, int unidade, const char *palavraPortuguês)
{
    if (node)
    {
        if (node->unidade == unidade)
        {
            printf("Palavra em Português: %s\n", palavraPortuguês);
            printf("Palavra em inglês: %s\n", node->palavraIngles);
        }
        imprimirTraducoes(node->esq, unidade, palavraPortuguês);
        imprimirTraducoes(node->dir, unidade, palavraPortuguês);
    }
}

void exibir_traducao_Portugues(Portugues23 **Raiz, const char *palavraPortugues)
{
    Portugues23 *resultado = NULL;
    if (Raiz != NULL)
    {
        resultado = BuscarPalavra(Raiz, palavraPortugues);
        if (resultado)
        {
            printf("Traduções em inglês para a palavra '%s':\n", palavraPortugues);

            if (strcmp(palavraPortugues, resultado->info1.palavraPortugues) == 0)
            {
                printBinaryTree(resultado->info1.palavraIngles);
            }
            else
            {
                printBinaryTree(resultado->info2.palavraIngles);
            }
        }
        else
        {
            printf("A palavra '%s' não foi encontrada na árvore.\n", palavraPortugues);
        }
    }
}

// ############################################# REMOÇÃO ############################################

void menorInfoDir(Portugues23 *Raiz, Portugues23 **no, Portugues23 **PaiNo)
{
    if (Raiz != NULL)
    {
        if (Raiz->esq != NULL)
        {
            *PaiNo = Raiz;                      // Atualiza o ponteiro do (*Pai)
            menorInfoDir(Raiz->esq, no, PaiNo); // Chama recursivamente para o filho esquerdo
        }
        else
        {
            *no = Raiz; // Encontrou o menor valor
        }
    }
}

void maiorInfoEsq(Portugues23 *Raiz, Portugues23 **no, Portugues23 **PaiNo)
{
    if (Raiz != NULL)
    {
        if (Raiz->dir != NULL)
        {
            *PaiNo = Raiz;                      // Atualiza o ponteiro do (*Pai)
            maiorInfoEsq(Raiz->dir, no, PaiNo); // Chama recursivamente para o filho direito
        }
        else
        {
            *no = Raiz; // Encontrou o maior valor
        }
    }
}

int ehInfo1(Portugues23 *raiz, char *valor)
{
    if (raiz == NULL || valor == NULL)
    {
        return 0; // Raiz ou valor nulo
    }
    return strcmp(raiz->info1.palavraPortugues, valor) == 0;
}

int ehInfo2(Portugues23 *raiz, char *valor)
{
    if (raiz == NULL || valor == NULL || raiz->nInfos < 2)
    {
        return 0; // Raiz nula, valor nulo ou o nó não tem info2
    }
    return strcmp(raiz->info2.palavraPortugues, valor) == 0;
}

// Portugues23 *buscar_pai(Portugues23 *raiz, char *valor)
// {
//     Portugues23 *pai = NULL;

//     if (raiz != NULL)
//     {
//         if (!eh_info1(*raiz, valor) && !eh_info2(*raiz, valor))
//         {
//             if (strcmp(valor, raiz->info1.palavraPortugues) < 0)
//                 pai = buscar_pai(raiz->esq, valor);  // Vai para o filho esquerdo
//             else if (raiz->nInfos == 1 || strcmp(valor, raiz->info2.palavraPortugues) < 0)
//                 pai = buscar_pai(raiz->cent, valor); // Vai para o filho central
//             else
//                 pai = buscar_pai(raiz->dir, valor);  // Vai para o filho direito

//             if (pai == NULL)
//                 pai = raiz;
//         }
//     }

//     return pai;
// }

// Portugues23 *buscar_maior_pai(Portugues23 *origem, Portugues23 *pai, char *valor)
// {
//     while (pai != NULL &&
//            ((pai->nInfos == 1 && strcmp(pai->info1.palavraPortugues, valor) < 0) ||
//             (pai->nInfos == 2 && strcmp(pai->info2.palavraPortugues, valor) < 0)))
//     {
//         pai = buscar_pai(origem, pai->info1.palavraPortugues); // Busca pelo pai baseado na palavra
//     }

//     return pai;
// }

// Portugues23 *buscar_menor_pai(Portugues23 *origem, Portugues23 *pai, char *valor)
// {
//     while (pai != NULL && strcmp(pai->info1.palavraPortugues, valor) > 0)
//     {
//         pai = buscar_pai(origem, pai->info1.palavraPortugues);  // Vai para o pai da palavra
//     }

//     return pai;
// }

int remover23(Portugues23 **Pai, Portugues23 **Raiz, char *valor, Portugues23 **origem)
{
    int removeu = 0;
    Portugues23 *no = NULL, *no1, *paiNo = NULL, *paiNo1 = NULL, **aux;
    aux = (Portugues23 **)malloc(sizeof(Portugues23 *));
    no1 = (Portugues23 *)malloc(sizeof(Portugues23));

    if (*Raiz != NULL)
    {
        if (ehFolha(*Raiz))
        {
            if ((*Raiz)->nInfos == 2)
            {
                if (strcmp(valor, (*Raiz)->info2.palavraPortugues) == 0)
                { // quando é folha, tem duas informações e o numero ta na segunda posição
                    (*Raiz)->info2.palavraIngles = NULL;
                    (*Raiz)->info2.palavraPortugues = NULL;
                    (*Raiz)->nInfos = 1;
                    removeu = 1;
                }
                else if (strcmp(valor, (*Raiz)->info1.palavraPortugues) == 0)
                { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
                    (*Raiz)->info1 = (*Raiz)->info2;
                    (*Raiz)->info2.palavraIngles = NULL;
                    (*Raiz)->info2.palavraPortugues = NULL;
                    (*Raiz)->nInfos = 1;
                    removeu = 1;
                }
            }
            else
            {
                if (*Pai == NULL)
                {
                    free(*Raiz);
                    *Raiz = NULL;
                    removeu = 1;
                }
                else
                {
                    if (*Raiz == (*Pai)->esq)
                    {
                        printf("%s - origem\n", (*origem)->info1.palavraPortugues);
                        (*Raiz)->info1 = (*Pai)->info1;
                        paiNo = *Pai;
                        menorInfoDir((*Pai)->cent, &no, &paiNo);
                        (*Pai)->info1 = no->info1;
                        removeu = 1;
                        if (no->nInfos == 2)
                        {
                            no->info1 = no->info2;
                            (*Raiz)->info2.palavraIngles = NULL;
                            (*Raiz)->info2.palavraPortugues = NULL;
                            no->nInfos = 1;
                        }
                        else
                        {
                            if (paiNo->nInfos == 1 && paiNo->info1.palavraPortugues == (*origem)->cent->info1.palavraPortugues)
                            {
                                no->info1.palavraPortugues = (*origem)->info1.palavraPortugues;
                                paiNo = *origem;
                                menorInfoDir((*origem)->dir, &no, &paiNo);
                                (*origem)->info2 = no->info1;
                                remover23(origem, &(*origem)->dir, (*origem)->info2.palavraPortugues, origem);
                            }else if (paiNo->nInfos == 1)
                            {
                                no->info1.palavraPortugues = (*origem)->info1.palavraPortugues;
                                paiNo = *origem;
                                menorInfoDir((*origem)->cent, &no, &paiNo);
                                (*origem)->info1 = no->info1;
                                remover23(origem, &(*origem)->cent, (*origem)->info1.palavraPortugues, origem);
                            }
                            else
                            {
                                no->info1 = paiNo->info2;
                                paiNo1 = paiNo;
                                menorInfoDir(paiNo->dir, &no1, &paiNo1);
                                paiNo->info2 = no1->info1;

                                if (no1->nInfos == 2)
                                {
                                    no1->info1 = no1->info2;
                                    no1->info2.palavraIngles = NULL;
                                    no1->info2.palavraPortugues = NULL;
                                    no1->nInfos = 1;
                                }
                                else
                                {
                                    no->info2 = paiNo->info2;
                                    no->nInfos = 2;
                                    paiNo->info2.palavraIngles = NULL;
                                    paiNo->info2.palavraPortugues = NULL;
                                    paiNo->nInfos = 1;
                                    free(no1);
                                    paiNo1->dir = NULL;
                                }
                            }
                        }
                    }
                    else if ((*Raiz) == (*Pai)->cent)
                    {
                        removeu = 1;
                        if ((*origem)->nInfos == 2)
                        {
                            if ((*Pai)->nInfos == 1)
                        {
                            
                            (*Raiz)->info1.palavraPortugues = (*origem)->info1.palavraPortugues;
                            paiNo = *origem;
                            menorInfoDir((*origem)->cent, &no, &paiNo);
                            (*origem)->info1 = no->info1;
                            if ((*origem)->cent->nInfos == 1)
                            {
                                remover23((origem), &(*origem)->cent, (*origem)->info1.palavraPortugues, origem);
                            }
                        }
                        }
                        
                        
                        else
                        {
                            (*Raiz)->info1 = (*Pai)->info2;
                            paiNo = *Pai;
                            menorInfoDir((*Pai)->dir, &no, &paiNo);
                            (*Pai)->info2 = no->info1;

                            if (no->nInfos == 2)
                            {
                                no->info1 = no->info2;
                                no->info2.palavraIngles = NULL;
                                no->info2.palavraPortugues = NULL;
                                no->nInfos = 1;
                            }
                            else
                            {
                                (*Raiz)->nInfos = 2;
                                (*Raiz)->info2 = (*Pai)->info2;
                                (*Pai)->info2.palavraIngles = NULL;
                                (*Pai)->info2.palavraPortugues = NULL;
                                (*Pai)->nInfos = 1;
                                free(no);
                                (*Pai)->dir = NULL;
                            }
                        }
                    }
                    else
                    {
                        removeu = 1;
                        paiNo = *Pai;
                        maiorInfoEsq((*Pai)->cent, &no, &paiNo);

                        if (no->nInfos == 1)
                        {
                            no->info2 = (*Pai)->info2;
                            (*Pai)->info2.palavraIngles = NULL;
                            (*Pai)->info2.palavraPortugues = NULL;
                            (*Pai)->nInfos = 1;
                            no->nInfos = 2;
                            free(*Raiz);
                            *Raiz = NULL;
                        }
                        else
                        {
                            (*Raiz)->info1 = (*Pai)->info2;
                            (*Pai)->info2 = no->info2;
                            no->info2.palavraIngles = NULL;
                            no->info2.palavraPortugues = NULL;
                            no->nInfos = 1;
                        }
                    }
                }
            }
        }
        else
        { // se nao é folha
            if (strcmp(valor, (*Raiz)->info1.palavraPortugues) < 0)
            { // Valor está na subárvore esquerda

                // Chamada recursiva para a subárvore esquerda
                origem = Pai;
                removeu = remover23(Raiz, &(*Raiz)->esq, valor, origem);
            }

            else if (strcmp(valor, (*Raiz)->info1.palavraPortugues) == 0)
            {

                paiNo = *Raiz;
                menorInfoDir((*Raiz)->cent, &no, &paiNo);
                (*Raiz)->info1 = no->info1;
                remover23(Raiz, &(*Raiz)->cent, (*Raiz)->info1.palavraPortugues, origem);

                removeu = 1;
            }
            else if (((*Raiz)->nInfos == 1) || (strcmp(valor, (*Raiz)->info2.palavraPortugues) < 0))
            {
                origem = Pai;
                removeu = remover23(Raiz, &(*Raiz)->cent, valor, origem);
            }
            else if (strcmp(valor, (*Raiz)->info2.palavraPortugues) == 0)
            {
                paiNo = *Pai;
                menorInfoDir((*Raiz)->dir, &no, &paiNo);
                (*Raiz)->info2 = no->info1;
                remover23(Raiz, &(*Raiz)->dir, (*Raiz)->info2.palavraPortugues, origem);
                removeu = 1;
            }
            else
            {
                origem = Pai;
                removeu = remover23(Raiz, &(*Raiz)->dir, valor, origem);
            }
        }
    }
    return removeu;
}

/*#########################################FREE#######################################################*/

// void freeInfo2_3(Info *info)
// {
//     free_arvore_binaria(info->palavraIngles);
//     free(info->palavraPortugues);
// }

// void freeTree(Portugues23 *no)
// {
//     if (no != NULL)
//     {
//         freeTree(no->esq);
//         freeInfo2_3(&no->info1);
//         freeTree(no->cent);
//         if (no->nInfos == 2)
//         {
//             freeInfo2_3(&no->info2);
//             freeTree(no->dir);
//         }
//         free(no);
//     }
// }

/*-----------------------------------------------------------------------------------------------------*/

/* (i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das
equivalentes em inglês */

/*-----------------------------------------------------------------------------------------------------*/

/* (ii) informar uma palavraPortugues em português e então imprima todas as palavras em inglês equivalente à palavraPortugues em
português dada, independente da unidade */

/*-----------------------------------------------------------------------------------------------------*/

/* (iii) informar uma palavraPortugues em inglês e a unidade a qual a mesma pertence removÊ-la das arvores binarias
das quais ela pertence. Caso ela seja a única palavraPortugues em uma das arvores binarias, remover tambem da
arvore 2-3 */

/*-----------------------------------------------------------------------------------------------------*/

/* (iv)informar uma palavraPortugues em português e a unidade a qual a mesma pertence e então removê-la, para isto
deve remover a palavraPortugues em inglês da arvore binaria correspondente a palavraPortugues em português da mesma
unidade. Caso ela seja a única palavraPortugues na arvore binaria, a palavraPortugues em português deve ser removida da
arvore 2-3 */
