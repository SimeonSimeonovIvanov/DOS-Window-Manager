/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	25.02.2007
	Last Update:	29.09.2007
	Full Change:	27.12.2007
	Last Update:	27.12.2007
	Last Update:	22.03.2008
*/

//#define __OPEN_WATCOM__

#define __MENU__
#define __LABEL__
#define __TAB_ORDER__

#include "src/window.h"

//#define STANDART_WINDOWS_APP

void t1(HWND hwnd, HANDLE id);
void calcProc(HWND hwnd, HANDLE button);
void aboutWindow(HWND hWnd, HANDLE button);
CALLBACK mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
CALLBACK aboutWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int main(void)
{
	char i;
	HWND hMain;
#ifdef STANDART_WINDOWS_APP
	MSG msg;

	InitVideo();
#endif

	InitWindowList();

	hMain = CreateWindow("Calculator", 33, 3, 19, 13, 10, 10, 10);

	CreateWindowMenu(hMain, "&File", NULL);

	ChangeDefWindowFunc(hMain, mainWindowProc);

	AddButton(hMain, WINDOW_BUTTON, " 0 ", 1|(7<<4), 1, 9, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 1 ", 1|(7<<4), 1, 7, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 2 ", 1|(7<<4), 5, 7, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 3 ", 1|(7<<4), 9, 7, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 4 ", 1|(7<<4), 1, 5, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 5 ", 1|(7<<4), 5, 5, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 6 ", 1|(7<<4), 9, 5, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 7 ", 1|(7<<4), 1, 3, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 8 ", 1|(7<<4), 5, 3, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " 9 ", 1|(7<<4), 9, 3, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " + ", 1|(7<<4), 5, 9, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " - ", 1|(7<<4), 9, 9, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " * ", 1|(7<<4), 13, 7, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " / ", 1|(7<<4), 13, 9, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " = ", 1|(7<<4), 13, 5, calcProc);
	AddButton(hMain, WINDOW_BUTTON, " C ", 1|(7<<4), 13, 3, calcProc);

	// Enable '>...<'
	for(i=0;i<16;i++) {
#ifdef __TAB_ORDER__
	  tabOrderAdd(hMain, WINDOW_BUTTON, i);
#endif
	  SetButtonFlag(hMain, WINDOW_BUTTON, i, 8 | 1);
	}

#ifdef __TAB_ORDER__
	tabOrderSetFocus(hMain, WINDOW_BUTTON, 0);
#else
	SetFocusFor(hMain, WINDOW_BUTTON, 0);
#endif

	AddLabel(hMain, "              ", 11|(1<<4), 2, 1);

#ifdef STANDART_WINDOWS_APP
	UpdateWindow();

	while(GetMessage(&msg, hMain, NULL, NULL))
	  DispachMessage(&msg);

	FreeWindowList();
	FreeMemory();
#else
	WindowManager();
#endif

	return 0;
}

