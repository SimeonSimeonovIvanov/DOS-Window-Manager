/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	26.12.2007
	Last Update:	28.12.2007
	Last Update:	10.02.2008
*/

#define __MENU__
#define __TIMER__
#define __LABEL__
#define __CHAR_MAP__
#define __EDIT_BOX__
#define __TAB_ORDER__

#include "src\\window.h"

void timer(HWND hWnd, HANDLE id);
CALLBACK hWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void main(void)
{
	HWND hWnd;

	CreateWindow(".:.", 1, 1, 22, 7, 5, 14, 6);
	hWnd = CreateWindow("hWnd", 4, 3, 33, 11, 5, 14, 6);
	ChangeDefWindowFunc(hWnd, hWndProc);

	AddCharMap(hWnd, 2, 5, 16, 4);
	PutCharToCharMap(hWnd, 0, 'A', 15, 1, 1);

	AddLabel(hWnd, "For EditBox 0", 0x70, 1, 1);
	AddLabel(hWnd, "For EditBox 1", 0x70, 19, 7);


	AddButton(hWnd, WINDOW_BUTTON, "Button", 15, 1,
				  3, NULL);

	AddTimer(hWnd, 3333, timer);

	AddEditBox(hWnd, 18, 127 & 0xef, 9, 3);

	AddEditBox(hWnd, 9, (7<<4)|12, 19, 5);

	tabOrderAdd(hWnd, WINDOW_EDIT_BOX, 0);
	tabOrderAdd(hWnd, WINDOW_EDIT_BOX, 1);
	tabOrderAdd(hWnd, WINDOW_BUTTON, 0);

	tabOrderSetFocus(hWnd, WINDOW_EDIT_BOX, 0);

	WindowManager();
}

CALLBACK hWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
	case MSG_EDIT_BOX_PUTC: { char buffer[33];
		LPLABEL lpLabel = GetPtrToLabel(hwnd, (HANDLE)lParam);

		if(wParam != 13) {
			windowPutcToEditBox(hwnd, (HANDLE)lParam, wParam);

			if(!lParam) {
				windowEditBoxGetText(hwnd, (HANDLE)lParam, buffer);
				SetLabel(hwnd, (HANDLE)lParam, buffer, lpLabel->text_color, lpLabel->x, lpLabel->y);
			}
		} else {
			windowEditBoxGetText(hwnd, (HANDLE)lParam, buffer);
			SetLabel(hwnd, (HANDLE)lParam, buffer, lpLabel->text_color, lpLabel->x, lpLabel->y);
		}

		UpdateWindow();
	 } return;
	}

	DefWindowProc(hwnd, msg, wParam, lParam);
}

void timer(HWND hWnd, HANDLE id)
{
	hWnd |= 0; id |= 0;
}