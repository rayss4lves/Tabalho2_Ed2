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
    }else{
        char linha[256];
     // Variável para armazenar a unidade atual
    Info promove;         // Armazena o valor promovido durante inserções
    Portugues23 *pai = NULL;
    int unidadeAtual = 0;  // Inicializa unidadeAtual
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove a quebra de linha ao final
        linha[strcspn(linha, "\n")] = 0;

        // Verifica se a linha indica uma unidade
        if (linha[0] == '%') {
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            // Processar linha com palavra em inglês e traduções em português
            char palavraIngles[50];
            char traducoesPortugues[200];

            if (sscanf(linha, "%[^:]: %[^;];", palavraIngles, traducoesPortugues) == 2) {
                // Processar cada tradução em português
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL) {
                    // Remove espaços em branco no início da tradução
                    while (*traducaoPortugues == ' ') traducaoPortugues++;
                    
                    // Criar um novo Info para a tradução em português
                    Info novoInfo = criaInfo(traducaoPortugues, palavraIngles, unidadeAtual);
                    //adicionarTraducaoEmIngles(&novoInfo, palavraIngles, unidadeAtual);  

                    // Inserir o novo Info na árvore 2-3
                    inserirArv23(arvore, &novoInfo, &promove, &pai);
                    //printf("Inseriu\n");
                    
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
    }

    
}



int main(){
    Portugues23 *raiz = NULL;
    carregarArquivo("/mnt/c/Users/Rayssa Alves/Documents/trabalho_ed2/Tabalho2_Ed2/trabalhoEd2.txt", &raiz);
    printf("Árvore 2-3 carregada:\n");
    
    //imprimirArvore(raiz, 0);
    
    exibir_tree23(raiz);

    

    //freeTree(raiz);

    return 0;
}