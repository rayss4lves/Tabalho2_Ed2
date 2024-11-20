#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arv23.h"
//#include "arvbin.h"


void carregarArquivo(const char *nomeArquivo, Portugues23 **arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    Info promove;
    Portugues23 *pai = NULL;
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;

        if (linha[0] == '%') {
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            char palavraIngles[50], traducoesPortugues[200];
            if (sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues) == 2) {
                printf("Lendo: Palavra Inglês = '%s', Traduções: '%s'\n", palavraIngles, traducoesPortugues);
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL) {
                    while (*traducaoPortugues == ' ') traducaoPortugues++;
                    Info novoInfo = criaInfo(traducaoPortugues, palavraIngles, unidadeAtual);
                    inserirArv23(arvore, &novoInfo, &promove, &pai);
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}



int main(){
    Portugues23 *raiz = NULL;
    carregarArquivo("/mnt/c/Users/Rayssa Alves/Documents/trabalho_ed2/Tabalho2_Ed2/trabalhoEd2.txt", &raiz);
    printf("Árvore 2-3 carregada:\n");
    
    //imprimirArvore(raiz, 0);
    
    exibir_tree23(raiz);
    printf("\n--------------------------------------------------------------- \n");
    imprimirArvorePorUnidade(raiz);

    printf("\n--------------------------------------------------------------- \n");
    printf("\nPalavras da unidade 1: \n");
    exibir_traducoes_unidade(raiz, 1);

    imprimirTraducoesEmIngles(raiz, "sistema");

    

    //freeTree(raiz);

    return 0;
}