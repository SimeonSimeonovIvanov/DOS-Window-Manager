/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	21.05.2007
	Last Update:	21.05.2007
	Full Change:    27.12.2007
	Last Update:    28.12.2007
	Last Update:	09.02.2008
	Last Update:	06.05.2009
*/

#ifndef __CHAR_MAP_H__
	#define __CHAR_MAP_H__

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

CHAR_MAP* GetPtrToCharMap(HWND hwnd, HANDLE id);

void freeCharMap(void *lpData)
{
	free(((CHAR_MAP*)lpData)->buffer);
}

HANDLE AddCharMap(HWND hWnd, UCHAR x, UCHAR y, UCHAR size_x, UCHAR size_y) // *
{
	CHAR_MAP *temp;
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd)))
	  if(NULL != (temp = (CHAR_MAP *) malloc(sizeof(CHAR_MAP))))
		if(NULL != (temp->buffer = (UCHAR *) malloc(2*(size_x*size_y)))) {
		  temp->x = x;				temp->y = y;
		  temp->size_x = size_x;	temp->size_y = size_y;
		  memset(temp->buffer, 0, 2 * size_x * size_y);

		  return addToList(&lpWindow->list_char_map, (void*)temp, freeCharMap);
		}

	return -1;
}

void RemoveCharMap( HWND hWnd, HANDLE hCharMap) // +
{
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd)))
	  removeFromList(&lpWindow->list_char_map, hCharMap);
}

void PutCharToCharMap(HWND hwnd, HANDLE id, UCHAR ch, UCHAR color, UCHAR x, UCHAR y) // +
{
	CHAR_MAP *lpCharMap;

	if(NULL != (lpCharMap = GetPtrToCharMap(hwnd, id))) {
	  if(lpCharMap->size_x > x && lpCharMap->size_y > y) {
		lpCharMap->buffer[y * lpCharMap->size_x + x] = ch;
		lpCharMap->buffer[y * lpCharMap->size_x + x + lpCharMap->size_x * lpCharMap->size_y] = color;
	  }
	}
}

void charMapCharColor(HWND hwnd, HANDLE id, UCHAR color, UCHAR x, UCHAR y) // +
{
	CHAR_MAP *lpCharMap;

	if(NULL != (lpCharMap = GetPtrToCharMap(hwnd, id))) {
	  if(lpCharMap->size_x > x && lpCharMap->size_y > y) {
		lpCharMap->buffer[y * lpCharMap->size_x + x + lpCharMap->size_x * lpCharMap->size_y] = color;
	  }
	}
}

CHAR_MAP* GetPtrToCharMap(HWND hwnd, HANDLE id) // +
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hwnd))) {
	  CHAR_MAP *lpCharMap;

	  lpList = lpWindow->list_char_map;
	  while(lpList) {
		lpCharMap = (CHAR_MAP*)lpList->lpData;
		if(id == lpList->id) return lpCharMap;
		lpList = lpList->next;
	  }
	}

	return NULL;
}
#endif // __CHAR_MAP_H__