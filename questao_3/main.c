#include <stdio.h>
#include <stdlib.h>

#include "src/arv23.h"

int quantidade_disponivel(Info info)
{
    return info.fim - info.inicio + 1;
}

void comparar_numero(char *texto, int *variavel, int minimo, int maximo)
{
    do
    {
        printf("%s", texto);
        scanf("%d", variavel);
        if(*variavel < minimo || *variavel >= maximo)
            printf("\nDigite um numero entre %d e %d\n", minimo, maximo-1);
    } while(*variavel < minimo || *variavel >= maximo);
    printf("Saiu1\n");
}

int cadastrar_nos(Arvore23 **arvore, int maximo)
{
    int status;

    do
    {
        printf("\nO primeiro no esta livre ou ocupado?");
        printf("\n[%d] - %s", LIVRE, "Livre");
        printf("\n[%d] - %s", OCUPADO, "Ocupado");
        printf("\nStatus: ");
        scanf("%d", &status);

        if(status != LIVRE && status != OCUPADO)
            printf("\nDigite uma opcao valida!\n");
    } while(status != LIVRE && status != OCUPADO);

    Info no, promove;
    comparar_numero("\nEndereço inicial: ", &no.inicio, 0, maximo);
    int minimo = no.inicio;
    do
    {
        printf("entrou1\n");
        no.status = status;
        comparar_numero("\nEndereço final: ", &no.fim, no.inicio, maximo);

        inserirArvore23(arvore, no);

        no.inicio = no.fim + 1;
        status = !status;
    }while(no.fim < (maximo - 1));

    return minimo;
}

Arvore23 *buscar_no_memoria(Arvore23 **arvore, int quant, int status, Info **info_escolhido)
{
    Arvore23 *no;
    if(*arvore != NULL)
    {
        no = buscar_no_memoria(&((*arvore)->esq), quant, status, info_escolhido);

        if(*info_escolhido == NULL)
        {
            if((*arvore)->info1.status == status && quantidade_disponivel((*arvore)->info1) >= quant)
            {
                *info_escolhido = &((*arvore)->info1);
                no = *arvore;
            }
            else
            {
                no = buscar_no_memoria(&((*arvore)->cent), quant, status, info_escolhido);
                if((*arvore)->nInfos == 2)
                {
                    if((*arvore)->info2.status == status && quantidade_disponivel((*arvore)->info2) >= quant)
                    {
                        *info_escolhido = &((*arvore)->info2);
                        no = *arvore;
                    }
                    else if(*info_escolhido == NULL)
                        no = buscar_no_memoria(&((*arvore)->dir), quant, status, info_escolhido);
                }
            }
        }
    }
    else
        *info_escolhido = NULL;

    return no;
}

void concatenar_no(Arvore23 **raiz, int *fim, int limite, int valor_remover)
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
        if(no->info1.inicio != info->inicio)
            menor = no;
        else
            menor = arvore23_buscar_menor_pai(*raiz, info->inicio);

        if(menor != NULL)
        {
            if(menor->nInfos == 2 && menor->info2.inicio < info->inicio)
                *valor_menor = &(menor->info2);
            else
                *valor_menor = &(menor->info1);
        }
    }
    else if(no->info1.inicio == info->inicio)
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
        if(no->nInfos == 2 && no->info1.inicio == info->inicio)
            maior = no;
        else
            maior = arvore23_buscar_maior_pai(*raiz, info->inicio);

        if (maior != NULL)
        {
            if(maior->nInfos == 2 && maior->info2.inicio > info->inicio)
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

void modificar_no(Arvore23 **raiz, Arvore23 *no, Info *info, int quant)
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
            data.fim = info->inicio + quant - 1;
            data.status = !(info->status);

            info->inicio += quant;
            inserirArvore23(raiz, data);
        }
        else
        {
            valor_menor->fim += quant;
            info->inicio += quant;
        }
    }
    else
    {
        Arvore23 *maior;
        Info *valor_maior;

        maior = buscar_maior_bloco(raiz, no, info, &valor_maior);

        if(menor == NULL && maior == NULL)
            info->status = !(info->status);
        else
        {
            if(menor == NULL)
            {
                info->status = !(info->status);
                concatenar_no(raiz, &(info->fim), valor_maior->fim, valor_maior->inicio);
            }
            else if(maior == NULL)
                concatenar_no(raiz, &(valor_menor->fim), info->fim, info->inicio);
            else
            {
                int numero = valor_maior->inicio;
                concatenar_no(raiz, &(valor_menor->fim), valor_maior->fim, info->inicio);
                arvore_2_3_remover(raiz, numero);
            }
        }
    }
}

void no23_exibir(Info no)
{
    printf("Bloco de [%d] até [%d] - [%s]\n", no.inicio, no.fim, no.status == LIVRE ? "Livre" : "Ocupado");
    // printf("%d -> ", no.inicio);
}

int alocar_desalocar_no(Arvore23 **arvore, int quant_nos, int status)
{
    Info *info_escolhido;
    info_escolhido = NULL;
    Arvore23 *no_escolhido;
    no_escolhido = buscar_no_memoria(arvore, quant_nos, status, &info_escolhido);

    if(info_escolhido != NULL)
    {
        printf("\nNo escolhido: \n");
        no23_exibir(*info_escolhido);

        modificar_no(arvore, no_escolhido, info_escolhido, quant_nos);
    }
    else
        printf("\nNao ha no disponível\n");
}

int menu()
{
    int op;
    printf("\nMENU");
    printf("\n[1] - Alocar Nos");
    printf("\n[2] - Liberar Nos");
    printf("\n[3] - Exibir Nos (Pré-Ordem)");
    printf("\n[4] - Exibir Nos (Em-Ordem)");
    printf("\n[5] - Exibir Nos (Pos-Ordem)");
    printf("\n[0] - Sair");
    scanf("%d", &op);

    return op;
}

int main()
{
    Arvore23 *arvore;
    arvore = NULL;

    int maximo = 0;

    do
    {
        printf("Quantidade maxima de Blocos de memoria: ");
        scanf("%d", &maximo);
        if(maximo <= 0)
            printf("\nDigite uma quantidade positiva!\n\n");
    } while(maximo <= 0);

    int minimo = cadastrar_nos(&arvore, maximo);

    int op, quant_nos, status;
    do
    {
        op = menu();
        
        switch(op)
        {
            case 1:
                comparar_numero("\nQuantidade de nós a serem alocados: ", &quant_nos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quant_nos, LIVRE);
                break;

            case 2:
                comparar_numero("\nQuantidade de nós a serem desalocados: ",&quant_nos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quant_nos, OCUPADO);
                break;

            case 3:
                printf("\nExibindo arvore [Pré-Ordem]\n");
                arvore23_exibir_pre(arvore);
                break;

            case 4:
                printf("\nExibindo arvore [Em-Ordem]\n");
                arvore23_exibir_ordem(arvore);
                break;

            case 5:
                printf("\nExibindo arvore [Pos-Ordem]\n");
                arvore23_exibir_pos(arvore);
                break;

            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpcao invalida\n");
        }
    } while (op != 0);
    
    freeTree(&arvore);
    return 0;
}