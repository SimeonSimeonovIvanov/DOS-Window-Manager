/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	12.03.2008
	Last Update:	23.03.2008
	Last Update:	12.07.2008
	Last Update:	04.05.2009
*/

#define __MENU__
#define __LABEL__
#define __EDIT_BOX__
#define __TAB_ORDER__

#include <ctype.h>
#include "src/window.h"

typedef struct car {
	char *model;
	UINT maxCarrying;
} CAR, *LPCAR;

struct view {
	char *buffer;
	UINT bufferSize;
	int x, y, size_x, size_y, xPos, yPos;
} View;

char fileName[12];
LPLIST lpCarList, lpCurrentCar;

void freeCar(void *lpData);
void saveCarList(LPLIST lpRoot, char *fileName);
void loadCarList(LPLIST *lpRoot, char *fileName);
void fileDialogUserButtonFunc(HWND hWnd, HANDLE id);
UINT createResultBuffer(LPLIST lpRoot, char **szOut);
void addCar(LPLIST *root, char *model, UINT maxCarrying);

void closeView(struct view *View);
void viewDisplay(HWND hWnd, struct view *View);
void viewResize(struct view *View, char size_x, char size_y);
void scrollView(struct view *View, char UpDown, char LeftRight);

CALLBACK mainDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
CALLBACK addCarDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
CALLBACK fileDialogDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
CALLBACK viewResultDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND FileDialog(HWND hParent, char type, void (*UserButtonFunc)(HWND hWnd, HANDLE id), char x, char y);

void main(void)
{
	ChangeDefWindowFunc(CreateWindow("Car (demo)", 7, 3, 34, 13, 5, 1, 15), mainDefWinProc);

	WindowManager();

	freeList(&lpCarList);
}

void addCar(LPLIST *root, char *model, UINT maxCarrying) // +
{
	LPCAR lpCar;

	if(NULL == (lpCar = (LPCAR)malloc(sizeof(CAR)))) return;
	if(NULL == (lpCar->model = (char*)malloc(strlen(model)+1))) return;

	strcpy(lpCar->model, model);
	lpCar->maxCarrying = maxCarrying;

	addToList(root, (void*)lpCar, freeCar);
}

void saveCarList(LPLIST lpRoot, char *fileName) // +
{
	FILE *fp;

	if(NULL != (fp = fopen(fileName, "wb"))) {
		int i;
		while(lpRoot) {
			i = strlen(((LPCAR)lpRoot->lpData)->model);
			fwrite(&i, sizeof(int), 1, fp);
			fwrite(((LPCAR)lpRoot->lpData)->model, i*sizeof(char), 1, fp);
			fwrite(&((LPCAR)lpRoot->lpData)->maxCarrying, sizeof(int), 1, fp);

			lpRoot = lpRoot->next;
		}

		fclose(fp);
	}
}

void loadCarList(LPLIST *lpRoot, char *fileName) // +
{
	FILE *fp;
	char *model;
	UINT maxCarrying;

	if(*lpRoot) *lpRoot = NULL;

	if(NULL != (fp = fopen(fileName, "rb"))) {
		int i;
		while(1) {
			if(!fread(&i, sizeof(int), 1, fp)) return;
			if(NULL == (model = (char*)malloc(1+i*sizeof(char)))) return;
			if(!fread(model, i*sizeof(char), 1, fp)) return;
			if(!fread(&maxCarrying, sizeof(int), 1, fp)) return;

			model[i] = 0;
			addCar(lpRoot, model, maxCarrying);
			free(model);
		}
	}
}

void freeCar(void *lpData)
{
	free(((LPCAR)lpData)->model);
}

