#include "maintest.h"

#define TEST_SIZE 30

Portugues23 *buscaTestes(Portugues23 **tree, char *codigo, int n)
{
    Portugues23 *no = NULL;
    if (*tree != NULL)
    {
        // Comparando strings (codigo e palavras na árvore)
        if (strcmp(codigo, (*tree)->info1.palavraPortugues) == 0)
        {
            printf("%s\n", (*tree)->info1.palavraPortugues);
            no = *tree;
        }
        else if ((*tree)->nInfos == 2 && strcmp(codigo, (*tree)->info2.palavraPortugues) == 0)
        {
            printf("%s\n", (*tree)->info2.palavraPortugues);
            no = *tree;
        }
        else
        {
            if (strcmp(codigo, (*tree)->info1.palavraPortugues) < 0)
            {
                printf("%d-left \n", n);
                no = buscaTestes(&(*tree)->esq, codigo, n + 1);
            }
            else if ((*tree)->nInfos == 1 || strcmp(codigo, (*tree)->info2.palavraPortugues) < 0)
            {
                printf("%d-center \n", n);
                no = buscaTestes(&(*tree)->cent, codigo, n + 1);
            }
            else
            {
                printf("%d-right \n", n);
                no = buscaTestes(&(*tree)->dir, codigo, n + 1);
            }
        }
    }
    return no;
}


int main_teste(void)
{
    clock_t start, end;
    float totalTime = 0;
    char *ingles[] = {
    "friend", "house", "window", "tree", "book",
    "pen", "dog", "cat", "chair", "table",
    "school", "city", "river", "mountain", "car",
    "plane", "train", "road", "beach", "ocean",
    "computer", "phone", "music", "movie", "game",
    "food", "water", "sun", "moon", "star"
};

char *portugues[] = {
    "amigo", "casa", "janela", "árvore", "livro",
    "caneta", "cachorro", "gato", "cadeira", "mesa",
    "escola", "cidade", "rio", "montanha", "carro",
    "avião", "trem", "estrada", "praia", "oceano",
    "computador", "telefone", "música", "filme", "jogo",
    "comida", "água", "sol", "lua", "estrela"
};


    Portugues23 *arvore = NULL;
    Info info, promove;
    Portugues23 *pai = NULL;

    for (int i = 0; i < 30; i++)
    {
        
        info = criaInfo(portugues[i], ingles[i], 1);
        inserirArv23(&arvore, &info, &promove, &pai);
    }

    // Busca na árvore
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
    {
        printf("%d busca\n", i);
        
        buscaTestes(&arvore, portugues[i], 0); // Busca na árvore
    }
    end = clock();

    totalTime = (float)(end - start) / CLOCKS_PER_SEC * 1000.0;
    float media = totalTime / TEST_SIZE;

    printf("\n\nTempo médio de busca: %.8f ms\n", media);
    return 0;
}
