/*
			The^day^of^DooM

			Borland C++ 3.1

	 Create Date:	26.12.2007
	 Last Update:	23.13.2007
	 Full Change:	27.12.2008
	 Last Update:	27.12.2008
	 Last Update:	01.08.2010
*/

#ifndef __EDIT_BOX_H__
	#define __EDIT_BOX_H__

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

char editBoxSetText(LPEDIT_BOX lpEditBox, char *szText);
char editBoxGetText(LPEDIT_BOX lpEditBox, char *szText);

void freeEditBox(void *lpData)
{
	free(((LPEDIT_BOX)lpData)->text);
}

HANDLE AddEditBox( HWND hWnd, UCHAR size, UCHAR color, UCHAR x, UCHAR y) // ?
{
	LPEDIT_BOX temp;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = (LPWINDOW) GetPtrToWindow(hWnd))) return -1;
	if(NULL == (temp = (LPEDIT_BOX) malloc(sizeof(EDIT_BOX)))) return -2;
	if(NULL == (temp->text = (UCHAR*) malloc(size))) return -3;

	temp->text[0] = 0;

	temp->flag = 3;
	temp->count = 0;
	temp->lenght = size;
	temp->x = x; temp->y = y;
	temp->text_color = color;

	return addToList(&lpWindow->list_edit_box, (void*)temp, freeEditBox);
}

void windowEditBoxSetText(HWND hWnd, HANDLE id, char *szText)
{
	editBoxSetText(GetPtrToEditBox(hWnd, id), szText);
}

char windowEditBoxGetText(HWND hWnd, HANDLE id, char *szText)
{
	return editBoxGetText(GetPtrToEditBox(hWnd, id), szText);
}

char editBoxSetText(LPEDIT_BOX lpEditBox, char *szText)
{
	UCHAR i = 0;

	if(NULL == lpEditBox) return -1;

	while(szText[i] && i < lpEditBox->lenght)
	   lpEditBox->text[i] = (UCHAR)szText[i++];

	lpEditBox->text[i] = 0;
	lpEditBox->count = i;
	return 0;
}

char editBoxGetText(LPEDIT_BOX lpEditBox, char *szText)
{
	if(NULL == lpEditBox) return -1;
	strcpy(szText, lpEditBox->text);
	return lpEditBox->count;
}

void windowPutcToEditBox(HWND hWnd, HANDLE idEditBox, WPARAM wpKey)
{
	LPEDIT_BOX lpEdit = (LPEDIT_BOX)GetPtrToEditBox(hWnd, idEditBox);

	switch(wpKey) {
	case 0: break;
	case 8:
		if(--(lpEdit->count)>=0) {
			lpEdit->text[lpEdit->count] = 0;
		} else lpEdit->count=0;
	 break;

	default:
		if(lpEdit->count < lpEdit->lenght) {
			lpEdit->text[lpEdit->count++] = wpKey;
			lpEdit->text[lpEdit->count] = 0;
		} else lpEdit->count = lpEdit->lenght;
	}

	UpdateWindow();
}

void RemoveEditBox(HWND hWnd, HANDLE hEditBox) // +
{
	register LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd))) {
	  removeFromList(&lpWindow->list_edit_box, hEditBox);
	}
}

UCHAR isSelectEditBox(LPLIST lpObj, UCHAR x, UCHAR y)
{
	if( y-1 == ((LPEDIT_BOX)lpObj->lpData)->y && x > ((LPEDIT_BOX)lpObj->lpData)->x &&
		x <= ((LPEDIT_BOX)lpObj->lpData)->x + ((LPEDIT_BOX)lpObj->lpData)->lenght
	) return 1;
	return 0;
}

LPEDIT_BOX GetPtrToEditBoxInList(LPLIST root, HANDLE id)
{
	while(root) {
	  if(id == root->id) return (LPEDIT_BOX)root->lpData;
	  root = root->next;
	}

	return NULL;
}

LPEDIT_BOX GetPtrToEditBox(HWND hWnd, HANDLE id)
{
	LPWINDOW window;

	if(NULL != (window = GetPtrToWindow(hWnd)))
	  return GetPtrToEditBoxInList(window->list_edit_box, id);

	return NULL;
}
#endif // __EDIT_BOX_H__