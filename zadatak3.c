/*
2. Definirati strukturu osoba (ime, prezime, godina ro�enja) i napisati program koji:
A. dinami�ki dodaje novi element na po�etak liste,
B. ispisuje listu,
C. dinami�ki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. bri�e odre�eni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 100
#define MEMORY_ALLOCATION_FAILED (-2)
#define ELEMENT_NOT_FOUND (-3)
#define FILE_ERROR (-4)

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

/* Zadaci 3: dodatne funkcije */
int addAfter(Position head, const char* targetSurname, const char* name, const char* surname, int birthYear);
int addBefore(Position* head, const char* targetSurname, const char* name, const char* surname, int birthYear);
int sortBySurname(Position head);
int writeToFile(Position head, const char* filename);
int readFromFile(Position* head, const char* filename);

/* Pomo�na: osloba�anje cijele liste */
void freeList(Position* head);

int main() {

	Position head = NULL;

	/* primjer dodavanja */
	addToBeginning(&head, "Ivan", "Ivic", 1990);
	addToBeginning(&head, "Ana", "Anic", 1992);
	addToEnd(&head, "Marko", "Markic", 1988);

	printf("Initial list:\n");
	printList(head);

	/* addAfter: dodajemo osobu iza "Anic" */
	addAfter(head, "Anic", "Petra", "Petric", 1995);
	printf("\nAfter addAfter (Petra after Anic):\n");
	printList(head);

	/* addBefore: dodajemo osobu ispred "Markic" */
	addBefore(&head, "Markic", "Luka", "Lukic", 1985);
	printf("\nAfter addBefore (Luka before Markic):\n");
	printList(head);

	/* sort */
	sortBySurname(head);
	printf("\nAfter sorting by surname:\n");
	printList(head);

	/* upis u datoteku */
	if (writeToFile(head, "people.txt") != 0) {
		printf("Error writing to file.\n");
	}
	else {
		printf("\nList written to people.txt\n");
	}

	/* brisanje jednog elementa */
	deleteBySurname(&head, "Ivic");
	printf("\nAfter deleting surname Ivic:\n");
	printList(head);

	/* �itanje iz datoteke (prvo o�istimo listu) */
	if (readFromFile(&head, "people.txt") != 0) {
		printf("Error reading from file.\n");
	}
	else {
		printf("\nList read from people.txt:\n");
		printList(head);
	}

	/* free */
	freeList(&head);

	return 0;
}