CALLBACK mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPWINDOW lpWindow = GetPtrToWindow(hwnd);

	switch(message) {
	case MSG_KB_PRESS_KEY:
		switch(wParam) {
		case '.':
		case ',': break;
		case '+': wParam = 10; break;
		case '-': wParam = 11; break;
		case '*': wParam = 12; break;
		case '/': wParam = 13; break;
		case '=': wParam = 14; break;
		case  83: wParam = 15; break;
		case 'F':
		case 'f': {
#ifdef STANDART_WINDOWS_APP
			MSG msg;
			HWND hMenu;

			hMenu = PopUpMenu(hwnd, "&About me&Exit", lpWindow->X+2, lpWindow->Y+2, NULL);
			while(GetMessage(&msg, hMenu, NULL, NULL)) DispachMessage(&msg);
#else
			PopUpMenu(hwnd, "&About me&Exit", lpWindow->X+2, lpWindow->Y+2, NULL);
#endif
		 }
		 return;

		default:
			if(wParam >= '0' && wParam <= '9') wParam -= '0';
			else return;
		}

		SendMessage(hwnd, MSG_SELECT_BUTTON, wParam, 0);
	 return;

	case MSG_WINDOW_MENU_SELECT_BUTTON:
		switch(wParam) {
		case 0: {
#ifdef STANDART_WINDOWS_APP
			MSG msg;
			HWND hMenu;

			while(mouse.button == 1) Mouse
			hMenu = PopUpMenu(hwnd, "&About me&Exit", lpWindow->X+2, lpWindow->Y+2, NULL);

			while(GetMessage(&msg, hMenu, NULL, NULL))
			  DispachMessage(&msg);
#else
			while(mouse.button == 1) Mouse
			PopUpMenu(hwnd, "&About me&Exit", lpWindow->X+2, lpWindow->Y+2, NULL);
#endif
		 }
		 return;
		}
	 break;

	case MSG_POP_UP_MENU_SELECT_BUTTON:
		switch(wParam) {
		case 0: aboutWindow(hwnd, wParam); break;
		case 1: PostQuitMsg(hwnd);
		}
	 break;

	case MSG_RESIZE_WINDOW: break;
	default: DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void aboutWindow(HWND hWnd, HANDLE button)
{
	HWND hAbout;
	WINDOW *temp;
#ifdef STANDART_WINDOWS_APP
	MSG msg;
#endif

	button |= button;
	hAbout = CreateWindow("About me", 33, 10, 28, 8, 3, 5, 7);

	temp = GetPtrToWindow(hAbout);
	temp->WindowFunc = aboutWindowProc;

	temp->flag &= 2^255;
	SetParentWindow(hAbout, hWnd);

	AddButton(hAbout, WINDOW_BUTTON, "Close", 15, 7, 4, NULL);

	UpdateWindow();
#ifdef STANDART_WINDOWS_APP
	while(GetMessage(&msg, hAbout, NULL, NULL))
	  DispachMessage(&msg);

	KillFocus(hWnd, 3);
#endif
}

CALLBACK aboutWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case MSG_SET_ON_TOP: break;
	case MSG_PAINT: PutStr2WindowXY(hwnd, (UCHAR*) "By The^Day^Of^DooM", 13|128, 3, 2); break;

	case MSG_SYSTEM_PRESS_KEY: if(27 == wParam) PostQuitMsg(hwnd); break;
	case MSG_KB_PRESS_KEY: if(27 == wParam) PostQuitMsg(hwnd); break;
	case MSG_SELECT_BUTTON: if(!wParam) PostQuitMsg(hwnd);
		while(mouse.button == 1) Mouse
	 break;
	case MSG_QUIT: SetFocusForParentWindow(hwnd);
	default: DefWindowProc(hwnd, message, wParam, lParam);
	}
}

void calcProc(HWND hwnd, HANDLE button)
{
	UCHAR i;
	LABEL *label;
	static UCHAR flag = 0, math = 0;
	static char OP0[14], OP1[14], counter = 13;

	label = GetPtrToLabel(hwnd, 0);

	if(flag == 3 || 15 == button) {
	  counter = 13;
	  flag = math = 0;

	  for(i=0;i<14;i++) {
		OP0[i] = OP1[i] = 0;
		label->text[i] = 0x20;
	  }
	}

	switch(button) {
	case 10: math = '+';	flag = 1; break;
	case 12: math = '*'; flag = 1; break;
	case 13: math = '/'; flag = 1; break;
	case 14: flag = 2; break;
	case 15:
	case 16: break;
	default:
		if(11 == button && 13 != counter) {
		  math = '-'; flag = 1;
		} else {

			if( (int)counter>-1) {
				char n;

				flag = 0;

				for(n=13; n; n--) {
				  if( ('.' == button || ',' == button || '-' == button) &&
						('.' == label->text[n] || ',' == label->text[n])
				  ) return;
				}

				for(i = 0; i < 14 - counter; i++)
				  label->text[i+counter] = label->text[i+counter+1];

				switch(button) {
				case '.':
				case ',': break;
				case  11: button = '-'; break;
				default: button += 48;
				}

				label->text[13] = button;

				--counter;
			}

		}
	}

	switch(flag) {
	case 1: flag = 0;

		for(i=0;i < 13 - counter; i++) {
			OP0[i] = label->text[i+counter+1];
			label->text[i+counter+1] = 0x20;
		}

		OP0[++i] = 0;
		counter = 13;
	 break;

	case 2: { float x, y;
		char buffer[14];

		flag = 3;

		for(i=0;i < 13 - counter; i++)
		  OP1[i] = label->text[i+counter+1];

		OP1[++i] = 0;

		x = atof(OP0);
		y = atof(OP1);

		switch(math) {
		case '+': x+=y; break;
		case '-': x-=y; break;
		case '*': x*=y; break;
		case '/': x/=y; break;
		default: return;
		}

		sprintf(buffer, "%7.5f", x);

		for(i = 0; i < 13 - strlen(buffer); i++)
		  label->text[i] = 0x20;

		label->text[++i] = 0;

		strcat((char*)label->text, (char*)buffer);
	 }
	}

	UpdateWindow();
}