UINT createResultBuffer(LPLIST lpRoot, char **szOut)
{
	UINT size = 0;
	char buffer[6];
	LPLIST lpCurrent = lpRoot;

	while(lpCurrent) {

		if(((LPCAR)lpCurrent->lpData)->maxCarrying > 1000) {
			itoa(((LPCAR)lpCurrent->lpData)->maxCarrying, buffer, 10);
			size += 29 + strlen(buffer) + strlen(((LPCAR)lpCurrent->lpData)->model);
		}

		lpCurrent = lpCurrent->next;
	}

	*szOut = (char*)malloc(size+1);
	szOut[size+1] = 0;
	strcpy(*szOut, "");

	lpCurrent = lpRoot;
	while(lpCurrent) {
		if(((LPCAR)lpCurrent->lpData)->maxCarrying > 1000) {
			sprintf(buffer, "%u", ((LPCAR)lpCurrent->lpData)->maxCarrying);

			strcat(*szOut, "Model: ");
			strcat(*szOut, ((LPCAR)lpCurrent->lpData)->model);
			strcat(*szOut, "\nMax Carrying: ");
			strcat(*szOut, buffer);
			strcat(*szOut, " kg.\n\n");
		}

		lpCurrent = lpCurrent->next;
	}

	return size;
}

void menuFile(HWND hWnd, HANDLE id)
{
	LPWINDOW lpPopUpWindow = GetPtrToWindow(hWnd);

	switch(id) {
	case 0: freeList(&lpCarList);
		lpCurrentCar = lpCarList;
		UpdateWindow();
	 break;
	case 1: lpPopUpWindow->parent_window = FileDialog(lpPopUpWindow->parent_window, 0, fileDialogUserButtonFunc, 22, 9); break;
	case 2: saveCarList(lpCarList, fileName); return;
	case 3: lpPopUpWindow->parent_window = FileDialog(lpPopUpWindow->parent_window, 1, fileDialogUserButtonFunc, 22, 9); break;
	case 4: PostQuitMsg(lpPopUpWindow->parent_window);
	}
}

void menuEdit(HWND hWnd, HANDLE id)
{
	switch(id) {

	case 0: { HWND hAddCar;
		LPWINDOW lpPopUpWindow = GetPtrToWindow(hWnd);

		hAddCar = CreateWindow("Add Car", 13, 6, 27, 10, 3, 7, 5);
		ChangeDefWindowFunc(hAddCar, addCarDefWinProc);

		SetParentWindow(hAddCar, lpPopUpWindow->parent_window);
		lpPopUpWindow->parent_window = hAddCar;
	 } break;

	case 1: removeFromList(&lpCarList, lpCurrentCar->id);
		lpCurrentCar = lpCarList;
	}

	UpdateWindow();
}

CALLBACK mainDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case MSG_WINDOW_FIRST_RUN:
		lpCarList = lpCurrentCar = NULL;

		CreateWindowMenu(hwnd, "&File&Edit&View&About Me", NULL);

		AddLabel(hwnd, "Model:", 15|(1<<4), 1, 3);
		AddLabel(hwnd, "Max Carrying:", 15|(1<<4), 1, 5);

		AddButton(hwnd, WINDOW_BUTTON, "Previous", 15|(7<<4), 7, 8, NULL);
		AddButton(hwnd, WINDOW_BUTTON, "  Next  ", 15|(7<<4), 22, 8, NULL);

		tabOrderAdd(hwnd, WINDOW_BUTTON, 0);
		tabOrderAdd(hwnd, WINDOW_BUTTON, 1);
		tabOrderSetFocus(hwnd, WINDOW_BUTTON, 0);

		UpdateWindow();
	 return;

	case MSG_PAINT:
		if(lpCarList) {
			char buffer[10];

			PutStr2WindowXY(hwnd, (UCHAR*)((LPCAR)lpCurrentCar->lpData)->model, 14|(1<<4), 15, 3);

			sprintf(buffer, "%u kg.", ((LPCAR)lpCurrentCar->lpData)->maxCarrying);
			PutStr2WindowXY(hwnd, (UCHAR*)buffer, 14|(1<<4), 15, 5);
		}
	 return;

	case MSG_SELECT_BUTTON:

		switch(wParam) {
		case 0:
			if(lpCarList) {
				if(lpCurrentCar == lpCarList) {
					while(lpCurrentCar->next) lpCurrentCar = lpCurrentCar->next;
				} else {
					LPLIST p;

					p = lpCarList;
					while(p) {
						if(p->next != lpCurrentCar) p = p->next;
						else break;
					}
					lpCurrentCar = p;
				}
			}
		 break;

		case 1:
			if(lpCurrentCar->next) {
				lpCurrentCar = lpCurrentCar->next;
			} else {
				lpCurrentCar = lpCarList;
			}
		}
	 break;

	case MSG_WINDOW_MENU_SELECT_BUTTON: { int x, y;

		GetButtonAbsPos(hwnd, WINDOW_MENU_BUTTON, wParam, &x, &y);
		y+=1;

		switch(wParam) {
		case 0: PopUpMenu(hwnd, "&New&Open&Save&Save as&Exit", x, y, menuFile); break;

		case 1: PopUpMenu(hwnd, "&Add Car&Remove Car", x, y, menuEdit); break;

		case 2: { HWND hViewResult;
			hViewResult = CreateWindow("View Result", mouse.x/8, mouse.y/8, 33, 11, 2, 7, 12);
			ChangeDefWindowFunc(hViewResult, viewResultDefWinProc);
			SetParentWindow(hViewResult, hwnd);
		 } break;

		case 3: AddLabel( CreateWindow("About Me", mouse.x/8, mouse.y/8, 22, 5, 2, 7, 12),
						  "The^Day^Of^DooM", 14|(7<<4), 1, 1);
		}

	 } break;

	}

	DefWindowProc(hwnd, message, wParam, lParam);
}

