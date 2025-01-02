#ifndef SRC_H
#define SRC_H

#define LIVRE 1
#define OCUPADO 0

typedef struct Info
{
    int inicio;
    int fim;
    int status;
} Info;

typedef struct Arvore23
{
    Info info1;
    Info info2;

    struct Arvore23 *esq;
    struct Arvore23 *cent;
    struct Arvore23 *dir;

    int nInfos;

} Arvore23;

int inserirArvore23(Arvore23 **arvore, Info info);
Arvore23 *criaNo(const Info informacao, Arvore23 *filhoesq, Arvore23 *filhocen);
void adicionaChave(Arvore23 *no, const Info informacao, Arvore23 *filho);
Arvore23 *quebraNo(Arvore23 *no, const Info informacao, Info *promove, Arvore23 *filho);
int ehFolha(const Arvore23 *no);
Arvore23 *inserirArv23(Arvore23 **no, Info informacao, Info *promove, Arvore23 **pai);


Arvore23 *BuscarPalavra(Arvore23 *no, int info);

void arvore23_exibir_pos(Arvore23 *raiz);
void arvore23_exibir_pre(Arvore23 *raiz);
void arvore23_exibir_ordem(Arvore23 *raiz);

Info *no23_maior_info(Arvore23 *raiz);
Arvore23 *arvore23_buscar_menor_pai(Arvore23 *raiz, int info);
Arvore23 *arvore23_buscar_menor_pai_2_infos(Arvore23 *raiz, int info);
Arvore23 *arvore23_buscar_maior_pai(Arvore23 *raiz, int info);
Arvore23 *arvore23_buscar_pai(Arvore23 *raiz, int info);
Arvore23 *arvore23_buscar_menor_filho(Arvore23 *raiz, Arvore23 **pai);
Arvore23 *arvore23_buscar_maior_filho(Arvore23 *raiz, Arvore23 **pai, Info **maior_valor);
int arvore23_rebalancear(Arvore23 **raiz, int info, Arvore23 **maior);
Arvore23 *no23_juntar(Arvore23 *filho1, Info info, Arvore23 *maior, Arvore23 **raiz);
int possivel_remover(Arvore23 *raiz);
int arvore23_remover2(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);
int arvore23_remover1(Arvore23 **raiz, int info, Arvore23 *pai, Arvore23 **origem, Arvore23 **maior);
int arvore23_remover_nao_folha2(Arvore23 **origem, Arvore23* raiz, Info *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior);
int arvore23_remover_nao_folha1(Arvore23 **origem, Arvore23* raiz, Info *info, Arvore23 *filho1, Arvore23 *filho2, Arvore23 **maior);
int arvore_2_3_remover(Arvore23 **raiz, int info);

void freeNo(Arvore23 **no);
void freeTree(Arvore23 **no);


#endif