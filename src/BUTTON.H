/*
			The^day^of^DooM

			Borland C++ 3.1
				  &&
			  Open Watcom

	Create Date:	28.03.2007
	Last Update:	29.09.2007
	Full Change:    26.12.2007
	Last Update:    30.12.2007
	Last Update:	12.07.2008
*/

#ifndef __BUTTON_H__
	#define __BUTTON_H__

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

void freeButton(void *lpData)
{
	free(((LPBUTTON)lpData)->text);
}

HANDLE AddButton( HWND hWnd, UCHAR type, char *text, UCHAR color, UCHAR x,
				  UCHAR y, void (*UserButtonFunc)(HWND hWnd, HANDLE id)
				) // +
{

	LPBUTTON temp;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hWnd))) return -1;

	if(NULL != (temp = (LPBUTTON) malloc(sizeof(LABEL))) ) {
	  if(NULL != (temp->text = (char*) malloc(strlen((char*)text)+1))) {

		temp->flag = 3;
		strcpy(temp->text, text);
		temp->x = x; temp->y = y;
		temp->text_color = color;
		temp->DefButtonFunc = UserButtonFunc;

		switch(type) {
		case WINDOW_BUTTON: return addToList(&lpWindow->list_button, (void *)temp, freeButton);
		case WINDOW_MENU_BUTTON: return addToList(&lpWindow->list_menu_button, (void *)temp, freeButton);
		}
	  }
	}

	return -2;
}

void RemoveButton( HWND hWnd, UCHAR type, HANDLE hButton) // +
{
	register LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd))) {
	  switch(type) {
	  case WINDOW_BUTTON: removeFromList(&lpWindow->list_button, hButton); break;
	  case WINDOW_MENU_BUTTON: removeFromList(&lpWindow->list_menu_button, hButton);
	  }
	}
}

UCHAR GetButtonPos(HWND hwnd, UCHAR type, HANDLE id, int *x, int *y)
{
	LPBUTTON lpButton;

	if(NULL == (lpButton = GetPtrToButton(hwnd, type, id))) return 1;

	(*x) = lpButton->x;	(*y) = lpButton->y;

	return 0;
}

UCHAR GetButtonAbsPos(HWND hwnd, UCHAR type, HANDLE id, int *x, int *y)
{
	int wX, wY;

	if(GetWindowPos(hwnd, &wX, &wY)) return 1;

	if(GetButtonPos(hwnd, type, id, x, y)) return 1;

	*x += wX+1;	*y += wY+1;

	return 0;
}

HANDLE GetSelectButton(HWND hwnd, UCHAR type) // +
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hwnd))) return -1;

	switch(type) {
	case WINDOW_BUTTON: lpList = lpWindow->list_button; break;
	case WINDOW_MENU_BUTTON: lpList = lpWindow->list_menu_button; break;
	default: return -1;
	}

	while(lpList) {
	  if(4 & ((LPBUTTON)lpList->lpData)->flag)
		return lpList->id;

	  lpList = lpList->next;
	}

	return -1;
}

UCHAR SelectButton(HWND hWnd, UCHAR type, HANDLE buttonID) // +
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hWnd))) return 1;

	switch(type) {
	case WINDOW_BUTTON: lpList = lpWindow->list_button; break;
	case WINDOW_MENU_BUTTON: lpList = lpWindow->list_menu_button; break;
	default: return 1;
	}

	while(lpList) {
	  if(buttonID != lpList->id) ((LPBUTTON)lpList->lpData)->flag &= 4^255;
	  else ((LPBUTTON)lpList->lpData)->flag |= 4;

	  lpList = lpList->next;
	}

	return 0;
}

UCHAR GetNumberOfButton(HWND hwnd, UCHAR type) // +
{
	LPLIST lpList;
	UCHAR counter = 0;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hwnd))) return 1;

	switch(type) {
	case WINDOW_BUTTON: lpList = lpWindow->list_button; break;
	case WINDOW_MENU_BUTTON: lpList = lpWindow->list_menu_button; break;
	default: return -1;
	}

	while(lpList) {
	  counter++;
	  lpList = lpList->next;
	}

	return counter;
}

void SetButtonFlag(HWND hWnd, UCHAR type, HANDLE hButton, UCHAR flag)
{
	LPBUTTON lpButton;
	if(NULL != (lpButton = GetPtrToButton(hWnd, type, hButton)))
	  lpButton->flag = flag;
}

UCHAR isSelectButton(LPLIST lpObj, UCHAR x, UCHAR y)
{
	if( y-1 == ((LPBUTTON)lpObj->lpData)->y && x > ((LPBUTTON)lpObj->lpData)->x &&
		x <= ((LPBUTTON)lpObj->lpData)->x + strlen((char*)((LPBUTTON)lpObj->lpData)->text)
	) return 1;

	return 0;
}

LPBUTTON GetPtrToButton(HWND hWnd, UCHAR type, HANDLE id) // +
{
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd))) {
	  switch(type) {
	  case WINDOW_BUTTON: return GetPtrToButtonInList(lpWindow->list_button, id);
	  case WINDOW_MENU_BUTTON: return GetPtrToButtonInList(lpWindow->list_menu_button, id);
	  }
	}

	return NULL;
}

LPBUTTON GetPtrToButtonInList(LPLIST root, HANDLE id) // +
{
	while(root) {
	  if(id == root->id) return (LPBUTTON)root->lpData;
	  root = root->next;
	}

	return NULL;
}
#endif // __BUTTON_H__