/* Funkcija za dodavanje elementa na po�etak liste */
int addToBeginning(Position* head, const char* name, const char* surname, int birthYear) {

	Position newPerson = (Position)malloc(sizeof(_Person));
	if (newPerson == NULL) {
		return MEMORY_ALLOCATION_FAILED;
	}
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	/* Postavljanje novog elementa na po�etak */
	newPerson->next = *head;
	*head = newPerson;

	return 0;// uspje�no dodano
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

/* Funkcija za pronala�enje osobe po prezimenu */
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
/*
3. Prethodnom zadatku dodati funkcije:
A. dinami�ki dodaje novi element iza odre�enog elementa,
B. dinami�ki dodaje novi element ispred odre�enog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. �ita listu iz datoteke.
*/

/* Pomo�na funkcija: alocira i postavlja strukturu osobe */

Position createPerson(const char* name, const char* surname, int birthYear) {

	Position newPerson = (Position)malloc(sizeof(_Person));

	if (newPerson == NULL) {
		return NULL;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	newPerson->next = NULL;

	return newPerson;

}

/* Pomo�na: osloba�a cijelu listu i postavlja head na NULL */

void freeList(Position* head) {
	Position temp;
	while (*head != NULL) {
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}

/* A. addAfter - dodaje novi element iza elementa koji ima targetSurname
   Vrati 0 ako uspje�no, ELEMENT_NOT_FOUND ako cilj nije prona�en, ili MEMORY_ALLOCATION_FAILED. */

int addAfter(Position head, const char* targetSurname, const char* name, const char* surname, int birthYear) {

	if (head == NULL) {

		return ELEMENT_NOT_FOUND;

	}

	Position target = findBySurname(head, targetSurname);
	if (target == NULL) {

		return ELEMENT_NOT_FOUND;

	}

	Position newPerson = createPerson(name, surname, birthYear);
	if (newPerson == NULL) {

		return MEMORY_ALLOCATION_FAILED;

	}

	newPerson->next = target->next;
	target->next = newPerson;

	return 0;

}

/* B. addBefore - dodaje novi element ispred elementa koji ima targetSurname
   Budu�i da nemamo globalni head, trebamo pokaziva� na head (Position*). */

int addBefore(Position *head, const char* targetSurname, const char* name, const char* surname, int birthYear) {

	if (*head == NULL) {
		return ELEMENT_NOT_FOUND;
	}
	/* Ako je cilj prvi element */
	if (strcmp((*head)->surname, targetSurname) == 0) {
		return addToBeginning(head, name, surname, birthYear);
	}
	Position prev = NULL;
	Position curr = *head;
	while (curr != NULL && strcmp(curr->surname, targetSurname) != 0) {
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL) {
		return ELEMENT_NOT_FOUND;/* cilj nije prona�en */
	}
	/* umetanje izme�u prev i curr */
	Position newPerson = createPerson(name, surname, birthYear);
	if (newPerson == NULL) {
		return MEMORY_ALLOCATION_FAILED;
	}
	prev->next = newPerson;
	newPerson->next = curr;

	return 0;
}
/* C. sortBySurname - sortira listu po prezimenima (rastuce) */

int sortBySurname(Position* head) {
	if (*head == NULL || (*head)->next == NULL)
		return 0; // nema �to sortirati

	int swapped;
	Position end = NULL;

	do {
		swapped = 0;
		Position* p = head; // pokaziva� NA pokaziva� na trenutni �vor

		while ((*p)->next != end) {
			Position a = *p;
			Position b = a->next;

			if (strcmp(a->surname, b->surname) > 0) {
				// zamjena pokaziva�a
				a->next = b->next;
				b->next = a;
				*p = b;

				swapped = 1;
			}

			// pomakni pokaziva� "p" na sljede�u vezu
			p = &((*p)->next);
		}

		// "end" ozna�ava zadnji ve� sortiran element
		end = *p;

	} while (swapped);

	return 0;
}

/* D. writeToFile - upisuje listu u tekstualnu datoteku, svak jedan zapis po redu:
   ime prezime godina
   Vrati 0 ako ok, FILE_ERROR ako ne mo�e otvoriti datoteku. */

int writeToFile(Position head, const char* filename) {
	
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		return FILE_ERROR;
	}
    
	Position p = head;
	while (p != NULL) {
		fprintf(fp, "%s %s %d\n", p->name, p->surname, p->birthYear);
		p = p->next;
	}
	fclose(fp);
	return 0;
}

/* E. readFromFile - �ita listu iz tekstualne datoteke iste strukture
   Prije �itanja osloba�a postoje�u listu da ne bi do�lo do curenja memorije.
   Svaka linija: ime prezime godina (razmak razdvaja).
   Vrati 0 ako ok, FILE_ERROR ako ne mo�e otvoriti datoteku, MEMORY_ALLOCATION_FAILED ako malloc ne uspije. */

int readFromFile(Position* head, const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		return FILE_ERROR;
	}
	/* O�istimo postoje�u listu */
	freeList(head);

	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int year;

	/* �itamo dok format odgovara */
	while (fscanf(fp, "%s %s %d", name, surname, &year) == 3) {
		if (addToEnd(head, name, surname, year) == MEMORY_ALLOCATION_FAILED) {
			fclose(fp);
			return MEMORY_ALLOCATION_FAILED;
		}
	}

	fclose(fp);
	return 0;
}