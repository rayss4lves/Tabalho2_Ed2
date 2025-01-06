#include "memoria.h"

int quantidade_disponivel(Info info)
{
    return info.fim - info.inicio + 1;
}

void inserir_endereco_inicial(int *variavel, int minimo, int maximo)
{
    do
    {
        printf("\nEndereco inicial: ");
        scanf("%d", variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um numero entre %d e %d\n", minimo, maximo-1);
    }while(*variavel < minimo || *variavel >= maximo);
}

void inserir_endereco_final(int *variavel, int minimo, int maximo)
{
    do
    {
        printf("\nEndereco final: ");
        scanf("%d", variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um numero entre %d e %d\n", minimo, maximo-1);
    }while(*variavel < minimo || *variavel >= maximo);
}

void trocar_status(int status, int *novoStatus){
    if(status == 0)
        *novoStatus = 1;
    else
        *novoStatus = 0;
}


void inserir_nos(Arvore23 **arvore, int inicio, int maximo, int status, int minimo) {
    Info no;
    no.endereco_inicial = inicio;

    do {
        no.status = status;
        no.inicio = minimo;
        no.fim = maximo - 1;
        inserir_endereco_final(&no.endereco_final, no.endereco_inicial, maximo);

        inserirArvore23(arvore, no);

        no.endereco_inicial = no.endereco_final + 1;

        trocar_status(status, &status);
    }while (no.endereco_final < (maximo - 1));
}

int pegar_status() {
    int status;
    int confirm = 0;

    while (!confirm){
        printf("\n-------------------------------------------\n");
        printf("\nEscolha um status para o no:");
        printf("\n1 - Livre");
        printf("\n0 - Ocupado");
        printf("\nStatus: \n");
        scanf("%d", &status);
        printf("\n-------------------------------------------\n");

        if (status != LIVRE && status != OCUPADO)
            printf("\nDigite uma opcao valida!\n");
        else
            confirm = 1;
    } 
    return status;
}

int cadastrar_nos(Arvore23 **arvore, int maximo) {
    int status = pegar_status();
    
    Info no;
    inserir_endereco_inicial(&no.endereco_inicial, 0, maximo);
    int minimo = no.endereco_inicial;
    inserir_nos(arvore, no.endereco_inicial, maximo, status, minimo);

    return minimo;
}

void compara_numero(int *variavel, int minimo, int maximo)
{
    do
    {
        scanf("%d", variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um numero entre %d e %d\n", minimo, maximo-1);
    }while(*variavel < minimo || *variavel >= maximo);
}


void buscar_no_memoria(Arvore23 **arvore, int quant, int status, Info **info_escolhido, Arvore23 **no)
{
    if(*arvore != NULL)
    {
        buscar_no_memoria(&((*arvore)->esq), quant, status, info_escolhido, no);

        if(*info_escolhido == NULL)
        {
            if((*arvore)->info1.status == status && quantidade_disponivel((*arvore)->info1) >= quant)
            {
                *info_escolhido = &((*arvore)->info1);
                *no = *arvore;
            }
            else
            {
                buscar_no_memoria(&((*arvore)->cent), quant, status, info_escolhido, no);
                if((*arvore)->nInfos == 2)
                {
                    if((*arvore)->info2.status == status && quantidade_disponivel((*arvore)->info2) >= quant)
                    {
                        *info_escolhido = &((*arvore)->info2);
                        *no = *arvore;
                    }
                    else if(*info_escolhido == NULL)
                        buscar_no_memoria(&((*arvore)->dir), quant, status, info_escolhido, no);
                }
            }
        }
    }
    else
        *info_escolhido = NULL;

}

void juntarNoMemoria(Arvore23 **raiz, int *fim, int limite, int valor_remover)
{
    *fim = limite;
    arvore_2_3_remover(raiz, valor_remover);
}

Arvore23 *buscar_menor_bloco(Arvore23 **raiz, Arvore23 *no, Info *info, Info **valor_menor)
{
    Arvore23 *menor, *pai;
    *valor_menor = NULL;

    if(ehFolha(no))
    {
        if(no->info1.endereco_inicial != info->endereco_inicial)
            menor = no;
        else
            menor = arvore23_buscar_menor_pai(*raiz, info->endereco_inicial);

        if(menor != NULL)
        {
            if(menor->nInfos == 2 && menor->info2.endereco_inicial < info->endereco_inicial)
                *valor_menor = &(menor->info2);
            else
                *valor_menor = &(menor->info1);
        }
    }
    else if(no->info1.endereco_inicial == info->endereco_inicial)
        menor = arvore23_buscar_maior_filho(no->esq, &pai, valor_menor);
    else
        menor = arvore23_buscar_maior_filho(no->cent, &pai, valor_menor);

    return menor;
}

Arvore23 *buscar_maior_bloco(Arvore23 **raiz, Arvore23 *no, Info *info, Info **valor_maior)
{
    Arvore23 *maior;
    Arvore23 *pai;
    *valor_maior = NULL;

    if(ehFolha(no))
    {
        if(no->nInfos == 2 && no->info1.endereco_inicial == info->endereco_inicial)
            maior = no;
        else
            maior = arvore23_buscar_maior_pai(*raiz, info->endereco_inicial);

        if (maior != NULL)
        {
            if(maior->nInfos == 2 && maior->info2.endereco_inicial > info->endereco_inicial)
                *valor_maior = &(maior->info2);
            else
                *valor_maior = &(maior->info1);
        }
        
    }
    else
    {
        if(no->info1.inicio == info->inicio)
            maior = arvore23_buscar_menor_filho(no->cent, &pai);
        else
            maior = arvore23_buscar_menor_filho(no->dir, &pai);

        if(maior != NULL)
            *valor_maior = &(maior->info1);
    }

    return maior;
}

void alterar_no(Arvore23 **raiz, Arvore23 *no, Info *info, int quant)
{
    Arvore23 *menor;
    Info *valor_menor;

    menor = buscar_menor_bloco(raiz, no, info, &valor_menor);

    if(quant < quantidade_disponivel(*info))
    {
        if(menor == NULL)
        {
            Info data;
            data.inicio = info->inicio;
            data.fim = info->fim;
            data.endereco_inicial = info->endereco_inicial;
            data.endereco_final = info->endereco_inicial + quant - 1;
            trocar_status(info->status, &(data.status));

            info->endereco_inicial += quant;
            inserirArvore23(raiz, data);
        }
        else
        {
            valor_menor->endereco_final += quant;
            info->endereco_inicial += quant;
        }
    }
    else
    {
        Arvore23 *maior;
        Info *valor_maior;

        maior = buscar_maior_bloco(raiz, no, info, &valor_maior);

        if(menor == NULL && maior == NULL)
            trocar_status(info->status, &(info->status));
        else
        {
            if(menor == NULL)
            {
                trocar_status(info->status, &(info->status));
                juntarNoMemoria(raiz, &(info->endereco_final), valor_maior->endereco_final, valor_maior->endereco_inicial);
            }
            else if(maior == NULL)
                juntarNoMemoria(raiz, &(valor_menor->endereco_final), info->endereco_final, info->endereco_inicial);
            else
            {
                int numero = valor_maior->endereco_inicial;
                juntarNoMemoria(raiz, &(valor_menor->endereco_final), valor_maior->endereco_final, info->endereco_inicial);
                arvore_2_3_remover(raiz, numero);
            }
        }
    }
}


int alocar_desalocar_no(Arvore23 **arvore, int quantidadeNos, int status)
{
    Info *info_escolhido;
    info_escolhido = NULL;
    Arvore23 *no_escolhido = NULL;
    buscar_no_memoria(arvore, quantidadeNos, status, &info_escolhido, &no_escolhido);

    if(info_escolhido != NULL)
    {
        printf("\nNo escolhido: \n");
        no23_exibir(*info_escolhido);

        alterar_no(arvore, no_escolhido, info_escolhido, quantidadeNos);
    }
    else
        printf("\nNao ha no disponivel\n");
}