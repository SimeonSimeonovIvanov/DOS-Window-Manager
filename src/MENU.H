/*
			The^day^of^DooM

			Borland C++ 3.1
				  &&
			  Open Watcom

	Create Date:	27.03.2007
	Last Update:	29.09.2007
	Last Update:	10.02.2008
	Full Change:    27.12.2008
	Last Update:	05.05.2009
*/

#ifndef __MENU_H__
	#define __MENU_H__

	#ifndef __BUTTON__
		#include "src/button.h"
		#define __BUTTON__
	#endif

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

//////////////////////////////////////////////////////////////////////////////
void DefPopUpMenuButtonProc(HWND hMenu, HANDLE id);
CALLBACK DefPopUpMenuProc(HWND hMenu, UINT message, WPARAM wParam, LPARAM lParam);
HWND PopUpMenu(HWND hParent, char *szButton, char x, char y, void (*UserButtonFunc)(HWND hWnd, HANDLE id));
UCHAR ParseMenuButton(HWND hMenu, UCHAR type, char *sButton, void (*UserButtonFunc)(HWND hWnd, HANDLE id));
//////////////////////////////////////////////////////////////////////////////
UCHAR CreateWindowMenu(HWND hWnd, char *sButton, void (*UserButtonFunc)(HWND hWnd, HANDLE id))
{
	if(GetPtrToWindow(hWnd)) {
	  if(ParseMenuButton(hWnd, WINDOW_MENU, sButton, UserButtonFunc)) return 1;

	  SelectButton(hWnd, WINDOW_MENU_BUTTON, 0);
	}

	return 2;
}

HWND PopUpMenu(HWND hParent, char *szButton, char x, char y, void (*UserButtonFunc)(HWND hWnd, HANDLE id)) // +
{
	HWND hMenu;
	LPWINDOW lpMenu;

	if(NULL == (lpMenu = GetPtrToWindow(hParent))) return -1;

	hMenu = CreateWindow("", x, y, 0, 0, 10, 9, 2);

	if(NULL != (lpMenu = GetPtrToWindow(hMenu))) {
	  // Remove close button; PopUpMenu
	  windowSetFlag(hMenu, (2^0xffff) & (32 | windowGetFlag(hMenu)));
	  SetParentWindow(hMenu, hParent);

	  lpMenu->WindowFunc = DefPopUpMenuProc;

	  if(ParseMenuButton(hMenu, WINDOW_POP_UP_MENU, szButton, UserButtonFunc)) return -2;

#ifdef __TAB_ORDER__
	  tabOrderSetFocus(hMenu, WINDOW_BUTTON, 0);
#else
	  SetFocusFor(hMenu, WINDOW_BUTTON, 0);
#endif

	  KillFocus(hMenu, 3); // Set Focus For Only PopUpMenu window.

	  UpdateWindow();

	  return hMenu;
	}

	return -3;
}

