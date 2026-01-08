/*
9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u èvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraæa
pokazivaè na korijen stabla.
b) Napisati funkciju replace koja æe svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat æe se u stablo na
slici Slika 2.
c) Prepraviti program na naèin da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira sluèajne brojeve u rasponu <10, 90>. Takoðer, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.*/#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node* NodePtr;
typedef struct node {
    int data;
    NodePtr left;
    NodePtr right;
} Node;


NodePtr createNode(int data);
NodePtr insert(NodePtr root, int data);
int replace(NodePtr root);
int inorder(NodePtr root, FILE* f);
int deleteTree(NodePtr root);

int main() {
    NodePtr root = NULL;
    NodePtr rootRandom = NULL;
    int niz[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int n = 10;

    FILE* f = fopen("stablo.txt", "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    /* a) stvaranje stabla iz zadanog niza */
    for (int i = 0; i < n; i++)
        root = insert(root, niz[i]);

    fprintf(f, "Inorder prije replace (zadani niz):\n");
    inorder(root, f);

    /* b) replace */
    replace(root);

    fprintf(f, "\nInorder nakon replace (zadani niz):\n");
    inorder(root, f);

    deleteTree(root);

    /* c) slucajni brojevi */
    srand((unsigned)time(NULL));

    for (int i = 0; i < n; i++) {
        int value = rand() % 81 + 10;
        rootRandom = insert(rootRandom, value);
    }

    fprintf(f, "\n\nInorder prije replace (random):\n");
    inorder(rootRandom, f);

    replace(rootRandom);

    fprintf(f, "\nInorder nakon replace (random):\n");
    inorder(rootRandom, f);

    deleteTree(rootRandom);
    fclose(f);

    return 0;
}

NodePtr createNode(int data) {
    NodePtr newNode = (NodePtr)malloc(sizeof(Node));
    if (!newNode)
        return NULL;

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* INSERT PO RAZINAMA */
NodePtr insert(NodePtr root, int data) {
    if (root == NULL)
        return createNode(data);

    NodePtr queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        NodePtr current = queue[front++];

        if (current->left == NULL) {
            current->left = createNode(data);
            return root;
        }
        else
            queue[rear++] = current->left;

        if (current->right == NULL) {
            current->right = createNode(data);
            return root;
        }
        else
            queue[rear++] = current->right;
    }

    return root;
}

/* replace – cvor postaje suma potomaka */
int replace(NodePtr root) {
    if (root == NULL)
        return 0;

    int oldValue = root->data;
    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    root->data = leftSum + rightSum;
    return oldValue + root->data;
}

/* inorder prolaz */
int inorder(NodePtr root, FILE* f) {
    if (root == NULL)
        return 0;

    inorder(root->left, f);
    fprintf(f, "%d ", root->data);
    inorder(root->right, f);

    return 0;
}

/* oslobadanje memorije */
int deleteTree(NodePtr root) {
    if (root == NULL)
        return 0;

    deleteTree(root->left);
    deleteTree(root->right);
    free(root);

    return 0;
}

