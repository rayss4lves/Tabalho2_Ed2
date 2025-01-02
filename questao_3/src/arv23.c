#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"

int inserirArvore23(Arvore23 **arvore, Info info)
{
    Info promove;
    Arvore23 *Pai = NULL;
    int inseriu = 0;

    if (inserirArv23(arvore, info, &promove, &Pai))
        inseriu = 1;

    return inseriu;
}

Arvore23 *criaNo(Info informacao, Arvore23 *filhoesq, Arvore23 *filhocen)
{
    Arvore23 *no;
    no = (Arvore23 *)malloc(sizeof(Arvore23)); // allocate memory

    // Preenche info1 com os dados da nova disciplina
    no->info1 = informacao;
    no->esq = filhoesq;
    no->cent = filhocen;
    no->nInfos = 1;

    no->dir = NULL;

    return no;
}

void adicionaChave(Arvore23 *no, Info informacao, Arvore23 *filho)
{
    if (informacao.inicio > (no)->info1.inicio)
    {
        // Adiciona a nova informação a info2
        (no)->info2 = informacao;
        (no)->dir = filho; // Ajusta o ponteiro do filho dir
    }
    else
    {
        // Move info1 para info2 e coloca a nova informação em info1
        (no)->info2 = (no)->info1;
        (no)->info1 = informacao;
        (no)->dir = (no)->cent;
        (no)->cent = filho;
    }
    (no)->nInfos = 2; // Atualiza o número de informações
}

Arvore23 *quebraNo(Arvore23 *no, Info informacao, Info *promove, Arvore23 *filho)
{
    Arvore23 *maior;

    if (informacao.inicio < (no)->info1.inicio)
    {
        *promove = (no)->info1;                             // Promove info1
        maior = criaNo((no)->info2, (no)->cent, (no)->dir); // Cria o novo nó
        (no)->info1 = informacao;                           // Atualiza info1 com a nova informação
        (no)->cent = filho;                                 // Atualiza o filho central
    }
    else if (informacao.inicio < (no)->info2.inicio)
    {
        *promove = informacao;                         // Promove a nova informação
        maior = criaNo((no)->info2, filho, (no)->dir); // Cria o novo nó
    }
    else
    {
        *promove = (no)->info2;                       // Promove info2
        maior = criaNo(informacao, (no)->dir, filho); // Cria o novo nó
    }
    (no)->nInfos = 1; // Atualizando a quantidade de informação no nó
    (no)->dir = NULL; // Ajusta o filho dir

    return maior;
}

int ehFolha(const Arvore23 *no)
{
    int achou = 0;

    if (no->esq == NULL)
        achou = 1; // Se não tem filho esq, é uma folha
    return achou;
}