CALLBACK DefPopUpMenuProc(HWND hMenu, UINT message, WPARAM wParam, LPARAM lParam) // +
{
	LPWINDOW lpMenu;
	char selectButton = GetSelectButton(hMenu, WINDOW_BUTTON);
	UCHAR number_of_button = GetNumberOfButton(hMenu, 0);

	lpMenu = GetPtrToWindow(hMenu);

	switch(message) {
	case MSG_MOVE_WINDOW:
	case MSG_RESIZE_WINDOW: return;

	case MSG_PAINT:
		PutChar2WindowXY(hMenu, '>', 15, 0, selectButton);
		PutChar2WindowXY(hMenu, '<', 15, lpMenu->SIZE_X-3, selectButton);
	 return;

	case MSG_KB_UKEY:
	case MSG_KB_RKEY: if((int)--selectButton == -1) selectButton = number_of_button-1;
#ifdef __TAB_ORDER__
		tabOrderSetFocus(hMenu, WINDOW_BUTTON, selectButton);
#else
		SetFocusFor(hMenu, WINDOW_BUTTON, selectButton);
#endif
		UpdateWindow();
	 return;

	case MSG_KB_DKEY:
	case MSG_KB_LKEY: if(++selectButton > number_of_button-1) selectButton = 0;
#ifdef __TAB_ORDER__
		tabOrderSetFocus(hMenu, WINDOW_BUTTON, selectButton);
#else
		SetFocusFor(hMenu, WINDOW_BUTTON, selectButton);
#endif
		UpdateWindow();
	 return;

	case MSG_KB_PRESS_KEY:
		switch(wParam) {
		case 27: PostQuitMsg(hMenu); return;
		}

	 break;

	case MSG_IS_SELECT:
		if(lpMenu->MsgList->lpMsg->pt.y > 0 && lpMenu->MsgList->lpMsg->pt.y < lpMenu->SIZE_Y-1) {
#ifdef __TAB_ORDER__
		  tabOrderSetFocus(hMenu, WINDOW_BUTTON, lpMenu->MsgList->lpMsg->pt.y - 1);
#else
		  SetFocusFor(hMenu, WINDOW_BUTTON, lpMenu->MsgList->lpMsg->pt.y - 1);
#endif
		}
	 return;


	case MSG_PRESS_BUTTON: {
		LPBUTTON lpButton = GetPtrToButton(hMenu, WINDOW_MENU, wParam);

		if(lpButton)
			if(lpButton->DefButtonFunc)
				if(lpButton->DefButtonFunc != DefPopUpMenuButtonProc)
					lpButton->DefButtonFunc(hMenu, wParam);

		DefPopUpMenuButtonProc(hMenu, wParam);
	} return;

	case MSG_MOUSE_CLICK: PostQuitMsg(hMenu); return;

	case MSG_QUIT: SetFocusForParentWindow(hMenu);
	default: DefWindowProc(hMenu, message, wParam, lParam); return;
	}

	UpdateWindow();
}

void DefPopUpMenuButtonProc(HWND hMenu, HANDLE id) // +
{
	LPWINDOW lpMenu;

	lpMenu = GetPtrToWindow(hMenu);

	PostQuitMsg(hMenu);

	SendMessage(lpMenu->parent_window, MSG_POP_UP_MENU_SELECT_BUTTON, id, 0);
}

UCHAR ParseMenuButton(HWND hMenu, UCHAR type, char *sButton, void (*UserButtonFunc)(HWND hWnd, HANDLE id)) // +
{
	HANDLE hButton;
	LPWINDOW lpMenu;
	char buffer[33],
		 i, size_x = 0,
		 number_of_button = 0,
		 last_x_pos = 0;

	if(NULL == (lpMenu = GetPtrToWindow(hMenu))) return 1;

	if(WINDOW_MENU == type && lpMenu->list_menu_button) return 1;

	while(*sButton) {
	  last_x_pos += size_x+1;

	  if('&' == *sButton++) {
		i = 0;

		if(WINDOW_MENU==type) size_x = 0;

		while(*sButton && ('&' != *sButton)) {
		  switch(type) {
		  case WINDOW_MENU: size_x++; break;
		  case WINDOW_POP_UP_MENU: if(size_x < i) size_x = i;
		  }

		  buffer[i++] = *sButton++;
		}

		buffer[i] = 0;

		switch(type) {
		case WINDOW_MENU:
			if(0 > (hButton = AddButton(hMenu, WINDOW_MENU_BUTTON, buffer, 1|(7<<4), last_x_pos, 0, UserButtonFunc)))
			  return 2;

			SetButtonFlag(hMenu, WINDOW_MENU_BUTTON, hButton, 8|1|2);
		 break;

		case WINDOW_POP_UP_MENU: {
			if(0 > (hButton = AddButton(hMenu, WINDOW_BUTTON, buffer,  9, 1, number_of_button, UserButtonFunc)))
				return 2;

#ifdef __TAB_ORDER__
			tabOrderAdd(hMenu, WINDOW_BUTTON, hButton);
#endif
		 }
		}

		++number_of_button;
	  }
	}

	if(WINDOW_POP_UP_MENU == type) {
	  lpMenu->SIZE_X = size_x + 5;
	  lpMenu->SIZE_Y = number_of_button + 2;
	}

	return 0;
}
#endif // __MENU_H_