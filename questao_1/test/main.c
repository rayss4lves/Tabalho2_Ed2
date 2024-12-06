#include "maintest.h"

#define TEST_SIZE 30
#define DIGIT 2

Portugues23 *buscaTestes(Portugues23 **tree, char *codigo, int n) {
    Portugues23 *no = NULL;
    if (*tree != NULL) {
        if (strcmp(codigo, (*tree)->info1.palavraPortugues) == 0) {
            no = *tree;
        } else if ((*tree)->nInfos == 2 && strcmp(codigo, (*tree)->info2.palavraPortugues) == 0) {
            no = *tree;
        } else {
            if (strcmp(codigo, (*tree)->info1.palavraPortugues) < 0) {
                printf("%d-left ", n);
                no = buscaTestes(&(*tree)->esq, codigo, n + 1);
            } else if ((*tree)->nInfos == 1 || strcmp(codigo, (*tree)->info2.palavraPortugues) < 0) {
                printf("%d-center ", n);
                no = buscaTestes(&(*tree)->cent, codigo, n + 1);
            } else {
                printf("%d-right ", n);
                no = buscaTestes(&(*tree)->dir, codigo, n + 1);
            }
        }
    }
    return no;
}

int main_teste(void) {
    clock_t start, end;
    float totalTime = 0;

    Portugues23 *arvore = NULL;
    Info info, promove;
    Portugues23 *pai = NULL;

    for (int i = 0; i < 1000; i++) {
        char str[DIGIT + 1]; // Aloca memória para a string
        sprintf(str, "%d", i); // Converte o número para string
        printf("\nValor: %d", i); // Imprime o valor de i
        info = criaInfo(str, str, i); // Cria nova informação
        arvore = inserirArv23(&arvore, &info, &promove, &pai);
    }

    // Busca na árvore
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        char str[DIGIT]; // Aloca memória para a string
        sprintf(str, "%d", i * 5); // Converte o valor para string
        buscaTestes(&arvore, str, 0); // Busca na árvore
    }
    end = clock();

    totalTime = (float)(end - start) / CLOCKS_PER_SEC * 1000.0;
    float media = totalTime / TEST_SIZE;

    printf("\n\nTempo médio de busca: %.8f ms\n", media);
    return 0;
}
