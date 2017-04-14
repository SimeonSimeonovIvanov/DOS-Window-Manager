/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	02.02.2008
	Last Update:	12.07.2008
*/

#ifndef __TAB_ORDER__H__
	#define __TAB_ORDER_H__

#ifndef __WINDOW_H__
	#define __TAB_ORDER__
	#include "src/window.h"
#endif

void tabOrderAdd(HWND hWnd, UCHAR type, HANDLE id)
{

	LPTAB_ORDER temp;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hWnd))) return;
	if(NULL == (temp = (LPTAB_ORDER) malloc(sizeof(TAB_ORDER)))) return;

	temp->id = id;
	temp->flag = 0;
	temp->type = type;

	addToList(&lpWindow->list_tab_order, (void *)temp, NULL);
}

UCHAR tabOrderSetFocus(HWND hWnd, UCHAR type, HANDLE id)
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hWnd))) return 1;

	lpList = lpWindow->list_tab_order;
	while(lpList) {

		if( type != ((LPTAB_ORDER)lpList->lpData)->type ||
			id != ((LPTAB_ORDER)lpList->lpData)->id
		) ((LPTAB_ORDER)lpList->lpData)->flag = 0;
		else {
			((LPTAB_ORDER)lpList->lpData)->flag = 1;
			SetFocusFor(hWnd, ((LPTAB_ORDER)lpList->lpData)->type, ((LPTAB_ORDER)lpList->lpData)->id);
		}

		lpList = lpList->next;
	}

	return 0;
}

void tabOrderKillFocusForAll(HWND hWnd)
{
	LPWINDOW current;

	if(NULL != (current = GetPtrToWindow(hWnd))) {
		LPLIST lpTemp;
#ifdef __BUTTON__
		lpTemp = current->list_button;
		while(lpTemp) {
			((LPBUTTON)lpTemp->lpData)->flag &= 4^255;
			lpTemp = lpTemp->next;
		}
#endif

#ifdef __EDIT_BOX__
		lpTemp =  current->list_edit_box;
		while(lpTemp) {
			((LPEDIT_BOX)lpTemp->lpData)->flag &= 4^255;
			lpTemp = lpTemp->next;
		}
#endif
	}
}

void tabOrderProcess(HWND hWnd)
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = GetPtrToWindow(hWnd))) return;
	if(NULL == (lpList = lpWindow->list_tab_order)) return;

	while(lpList) {
		if(((LPTAB_ORDER)lpList->lpData)->flag) break;
		lpList = lpList->next;
	}

	if(lpList->next) {
		tabOrderSetFocus(hWnd, ((LPTAB_ORDER)lpList->next->lpData)->type, ((LPTAB_ORDER)lpList->next->lpData)->id);
	} else {
		lpList = lpWindow->list_tab_order;
		tabOrderSetFocus(hWnd, ((LPTAB_ORDER)lpList->lpData)->type, ((LPTAB_ORDER)lpList->lpData)->id);
	}
}
#endif