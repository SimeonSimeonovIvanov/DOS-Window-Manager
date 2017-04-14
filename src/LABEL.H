/*
			The^day^of^DooM
			Borland C++ 3.1

	Create Date:	31.03.2007
	Last Update:	29.09.2007
	Full Change:    26.12.2007
	Last Update:    28.12.2007
	Last Update:	12.07.2008
	Last Update:	05.04.2009
*/

#ifndef __LABEL_H__
	#define __LABEL_H__

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

void freeLabel(void *lpData)
{
	free(((LPLABEL)lpData)->text);
}

HANDLE AddLabel(HWND hWnd, char *text, UCHAR color, UCHAR x, UCHAR y) // +
{
	LPLABEL temp;
	LPWINDOW window;

	if(NULL == (window = GetPtrToWindow(hWnd))) return -1;
	if(NULL == (temp = (LABEL *) malloc(sizeof(LABEL)))) return -2;
	if(NULL == (temp->text = (char*) malloc(strlen((char*)text)+1))) {
		free(temp);
		return -3;
	}

	temp->flag = 1;
	strcpy(temp->text, text);
	temp->x = x; temp->y = y;
	temp->text_color = color;

	return addToList(&window->list_label, (void *) temp, freeLabel);
}

void RemoveLabel( HWND hWnd, HANDLE hLabel) // +
{
	register LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd)))
	  removeFromList(&lpWindow->list_label, hLabel);
}

UCHAR SetLabel( HWND hWnd, HANDLE id, char *text,
				UCHAR color, UCHAR x, UCHAR y
			  ) // +
{
	LPLABEL lpLabel;

	if(NULL != (lpLabel = GetPtrToLabel(hWnd, id))) {
	  free(lpLabel->text);
	  lpLabel->text = (char*)malloc(strlen(text)+1);
	  strcpy(lpLabel->text, text);

	  lpLabel->text_color = color;
	  lpLabel->x = x;	lpLabel->y = y;
	  return 0;
	}

	return 0;
}

LPLABEL GetPtrToLabel(HWND hwnd, HANDLE id) // +
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hwnd))) {

	  lpList = lpWindow->list_label;
	  while(lpList) {
		if(id == lpList->id) return (LPLABEL)lpList->lpData;
		lpList = lpList->next;
	  }
	}

	return NULL;
}
#endif // __LABEL_H__