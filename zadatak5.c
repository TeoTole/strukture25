/*
5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.
*/
#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED (-1)
#define MEMORY_ALLOCATION_ERROR (-2)
#define INVALID_EXPRESSION_ERROR (-3)
#define MAX_LINE (1024)

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct _StackNode;
typedef struct _StackNode* Position;
typedef struct _StackNode {
	double value;
	Position next;
}_StackNode;

// PROTOTIPOVI FUNKCIJA
int readFromFile(char* fileName, char* buffer, int bufferSize);
int push(Position head, double number);
int pop(Position head, double* result);
int calculatePostfixFromFile(char* fileName, double* result);
int performOperation(Position head, char operation);
int isNumber(char* token);

// MAIN FUNKCIJA
int main() {
	char fileName[MAX_LINE] = { 0 };
	double result = 0.0;
	int status = 0;

	printf("Enter file name containing postfix expression: ");
	scanf(" %s", fileName);

	status = calculatePostfixFromFile(fileName, &result);

	if (status == 0)
		printf("Result of postfix expression: %.2lf\n", result);
	else if (status == FILE_NOT_OPENED)
		printf("Error: Could not open file.\n");
	else if (status == MEMORY_ALLOCATION_ERROR)
		printf("Error: Memory allocation failed.\n");
	else if (status == INVALID_EXPRESSION_ERROR)
		printf("Error: Invalid postfix expression.\n");
	else
		printf("Unknown error occurred.\n");

	return 0;
}

/*
Funkcija èita sadržaj datoteke i sprema ga u buffer.
*/
int readFromFile(char* fileName, char* buffer, int bufferSize) {
	FILE* filePointer = NULL;
	filePointer = fopen(fileName, "r");
	if (filePointer == NULL) {
		return FILE_NOT_OPENED;
	}
	fgets(buffer, bufferSize, filePointer);
	fclose(filePointer);
	return 0;
}

/*
Funkcija dodaje broj na vrh stoga.
*/
int push(Position head, double number) {
	Position newNode = (Position)malloc(sizeof(StackNode));
	if (newNode == NULL) {
		return MEMORY_ALLOCATION_ERROR;
	}
	newNode->value = number;
	newNode->next = head->next;
	head->next = newNode;
	return 0;
}

/*
Funkcija uklanja broj s vrha stoga i vraæa ga kroz parametar result.
*/
int pop(Position head, double* result) {
	Position temp = NULL;
	if (head->next == NULL) {
		return INVALID_EXPRESSION_ERROR;
	}
	temp = head->next;
	*result = temp->value;
	head->next = temp->next;
	free(temp);
	return 0;
}

/*
Funkcija provjerava je li string broj.
*/
int isNumber(char* token) {
	int i = 0;
	if (token[0] == '-' && strlen(token) > 1)
		i = 1; // podrška za negativne brojeve
	for (; token[i] != '\0'; i++) {
		if (!isdigit(token[i]) && token[i] != '.')
			return 0;
	}
	return 1;
}

/*
Funkcija izvodi aritmetièku operaciju nad dva broja sa stoga.
*/
int performOperation(Position head, char operation) {
	double operand1 = 0.0, operand2 = 0.0;
	int status = 0;
	status = pop(head, &operand2);
	if (status != 0) {
		return status;
	}
	status = pop(head, &operand1);
	if (status != 0) {
		return status;
	}
	switch (operation) {
	case '+':
		status = push(head, operand1 + operand2);
		break;
	case '-':
		status = push(head, operand1 - operand2);
		break;
	case '*':
		status = push(head, operand1 * operand2);
		break;
	case '/':
		if (operand2 == 0)
			return INVALID_EXPRESSION_ERROR;
		status = push(head, operand1 / operand2);
		break;
	default:
		return INVALID_EXPRESSION_ERROR;
	}
	return status;
}

/*
Glavna funkcija koja èita postfiks izraz iz datoteke i raèuna rezultat.
*/
int calculatePostfixFromFile(char* fileName, double* result) {
	char buffer[MAX_LINE] = { 0 };
	char* currentToken = NULL;
	_StackNode head = { .next = NULL, .value = 0 };
	int status = 0;

	status = readFromFile(fileName, buffer, MAX_LINE);
	if (status != 0) {
		return status;
	}

	currentToken = strtok(buffer, " \t\n");

	while (currentToken != NULL) {
		if (isNumber(currentToken)) {
			double number = atof(currentToken);
			status = push(&head, number);
			if (status != 0) {
				return status;
			}
		}
		else if (strlen(currentToken) == 1) {
			status = performOperation(&head, currentToken[0]);
			if (status != 0)
				return status;
		}
		else {
			return INVALID_EXPRESSION_ERROR;
		}

		currentToken = strtok(NULL, " \t\n");
	}
	// Nakon parsiranja izraza, rezultat bi trebao biti jedini broj na stogu
	status = pop(&head, result);
	if (status != 0)
		return status;

	// Ako stog nije prazan, izraz nije ispravan
	if (head.next != NULL)
		return INVALID_EXPRESSION_ERROR;

	return 0;
}


