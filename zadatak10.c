/*
10. Napisati program koji èita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki èvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo država sortirano po nazivu. Svaki èvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omoguæiti korisniku putem
tastature pretragu gradova odreðene države koji imaju broj stanovnika veæi od unosa na
tastaturi.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define FILE_NOT_OPENED (-1)

/* Struktura stabla gradova */
struct _CityTree;
typedef struct _CityTree* CityTreePosition;

typedef struct _CityTree {
    char name[MAX_NAME];
    int population;
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

/* Struktura liste gradova */
struct _CityList;
typedef struct _CityList* CityListPosition;

typedef struct _CityList {
    char name[MAX_NAME];
    int population;
    CityListPosition next;
} CityList;

/* Struktura liste drzava */
struct _CountryList;
typedef struct _CountryList* CountryListPosition;

typedef struct _CountryList {
    char name[MAX_NAME];
    CityTreePosition cityTree;
    CountryListPosition next;
} CountryList;

/* Struktura stabla drzava */
struct _CountryTree;
typedef struct _CountryTree* CountryTreePosition;

typedef struct _CountryTree {
    char name[MAX_NAME];
    CityListPosition cityList;
    CountryTreePosition left;
    CountryTreePosition right;
} CountryTree;

/* Deklaracije funkcija */
CountryListPosition insertCountryList(CountryListPosition head, char* name, char* fileName);
CountryTreePosition insertCountryTree(CountryTreePosition root, char* name, char* fileName);

CityTreePosition insertCityTree(CityTreePosition root, char* name, int population);
CityListPosition insertCityList(CityListPosition head, char* name, int population);

int readCitiesTree(char* fileName, CityTreePosition* root);
int readCitiesList(char* fileName, CityListPosition* head);

int printCountriesList(CountryListPosition head);
int printCountriesTree(CountryTreePosition root);

int searchCitiesTree(CityTreePosition root, int minPopulation);

int main() {
    FILE* file = fopen("drzave.txt", "r");
    char countryName[MAX_NAME];
    char cityFile[MAX_NAME];
    int minPopulation = 0;

    CountryListPosition countryList = NULL;
    CountryTreePosition countryTree = NULL;

    if (!file)
        return FILE_NOT_OPENED;

    /* Ucitavanje drzava iz datoteke */
    while (fscanf(file, "%s %s", countryName, cityFile) == 2) {
        countryList = insertCountryList(countryList, countryName, cityFile);
        countryTree = insertCountryTree(countryTree, countryName, cityFile);
    }

    fclose(file);

    printf("Lista drzava:\n");
    printCountriesList(countryList);

    printf("\nStablo drzava:\n");
    printCountriesTree(countryTree);

    printf("\nUnesite minimalni broj stanovnika: ");
    scanf("%d", &minPopulation);

    printf("\nGradovi s vise od %d stanovnika:\n", minPopulation);
    while (countryList) {
        printf("\n%s:\n", countryList->name);
        searchCitiesTree(countryList->cityTree, minPopulation);
        countryList = countryList->next;
    }

    return 0;
}

/* Dodavanje drzave u sortiranu vezanu listu */
CountryListPosition insertCountryList(CountryListPosition head, char* name, char* fileName) {
    CountryListPosition newCountry = NULL, prev = NULL, curr = head;

    newCountry = (CountryListPosition)malloc(sizeof(CountryList));
    if (!newCountry)
        return head;

    strcpy(newCountry->name, name);
    newCountry->cityTree = NULL;
    newCountry->next = NULL;

    readCitiesTree(fileName, &newCountry->cityTree);

    while (curr && strcmp(curr->name, name) < 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        newCountry->next = head;
        return newCountry;
    }

    prev->next = newCountry;
    newCountry->next = curr;
    return head;
}

/* Dodavanje drzave u binarno stablo */
CountryTreePosition insertCountryTree(CountryTreePosition root, char* name, char* fileName) {
    if (!root) {
        root = (CountryTreePosition)malloc(sizeof(CountryTree));
        if (!root)
            return NULL;

        strcpy(root->name, name);
        root->cityList = NULL;
        root->left = root->right = NULL;

        readCitiesList(fileName, &root->cityList);
        return root;
    }

    if (strcmp(name, root->name) < 0)
        root->left = insertCountryTree(root->left, name, fileName);
    else
        root->right = insertCountryTree(root->right, name, fileName);

    return root;
}

/* Dodavanje grada u stablo sortirano po broju stanovnika */
CityTreePosition insertCityTree(CityTreePosition root, char* name, int population) {
    if (!root) {
        root = (CityTreePosition)malloc(sizeof(CityTree));
        if (!root)
            return NULL;

        strcpy(root->name, name);
        root->population = population;
        root->left = root->right = NULL;
        return root;
    }

    if (population < root->population ||
        (population == root->population && strcmp(name, root->name) < 0))
        root->left = insertCityTree(root->left, name, population);
    else
        root->right = insertCityTree(root->right, name, population);

    return root;
}

/* Dodavanje grada u sortiranu listu */
CityListPosition insertCityList(CityListPosition head, char* name, int population) {
    CityListPosition newCity = NULL, prev = NULL, curr = head;

    newCity = (CityListPosition)malloc(sizeof(CityList));
    if (!newCity)
        return head;

    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->next = NULL;

    while (curr &&
        (curr->population < population ||
            (curr->population == population && strcmp(curr->name, name) < 0))) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        newCity->next = head;
        return newCity;
    }

    prev->next = newCity;
    newCity->next = curr;
    return head;
}

/* Ucitavanje gradova u stablo */
int readCitiesTree(char* fileName, CityTreePosition* root) {
    FILE* file = fopen(fileName, "r");
    char name[MAX_NAME];
    int population;

    if (!file)
        return FILE_NOT_OPENED;

    while (fscanf(file, "%[^,],%d\n", name, &population) == 2)
        *root = insertCityTree(*root, name, population);

    fclose(file);
    return 0;
}

/* Ucitavanje gradova u listu */
int readCitiesList(char* fileName, CityListPosition* head) {
    FILE* file = fopen(fileName, "r");
    char name[MAX_NAME];
    int population;

    if (!file)
        return FILE_NOT_OPENED;

    while (fscanf(file, "%[^,],%d\n", name, &population) == 2)
        *head = insertCityList(*head, name, population);

    fclose(file);
    return 0;
}

/* Ispis liste drzava */
int printCountriesList(CountryListPosition head) {
    while (head) {
        printf("%s\n", head->name);
        head = head->next;
    }
    return 0;
}

/* Ispis stabla drzava */
int printCountriesTree(CountryTreePosition root) {
    if (!root)
        return 0;

    printCountriesTree(root->left);
    printf("%s\n", root->name);
    printCountriesTree(root->right);
    return 0;
}

/* Pretraga gradova s vise stanovnika od zadanog broja */
int searchCitiesTree(CityTreePosition root, int minPopulation) {
    if (!root)
        return 0;

    searchCitiesTree(root->left, minPopulation);

    if (root->population > minPopulation)
        printf("%s (%d)\n", root->name, root->population);

    searchCitiesTree(root->right, minPopulation);
    return 0;
}
