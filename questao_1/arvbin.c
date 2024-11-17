// #include "structs.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>


// Função para criar um novo nó na árvore binária de busca
// Inglesbin* createNode(const char* word, int unidade) {
//     Inglesbin* newNode = (Inglesbin*)malloc(sizeof(Inglesbin));
//     if (newNode != NULL) {
//         strcpy(newNode->palavraIngles, word);
//         //newNode->unidade = unidade;
//         newNode->esq = newNode->dir = NULL;
//     }
//     return newNode;
// }

// // Função para inserir uma palavra em inglês na árvore binária de busca
// Inglesbin* insertpalavraIngles(Inglesbin* root, const char* word, int unidade) {
//     Inglesbin *result;
//     if (root == NULL) {
//         result = createNode(word, unidade);
//     } else {
//         if (strcmp(word, root->palavraIngles) < 0) {
//             root->esq = insertpalavraIngles(root->esq, word, unidade);
//         } else if (strcmp(word, root->palavraIngles) > 0) {
//             root->dir = insertpalavraIngles(root->dir, word, unidade);
//         }
//         result = root;
//     }
//     return result;
// }

// Função para buscar uma palavra em inglês na árvore binária de busca
// Inglesbin* searchpalavraIngles(Inglesbin* root, const char* word) {
//     Inglesbin *result;
//     if (root == NULL || strcmp(root->palavraIngles, word) == 0) {
//         result = root;
//     } else {
//         if (strcmp(word, root->palavraIngles) < 0) {
//             result = searchpalavraIngles(root->esq, word);
//         } else {
//             result = searchpalavraIngles(root->dir, word);
//         }
//     }
//     return result;
// }

// // Função para imprimir a árvore binária em ordem alfabética (simples visualização)
// void printBinaryTree(Inglesbin* root) {
//     if (root != NULL) {
//         printBinaryTree(root->esq);
//         printf("%s (unidade %d)\n", root->palavraIngles, root->unidade);
//         printBinaryTree(root->dir);
//     }
// }

// Inglesbin* removepalavraIngles(Inglesbin* root, const char* word) {
//     if (root != NULL) {
//         if (strcmp(word, root->palavraIngles) < 0) {
//             // A palavra a ser removida está na subárvore esquerda
//             root->esq = removepalavraIngles(root->esq, word);
//         } else if (strcmp(word, root->palavraIngles) > 0) {
//             // A palavra a ser removida está na subárvore direita
//             root->dir = removepalavraIngles(root->dir, word);
//         } else {
//             // Encontrou o nó a ser removido
//             if (root->esq == NULL && root->dir == NULL) {
//                 // Caso 1: O nó é uma folha
//                 free(root);
//                 root = NULL;
//             } else if (root->esq == NULL) {
//                 // Caso 2: O nó tem apenas um filho à direita
//                 Inglesbin* aux = root;
//                 root = root->dir;
//                 free(aux);
//             } else if (root->dir == NULL) {
//                 // Caso 3: O nó tem apenas um filho à esquerda
//                 Inglesbin* aux = root;
//                 root = root->esq;
//                 free(aux);
//             } else {
//                 // Caso 4: O nó tem dois filhos
//                 // Encontrar o maior elemento da subárvore esquerda (ou o menor da subárvore direita)
//                 Inglesbin* aux = root->esq;
//                 while (aux->dir != NULL) {
//                     aux = aux->dir;
//                 }
//                 // Copiar o valor do maior elemento encontrado para o nó atual
//                 strcpy(root->palavraIngles, aux->palavraIngles);
//                 root->unidade = aux->unidade;
//                 // Remover o nó duplicado da subárvore esquerda
//                 root->esq = removepalavraIngles(root->esq, aux->palavraIngles);
//             }
//         }
//     }
//     return root;
// }

// void imprimirTraducoes(Inglesbin *node, const char *palavraPortugues, int *primeira) {
//     if (node) {
//         // Imprime o nó esquerdo
//         imprimirTraducoes(node->esq, palavraPortugues, primeira);

//         // Imprime a palavra em inglês e a tradução em português
//         if (!*primeira) {
//             printf("\n");
//         }
//         printf("%s: %s;", node->palavraIngles, palavraPortugues);
//         *primeira = 0;

//         // Imprime o nó direito
//         imprimirTraducoes(node->dir, palavraPortugues, primeira);
//     }
// }