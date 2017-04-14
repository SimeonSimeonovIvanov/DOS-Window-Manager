/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	25.02.2007
	Last Update:	01.11.2007
	Full Change:	27.12.2007
	Last Update:	27.12.2007
	Last Update:	22.03.2008
*/

//#define __OPEN_WATCOM__

#define __MENU__
#define __TIMER__
#define __LABEL__
#define __CHAR_MAP__
#define __TAB_ORDER__

#include "src/window.h"

void charMapTimer(HWND hwnd, HANDLE id);
void moveBall(HWND hwnd, HANDLE idTimer);
void window0timerFunc(HWND hwnd, HANDLE idTimer);
void window1timerFunc(HWND hwnd, HANDLE idTimer);
void drawToBackGround(HWND hwnd, HANDLE idTimer);
CALLBACK mainWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

void main(void)
{
	InitWindowList();


	ChangeDefWindowFunc( CreateWindow("typeWindow 0", 3, 2, 33, 17, 10, 0, 2),
						 mainWinProc
	);

	ChangeDefWindowFunc( CreateWindow("Char Map test", 11, 5, 43, 20, 11, 0, 2),
						 mainWinProc
	);

	CreateWindowMenu(0, "&File", NULL);
	CreateWindowMenu(1, "&File&Run Timer&Stop Timer&Remove Timer", NULL);

	AddCharMap(1, 1, 2, 10, 3);
	AddCharMap(1, 20, 10, 16, 7);

	AddTimer(0, 12222, window0timerFunc);
	AddLabel(0, "", 0, 0, 0);

	AddTimer(1, 1111, charMapTimer);
	AddTimer(1, 1, drawToBackGround);
	AddTimer(1, 49456, window1timerFunc);
	AddLabel(1, "", 0, 0, 0);

	CreateWindow("Ball", 4, 3, 30, 17, 10, 0, 2);
	AddCharMap(2, 1, 2, 17, 17);
	AddTimer(2, 12233, moveBall);

	WindowManager();
}

CALLBACK mainWinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case MSG_PAINT: {
		FILE *fp;
		char *str;
		int x, y = 1, i;

		if(hwnd) return;

		str = malloc(256);
		fp = fopen("typeMain.c", "r");

		while(fgets(str, 255, fp)) {
		  x = i = 0;

		  while(str[i]) {

			switch(str[i]) {
			case 10: break;
			case '\t': x+=4; break;
			default: PutChar2WindowXY(hwnd, str[i], 15, x++, y);
			}

			++i;
		  }

		  ++y;
		}

		free(str);
		fclose(fp);
	 }
	 return;

	case MSG_POP_UP_MENU_SELECT_BUTTON: {
		switch(wParam) {
		case 3: PostQuitMsg(hwnd); return;
		case 1: CreateWindow("Save ", 2, 1, 1, 5, 3, 4, 6);
		}

		UpdateWindow();
	 }
	 return;

	case MSG_KB_PRESS_KEY: if(27 == wParam) PostQuitMsg(hwnd); return;

	case MSG_WINDOW_MENU_SELECT_BUTTON: { int x, y;

		GetButtonAbsPos(hwnd, WINDOW_MENU_BUTTON, wParam, &x, &y);

		switch(wParam) {
		case 0: while(mouse.button == 1) Mouse
			PopUpMenu(hwnd, "&New&Save&Save as...&Exit", x, ++y, NULL);
		 break;
		case 1: RunTimer(hwnd, 0); break;
		case 2: StopTimer(hwnd, 0); break;
		case 3: RemoveTimer(hwnd, 0); break;
		}
	 }

	default: DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void window0timerFunc(HWND hwnd, HANDLE idTimer)
{

	static char str[] = { "Window[0]->Timer[0] = 0" };

	SetLabel(hwnd, 0, str, 15|(3<<4), 8, 0);
	UpdateWindow();

	str[17] = idTimer + 48;

	if(++str[22] > '9') str[22] = '0';
}

void window1timerFunc(HWND hwnd, HANDLE idTimer)
{
	static char str[] = { "Timer: A" };

	idTimer|=0;

	SetLabel(hwnd, 0, str, 15|128, 14, 1);
	UpdateWindow();

	if(++str[7] > 'A'+ 25) str[7] = 'A';
}

void drawToBackGround(HWND hwnd, HANDLE idTimer)
{
	RemoveTimer(hwnd, idTimer);
	PutStrToBuffer(BackGroundBuffer, (UCHAR*)"Test BackGround...", 10, 55, 1);
}

void charMapTimer(HWND hwnd, HANDLE id)
{
	int i, j;

	SetTimer(hwnd, id, 22000);

	for(i=0;i<16;i++)
	  for(j=0;j<7;j++) {
		PutCharToCharMap(hwnd, 0, '*', random(15), i, j);
		PutCharToCharMap(hwnd, 1, '*', random(4), i, j);
	  }

	UpdateWindow();
}

void moveBall(HWND hwnd, HANDLE idTimer)
{
	static char ballx=10, bally=11, dx=1, dy=2;

	PutCharToCharMap(hwnd, 0, ' ', 0, ballx, bally);

	ballx += dx;
	if(ballx>16) dx=-1;
	if(ballx<0) dx=1;

	bally += dy;
	if(bally>14) dy=-1;
	if(bally<0) dy=1;

	PutCharToCharMap(hwnd, 0, '*', 15, ballx, bally);

	UpdateWindow();
	idTimer |= 0;
}