Arvore23 *inserirArv23(Arvore23 **no, Info informacao, Info *promove, Arvore23 **Pai)
{
    Info promove1;
    Arvore23 *maiorNo = NULL;

    if (*no == NULL)
        *no = criaNo(informacao, NULL, NULL); // Cria um novo nó caso seja nulo
    else
    {
        if (ehFolha(*no))
        { // Verifica se é folha
            if ((*no)->nInfos == 1)
                adicionaChave(*no, informacao, NULL); // O nó tem espaço para a nova chave
            else
            {
                // O nó precisa ser quebrado
                // Arvore23 *novo;
                maiorNo = quebraNo(*no, informacao, promove, NULL); // Quebra o nó e sobe a informação
                if (*Pai == NULL)
                {                                         // Quando não há pai, cria nova raiz
                    *no = criaNo(*promove, *no, maiorNo); // Nova raiz com o nó atual à esquerda e maiorNo à direita
                    maiorNo = NULL;                       // Não há mais necessidade de maiorNo
                }
            }
        }
        else
        { // Nó não é folha
            // Navega para o filho apropriado
            if (informacao.inicio < (*no)->info1.inicio)
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no); // Insere na subárvore à esq
            else if ((*no)->nInfos == 1 || informacao.inicio < (*no)->info2.inicio)
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no); // Insere na subárvore do cent
            else
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no); // Insere na subárvore à dir

            // Após inserir, verifica se houve promoção
            if (maiorNo != NULL)
            {
                if ((*no)->nInfos == 1)
                {
                    adicionaChave(*no, *promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                { // Quando não tem espaço
                    // O nó precisa ser quebrado
                    Arvore23 *novo = NULL;
                    novo = quebraNo((*no), *promove, &promove1, maiorNo); // Quebra o nó e sobe a informação
                    if (*Pai == NULL)
                    {
                        Arvore23 *novaRaiz;
                        novaRaiz = criaNo(promove1, *no, novo); // Cria nova raiz se necessário
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

Arvore23 *BuscarPalavra(Arvore23 *no, int info)
{
    Arvore23 *inserida = NULL; // Inicializa o retorno como NULL

    if (no != NULL)
    {
        if (info == (no)->info1.inicio)
            inserida = no; // Palavra encontrada, retorna o nó
        else if (info == (no)->info2.inicio)
            inserida = no;
        else
        {
            // Continua a busca nos filhos
            if (info < (no)->info1.inicio)
                inserida = BuscarPalavra((no)->esq, info);
            else if ((no)->nInfos == 1 || info < (no)->info2.inicio)
                inserida = BuscarPalavra((no)->cent, info);
            else
                inserida = BuscarPalavra((no)->dir, info);
        }
    }

    return inserida;
}

void arvore23_exibir_pre(Arvore23 *raiz)
{
    if (raiz != NULL)
    {
        printf("[1º] %d -> ", raiz->info1.inicio);
        if (raiz->nInfos == 2)
            printf("[2º] %d -> ", raiz->info2.inicio);

        arvore23_exibir_pre(raiz->esq);
        arvore23_exibir_pre(raiz->cent);
        if (raiz->nInfos == 2)
            arvore23_exibir_pre(raiz->dir);
    }
}

void arvore23_exibir_ordem(Arvore23 *raiz)
{
    if (raiz != NULL)
    {
        arvore23_exibir_ordem(raiz->esq);
        printf("[1º] %d -> ", raiz->info1.inicio);
        arvore23_exibir_ordem(raiz->cent);

        if (raiz->nInfos == 2)
        {
            printf("[2º] %d -> ", raiz->info2.inicio);
            arvore23_exibir_ordem(raiz->dir);
        }
    }
}

void arvore23_exibir_pos(Arvore23 *raiz)
{
    if (raiz != NULL)
    {
        arvore23_exibir_pos(raiz->esq);
        arvore23_exibir_pos(raiz->cent);
        if (raiz->nInfos == 2)
            arvore23_exibir_pos(raiz->dir);

        printf("[1º] %d -> ", raiz->info1.inicio);
        if (raiz->nInfos == 2)
            printf("[2º] %d -> ", raiz->info2.inicio);
    }
}

// ############################################# REMOÇÃO ############################################

Info *no23_maior_info(Arvore23 *raiz)
{
    return raiz->nInfos == 2 ? &(raiz->info2) : &(raiz->info1);
}

static int eh_info1(Arvore23 no, int info)
{
    return info == no.info1.inicio;
}

static int eh_info2(Arvore23 no, int info)
{
    return no.nInfos == 2 && info == no.info2.inicio;
}

Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Info **maior_valor)
{
    Arvore23 *filho;
    filho = raiz;

    while (!ehFolha(filho))
    {
        *pai = filho;
        if (filho->nInfos == 1)
            filho = filho->cent;
        else
            filho = filho->dir;
    }

    if (filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai)
{
    Arvore23 *filho;
    filho = raiz;

    while (!ehFolha(filho))
    {
        *pai = filho;
        filho = filho->esq;
    }

    return filho;
}

static int calcular_altura(Arvore23 *no)
{
    int altura = -1;
    if (no != NULL)
        altura = 1 + calcular_altura(no->esq);
    return altura;
}

static int movimento_onda(Info saindo, Info *entrada, Arvore23 *pai, Arvore23 **origem, Arvore23 **raiz, Arvore23 **maior, int (*funcao_remover)(Arvore23 **, int, Arvore23 *, Arvore23 **, Arvore23 **))
{
    int removeu = funcao_remover(raiz, saindo.inicio, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}

Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < (raiz)->info1.inicio)
                pai = arvore23_buscar_pai((raiz)->esq, info);
            else if ((raiz)->nInfos == 1 || info < (raiz)->info2.inicio)
                pai = arvore23_buscar_pai((raiz)->cent, info);
            else
                pai = arvore23_buscar_pai((raiz)->dir, info);

            if (pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < (raiz)->info1.inicio)
                pai = arvore23_buscar_maior_pai((raiz)->esq, info);
            else if ((raiz)->nInfos == 1 || info < (raiz)->info2.inicio)
                pai = arvore23_buscar_maior_pai((raiz)->cent, info);
            else
                pai = arvore23_buscar_maior_pai((raiz)->dir, info);

            if (pai == NULL &&
                ((raiz->nInfos == 1 && raiz->info1.inicio > info) ||
                 (raiz->nInfos == 2 && raiz->info2.inicio > info)))
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_menor_pai_2_infos(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < (raiz)->info1.inicio)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->esq, info);
            else if (raiz->nInfos == 1 || info < (raiz)->info2.inicio)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->cent, info);
            else
                pai = arvore23_buscar_menor_pai_2_infos(raiz->dir, info);

            if (pai == NULL && raiz->nInfos == 2 && info < (raiz)->info2.inicio)
                pai = raiz;
        }
    }

    return pai;
}

Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, int info)
{
    Arvore23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < raiz->info1.inicio)
                pai = arvore23_buscar_menor_pai(raiz->esq, info);
            else if (raiz->nInfos == 1 || info < raiz->info2.inicio)
                pai = arvore23_buscar_menor_pai(raiz->cent, info);
            else
                pai = arvore23_buscar_menor_pai(raiz->dir, info);

            if (pai == NULL && raiz->info1.inicio < info)
                ;
            pai = raiz;
        }
    }

    return pai;
}

