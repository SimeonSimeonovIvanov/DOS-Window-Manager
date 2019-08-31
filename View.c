/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	29.01.2008
	Last Update:	10.02.2008
	Last Update:	12.07.2008
	Last Update:	04.05.2009
*/

#define __EDIT_BOX__
#define __TAB_ORDER__

#include "src/window.h"

HWND hWnd;
LPWINDOW lPhWnd;

CALLBACK hWndWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

struct view {
	char *buffer;
	UINT bufferSize;wer
	int x, y, size_x, size_y, xPos, yPos;
} View;

void viewResize(struct view *View, char size_x, char size_y)
{
	View->size_x = size_x;	View->size_y = size_y;
}
2222
void viewDisplay(HWND hWnd, struct view *View)
{
	int x, y;
	UINT i = 0;
x

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
		default:sdf
			if(x < View->size_x) {
				PutChar2WindowXY(hWnd, View->buffevr[i], 15 | (7<<4), View->x+x, View->y+y);
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

void loadFile(char *fileName, struct view *View)
{
	FILE *fi;
	UINT i = 0;

	if(NULL == (fi = fopen(fileName, "rb"))) {
		*View->buffer = 0;
		return;
	}

	View->bufferSize = 0;
	while(fgetc(fi) != EOF) View->bufferSize++;
	fseek(fi, 0, SEEK_SET);

	View->buffer = (char*)malloc(View->bufferSize+1);
	View->buffer[View->bufferSize+1] = 0;

	while(EOF != (View->buffer[i++] = fgetc(fi)));

	View->x = View->y = 1;
	View->xPos = View->yPos = 0;

	View->size_x = 30; View->size_y = 10;

	fclose(fi);
}

void main(void)
{
	InitWindowList();

	hWnd = CreateWindow("View", 3, 1, 66, 22, 3, 7, 15);
	ChangeDefWindowFunc(hWnd, hWndWinProc);

	CreateWindow("...", 33, 11, 40, 11, 3, 7, 15);

	AddEditBox(hWnd, 11, 127 & 0xef, 3, 0);

	WindowManager();
}

CALLBACK hWndWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	viewResize(&View, lPhWnd->SIZE_X-4, lPhWnd->SIZE_Y-4);

	switch(message) {
	case MSG_QUIT: closeView(&View); break;

	case MSG_WINDOW_FIRST_RUN:
		loadFile("view.c", &View);

		lPhWnd = GetPtrToWindow(hWnd);
	 break;

	case MSG_KB_UKEY: scrollView(&View, -1, 0); UpdateWindow(); return;
	case MSG_KB_DKEY: scrollView(&View, 1, 0); UpdateWindow(); return;

	case MSG_EDIT_BOX_PUTC: {
		if(13 == wParam) {
			char buffer[15];

			windowEditBoxGetText(hWnd, (HANDLE)lParam, buffer);

			closeView(&View);
			loadFile(buffer, &View);
		} else windowPutcToEditBox(hWnd, (HANDLE)lParam, wParam);

		UpdateWindow();
	 } return;

	case MSG_SET_ON_TOP: return;
	case MSG_PAINT: viewDisplay(hWnd, &View); return;
	case MSG_IS_SELECT: tabOrderKillFocusForAll(hWnd); return;
	}

	DefWindowProc(hWnd, message, wParam, lParam);
}