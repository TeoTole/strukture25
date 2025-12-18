#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ERROR (-1)
#define ELEMENT_NOT_FOUND (-2)
#define SUCCESS 0

// Struktura èvora stabla
struct _Node;
typedef struct _Node* Position;

typedef struct _Node
{
    int value;
    Position left;
    Position right;
} Node;

// Deklaracije funkcija
Position create(int value);
int insert(Position* root, int value);
Position find(Position root, int value);
int delete(Position* root, int value);
int inorder(Position root);
int preorder(Position root);
int postorder(Position root);
int levelOrder(Position root);
int printLevel(Position root, int level);
int height(Position root);
Position findMin(Position root);
int freeTree(Position root);

int main()
{
    Position root = NULL; // inicijalizacija korijena
    int choice = 0;
    int value = 0;
    Position found = NULL;

    do
    {
        printf("\nOperacije nad BST:\n");
        printf("1. Unos elementa\n");
        printf("2. Pronadi element\n");
        printf("3. Obrisi element\n");
        printf("4. Inorder ispis\n");
        printf("5. Preorder ispis\n");
        printf("6. Postorder ispis\n");
        printf("7. Level order ispis\n");
        printf("0. Izlaz\n");
        printf("Unesi izbor: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            if (insert(&root, value) < 0)
                printf("Greska kod alociranja memorije!\n");
            else
                printf("Element uspjesno dodan.\n");
            break;
        case 2:
            printf("Unesi vrijednost za pronalazak: ");
            scanf("%d", &value);
            found = find(root, value);
            if (found != NULL)
                printf("Element %d je pronaden.\n", value);
            else
                printf("Element %d nije pronaden.\n", value);
            break;
        case 3:
            printf("Unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            if (delete(&root, value) == ELEMENT_NOT_FOUND)
                printf("Element nije pronaden.\n");
            else
                printf("Element uspjesno obrisan.\n");
            break;
        case 4:
            printf("Inorder ispis: ");
            inorder(root);
            printf("\n");
            break;
        case 5:
            printf("Preorder ispis: ");
            preorder(root);
            printf("\n");
            break;
        case 6:
            printf("Postorder ispis: ");
            postorder(root);
            printf("\n");
            break;
        case 7:
            printf("Level order ispis: ");
            levelOrder(root);
            printf("\n");
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Neispravan izbor!\n");
        }

    } while (choice != 0);

    // Oslobodi cijelo stablo prije izlaza
    freeTree(root);
    root = NULL;

    return 0;
}

// Kreira novi èvor stabla
Position create(int value)
{
    Position newNode = (Position)malloc(sizeof(Node));
    if (!newNode)
        return NULL;

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// Dodaje novi èvor u stablo
int insert(Position* root, int value)
{
    if (*root == NULL)
    {
        *root = create(value);
        if (*root == NULL)
            return MEMORY_ERROR;
        return SUCCESS;
    }

    if (value < (*root)->value)
        return insert(&(*root)->left, value);
    else if (value > (*root)->value)
        return insert(&(*root)->right, value);
    else
        return SUCCESS; // ignorira duplikate
}

// Traži èvor u stablu
Position find(Position root, int value)
{
    if (root == NULL)
        return NULL;
    if (root->value == value)
        return root;
    else if (value < root->value)
        return find(root->left, value);
    else
        return find(root->right, value);
}

// Traži najmanji èvor u stablu
Position findMin(Position root)
{
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

// Briše èvor iz stabla
int delete(Position* root, int value)
{
    if (*root == NULL)
        return ELEMENT_NOT_FOUND;

    if (value < (*root)->value)
        return delete(&(*root)->left, value);
    else if (value > (*root)->value)
        return delete(&(*root)->right, value);
    else
    {
        // Èvor s jednim ili nijednim djetetom
        if ((*root)->left == NULL)
        {
            Position temp = (*root)->right;
            free(*root);
            *root = temp;
            return SUCCESS;
        }
        else if ((*root)->right == NULL)
        {
            Position temp = (*root)->left;
            free(*root);
            *root = temp;
            return SUCCESS;
        }

        // Èvor s dva djeteta
        Position temp = findMin((*root)->right);
        (*root)->value = temp->value;
        return delete(&(*root)->right, temp->value);
    }
}

// Inorder ispis
int inorder(Position root)
{
    if (root == NULL)
        return SUCCESS;
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
    return SUCCESS;
}

// Preorder ispis
int preorder(Position root)
{
    if (root == NULL)
        return SUCCESS;
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
    return SUCCESS;
}

// Postorder ispis
int postorder(Position root)
{
    if (root == NULL)
        return SUCCESS;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);
    return SUCCESS;
}

// Level order ispis
int levelOrder(Position root)
{
    int h = height(root);
    for (int i = 1; i <= h; i++)
    {
        printLevel(root, i);
    }
    return SUCCESS;
}

// Ispis èvorova na odreðenom nivou
int printLevel(Position root, int level)
{
    if (root == NULL)
        return SUCCESS;
    if (level == 1)
        printf("%d ", root->value);
    else if (level > 1)
    {
        printLevel(root->left, level - 1);
        printLevel(root->right, level - 1);
    }
    return SUCCESS;
}

// Visina stabla
int height(Position root)
{
    if (root == NULL)
        return 0;
    int leftH = height(root->left);
    int rightH = height(root->right);
    return (leftH > rightH ? leftH : rightH) + 1;
}

// Oslobaða cijelo stablo
int freeTree(Position root)
{
    if (root == NULL)
        return SUCCESS;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return SUCCESS;
}