int arvore23_remover_nao_folha1(Arvore23 **origem, Arvore23 *raiz, Info *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu;
    Arvore23 *filho, *pai;
    Info *info_filho;

    pai = raiz;

    filho = arvore23_buscar_maior_filho(filho1, &pai, &info_filho);

    if (filho->nInfos == 2)
    {
        *info = *info_filho;
        filho->nInfos = 1;
        removeu = 1;
    }
    else
    {
        filho = arvore23_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, arvore23_remover1);
    }

    return removeu;
}

int arvore23_remover_nao_folha2(Arvore23 **origem, Arvore23 *raiz, Info *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior)
{
    int removeu = 0;
    Arvore23 *filho, *pai;
    Info *info_filho;

    pai = raiz;

    filho = arvore23_buscar_menor_filho(filho1, &pai);

    if (filho->nInfos == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->nInfos = 1;
        removeu = 1;
    }
    else
    {
        filho = arvore23_buscar_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(*info_filho, info, pai, origem, &filho, maior, arvore23_remover2);
    }

    return removeu;
}

int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*raiz))
            {
                if ((*raiz)->nInfos == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->nInfos = 1;
                }
                else
                {
                    if (pai == NULL)
                        freeNo(raiz);
                    else
                    {
                        Arvore23 *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->esq || (pai->nInfos == 2 && *raiz == pai->cent))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.inicio);

                            if (*raiz == pai->esq)
                                info_pai = pai->info1;
                            else
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover1);
                        }
                        else
                        {
                            pai_aux = arvore23_buscar_maior_pai(*origem, (*raiz)->info1.inicio);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.inicio);

                            if (pai_aux != NULL)
                            {
                                if (pai_aux->info1.inicio > (*raiz)->info1.inicio)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;
                            }

                            int altura_menor_pai = calcular_altura(menor_pai);
                            int altura_pai_aux = calcular_altura(pai_aux);

                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL &&
                                                    altura_menor_pai <= altura_pai_aux && info_pai.inicio > menor_pai->info2.inicio))
                            {
                                *maior = pai;
                                (*raiz)->nInfos = 0;
                                removeu = -1;
                            }
                            else
                            {
                                Arvore23 *avo;
                                avo = arvore23_buscar_pai(*origem, info_pai.inicio);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = arvore23_remover_nao_folha1(origem, *raiz, &((*raiz)->info2), (*raiz)->cent, (*raiz)->dir, maior);
            else if (info1)
                removeu = arvore23_remover_nao_folha1(origem, *raiz, &((*raiz)->info1), (*raiz)->esq, (*raiz)->cent, maior);
        }
        else
        {
            if (info < (*raiz)->info1.inicio)
                removeu = arvore23_remover1(&(*raiz)->esq, info, *raiz, origem, maior);
            else if ((*raiz)->nInfos == 1 || (info < (*raiz)->info2.inicio))
                removeu = arvore23_remover1(&(*raiz)->cent, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover1(&(*raiz)->dir, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover2(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*raiz))
            {
                if ((*raiz)->nInfos == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->nInfos = 1;
                }
                else
                {
                    if (pai == NULL)
                        freeNo(raiz);
                    else
                    {
                        Arvore23 *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->cent || (pai->nInfos == 2 && *raiz == pai->dir))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.inicio);

                            if (*raiz == pai->cent)
                                info_pai = pai->info1;
                            else
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover2);
                        }
                        else
                        {
                            pai_aux = arvore23_buscar_menor_pai(*origem, (*raiz)->info1.inicio);

                            Arvore23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.inicio);

                            Arvore23 *avo;
                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if (pai_aux->nInfos == 2 && pai_aux->info2.inicio < (*raiz)->info1.inicio)
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = arvore23_buscar_pai(*origem, info_pai.inicio);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = arvore23_remover_nao_folha2(origem, *raiz, &((*raiz)->info2), (*raiz)->dir, (*raiz)->cent, maior);
            else if (info1)
                removeu = arvore23_remover_nao_folha2(origem, *raiz, &((*raiz)->info1), (*raiz)->cent, (*raiz)->esq, maior);
        }
        else
        {
            if (info < (*raiz)->info1.inicio)
                removeu = arvore23_remover2(&(*raiz)->esq, info, *raiz, origem, maior);
            else if ((*raiz)->nInfos == 1 || (info < (*raiz)->info2.inicio))
                removeu = arvore23_remover2(&(*raiz)->cent, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover2(&(*raiz)->dir, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int possivel_remover(Arvore23 *raiz)
{
    int possivel = 0;

    if (raiz != NULL)
    {
        possivel = raiz->nInfos == 2;

        if (!possivel)
        {
            possivel = possivel_remover(raiz->cent);

            if (!possivel)
                possivel = possivel_remover(raiz->esq);
        }
    }

    return possivel;
}

Arvore23 *no23_juntar(Arvore23 *filho1, Info info, Arvore23 *maior, Arvore23 **raiz)
{
    adicionaChave(filho1, info, maior);

    (*raiz)->nInfos--;

    if ((*raiz)->nInfos == 0)
        freeNo(raiz);

    return filho1;
}

static void balanceamento(Arvore23 **raiz, Arvore23 *filho1, Arvore23 **filho2, Info info, Arvore23 **maior)
{
    if (*filho2 == NULL || (*filho2)->nInfos == 0)
    {
        if (*filho2 != NULL)
            freeNo(filho2);
        *maior = no23_juntar(filho1, info, *maior, raiz);
    }
}

int arvore23_rebalancear(Arvore23 **raiz, int info, Arvore23 **maior)
{
    int balanceou = 0;
    if (*raiz != NULL)
    {
        if (!ehFolha(*raiz))
        {
            if (info < (*raiz)->info1.inicio)
                balanceou = arvore23_rebalancear(&((*raiz)->esq), info, maior);
            else if ((*raiz)->nInfos == 1 || (info < (*raiz)->info2.inicio))
            {
                if ((*raiz)->esq->nInfos == 2 && !possivel_remover((*raiz)->cent))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->cent), info, maior);
            }
            else
            {
                if ((*raiz)->cent->nInfos == 2 && !possivel_remover((*raiz)->dir))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->dir), info, maior);
            }

            if (balanceou != -1)
            {
                if ((*raiz)->nInfos == 1)
                    balanceamento(raiz, (*raiz)->esq, &((*raiz)->cent), (*raiz)->info1, maior);
                else if ((*raiz)->nInfos == 2)
                    balanceamento(raiz, (*raiz)->cent, &((*raiz)->dir), (*raiz)->info2, maior);
                balanceou = 1;
            }
        }
    }

    return balanceou;
}