CALLBACK addCarDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case MSG_RESIZE_WINDOW: return;
	case MSG_QUIT: windowParentSetFlag(hwnd, 128 | 4 | 2 | 1); break;

	case MSG_PAINT:	PutStr2WindowXY(hwnd, (UCHAR*)"kg.", 15|(7<<4), 21, 3); return;

	case MSG_WINDOW_FIRST_RUN:
		windowParentSetFlag(hwnd, 2 | 1);

		AddLabel(hwnd, "Model:", 15|(7<<4), 1, 1);
		AddEditBox(hwnd, 16, 127 & 0xef, 8, 1);

		AddLabel(hwnd, "Max Carrying:", 15|(7<<4), 1, 3);
		AddEditBox(hwnd, 5, 127 & 0xef, 15, 3);

		AddButton(hwnd, WINDOW_BUTTON, " OK ", 15|(2<<4), 5, 6, NULL);

		tabOrderAdd(hwnd, WINDOW_EDIT_BOX, 0);
		tabOrderAdd(hwnd, WINDOW_EDIT_BOX, 1);
		tabOrderAdd(hwnd, WINDOW_BUTTON, 0);
		tabOrderSetFocus(hwnd, WINDOW_EDIT_BOX, 0);

		UpdateWindow();
	 return;

	case MSG_EDIT_BOX_PUTC:
		if(lParam && ((wParam<'0' || wParam>'9') && wParam!=8)) return;
	 break;

	//case MSG_SELECT_BUTTON: { char buffer[6], model[17];
	case MSG_PRESS_BUTTON: { char buffer[6], model[17];
		float maxCarrying;

		windowEditBoxGetText(hwnd, 0, model);
		windowEditBoxGetText(hwnd, 1, buffer);
		maxCarrying = (UINT)atoi(buffer);

		addCar(&lpCarList, model, maxCarrying);
		lpCurrentCar = lpCarList;
		PostQuitMsg(hwnd);
	 } return;
	}

	DefWindowProc(hwnd, message, wParam, lParam);
}

CALLBACK viewResultDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPWINDOW lpWindow = GetPtrToWindow(hwnd);

	viewResize(&View, lpWindow->SIZE_X-3, lpWindow->SIZE_Y-3);

	switch(message) {
	case MSG_PAINT:
		viewDisplay(hwnd, &View);
		PutStr2WindowXY(hwnd, (UCHAR*)"Max Carrying > 1000 kg.", 15|(7<<4), 3, 0);
	 return;

	case MSG_KB_UKEY: scrollView(&View, -1, 0); UpdateWindow(); return;
	case MSG_KB_DKEY: scrollView(&View, 1, 0); UpdateWindow(); return;

	case MSG_WINDOW_FIRST_RUN:
		View.xPos = View.yPos = 0;
		View.x = 1;		View.y = 2;
		windowParentSetFlag(hwnd, 2 | 1);
		View.bufferSize = createResultBuffer(lpCarList, &View.buffer);
		UpdateWindow();
	 return;

	case MSG_QUIT: closeView(&View);
		windowParentSetFlag(hwnd, 128 | 4 | 2 | 1);
	 break;
	}

	DefWindowProc(hwnd, message, wParam, lParam);
}

