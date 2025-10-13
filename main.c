/* Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100 */

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 80

typedef struct{
   
   char name[MAX];
   int points;
    
}_student;


int main(){
    
    int numberoflines=0, i, maxpoints=100;
    float relpoints[MAX];
    char tmp[100];
    _student *stud;
    FILE *input;
    
    // otvaranje datoteke
    input = fopen("student.txt", "r");
    if(input == NULL){
        printf("Greška pri otvaranju datoteke!");
        exit(1);
    }
    
    // brojanje redaka u datoteci
    char line[200];
        while (fgets(line, sizeof(line), input) != NULL)
            numberoflines++;
            
    // dinamička alokacija memorije        
    stud = (_student *)malloc(numberoflines * sizeof(_student));
    rewind(input);
    
    // učitavanje podataka o studentima
    for(i=0; i<numberoflines; i++){
        
        fscanf(input, "%s %s %d", stud[i].name, tmp, &stud[i].points);
        strcat(stud[i].name, " ");
        strcat(stud[i].name, tmp);
        
    }
    
    // ispis tablice rezultata
    printf("\n%-20s %-15s %-20s\n", "IME I PREZIME", "APSOLUTNI", "RELATIVNI (%)");
    printf("--------------------------------------------------------------\n");
    
    // izračun i ispis relativnih bodova
    for(i=0; i<numberoflines; i++){
        
        relpoints[i]=(float)stud[i].points/maxpoints*100;
        printf("%-20s %-15d %-6.2f%%\n", stud[i].name, stud[i].points, relpoints[i]);
        
        
        
    }
    
    // oslobađanje memorije i zatvaranje datoteke
    free(stud);
    fclose(input);
    
    return 0;
}