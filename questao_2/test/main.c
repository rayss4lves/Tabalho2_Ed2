#include "maintest.h"

#define TEST_SIZE 30

PortuguesRB *buscaTestes(PortuguesRB **tree, char *codigo, int n)
{
    PortuguesRB *no = NULL;
    if (*tree != NULL)
    {
        // Comparando strings (codigo e palavras na árvore)
        if (strcmp(codigo, (*tree)->info.palavraPortugues) == 0)
        {
            printf("Info: %s\n\n", (*tree)->info.palavraPortugues);
            no = *tree;
        }
        else if (strcmp(codigo, (*tree)->info.palavraPortugues) < 0)
        {
            printf("%d-esquerda\n", n);
            no = buscaTestes(&(*tree)->esq, codigo, n + 1);
        }
        else
        {
            printf("%d-direita\n", n);
            no = buscaTestes(&(*tree)->dir, codigo, n + 1);
        }
    }
    return no;
}

void carregarArquivoTeste(const char *nomeArquivo, PortuguesRB **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL)
    {
        char linha[256];

        char unidadeAtual[50];

        while (fgets(linha, sizeof(linha), arquivo))
        {

            if (linha[0] == '%')
            {
                // Atualiza a unidade corretamente
                sscanf(linha, "%% Unidade %[^\n]", unidadeAtual);
            }
            else
            {
                char palavraIngles[50], traducoesPortugues[200];
                sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }

        fclose(arquivo);
    }
}

void buscaCrescente(char palavras[][20], PortuguesRB *raiz)
{
    clock_t start, end;
    double totalTime, media;

    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
        buscaTestes(&raiz, palavras[i], 0); // Busca na árvore
    end = clock();
    totalTime = (double)(end - start) / CLOCKS_PER_SEC;
    media = totalTime / TEST_SIZE;
    printf("\nTempo medio de busca: %.8lf ms\n\n", media);
}

void buscaAleatoria(char palavrasAleatorias[][20], PortuguesRB *raiz)
{
    clock_t start, end;
    double totalTime, media;

    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
        buscaTestes(&raiz, palavrasAleatorias[i], 0); // Busca na árvore
    end = clock();

    totalTime = (double)(end - start) / CLOCKS_PER_SEC;
    media = totalTime / TEST_SIZE;
    printf("\nTempo medio de busca (aleatorio): %.8lf ms\n", media);
}

int main_teste(void)
{

    PortuguesRB *raiz = NULL;
    char palavras[30][20] = {
        "abacaxi", "aluno", "amigo", "avião", "bolsa", "cachorro", "carro", "casa",
        "chave", "computador", "escola", "felicidade", "flor", "floresta", "gato",
        "guerra", "janela", "jardim", "livro", "lua", "mesa", "oceano", "porta",
        "praia", "roupas", "sol", "tecnologia", "telefone", "universidade", "água"};
    char palavrasAleatorias[30][20] = {
        "universidade", "bolsa", "roupas", "felicidade", "sol", "avião", "gato", "tecnologia", "escola", "oceano",
        "cachorro", "abacaxi", "guerra", "floresta", "computador", "flor", "porta", "lua", "praia", "jardim", "mesa",
        "carro", "amigo", "janela", "casa", "água", "telefone", "livro", "chave", "aluno"};

    carregarArquivoTeste("../teste.txt", &raiz);

    // Busca na árvore
    printf("\nBusca crescente:\n");
    buscaCrescente(palavras, raiz);
    printf("\nBusca aleatoria:\n");
    buscaAleatoria(palavrasAleatorias, raiz);

    

    return 0;
}
