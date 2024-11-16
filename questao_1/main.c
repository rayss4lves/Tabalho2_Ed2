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
    int unidadeAtual = 0; // Variável para armazenar a unidade atual
    Info promove;         // Armazena o valor promovido durante inserções
    Portugues23 *pai = NULL;

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove a quebra de linha ao final
        linha[strcspn(linha, "\n")] = 0;

        // Verifica se a linha indica uma unidade
        if (linha[0] == '%') {
            if (sscanf(linha, "%% Unidade %d", &unidadeAtual) == 1) {
                printf("Carregando Unidade %d...\n", unidadeAtual); // Opcional: mensagem de depuração
            }
        } else {
            // Processar linha com palavra em inglês e traduções em português
            char palavraIngles[50];
            char traducoesPortugues[200];

            if (sscanf(linha, "%49[^:]: %199[^\n]", palavraIngles, traducoesPortugues) == 2) {
                // Processar cada tradução em português
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL) {
                    // Remove espaços em branco no início da tradução
                    while (*traducaoPortugues == ' ') traducaoPortugues++;

                    // Criar um novo Info para a tradução em português
                    Info novoInfo = {
                        .palavraPortugues = strdup(traducaoPortugues), // Alocar string dinamicamente
                        .palavraIngles = NULL,                        // Preencher conforme necessário
                        .unidade = unidadeAtual
                    };

                    // Inserir o novo Info na árvore 2-3
                    inserirArv23(arvore, &novoInfo, &promove, &pai);

                    // Próxima tradução
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}


//################################### CORRIJIR ESSA PARTE #########################################
// void exibirPalavrasPorUnidade(Portugues23 *arvore, int unidade) {
//     if (arvore) {
//         // Percorre a subárvore esquerda
//         exibirPalavrasPorUnidade(arvore->esq, unidade);

//         // Verifica e exibe palavras associadas à unidade em `info1`
//         if (arvore->info1.unidade == unidade) {
//             printf("%s: ", arvore->info1.palavraPortugues);
//             printBinaryTree(arvore->info1.palavraIngles); // Exibe as traduções associadas
//         }

//         // Verifica e exibe palavras associadas à unidade em `info2` (se existir)
//         if (arvore->nInfos == 2 && arvore->info2.unidade == unidade) {
//             printf("%s: ", arvore->info2.palavraPortugues);
//             printBinaryTree(arvore->info2.palavraIngles); // Exibe as traduções associadas
//         }

//         // Percorre a subárvore do meio
//         exibirPalavrasPorUnidade(arvore->cent, unidade);

//         // Percorre a subárvore direita, se houver
//         if (arvore->nInfos == 2) {
//             exibirPalavrasPorUnidade(arvore->dir, unidade);
//         }
//     }
// }


// // Função auxiliar para verificar se há palavras em uma unidade específica
// void verificarUnidade(Portugues23 *no, int unidade, int *temPalavras) {
//     if (no) {
//         if (no->info1.unidade == unidade || (no->nInfos == 2 && no->info2.unidade == unidade)) {
//             *temPalavras = 1;
//         }
//         verificarUnidade(no->esq, unidade, temPalavras);
//         verificarUnidade(no->cent, unidade, temPalavras);
//         if (no->nInfos == 2) {
//             verificarUnidade(no->dir, unidade, temPalavras);
//         }
//     }
// }

// void exibirArvoreFormatoArquivo(Portugues23 *arvore) {
//     int unidade = 1;

//     while (1) {
//         int temPalavras = 0;

//         // Verifica se há palavras na unidade atual
//         verificarUnidade(arvore, unidade, &temPalavras);

//         // Se não houver palavras, interrompe o loop
//         if (!temPalavras) {
//             break;
//         }

//         // Exibe o cabeçalho da unidade
//         printf("%% Unidade %d\n", unidade);

//         // Exibe as palavras da unidade atual
//         exibirPalavrasPorUnidade(arvore, unidade);

//         // Passa para a próxima unidade
//         unidade++;
//     }
// }


int main(){
    Portugues23 *raiz = NULL;
    carregarArquivo("/mnt/c/Users/Rayssa Alves/Documents/trabalho_ed2/Tabalho2_Ed2/trabalhoEd2.txt", &raiz);
    printf("Árvore 2-3 carregada:\n");
    exibir_tree23(raiz);
    //exibirArvoreFormatoArquivo(raiz);
    // Info info2 = criaInfo("gato", 2);
    // adicionarTraducaoEmIngles(&info2, "cat");

    // Info info1 = criaInfo("cachorro", 1);
    // adicionarTraducaoEmIngles(&info1, "dog");

    // Info info3 = criaInfo("passarinho", 3);
    // adicionarTraducaoEmIngles(&info3, "bird");

    // Info info4 = criaInfo("peixe", 4);
    // adicionarTraducaoEmIngles(&info4, "fish");

    // Info info5 = criaInfo("cavalo", 5);
    // adicionarTraducaoEmIngles(&info5, "horse");

    // Info promovido;
    // inserirArv23(&raiz, &info1, &promovido, &raiz);
    // printf(("\n\nTESTE 1\n\n"));
    // exibir_tree23(raiz);
    // inserirArv23(&raiz, &info2, &promovido, &raiz);
    // printf(("\n\nTESTE 2\n\n"));
    // exibir_tree23(raiz);
    // inserirArv23(&raiz, &info3, &promovido, &raiz);
    // printf(("\n\nTESTE 3\n\n"));
    // exibir_tree23(raiz);
    // inserirArv23(&raiz, &info4, &promovido, &raiz);
    // printf(("\n\nTESTE 4\n\n"));
    // exibir_tree23(raiz);
    // inserirArv23(&raiz, &info5, &promovido, &raiz);
    // printf(("\n\nTESTE 5\n\n"));
    // exibir_tree23(raiz);

    // printf("\nExibindo a arvore 23 com traducoes:\n");
    // exibir_tree23(raiz);

    //buscarValorArvore(Portugues23 **arvore, const char *valor, Portugues23 **irmao)

    // freeTree(&raiz);

    return 0;
}