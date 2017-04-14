/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	26.12.2007
	Last Update:	28.12.2007
	Last Update:	12.07.2008
	Full Channge:	30.04.2010
	Last Update:	30.04.2010
*/


#include <stdio.h>
#include <conio.h>

#include "src/list.h"

typedef struct node {
	unsigned int id;
} DATA, *LPDATA;

void freeData(void *node)
{
	printf("free id: %d\n", ((LPDATA)node)->id);
}

void main(void)
{
	LPDATA newNode, temp;
	LPLIST root = NULL, current;

	clrscr();


	newNode = (LPDATA)malloc(sizeof(DATA));
	newNode->id = 0;
	addToList(&root, (void*)newNode, freeData);

	newNode = (LPDATA)malloc(sizeof(DATA));
	newNode->id = 1;
	addToList(&root, (void*)newNode, NULL);

	newNode = (LPDATA)malloc(sizeof(DATA));
	newNode->id = 2;
	addToList(&root, (void*)newNode, freeData);

	removeFromList(&root, 1);

	current = root;
	while(current) {
		temp = (LPDATA)current->lpData;
		printf("ID: %d\n", temp->id);
		current = current->next;
	}

	while(root) {
		current = root;
		printf("Remove ID: %d\n", current->id);
		removeFromList(&root, current->id);
	}

	getch();
}