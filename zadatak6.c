#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 100

// Struktura za artikl
// Svaki artikl ima ime, kolicinu, cijenu i pokazivac na sljedeci artikl
typedef struct article {
    char name[MAX_NAME];
    int quantity;
    int price;
    struct article* next;
} Article;
typedef Article* ArticlePosition;

// Struktura za racun
// Svaki racun ima datum, vezanu listu artikala (sa dummy zaglavljem) i pokazivac na sljedeci racun
typedef struct receipt {
    char date[11];
    Article headArticle; // dummy head
    struct receipt* next;
} Receipt;
typedef Receipt* ReceiptPosition;


// Dodavanje artikla u sortiranu listu po nazivu (leksikografski rastuce)

int addArticle(ArticlePosition head, ArticlePosition article) {
    ArticlePosition current = head;

    // Nalazi mjesto gdje treba umetnuti artikl kako bi lista ostala sortirana
    while (current->next != NULL && strcmp(current->next->name, article->name) < 0)
        current = current->next;

    article->next = current->next;
    current->next = article;

    return 0;
}


// Dodavanje racuna u sortiranu listu po datumu (od najranijeg prema najkasnijem)

int addReceipt(ReceiptPosition head, ReceiptPosition receipt) {
    ReceiptPosition current = head;

    // Nalazi mjesto umetanja prema datumu
    while (current->next != NULL && strcmp(current->next->date, receipt->date) < 0)
        current = current->next;

    receipt->next = current->next;
    current->next = receipt;

    return 0;
}



// Citanje pojedinacnog racuna iz datoteke i dodavanje u globalnu listu

int readReceipt(char* fileName, ReceiptPosition headReceipt) {

    char dateString[20] = { 0 };
    char line[MAX_LINE] = { 0 };
    FILE* filePointer = NULL;
    ReceiptPosition newReceipt = NULL;

    // Pokusaj otvaranja jedne datoteke racuna
    filePointer = fopen(fileName, "r");
    if (filePointer == NULL) {
        printf("Ne mogu otvoriti datoteku '%s'.\n", fileName);
        return -1;
    }

    // Citanje datuma (prva linija datoteke)
    fgets(dateString, sizeof(dateString), filePointer);
    dateString[strcspn(dateString, "\r\n")] = 0;

    // Alokacija memorije za novi racun
    newReceipt = (ReceiptPosition)malloc(sizeof(Receipt));
    if (newReceipt == NULL) {
        printf("Greska pri alokaciji memorije za racun.\n");
        fclose(filePointer);
        return -1;
    }

    // Postavljanje vrijednosti
    strcpy(newReceipt->date, dateString);
    newReceipt->headArticle.next = NULL;
    newReceipt->next = NULL;

    // Citanje artikala red po red
    while (fgets(line, sizeof(line), filePointer)) {

        ArticlePosition newArticle = (ArticlePosition)malloc(sizeof(Article));
        if (newArticle == NULL) {
            printf("Greska pri alokaciji memorije za artikl.\n");
            fclose(filePointer);
            return -1;
        }

        // Parsiranje linije u format: ime, kolicina, cijena
        if (sscanf(line, " %[^,], %d, %d",
            newArticle->name, &newArticle->quantity, &newArticle->price) != 3) {

            free(newArticle);
            continue; // preskoci losi unos
        }

        // Dodavanje artikla u sortiranu listu
        addArticle(&newReceipt->headArticle, newArticle);
    }

    // Dodavanje racuna u sortiranu listu racuna
    addReceipt(headReceipt, newReceipt);

    fclose(filePointer);
    return 0;
}


// Ispis artikala iz jedne liste

int printArticles(ArticlePosition current) {
    while (current != NULL) {
        printf("\t%s, %d, %d\n", current->name, current->quantity, current->price);
        current = current->next;
    }
    return 0;
}



// Ispis svih racuna i njihovih artikala

int printReceipts(ReceiptPosition current) {
    while (current != NULL) {
        printf("Racun datum %s:\n", current->date);
        printArticles(current->headArticle.next);
        current = current->next;
    }
    return 0;
}



// Pretraga koliko je troseno na odredeni artikl u zadanom periodu

int getArticleInPeriod(ReceiptPosition currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count) {

    // Pomak do prvog racuna koji je >= pocetnom datumu
    while (currentReceipt != NULL && strcmp(currentReceipt->date, startDate) < 0)
        currentReceipt = currentReceipt->next;

    // Prolazak kroz sve racune unutar raspona
    while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {

        ArticlePosition currentArticle = currentReceipt->headArticle.next;

        while (currentArticle != NULL) {

            if (strcmp(currentArticle->name, article) == 0) {
                *income += currentArticle->quantity * currentArticle->price;
                *count += currentArticle->quantity;
            }

            currentArticle = currentArticle->next;
        }

        currentReceipt = currentReceipt->next;
    }

    return 0;
}



// Oslobadanje artikala jednog racuna

int deleteArticles(ArticlePosition articleHead) {
    ArticlePosition temp = NULL;

    while (articleHead->next != NULL) {
        temp = articleHead->next;
        articleHead->next = temp->next;
        free(temp);
    }

    return 0;
}



// Oslobadanje svih racuna i svih artikala

int deleteReceipts(ReceiptPosition receiptHead) {
    ReceiptPosition temp = NULL;

    while (receiptHead->next != NULL) {
        temp = receiptHead->next;
        receiptHead->next = temp->next;
        deleteArticles(&temp->headArticle);
        free(temp);
    }

    return 0;
}


int main() {

    int income = 0, count = 0;
    char article[MAX_NAME] = { 0 };
    char startDate[20] = { 0 }, endDate[20] = { 0 };
    char currReceipt[MAX_LINE] = { 0 };

    // Otvaranje glavne datoteke sa popisom racuna
    FILE* filePointer = fopen("racuni.txt", "r");
    if (filePointer == NULL) {
        printf("Ne mogu otvoriti datoteku racuni.txt.\n");
        return -1;
    }

    // Dummy glava liste racuna
    Receipt headReceipt = {
        .date = {0},
        .headArticle = {.next = NULL},
        .next = NULL
    };

    // Citanje naziva racuna jedan po jedan
    while (fgets(currReceipt, MAX_LINE, filePointer)) {
        currReceipt[strcspn(currReceipt, "\r\n")] = 0;
        if (strlen(currReceipt) == 0) continue;

        readReceipt(currReceipt, &headReceipt);
    }
    fclose(filePointer);

    // Ispis svih podataka
    printReceipts(headReceipt.next);

    // Korisnicki unos
    printf("\nUnesite naziv artikla: ");
    scanf("%s", article);
    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    scanf("%s", startDate);
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", endDate);

    // Izracun troska i kolicine
    getArticleInPeriod(&headReceipt, article, startDate, endDate, &income, &count);

    printf("\nUkupno kupljeno jedinica artikla \"%s\" od %s do %s: %d\n",
        article, startDate, endDate, count);
    printf("Ukupno potroseno: %d\n", income);

    // Brisanje svih alociranih struktura
    deleteReceipts(&headReceipt);

    return 0;
}



