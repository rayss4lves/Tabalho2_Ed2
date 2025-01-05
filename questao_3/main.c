#include <stdio.h>
#include <stdlib.h>
#include "src/arv23.h"
#include "src/memoria.h"

int menu()
{
    int op;
    printf("\n-----------------------------------------------\n");
    printf("\nMENU");
    printf("\n1 - Alocar Nos");
    printf("\n2 - Liberar Nos");
    printf("\n3 - Exibir Nos");
    printf("\n0 - Sair");
    printf("\n-----------------------------------------------\n");
    scanf("%d", &op);

    return op;
}

int main()
{
    Arvore23 *arvore;
    arvore = NULL;

    int maximo = 0;
    printf("---------------------------------------------------------------------\n");
    printf("        Bem-vindo ao programa de gerenciamento de memoria!        ");
    printf("\n---------------------------------------------------------------------\n");

    do
    {
        printf("\n--------------------------------------------------------------------\n");
        printf("        Informe a quantidade maxima de blocos de memoria:         \n");
        scanf("%d", &maximo);
        if(maximo <= 0)
            printf("\nDigite uma quantidade positiva!\n\n");
    } while(maximo <= 0);
    printf("\n--------------------------------------------------------------------\n\n");
    int minimo = cadastrar_nos(&arvore, maximo);

    int op, quantidadeNos, status;
    do
    {
        op = menu();
        
        switch(op)
        {
            case 1:
                printf("--------------------------------------------------------------------\n\n");
                printf("\ninforme a quantidade de nos a serem alocados: ");
                compara_numero(&quantidadeNos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quantidadeNos, LIVRE);
                printf("--------------------------------------------------------------------\n\n");
                break;

            case 2:
                printf("--------------------------------------------------------------------\n\n");
                printf("\ninforme a quantidade de nos a serem liberados: ");
                compara_numero(&quantidadeNos, minimo, maximo+1);
                alocar_desalocar_no(&arvore, quantidadeNos, OCUPADO);
                printf("--------------------------------------------------------------------\n\n");
                break;

            case 3:
                printf("--------------------------------------------------------------------\n\n");
                printf("\nExibindo arvore Pre-Ordem\n");
                exibir_Arvore23(arvore);
                printf("--------------------------------------------------------------------\n\n");
                break;

            case 0:
                printf("--------------------------------------------------------------------\n");
                printf("\nSaindo do programa...\n");
                break;

            default:
                printf("\nInsira uma opcao valida!\n");
                break;
        }
    } while (op != 0);
    
    freeTree(&arvore);

    return 0;
}