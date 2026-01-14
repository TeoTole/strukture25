#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11
#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2

struct cityTree;
typedef struct cityTree* CityTreePosition;
typedef struct cityTree {
    int population;
    char name[20];
    CityTreePosition left;
    CityTreePosition right;
} CityTree;

struct countryList;
typedef struct countryList* CountryListPosition;
typedef struct countryList {
    char name[20];
    CityTreePosition cities;
    CountryListPosition next;
} CountryList;

typedef struct {
    CountryList buckets[TABLE_SIZE];   // svaka pozicija ima svoju listu
} HashTable;

int hash(char* countryName);
int insertCountry(HashTable* table, CountryListPosition newCountry);
CountryListPosition findCountry(HashTable* table, char* name);
CityTreePosition insertCityTree(CityTreePosition root, CityTreePosition newCity);
int printCitiesGreaterThan(CityTreePosition root, int limit);

int main()
{
    HashTable table = { 0 };
    FILE* fp = NULL;
    FILE* cityFile = NULL;

    char country[20], city[20], fileName[30];
    int population = 0, limit = 0;

    fp = fopen("drzave.txt", "r");
    if (!fp)
        return FILE_OPEN_ERROR;

    // dummy heads da ne moram paziti na prvi element
    for (int i = 0; i < TABLE_SIZE; i++)
        table.buckets[i].next = NULL;

    while (fscanf(fp, "%[^,], %s", country, fileName) == 2)
    {
        CountryListPosition newCountry = malloc(sizeof(CountryList));
        if (!newCountry)
            return MALLOC_ERROR;

        strcpy(newCountry->name, country);
        newCountry->cities = NULL;
        newCountry->next = NULL;

        insertCountry(&table, newCountry);

        cityFile = fopen(fileName, "r");
        if (!cityFile)
            return FILE_OPEN_ERROR;

        while (fscanf(cityFile, "%[^,], %d", city, &population) == 2)
        {
            CityTreePosition newCity = malloc(sizeof(CityTree));
            if (!newCity)
                return MALLOC_ERROR;

            strcpy(newCity->name, city);
            newCity->population = population;
            newCity->left = NULL;
            newCity->right = NULL;

            newCountry->cities = insertCityTree(newCountry->cities, newCity);
        }

        fclose(cityFile);
    }

    fclose(fp);

    while (1) {
        char choice[20];

        printf("\nUnesi drzavu (0 za izlaz): ");
        scanf(" %19[^\n]", choice);

        if (!strcmp(choice, "0"))
            break;

        CountryListPosition found = findCountry(&table, choice);
        if (!found) {
            printf("Drzava ne postoji\n");
            continue;
        }

        printf("Unesi broj stanovnika: ");
        scanf("%d", &limit);

        if (!printCitiesGreaterThan(found->cities, limit))
            printf("Nema takvih gradova\n");
    }

    return EXIT_SUCCESS;
}

int hash(char* countryName)
{
    int sum = 0;

    // uzimam samo prvih 5 slova
    for (int i = 0; i < 5 && countryName[i] != '\0'; i++)
        sum += countryName[i];

    return sum % TABLE_SIZE;
}

int insertCountry(HashTable* table, CountryListPosition newCountry)
{
    int index = hash(newCountry->name);
    CountryListPosition current = &table->buckets[index];

    // drzave moraju ostati sortirane
    while (current->next && strcmp(current->next->name, newCountry->name) < 0)
        current = current->next;

    newCountry->next = current->next;
    current->next = newCountry;

    return EXIT_SUCCESS;
}

CountryListPosition findCountry(HashTable* table, char* name)
{
    int index = hash(name);
    CountryListPosition current = table->buckets[index].next;

    while (current && strcmp(current->name, name))
        current = current->next;

    return current;
}

CityTreePosition insertCityTree(CityTreePosition root, CityTreePosition newCity)
{
    if (!root)
        return newCity;

    if (newCity->population < root->population)
        root->left = insertCityTree(root->left, newCity);
    else if (newCity->population > root->population)
        root->right = insertCityTree(root->right, newCity);
    else {
        // ako je isti broj stanovnika, gledam ime
        if (strcmp(newCity->name, root->name) < 0)
            root->left = insertCityTree(root->left, newCity);
        else
            root->right = insertCityTree(root->right, newCity);
    }

    return root;
}

int printCitiesGreaterThan(CityTreePosition root, int limit)
{
    int found = 0;

    if (!root)
        return 0;

    if (root->population > limit) {
        printf("\t%s, %d\n", root->name, root->population);
        found = 1;
    }

    found |= printCitiesGreaterThan(root->left, limit);
    found |= printCitiesGreaterThan(root->right, limit);

    return found;
}