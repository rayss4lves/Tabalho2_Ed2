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

// ############################################## ArVOrE 2-3 ##############################################


/* (pre-itens) funções necessarias para que os itens i, ii, iii e iv possam ocorrer */
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
    Portugues23 *no = (Portugues23 *)malloc(sizeof(Portugues23));
    if (!no)
    {
        printf("Erro ao alocar memoria para no");
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

Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **pai)
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
        { // Caso seja folha
            if ((*no)->nInfos == 1)
            {
                // O nó tem espaço para a nova chave
                *no = adicionaChave(*no, informacao, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                maiorNo = quebraNo(no, informacao, promove, NULL);
                if (*pai == NULL)
                { // Se não há pai, criar nova raiz
                    *no = criaNo(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        { // Nó não e folha
            // Navega para o filho apropriado
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

            // Após inserir, verifica se houve promoção
            if (maiorNo)
            {
                if ((*no)->nInfos == 1)
                {
                    // Adiciona chave promovida no nó atual
                    *no = adicionaChave(*no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    // O nó precisa ser quebrado
                    maiorNo = quebraNo(no, promove, &promove1, &maiorNo);
                    if (*pai == NULL)
                    {
                        *no = criaNo(&promove1, *no, maiorNo);
                        maiorNo = NULL;
                    }
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

// ############################################## FUNÇOES PArA EXIBIr ##############################################

void exibir_tree23(const Portugues23 *raiz)
{
    if (raiz != NULL)
    {
        exibir_tree23(raiz->esq);
        printf("Palavra (PT): %s", raiz->info1.palavraPortugues);
        if (raiz->info1.palavraIngles != NULL && raiz->info1.palavraIngles->palavraIngles != NULL)
        {
            printBinaryTree(raiz->info1.palavraIngles);
            printf("\n");
        }
        exibir_tree23(raiz->cent);
        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if (raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s", raiz->info2.palavraPortugues);

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
        imprimirInfoUnidade(arvore->esq, unidade);
        imprimirTraducoes(arvore->info1.palavraIngles, unidade, arvore->info1.palavraPortugues);

        imprimirInfoUnidade(arvore->cent, unidade);
        if (arvore->nInfos == 2)
        {
            imprimirTraducoes(arvore->info2.palavraIngles, unidade, arvore->info2.palavraPortugues);
        }
        imprimirInfoUnidade(arvore->dir, unidade);
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

void exibir_traducao_Portugues(Portugues23 **raiz, const char *palavraPortugues)
{
    Portugues23 *resultado = NULL;
    if (raiz != NULL)
    {
        resultado = BuscarPalavra(raiz, palavraPortugues);
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

// ############################################# rEMOÇÃO ############################################

int ehFolhas(Inglesbin *raiz){
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin *raiz){
    Inglesbin *aux;
    aux = NULL;

    if(raiz->dir == NULL){
        aux = raiz->esq;
    }else if(raiz->esq == NULL){
        aux = raiz->dir;
    }

    return aux;
}

Inglesbin *menorFilho(Inglesbin *raiz){
    Inglesbin *aux;
    aux = raiz;

    if(raiz){
        if(raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, char *palavra) {
    Inglesbin *endFilho;
    int existe = 0;

    if (*raiz) {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0) {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            Inglesbin *aux = *raiz;
            if (ehFolhas(*raiz)) {
                free(aux);
                *raiz = NULL;
            } else if ((endFilho = soUmFilho(*raiz)) != NULL) {
                free(aux);
                *raiz = endFilho;
            } else {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidade = endFilho->unidade;

                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        } else if (strcmp(palavra, (*raiz)->palavraIngles) < 0) {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        } else {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}

void BuscarPalavraIngles(Portugues23 **raiz, char *palavraIngles, int unidade) 
{ 
    if (*raiz != NULL) 
    {
        BuscarPalavraIngles(&(*raiz)->esq, palavraIngles, unidade);

        if ((*raiz)->info1.palavraIngles != NULL && (*raiz)->info1.palavraIngles->unidade == unidade) 
        {
            removerPalavraIngles(&(*raiz)->info1.palavraIngles, palavraIngles);
        }
        BuscarPalavraIngles(&(*raiz)->cent, palavraIngles, unidade);

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL && (*raiz)->info2.palavraIngles->unidade == unidade) 
        {
            removerPalavraIngles(&(*raiz)->info2.palavraIngles, palavraIngles);
        }

        if ((*raiz)->nInfos == 2 && (*raiz)->info2.palavraIngles != NULL) 
        {
            BuscarPalavraIngles(&(*raiz)->dir, palavraIngles, unidade);
        }
           
    }
}

static void onda(const Info info, Info *entrada, const Portugues23 *pai, Portugues23 **fonte, Portugues23 **raiz) {
    removerArv23(raiz, info.palavraPortugues, pai, fonte);
    if(entrada)
        *entrada = info;
}

static int removivel(const Portugues23 *raiz) {
    int pode = 0;

    if(raiz != NULL) {
        if(raiz->nInfos == 2)
            pode = 1;
        if(!pode) {
            pode = removivel(raiz->cent);
            if(!pode)
                pode = removivel(raiz->esq);
        }
    }
    return pode;
}

static Portugues23 *menorfilho(Portugues23 *no, Portugues23 **pai, Info *res){
    *pai = no;
    Portugues23 *filho = no->esq;

    while(filho != NULL && !ehFolha(filho)){
        *pai = filho;
        filho = filho->esq;
    }
    if(filho != NULL)
        *res = filho->info1;

    return filho;
}

static Portugues23 *maior(const Portugues23 *raiz) {
    return raiz->nInfos == 2 ? raiz->dir : raiz->cent;
}

static Portugues23 *maiorfilho(Portugues23 *raiz, Portugues23 **pai, Info *maiorinfo) {
    Portugues23 *filho = maior(raiz);
    *pai = raiz;

    while(filho != NULL && maior(filho) != NULL) {
        *pai = filho;
        filho = maior(filho);
    }
    if(filho != NULL)
        *maiorinfo = filho->nInfos ? filho->info2 : filho->info1;

    return filho;
}

static void desalocaNo(Portugues23 **no) {
    free(*no);
    *no = NULL;
}

static Portugues23 *juntaNo(Portugues23 *filho1, Portugues23 *filho2, Portugues23 **filho3) {
    Portugues23 *maior = NULL;
    if(!ehFolha(filho2)) {
        maior = juntaNo(filho2->esq, filho2->cent, &(filho1->dir));

        const Info aux = filho2->info1;
        filho2->info1 = maior->info1;
        maior->info1 = aux;
    }

    filho1->info2 = filho2->info1;
    maior = filho1;
    *filho3 = maior;
    desalocaNo(&filho2);

    return maior;
}

static int ehInfo1(const Portugues23 no, const char *palavra){
    int aux = 0;
    if(no.nInfos == 1 && palavra && no.info1.palavraPortugues)
        aux = strcmp(no.info1.palavraPortugues, palavra) == 0;
    return aux;
}

static int ehInfo2(const Portugues23 no, const char *palavra){
    int aux = 0;
    if(no.nInfos == 2 && palavra && no.info2.palavraPortugues)
        aux = strcmp(no.info2.palavraPortugues, palavra) == 0;
    return aux;
}

static Portugues23 *buscapai(Portugues23 *raiz, const char *palavra) {
    Portugues23 *pai = NULL;

    if(raiz != NULL) {
        if(!ehInfo1(*raiz, palavra) && !ehInfo2(*raiz, palavra)) {
            if(strcmp(palavra, raiz->info1.palavraPortugues) < 0)
                pai = buscapai(raiz->esq, palavra);
            else if(raiz->nInfos == 1 || strcmp(palavra, raiz->info1.palavraPortugues) < 0)
                pai = buscapai(raiz->cent, palavra);
            else
                pai = buscapai(raiz->dir, palavra);

            if(!pai)
                pai = raiz;
        }
    }
    return pai;
}

int removerArv23(Portugues23 **raiz, const char *info, const Portugues23 *pai, Portugues23 **ref) {
    int removeu = 0;

    if(*raiz != NULL) {
        const int info1 = ehInfo1(**raiz, info);
        const int info2 = ehInfo2(**raiz, info);
        if(info1 || info2) {
            if(ehFolha(*raiz)) {
                if((*raiz)->nInfos == 2) {
                    if(info1)
                        (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->nInfos = 1;
                    removeu = 1;
                }
                else {
                    if(pai != NULL) {
                        if((*raiz) == pai->esq)
                            onda(pai->info1, &((*raiz)->info1), NULL, ref, ref);
                        else {
                            if(pai->nInfos == 2) {
                                if(*raiz == pai->cent)
                                    onda(pai->info2, &((*raiz)->info1), NULL, ref, ref);
                                else
                                    onda(pai->info2, &(pai->cent->info2), NULL, ref, ref);
                            }
                            else
                                onda(pai->info1, &(pai->esq->info2), NULL, ref, ref);
                        }
                        removeu = 1;
                    }
                    else {
                        free(*raiz);
                        *raiz = NULL;
                        removeu = 1;
                    }
                }
            }
            else {
                removeu = 1;
                Portugues23 *filho, *auxpai;
                Info auxinfo;
                int juntou = 0;
                if(info2) {
                    if(removivel((*raiz)->dir))
                        filho = menorfilho((*raiz)->dir, &auxpai, &auxinfo);
                    else if(removivel((*raiz)->cent))
                        filho = maiorfilho((*raiz)->cent, &auxpai, &auxinfo);
                    else {
                        juntaNo((*raiz)->cent, (*raiz)->dir, &(*raiz)->cent);
                        juntou = 1;
                    }
                    if(!juntou)
                        onda(auxinfo, &((*raiz)->info2), auxpai, &filho, raiz);
                }
                if(info1){
                    if(removivel((*raiz)->esq))
                        filho = maiorfilho((*raiz)->esq, &auxpai, &auxinfo);
                    else if(removivel((*raiz)->cent))
                        filho = maiorfilho((*raiz)->cent, &auxpai, &auxinfo);
                    else if((*raiz)->nInfos == 1) {
                        if(pai != NULL) {
                            if(*raiz == pai->esq || (pai->nInfos == 2 && (*raiz == pai->cent))) {
                                filho = menorfilho((*raiz)->cent, &auxpai, &auxinfo);
                                auxpai = buscapai(*ref, pai->info1.palavraPortugues);

                                if(*raiz == pai->esq)
                                    onda(pai->info1, &(filho->info2), auxpai, ref, ref);
                                else
                                    onda(pai->info2, &(filho->info2), auxpai, ref, ref);
                            }
                            else {
                                filho = maiorfilho((*raiz)->esq, &auxpai, &auxinfo);
                                auxpai = buscapai(*ref, pai->info1.palavraPortugues);
                                filho->info2 = filho->info1;
                                onda((pai->nInfos == 2 && (*raiz == pai->dir)) ? pai->info2 : pai->info1, &((*raiz)->info1), auxpai, ref, ref);
                            }
                        }
                        else {
                            Portugues23 *aux = *raiz;
                            juntaNo((*raiz)->esq, (*raiz)->cent, raiz);
                            juntou = 1;
                            desalocaNo(&aux);
                        }
                    }
                    if(pai != NULL && !juntou)
                        onda(auxinfo, &((*raiz)->info1), auxpai, ref, &filho);
                }
            }
        }
        else {
            if(strcmp(info, (*raiz)->info1.palavraPortugues) < 0)
                removeu = removerArv23((&(*raiz)->esq), info, *raiz, ref);
            else if((*raiz)->nInfos == 1 || strcmp(info, (*raiz)->info2.palavraPortugues) < 0)
                removeu = removerArv23((&(*raiz)->cent), info, *raiz, ref);
            else
                removeu = removerArv23((&(*raiz)->dir), info, *raiz, ref);
        }
    }

    return removeu;
}

void removerElemento(Portugues23 **raiz, const char *palavra) {
    if (removerArv23(raiz, palavra, NULL, raiz))
        printf("Elemento '%s' removido com sucesso.\n", palavra);
    else
        printf("Elemento '%s' nao encontrado.\n", palavra);
}

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
