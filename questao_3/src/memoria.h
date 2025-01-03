#include "arv23.h"
#include <stdlib.h>
#include <stdio.h>

int quantidade_disponivel(Info info);

void inserir_endereco_inicial(int *variavel, int minimo, int maximo);

void inserir_endereco_final(int *variavel, int minimo, int maximo);

void inserir_nos(Arvore23 **arvore, int inicio, int maximo, int status);

int cadastrar_nos(Arvore23 **arvore, int maximo);

void compara_numero(int *variavel, int minimo, int maximo);
void buscar_no_memoria(Arvore23 **arvore, int quant, int status, Info **info_escolhido, Arvore23 **no);
void trocar_status(int status, int *novoStatus);
void juntarNoMemoria(Arvore23 **raiz, int *fim, int limite, int valor_remover);

Arvore23 *buscar_menor_bloco(Arvore23 **raiz, Arvore23 *no, Info *info, Info **valor_menor);

Arvore23 *buscar_maior_bloco(Arvore23 **raiz, Arvore23 *no, Info *info, Info **valor_maior);

void alterar_no(Arvore23 **raiz, Arvore23 *no, Info *info, int quant);

void no23_exibir(Info no);
void arvore23_exibir_pre(Arvore23 *raiz);

void arvore23_exibir_ordem(Arvore23 *raiz);

void arvore23_exibir_pos(Arvore23 *raiz);


int alocar_desalocar_no(Arvore23 **arvore, int quantidadeNos, int status);