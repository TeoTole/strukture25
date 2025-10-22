/*
2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100
#define MEMORY_ALLOCATION_FAILED (-2)
#define ELEMENT_NOT_FOUND (-3)

/* Definicija strukture osobe */
struct _Person;
typedef struct _Person* Position;
typedef struct _Person {

	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;

}_Person;

/* Funkcijski prototipi */
int addToBeginning(Position* head, const char* name, const char* surname, int birthYear);
int printList(Position head);
int addToEnd(Position* head, const char* name, const char* surname, int birthYear);
Position findBySurname(Position head, const char* surname);
int deleteBySurname(Position* head, const char* surname);

int main() {

	/* Definiranje poèetne prazne liste */
	Position head = NULL;

	/* Dodavanje elemenata na poèetak liste */
	if (addToBeginning(&head, "Ivan", "Ivic", 1990) != 0)
		printf("Error adding Ivan Ivic to the beginning.\n");

	if (addToBeginning(&head, "Ana", "Anic", 1992) != 0)
		printf("Error adding Ana Anic to the beginning.\n");

	/* Dodavanje elementa na kraj liste */
	if (addToEnd(&head, "Marko", "Markic", 1988) != 0)
		printf("Error adding Marko Markic to the end.\n");

	/* Ispis liste */
	if (printList(head) != 0)
		printf("Error printing the list.\n");

	/* Pronalaženje osobe po prezimenu */
	char surnameToFind[MAX_LENGTH] = "Anic";
	Position foundPerson = findBySurname(head, surnameToFind);
	if (foundPerson != NULL)
		printf("Found person: %s %s, %d\n", foundPerson->name, foundPerson->surname, foundPerson->birthYear);
	else
		printf("Person with surname %s not found.\n", surnameToFind);

	/* Brisanje osobe po prezimenu */
	if (deleteBySurname(&head, "Ivic") != 0)
		printf("Error deleting person with surname Ivic.\n");

	/* Ispis liste nakon brisanja */
	printf("List after deletion:\n");
	if (printList(head) != 0)
		printf("Error printing the list.\n");

	/* Oslobaðanje memorije */
	while (head != NULL)
	{
		Position temp = head;
		head = head->next;
		free(temp);
	}
	return 0;
}


/* Funkcija za dodavanje elementa na poèetak liste */
int addToBeginning(Position* head, const char* name, const char* surname, int birthYear) {

	Position newPerson = (Position)malloc(sizeof(_Person));
	if (newPerson == NULL) {
		return MEMORY_ALLOCATION_FAILED;
	}
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	/* Postavljanje novog elementa na poèetak */
	newPerson->next = *head;
	*head = newPerson;

	return 0;// uspješno dodano
}

/* Funkcija za ispis liste */
int printList(Position head) {

	if (head == NULL) {

		printf("The list is empty!\n");

		return 0;
	}

	Position temp = head;
	while (temp != NULL) {

		printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;
	}

	return 0;
}

/* Funkcija za dodavanje elementa na kraj liste */
int addToEnd(Position* head, const char* name, const char* surname, int birthYear) {

	Position newPerson = (Position)malloc(sizeof(_Person));
	if (newPerson == NULL) {
		return MEMORY_ALLOCATION_FAILED;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;
    
	if (*head == NULL) {
		*head = newPerson;
		return 0;
	}

	Position temp = *head;
	while (temp->next != NULL) {
		temp = temp->next;

	}
	temp->next = newPerson;

	return 0;
}

/* Funkcija za pronalaženje osobe po prezimenu */
Position findBySurname(Position head, const char* surname) {

	Position temp = head;
	while (temp != NULL) {
		if (strcmp(temp->surname, surname) == 0) {
			return temp;
		}
		temp = temp->next;
	}

	return NULL;

}

/* Funkcija za brisanje osobe po prezimenu */
int deleteBySurname(Position* head, const char* surname) {

	if (*head == NULL) {
		return ELEMENT_NOT_FOUND;
	}

	Position temp = *head;
	Position prev = NULL;

	while (temp != NULL) {

		if (strcmp(temp->surname, surname) == 0) {
			if (prev == NULL)
				*head = temp->next;

			else
				prev->next = temp->next;

			free(temp);
			return 0;

		}
		prev = temp;
		temp = temp->next;
	}
	
	return ELEMENT_NOT_FOUND;
}