void fileDialogUserButtonFunc(HWND hWnd, HANDLE id)
{
	LPWINDOW lpWindow = GetPtrToWindow(hWnd);

	if(!id) {
		char buffer[12];
		LPWINDOW lpParent = GetPtrToWindow(lpWindow->parent_window);

		windowEditBoxGetText(hWnd, 0, buffer);

		free(lpParent->Title);
		lpParent->Title = (char*)malloc(strlen(buffer) + 7);
		sprintf(lpParent->Title, "Car (%s)", buffer);

		strcpy(fileName, buffer);

		if(lpWindow->Title[0] == 'O') {
			freeList(&lpCarList);
			loadCarList(&lpCarList, buffer);
			lpCurrentCar = lpCarList;
		} else saveCarList(lpCarList, buffer);
	}

	PostQuitMsg(hWnd);
}

HWND FileDialog(HWND hParent, char type, void (*UserButtonFunc)(HWND hWnd, HANDLE id), char x, char y)
{
	HWND hWnd;
	char *dialogName[] = { "Open File", "Save File As" };

	hWnd = CreateWindow(dialogName[type], x, y, 19, 6, 3, 7, 5);

	ChangeDefWindowFunc(hWnd, fileDialogDefWinProc);
	SetParentWindow(hWnd, hParent);

	windowParentSetFlag(hWnd, 2 | 1);

	AddEditBox(hWnd, 11, 127 & 0xef, 3, 0);
	AddButton(hWnd, WINDOW_BUTTON, "  OK   ", 15|(2<<4), 1, 2, UserButtonFunc);
	AddButton(hWnd, WINDOW_BUTTON, " Close ", 15|(2<<4), 9, 2, UserButtonFunc);

	tabOrderAdd(hWnd, WINDOW_EDIT_BOX, 0);
	tabOrderAdd(hWnd, WINDOW_BUTTON, 0);
	tabOrderAdd(hWnd, WINDOW_BUTTON, 1);
	tabOrderSetFocus(hWnd, WINDOW_EDIT_BOX, 0);

	UpdateWindow();

	return hWnd;
}

CALLBACK fileDialogDefWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case MSG_RESIZE_WINDOW: return;
	case MSG_QUIT: windowParentSetFlag(hwnd, 128 | 4 | 2 | 1); break;

	case MSG_EDIT_BOX_PUTC:
		if(!isalpha(wParam) && wParam!=8 && wParam!='.') return;
	 break;
	}

	DefWindowProc(hwnd, message, wParam, lParam);
}

void viewResize(struct view *View, char size_x, char size_y)
{
	View->size_x = size_x;	View->size_y = size_y;
}

void viewDisplay(HWND hWnd, struct view *View)
{
	int x, y;
	UINT i = 0;

	if(!*View->buffer) return;

	y = View->yPos;
	while(View->buffer[i] && y>0) {
		if(View->buffer[i] == '\n') --y;
		++i;
	}
	if(i>View->bufferSize-1) i = View->bufferSize;

	x = y = 0;
	while(View->buffer[i] && y < View->size_y) {
		switch(View->buffer[i]) {
		case   13: break;
		case '\t': x+=4; break;
		case '\n': x = 0; ++y; break;
		default:
			if(x < View->size_x) {
				PutChar2WindowXY(hWnd, View->buffer[i], 15 | (7<<4), View->x+x, View->y+y);
				++x;
			}
		}

		++i;
	}
}

void scrollView(struct view *View, char UpDown, char LeftRight)
{
	switch(UpDown) {
	case -1: if(--View->yPos<0) View->yPos = 0; break;
	case  1: if(++View->yPos>View->bufferSize) View->yPos = View->bufferSize;
	}

	switch(LeftRight) {
	case -1: if(--View->xPos<0) View->xPos = 0; break;
	case  1: View->xPos++;
	}
}

void closeView(struct view *View)
{
	if(*View->buffer) {
		free(View->buffer);
		*View->buffer = 0;
	}
}
