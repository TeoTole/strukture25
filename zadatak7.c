#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Dir {
    char name[50];
    struct Dir* child;
    struct Dir* sibling;
    struct Dir* parent;
} Dir;


Dir* createDir(char* name, Dir* parent) {
    Dir* d = (Dir*)malloc(sizeof(Dir));
    if (!d) exit(-1);

    strcpy(d->name, name);
    d->child = NULL;
    d->sibling = NULL;
    d->parent = parent;

    return d;
}


void makeDir(Dir* current) {
    char name[50];
    printf("Ime direktorija: ");
    scanf("%s", name);

    Dir* newDir = createDir(name, current);

    if (current->child == NULL) {
        current->child = newDir;
    }
    else {
        Dir* t = current->child;
        while (t->sibling)
            t = t->sibling;
        t->sibling = newDir;
    }
}


Dir* changeDir(Dir* current) {
    char name[50];
    printf("Ime direktorija: ");
    scanf("%s", name);

    Dir* t = current->child;
    while (t) {
        if (strcmp(t->name, name) == 0)
            return t;
        t = t->sibling;
    }

    printf("Direktorij ne postoji.\n");
    return current;
}


void listDir(Dir* current) {
    Dir* t = current->child;
    if (!t) {
        printf("Direktorij je prazan.\n");
        return;
    }

    while (t) {
        printf("%s\n", t->name);
        t = t->sibling;
    }
}


int main() {
    Dir* root = createDir("C:", NULL);
    Dir* current = root;

    int choice;

    do {
        printf("\nTrenutni direktorij: %s\n", current->name);
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            makeDir(current);
            break;
        case 2:
            current = changeDir(current);
            break;
        case 3:
            if (current->parent)
                current = current->parent;
            break;
        case 4:
            listDir(current);
            break;
        }
    } while (choice != 5);

    return 0;
}