int arvore_2_3_remover(Arvore23 **raiz, int info)
{
    Arvore23 *maior, *posicao_juncao;
    int removeu = arvore23_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if (removeu == -1)
    {
        Info *valor_juncao = no23_maior_info(posicao_juncao);
        maior = NULL;
        removeu = arvore23_rebalancear(raiz, valor_juncao->inicio, &maior);

        if (removeu == -1)
        {
            Arvore23 *pai, *posicao_juncao2;
            Info *entrada;
            pai = arvore23_buscar_pai(*raiz, valor_juncao->inicio);

            if (eh_info1(*posicao_juncao, valor_juncao->inicio))
                entrada = &(posicao_juncao->cent->info1);
            else
                entrada = &(posicao_juncao->dir->info1);

            removeu = movimento_onda(*valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, arvore23_remover2);

            if (removeu == -1)
            {
                *valor_juncao = posicao_juncao2->info1;
                pai = arvore23_buscar_pai(*raiz, valor_juncao->inicio);
                removeu = movimento_onda(*valor_juncao, &(posicao_juncao2->esq->info1), pai, raiz, &posicao_juncao2, &posicao_juncao, arvore23_remover1);

                valor_juncao = no23_maior_info(posicao_juncao);
                maior = NULL;
                removeu = arvore23_rebalancear(raiz, valor_juncao->inicio, &maior);
            }
        }

        if (*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

/*#########################################FREE#######################################################*/
void freeNo(Arvore23 **no)
{
    free(*no);
    *no = NULL;
}
void freeTree(Arvore23 **no)
{
    if (*no != NULL)
    {
        freeTree(&((*no)->esq));
        freeTree(&((*no)->cent));
        if ((*no)->nInfos == 2)
            freeTree(&((*no)->dir));
        freeNo(no);
    }
}