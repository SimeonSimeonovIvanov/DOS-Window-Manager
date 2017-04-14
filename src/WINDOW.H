/*
		The^day^of^DooM

		Borland C++ 3.1
			  &&
		  Open Watcom

	Create Date:	XX.XX.2005
	Full Change:    12.02.2007
	Last Update:    20.02.2007
	Full Change:    20.02.2007
	Last Update:    09.12.2007
	Full Change:    26.12.2007
	Last Update:    31.12.2007
	Last Update:	22.02.2008
	Full Change:    22.03.2008
	Last Update:	04.05.2009
*/

#ifndef __WINDOW_H__
	#define __WINDOW_H__

#ifndef __TYPE_H__
	#include "src/type.h"
#endif

#ifndef __LIST_H_
	#include "src/list.h"
#endif

#ifndef __MOUSE_H__
	#include "src/mouse.h"
#endif

#ifdef __CHAR_MAP__
	#ifndef __CHAR_MAP_H__
		#include "src/charmap.h"
	#endif
#endif

#ifdef __TIMER__
	#ifndef __TIMER_H__
		#include "src/timer.h"
	#endif
#endif

#ifdef __LABEL__
	#ifndef __LABEL_H__
		#include "src/label.h"
	#endif
#endif

#ifdef __BUTTON__
	#ifndef __BUTTON_H__
		#include "src/button.h"
	#endif
#endif

#ifdef __EDIT_BOX__
	#ifndef __EDIT_BOX_H__
		#include "src/editbox.h"
	#endif
#endif

#ifdef __MENU__
	#ifndef __MENU_H__
		#include "src/menu.h"
	#endif
#endif

#ifdef __TAB_ORDER__
	#ifndef __TAB_ORDER_H__
		#include "src/taborder.h"
	#endif
#endif

//////////////////////////////////////////////////////////////////////////////
#define SetColor(color) { Color=(UCHAR)color; }
#define GoTo(x,y) { xScr=(UCHAR)x;  yScr=(UCHAR)y; }

#define WaitForVerticalRetrace {		\
	while((inportb(0x3DA) & 0x08));		\
	while(!(inportb(0x3DA) & 0x08));	\
}

#define LOWORD(dword)(0x00ff & dword)
#define HIWORD(dword)(0x00ff & dword>>8)

#define InitWindowList() DefaultWindowList = NULL;

#define GetWindowPosX(hwnd)		GetWindowPosXorY((HWND)hwnd, 0)
#define GetWindowPosY(hwnd)		GetWindowPosXorY((HWND)hwnd, 1)

#define PutCharXY(ch, color, x, y) {				       			\
	UCHAR far *video_memory=(UCHAR far *) 0xB8000000;				\
	*(video_memory+(((UCHAR)x*2)+((UCHAR)y*160)))=ch;	       		\
	*(video_memory+((((UCHAR)x*2)+((UCHAR)y*160))+1))=(UCHAR)color;	\
}

#define UpdateVideoMemory() {					    				\
	HideMouse														\
	memcpy((int far *) 0xB8000000, VideoBuffer, 2000*sizeof(int));	\
	ShowMouse														\
}

#define RestoreBackGround()		memcpy(VideoBuffer, BackGroundBuffer, 2000*sizeof(int));

#define SaveBackGround()												\
{																		\
	HideMouse							    							\
	memcpy(BackGroundBuffer, (int far *)0xB8000000, 2000*sizeof(int));	\
	ShowMouse															\
};
//////////////////////////////////////////////////////////////////////////////
LPWINDOW DefaultWindowList;
UCHAR xScr, yScr, Color;
UINT *VideoBuffer, *BackGroundBuffer;
//////////////////////////////////////////////////////////////////////////////

char SetFocusForParentWindow(HWND hChild);

void IORead(void);
void InitVideo(void);
void FreeMemory(void);
void TimerManager(void);

char GetChar(void);
char GetColor(void);
void PutChar(char ch);
void PutStr(char *str);

void PutCharToBuffer(UINT *buffer, UCHAR ch, UCHAR color, char x, char y);
void PutStrToBuffer(UINT *buffer, UCHAR *str, UCHAR color, char x, char y);

void _freeList(void **root, UCHAR type);

UCHAR DrawWindow(HWND hWnd);
UCHAR RemoveWindow(HWND hWnd);
void WindowSetOnTop(HWND hWnd);
void CheckWindowSize(HWND hWnd);
void windowSetFlag(HWND hWnd, UINT newFlag);
void MoveWindow(HWND hWnd, char xNew, char yNew);
void PutStr2WindowXY(HWND hWnd, UCHAR *str, UCHAR color, char x, char y);
UCHAR ChangeDefWindowFunc(HWND hwnd, CALLBACK (*NewWindowFunc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam));
//////////////////////////////////////////////////////////////////////////////

void WindowManager(void) // +
{
	UINT msg;
	MSG_LIST *temp;
	LPWINDOW current;

	InitVideo();
	UpdateWindow();

	current = DefaultWindowList;
	while(current) {
	  msg = 0;

	  IORead();

#ifdef __TIMER__
	  TimerManager();
#endif

	  if(current->MsgList) {
		msg = current->MsgList->lpMsg->message;
		current->WindowFunc(current->hWnd, msg, current->MsgList->lpMsg->wParam, current->MsgList->lpMsg->lParam);

		if(msg != MSG_QUIT) {
		  temp = current->MsgList->next;

		  free(current->MsgList->lpMsg);
		  free(current->MsgList);
		  current->MsgList = temp;
		}
	  }

	  if(msg != MSG_QUIT) {
		if(NULL == (current = current->next))
		  current = DefaultWindowList;
	  } else current = DefaultWindowList;
	}

	FreeMemory();
}

#ifdef __TIMER__
static void TimerManager(void)
{
	LPLIST lpList;
	LPWINDOW current_window_timer;

	current_window_timer = DefaultWindowList;
	while(current_window_timer) {

	  lpList = current_window_timer->list_timer;
	  while(lpList) {

		if(1 == TimerStatus(current_window_timer->hWnd, lpList->id)) {
		  if(DecTimer(current_window_timer->hWnd, lpList->id)) {
			current_window_timer->WindowFunc(current_window_timer->hWnd, MSG_WINDOW_TIMER, lpList->id, 0);
			//SendMessage(current_window_timer->hWnd, MSG_WINDOW_TIMER, lpTimer->id, NULL);
			ResetTimer(current_window_timer->hWnd, lpList->id);
		  }
		}

		lpList = lpList->next;
	  }

	  current_window_timer = current_window_timer->next;
	}
}
#endif

HWND CreateWindow( char *Title,char x,char y,char size_x,
				   char size_y, char border_color, char window_color,
				   char title_color
				 ) // +
{
	HWND hWnd;
	LPWINDOW temp;

	if(NULL == (temp = (WINDOW *) malloc(sizeof(WINDOW)))) return -1;
	if(NULL == (temp->Title = (char *) malloc(strlen(Title)+1))) return -1;

	strcpy(temp->Title, Title);

	if(size_y<2)
		size_y=2;
	if(size_x<=strlen(Title))
		size_x=strlen(Title)+4;

	if(size_y>25) size_y=25;
	if(size_x>80) size_x=80;

	temp->hWnd = 0;

	temp->next = NULL;
	temp->previous = NULL;
	temp->MsgList = NULL;

#ifdef __CHAR_MAP__
	temp->list_char_map = NULL;
#endif

#ifdef __TIMER__
	temp->list_timer = NULL;
#endif

#ifdef __LABEL__
	temp->list_label = NULL;
#endif

#ifdef __BUTTON__
	temp->list_button = NULL;
#endif

#ifdef __MENU__
	temp->list_menu_button = NULL;
#endif

#ifdef __EDIT_BOX__
	temp->list_edit_box = NULL;
#endif

#ifdef __TAB_ORDER__
	temp->list_tab_order = NULL;
#endif

	temp->WindowFunc = DefWindowProc;

	temp->flag = 128 | 4 | 2 | 1;

	temp->title_color = title_color;
	temp->border_color = border_color;
	temp->window_color = window_color;

	temp->X = x;  temp->SIZE_X = size_x;
	temp->Y = y;  temp->SIZE_Y = size_y;

	if( 0 <= (hWnd = add2List((void **)&DefaultWindowList, LIST_WINDOW, (void *)temp)))
	  SendMessage(hWnd, MSG_WINDOW_FIRST_RUN, 0, 0);

	return hWnd;
}

HANDLE lpWindowFindSelectObjInList(LPWINDOW lpWindow, LPLIST lpList, UCHAR(*isSelectObj)(LPLIST lpObj, UCHAR x, UCHAR y)) // +
{
	UCHAR x, y;

	x = mouse.x/8 - lpWindow->X;
	y = mouse.y/8 - lpWindow->Y;

	while(lpList) {
	  if(isSelectObj(lpList, x, y)) return lpList->id;
		lpList = lpList->next;
	}

	return -1;
}

void MouseHandler1(void) // .................
{
	char mX, mY;
	LPWINDOW current;

	if(NULL == DefaultWindowList) return;

	Mouse
	mX = mouse.x/8; mY = mouse.y/8;

	current = DefaultWindowList;
	while(current) {
	   current->WindowFunc(current->hWnd, MSG_MOUSE_CLICK, (mY<<8) | mX, mouse.button);
	   current = current->next;
	}
}

void MouseHandler(void)
{
	char mX, mY;
	LPWINDOW current;

	if(NULL == DefaultWindowList) return;

	Mouse
	mX = mouse.x/8; mY = mouse.y/8;

	switch(mouse.button) {
	case 1: current = DefaultWindowList;

		while(current) {
		  if( 128 & current->flag && (mX < current->X || mX > current->X +current->SIZE_X
			  || mY < current->Y || mY > current->Y + current->SIZE_Y)
		  ) current->WindowFunc(current->hWnd, MSG_MOUSE_CLICK, (mY<<8) | mX, mouse.button);

		  current = current->next;
		}

		current = DefaultWindowList;
		while(current->next) current = current->next;

		while(current) {
		  if( mX >= current->X &&
			  mX < current->X +current->SIZE_X &&
			  mY >= current->Y && mY < current->Y + current->SIZE_Y
		  ) {
			if(128 & current->flag) {
			  HANDLE id;

			  /*char i = 0;
			  MSG_LIST *lpList = current->MsgList;
			  while(lpList) {
				++i;
				lpList = lpList->next;
			  }
			  if(i>20) return;*/

			  if(current->MsgList) return;

			  SendMessage(current->hWnd, MSG_IS_SELECT, 0, 0);

			  if(NULL != current->next)
				SendMessage(current->hWnd, MSG_SET_ON_TOP, 0, 0);

			  if(!(4 & current->flag))
				SendMessage(current->hWnd, MSG_SET_FOCUS, 0, 0);

#ifdef __MENU__
			  if(-1 != (id = lpWindowFindSelectObjInList(current, current->list_menu_button, isSelectButton))) {
				SendMessage(current->hWnd, MSG_WINDOW_MENU_SELECT_BUTTON, id, 0);
				return;
			  }
#endif

			  if( mX > current->X &&
				  mX < current->X +current->SIZE_X-1 &&
				  mY > current->Y && mY < current->Y + current->SIZE_Y-1
			  ) {
#ifdef __BUTTON__
			  if(-1 != (id = lpWindowFindSelectObjInList(current, current->list_button, isSelectButton))) {
				SendMessage(current->hWnd, MSG_SELECT_BUTTON, id, 0);
				return;
			  }
#endif

#ifdef __EDIT_BOX__
			  if(-1 != (id = lpWindowFindSelectObjInList(current, current->list_edit_box, isSelectEditBox))) {
				SendMessage(current->hWnd, MSG_SELECT_EDIT_BOX, id, 0);
				return;
			  }
#endif
			  }

			  { // Move || Resize || Close
			  UINT message = 0;
			  WPARAM wParam = 0;

			  if(mY == current->Y) {
				if(2 & current->flag && mX == (current->X + current->SIZE_X) - 2) {
				  SendMessage(current->hWnd, MSG_CLOSE_BUTTON, 0, 0);
				  return;
				}

				message = MSG_RESIZE_WINDOW;

				if(mX == current->X) wParam = 0;
				else if(mX+1 == (current->X + current->SIZE_X)) wParam = 1;
					 else message = MSG_MOVE_WINDOW;
			  } else {
				if(mY+1 == current->Y + current->SIZE_Y) {
				  wParam = 4;
				  message = MSG_RESIZE_WINDOW;

				  if(mX == current->X) wParam = 3;
				  else if(mX+1 == (current->X + current->SIZE_X)) wParam = 2;
					   else message = MSG_IS_SELECT;
				}
			  }
			  SendMessage(current->hWnd, message, wParam, 0);
			  }
			}
			return;
		  }
		  current = current->previous;
		} // while(current)...

	 break;
	case 2:
	 break;
	case 3:
	 break;
	}
}

UCHAR SetFocusFor(HWND hWnd, UCHAR type, HANDLE id) // +
{
	LPWINDOW lpWindow;
	LPLIST lpList = NULL, lpTemp;

	id|=0;	type|=0;

	if(NULL == (lpWindow = (LPWINDOW)GetPtrToWindow(hWnd))) return 0;

	switch(type) {
#ifdef __BUTTON__
	case WINDOW_BUTTON: lpList = lpWindow->list_button; break;
#endif

#ifdef __EDIT_BOX__
	case WINDOW_EDIT_BOX: lpList = lpWindow->list_edit_box; break;
#endif
	case -1: return 0;
	default: return 0;
	}

#ifdef __BUTTON__
	lpTemp = lpWindow->list_button;
	while(lpTemp) {
		((LPBUTTON)lpTemp->lpData)->flag &= 4^255;
		((LPBUTTON)lpTemp->lpData)->text_color &= 127;
		lpTemp = lpTemp->next;
	}
#endif

#ifdef __EDIT_BOX__
	lpTemp = lpWindow->list_edit_box;
	while(lpTemp) {
		((LPEDIT_BOX)lpTemp->lpData)->flag &= 4^255;
		lpTemp = lpTemp->next;
	}
#endif

	while(lpList) {
		if(lpList->id == id) {
			switch(type) {
			case WINDOW_BUTTON: ((LPBUTTON)lpList->lpData)->flag|=4;
				((LPBUTTON)lpList->lpData)->text_color |= 128;
			 break;
			case WINDOW_EDIT_BOX: ((LPEDIT_BOX)lpList->lpData)->flag|=4;
			}

			return 1;
		}

		lpList = lpList->next;
	}

	return 0;
}

/*void KillFocusFor(HWND hwnd, UCHAR type, HANDLE id)
{
	id|=0; 	type|=0;	hwnd|=0;
}*/

UCHAR GetControlInFocus(HWND hWnd, UCHAR *type, HANDLE *id) // +
{
	LPLIST lpList;
	LPWINDOW lpWindow;

	if(NULL == (lpWindow = (LPWINDOW)GetPtrToWindow(hWnd))) return 0;

#ifdef __BUTTON__
	lpList = lpWindow->list_button;
	while(lpList) {
		if(4 & ((LPBUTTON)lpList->lpData)->flag) {
			*id = lpList->id;
			*type = WINDOW_BUTTON;
			return 1;
		}

		lpList = lpList->next;
	}
#endif

#ifdef __EDIT_BOX__
	lpList = lpWindow->list_edit_box;
	while(lpList) {
		if(4 & ((LPEDIT_BOX)lpList->lpData)->flag) {
			*id = lpList->id;
			*type = WINDOW_EDIT_BOX;
			return 1;
		}

		lpList = lpList->next;
	}
#endif
	return 0;
}

CALLBACK DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPWINDOW current;

	current = GetPtrToWindow(hWnd);

	lParam |= 0;

	switch(message) {
	case MSG_PAINT:
	case MSG_IS_SELECT:
	case MSG_KB_PRESS_KEY:
	case MSG_KB_LKEY:
	case MSG_KB_RKEY:
	case MSG_KB_UKEY:
	case MSG_KB_DKEY:
	case MSG_USER_DEFINED_MESSAGE:
	case MSG_MOUSE_CLICK_ON_WINDOW: return;
	case MSG_QUIT: SetFocusForParentWindow(hWnd); RemoveWindow(hWnd); return;
	case MSG_CLOSE_BUTTON: PostQuitMsg(current->hWnd);
		while(1 == mouse.button) Mouse
	 return;

	case MSG_WINDOW_FIRST_RUN: break;
	case MSG_SET_FOCUS: KillFocus(hWnd, 3); break;
	/*
	case MSG_MOUSE_CLICK: { char mX = mouse.x/8, mY = mouse.y/8;

		  if( mX >= current->X &&
			  mX < current->X +current->SIZE_X &&
			  mY >= current->Y && mY < current->Y + current->SIZE_Y
		  ) ;//SendMessage(hWnd, CLICK_WINDOW, wParam, lParam);

	}
	 return;

	case CLICK_WINDOW: { char mX = mouse.x/8, mY = mouse.y/8;


	 }
	 return;
	*/
	case MSG_SYSTEM_PRESS_KEY: {
		HANDLE id;
		UCHAR type;

#ifdef __TAB_ORDER__
		if(wParam == '\t') {
			tabOrderProcess(hWnd);
			UpdateWindow();
			return;
		}
#endif

		if(GetControlInFocus(hWnd, &type, &id)) {
			switch(type) {
			case WINDOW_BUTTON:
				if(13 == wParam) //current->WindowFunc(hWnd, MSG_SELECT_BUTTON, id, 0);
					SendMessage(hWnd, MSG_SELECT_BUTTON, id, 0);
			 break;
#ifdef __EDIT_BOX__
			case WINDOW_EDIT_BOX: //current->WindowFunc(hWnd, MSG_EDIT_BOX_PUTC, wParam, id);
				SendMessage(hWnd, MSG_EDIT_BOX_PUTC, wParam, id);
#endif
			}
		} else;
		SendMessage(hWnd, MSG_KB_PRESS_KEY, wParam, 0);
	 } return;

#ifdef __EDIT_BOX__
	case MSG_SELECT_EDIT_BOX:
#ifdef __TAB_ORDER__
		if(!tabOrderSetFocus(hWnd, WINDOW_EDIT_BOX, wParam))
#endif
			if(!SetFocusFor(hWnd, WINDOW_EDIT_BOX, wParam)) return;
	 break;

	case MSG_EDIT_BOX_PUTC:
		windowPutcToEditBox(hWnd, (HANDLE)lParam, wParam);
	 return;
#endif

	case MSG_SET_ON_TOP: WindowSetOnTop(current->hWnd); return;

	case MSG_DRAW_WINDOW: DrawWindow(hWnd); return;

	case MSG_WINDOW_TIMER: {
#ifdef __TIMER__
		LPTIMER lpTimer;
		if(NULL != (lpTimer = GetPtrToTimer(hWnd, wParam)))
		  if(lpTimer->DefTimerFunc)
			lpTimer->DefTimerFunc(hWnd, wParam);
#endif
	 }
	 return;

	case MSG_SELECT_BUTTON: {
#ifdef __BUTTON__
		char mX, mY;
		UCHAR flagMouse = 0;
		LPBUTTON button = GetPtrToButton(hWnd, WINDOW_BUTTON, wParam);

#ifdef __TAB_ORDER__
		if(!tabOrderSetFocus(hWnd, WINDOW_BUTTON, wParam))
#endif
			if(!SetFocusFor(hWnd, WINDOW_BUTTON, wParam))
				return;

		button->text_color |= 128;
		UpdateWindow();

		while(1 == mouse.button) {
		  Mouse
		  flagMouse = 1;
#ifdef __TIMER__
		  TimerManager();
#endif
		}

		if(flagMouse) {
			button->text_color &= 127;
			UpdateWindow();
		}

		mX = mouse.x/8;	mY = mouse.y/8;
		if(!flagMouse || (mY == button->y+current->Y+1 && mX > button->x + current->X && mX < button->x + current->X + strlen((char*)button->text) + 1))
			SendMessage(hWnd, MSG_PRESS_BUTTON, wParam, 0);
#endif
	 } return;

	case MSG_PRESS_BUTTON: {
#ifdef __BUTTON__
		LPBUTTON lpButton = GetPtrToButton(hWnd, WINDOW_BUTTON, wParam);

		if(lpButton && lpButton->DefButtonFunc)
			lpButton->DefButtonFunc(current->hWnd, wParam);
#endif
	} return;

	case MSG_WINDOW_MENU_SELECT_BUTTON: {
#ifdef __MENU__
		LPBUTTON button = GetPtrToButton(hWnd, WINDOW_MENU_BUTTON, wParam);

		SelectButton(hWnd, WINDOW_MENU_BUTTON, wParam);

		button->text_color |= 128;
		UpdateWindow();

		while(1 == mouse.button) {
		  Mouse
#ifdef __TIMER__
		  TimerManager();
#endif
		}

		button->text_color &= 128^255;
		UpdateWindow();

		if(button->DefButtonFunc) {
		  char mX, mY;

		  mX = mouse.x/8;       mY = mouse.y/8;
		  if(mY == button->y+current->Y+1 && mX > button->x + current->X && mX < button->x + current->X + strlen((char*)button->text) + 1)
			 button->DefButtonFunc(current->hWnd, wParam);
		}
#endif
	}
	 return;

	case MSG_MOVE_WINDOW: { int xOld = mouse.x, yOld = mouse.y;

		while(1 == mouse.button) {
		  Mouse

		  if(xOld != mouse.x || yOld != mouse.y) {
			MoveWindow(current->hWnd, mouse.x/8 - current->SIZE_X/2, mouse.y/8);
			xOld = mouse.x; yOld = mouse.y;
			UpdateWindow();
		  }
#ifdef __TIMER__
		  TimerManager();
#endif
		}
	 }
	 return;

	case MSG_RESIZE_WINDOW: { char mX, mY;
		int xOld = mouse.x, yOld = mouse.y;

		while(1 == mouse.button) {
		  Mouse

		  if(xOld != mouse.x || yOld != mouse.y) {
			mX = mouse.x/8; mY = mouse.y/8;

			if(0 == wParam || 3 == wParam) {
			  if(mX < current->X) {
				current->SIZE_X += current->X - mX;
				current->X = mX;
			  } else {
				 if(mX < (current->X+current->SIZE_X) - (strlen(current->Title)+4)) {
					current->SIZE_X -= mX - current->X;

					current->X = mX;
				 }
			  }
			}

			if(1 == wParam || 2 == wParam) {
			  if(mX < current->X + current->SIZE_X) {
				if(mX > current->X + (strlen(current->Title)+2))
				  current->SIZE_X -= current->X + current->SIZE_X - (mX+1);
			  } else
				 current->SIZE_X += mX+1 - (current->X + current->SIZE_X);
			}

			if(0 == wParam ||  1 == wParam) {
			  if(mY < current->Y) {
				current->SIZE_Y += (current->Y - mY);
				current->Y = mY;
			  } else {
				 if((current->Y + current->SIZE_Y)-1 > mY) {
					current->SIZE_Y -= mY - current->Y;
					current->Y = mY;
				 }
			  }
			}

			if(4 == wParam || 2 == wParam ||  3 == wParam) {
			  if(mY+1 < current->Y + current->SIZE_Y) {
			  if(mY > current->Y)
			  current->SIZE_Y -= (current->Y + current->SIZE_Y) - (mY+1);
			} else
			  current->SIZE_Y += mY+1 - (current->Y + current->SIZE_Y);
			}

			xOld = mouse.x; yOld = mouse.y;
			CheckWindowSize(current->hWnd);
			UpdateWindow();
		  }
#ifdef __TIMER__
		  TimerManager();
#endif
		}
	 }
	 return; // MSG_RESIZE_WINDOW
	}

	UpdateWindow();
}

void CheckWindowSize(HWND hWnd) // +
{
	LPWINDOW Window;

	if(NULL != (Window = GetPtrToWindow(hWnd))) {
		if(Window->SIZE_Y<2) Window->SIZE_Y = 2;

		if(Window->SIZE_X < strlen(Window->Title) + 5)
			Window->SIZE_X = strlen(Window->Title) + 5;

		if(Window->SIZE_Y > 25) Window->SIZE_Y = 25;
			if(Window->SIZE_X > 80) Window->SIZE_X = 80;
	}
}

UCHAR GetWindowPos(HWND hwnd, int *x, int *y)
{
	LPWINDOW Window;

	if(NULL != (Window = GetPtrToWindow(hwnd))) {
		(*x) = Window->X;	(*y) = Window->Y;
		return 0;
	}

	return 1;
}

int GetWindowPosXorY(HWND hwnd, UCHAR pos)
{
	int x, y;

	if(GetWindowPos(hwnd, &x, &y)) return -1000;

	if(!pos) return x;
	else return y;

}

void CheckWindowPosition(HWND hWnd) // +
{
	LPWINDOW Window;

	if(NULL != (Window = GetPtrToWindow(hWnd))) {
		if((int)Window->X < 0) Window->X=0;
		else if(Window->X+Window->SIZE_X>80) Window->X=80-Window->SIZE_X;

		if((int)Window->Y < 0) Window->Y=0;
		else if(Window->Y+Window->SIZE_Y>25) Window->Y=25-Window->SIZE_Y;
	}
}

void FreeWindowList(void) // +
{
	LPWINDOW last;

	do {
		last = DefaultWindowList;
		while(last) last = last->next;
		RemoveWindow(last->hWnd);
	} while(DefaultWindowList);
	RestoreBackGround();
}

UCHAR RemoveWindow(HWND hWnd) // +
{
	LPWINDOW current;

	if(NULL == DefaultWindowList) return 1;
	if(NULL == (current = GetPtrToWindow(hWnd))) return 1;

	if(DefaultWindowList == current) { // Remove Root Window.
	  if(NULL == current->next) { // Root Is Only Window.
		 DefaultWindowList = NULL;
	  } else {
		  current->next->previous = NULL;
		  DefaultWindowList = current->next;
	  }
	} else {
		if(NULL == current->next) { // Remove Last Window.
		  current->previous->next = NULL;
		} else {
			current->previous->next = current->next;
			current->next->previous = current->previous;
		}
	}

	_freeList((void **)&current->MsgList, LIST_WINDOW_MSG);

#ifdef __CHAR_MAP__
	_freeList((void **)&current->list_char_map, WINDOW_LIST_CHAR_MAP);
#endif

#ifdef __TIMER__
	_freeList((void **)&current->list_timer, WINDOW_LIST_TIMER);
#endif

#ifdef __LABEL__
	_freeList((void **)&current->list_label, WINDOW_LIST_LABEL);
#endif

#ifdef __EDIT_BOX__
	_freeList((void **)&current->list_edit_box, WINDOW_LIST_EDIT_BOX);
#endif

#ifdef __MENU__
	_freeList((void **)&current->list_button, WINDOW_LIST_BUTTON);
	_freeList((void **)&current->list_menu_button, WINDOW_LIST_MENU_BUTTON);
#else
  #ifdef __BUTTON__
	_freeList((void **)&current->list_button, WINDOW_LIST_BUTTON);
  #endif
#endif

#ifdef __TAB_ORDER__
	_freeList((void **)&current->list_tab_order, WINDOW_LIST_TAB_ORDER);
#endif

	free(current->Title);
	free(current);

	UpdateWindow();
	return 0;
}

void OpenWindow(HWND hWnd) // +
{
	LPWINDOW temp;

	if(NULL != (temp = GetPtrToWindow(hWnd))) temp->flag |= 1;
}

void CloseWindow(HWND hWnd) // +
{
	LPWINDOW temp;

	if(NULL != (temp = GetPtrToWindow(hWnd))) temp->flag &= 1^255;
}

void MoveWindow(HWND hWnd, char xNew, char yNew) // +
{
	LPWINDOW temp;
	if(NULL != (temp = GetPtrToWindow(hWnd))) {
	  temp->X = xNew;
	  temp->Y = yNew;
	}
}

UCHAR DrawWindow(HWND hWnd) // +
{
	LPLIST lpList;
	WINDOW *Window;
	register char i,j;

	if(NULL != (Window = GetPtrToWindow(hWnd))) {
	  if(1 & Window->flag) {
		 UCHAR border_color = Window->border_color;

		 for(i=Window->X+1;i<Window->X+Window->SIZE_X-1;i++)
			for(j=Window->Y+1;j<Window->Y+Window->SIZE_Y-1;j++)
			  PutCharToBuffer(VideoBuffer, 219, Window->window_color, i, j);

		 PutCharToBuffer(VideoBuffer, 201, 7, Window->X, Window->Y);
		 PutCharToBuffer(VideoBuffer, 187, 7, Window->X+Window->SIZE_X-1, Window->Y);

		 if(Window->flag & 4) border_color |= 64;
		 for(i=Window->X+1;i<Window->X+Window->SIZE_X-1;i++) {
			PutCharToBuffer(VideoBuffer, 205, border_color, i, Window->Y);
			PutCharToBuffer(VideoBuffer, 205, Window->border_color, i, Window->Y+Window->SIZE_Y-1);
		 }

		 PutCharToBuffer(VideoBuffer, 200, 7, Window->X,          Window->Y+Window->SIZE_Y-1);
		 PutCharToBuffer(VideoBuffer, 188, 7, Window->X+Window->SIZE_X-1, Window->Y+Window->SIZE_Y-1);

		 for(i=Window->Y+1;i<Window->Y+Window->SIZE_Y-1;i++) {
			PutCharToBuffer(VideoBuffer, 186, Window->border_color, Window->X,i);
			PutCharToBuffer(VideoBuffer, 186, Window->border_color, Window->X+Window->SIZE_X-1,i);
		 }

		 PutStrToBuffer(VideoBuffer, (UCHAR*)Window->Title, Window->title_color, Window->X+2,Window->Y);

		 if(2 & Window->flag)
			PutCharToBuffer(VideoBuffer, 'X', 10, (Window->X + Window->SIZE_X) - 2, Window->Y);

#ifdef __CHAR_MAP__
		lpList = Window->list_char_map;
		while(lpList) {
		   UCHAR i, j;

		   for(i=0;i< ((CHAR_MAP*)lpList->lpData)->size_x ;i++)
			 for(j=0;j< ((CHAR_MAP*)lpList->lpData)->size_y;j++)
			   PutChar2WindowXY(hWnd,
					((CHAR_MAP*)lpList->lpData)->buffer[j * ((CHAR_MAP*)lpList->lpData)->size_x + i],
					((CHAR_MAP*)lpList->lpData)->buffer[j * ((CHAR_MAP*)lpList->lpData)->size_x + i + ((CHAR_MAP*)lpList->lpData)->size_x * ((CHAR_MAP*)lpList->lpData)->size_y],
					((CHAR_MAP*)lpList->lpData)->x + i, ((CHAR_MAP*)lpList->lpData)->y + j
				);

			lpList = lpList->next;
		}
#endif

#ifdef __MENU__
		 lpList = Window->list_menu_button;

		 if(lpList)
		   for(i = 0; i < Window->SIZE_X - 1; i++)
			 PutChar2WindowXY(hWnd, 0x20, 1|(3<<4), i, 0);

		 while(lpList) {
			if(1 & ((LPBUTTON)lpList->lpData)->flag)
			  PutStr2WindowXY(Window->hWnd, (UCHAR*)((LPBUTTON)lpList->lpData)->text, ((LPBUTTON)lpList->lpData)->text_color, ((LPBUTTON)lpList->lpData)->x, ((LPBUTTON)lpList->lpData)->y);

			  if((8 & ((LPBUTTON)lpList->lpData)->flag) && (4 & ((LPBUTTON)lpList->lpData)->flag)) {
				PutChar2WindowXY(hWnd, '>', 15|(3<<4), ((LPBUTTON)lpList->lpData)->x - 1, 0);
				PutChar2WindowXY(hWnd, '<', 15|(3<<4), ((LPBUTTON)lpList->lpData)->x + strlen((char*)((LPBUTTON)lpList->lpData)->text), 0);
			  }

			lpList = lpList->next;
		 }
#endif

#ifdef __EDIT_BOX__
		 lpList = Window->list_edit_box;
		 while(lpList) {
			if(1 & ((LPEDIT_BOX)lpList->lpData)->flag) {
			  PutStr2WindowXY(Window->hWnd, ((LPEDIT_BOX)lpList->lpData)->text, ((LPEDIT_BOX)lpList->lpData)->text_color, ((LPEDIT_BOX)lpList->lpData)->x, ((LPEDIT_BOX)lpList->lpData)->y);

			  for(i=strlen(((LPEDIT_BOX)lpList->lpData)->text); i<((LPEDIT_BOX)lpList->lpData)->lenght; i++)
				 PutChar2WindowXY(Window->hWnd, 255, ((LPEDIT_BOX)lpList->lpData)->text_color, i + ((LPEDIT_BOX)lpList->lpData)->x, ((LPEDIT_BOX)lpList->lpData)->y);

			  if(((LPEDIT_BOX)lpList->lpData)->count < ((LPEDIT_BOX)lpList->lpData)->lenght)
				if(((LPEDIT_BOX)lpList->lpData)->flag & 4)
				  PutChar2WindowXY(Window->hWnd, 22, 128|((LPEDIT_BOX)lpList->lpData)->text_color, ((LPEDIT_BOX)lpList->lpData)->x + ((LPEDIT_BOX)lpList->lpData)->count, ((LPEDIT_BOX)lpList->lpData)->y);
				else PutChar2WindowXY(Window->hWnd, 255, ((LPEDIT_BOX)lpList->lpData)->text_color, ((LPEDIT_BOX)lpList->lpData)->x + ((LPEDIT_BOX)lpList->lpData)->count, ((LPEDIT_BOX)lpList->lpData)->y);
			}

			lpList = lpList->next;
		 }
#endif

#ifdef __BUTTON__
		lpList = Window->list_button;
		while(lpList) {
			if(1 & ((LPBUTTON)lpList->lpData)->flag)
			  PutStr2WindowXY(Window->hWnd, (UCHAR*)((LPBUTTON)lpList->lpData)->text, ((LPBUTTON)lpList->lpData)->text_color, ((LPBUTTON)lpList->lpData)->x, ((LPBUTTON)lpList->lpData)->y);

			if((8 & ((LPBUTTON)lpList->lpData)->flag) && (4 & ((LPBUTTON)lpList->lpData)->flag)) {
			  PutChar2WindowXY(hWnd, '>', 15|(3<<4), ((LPBUTTON)lpList->lpData)->x - 1, ((LPBUTTON)lpList->lpData)->y);
			  PutChar2WindowXY(hWnd, '<', 15|(3<<4), ((LPBUTTON)lpList->lpData)->x + strlen((char*)((LPBUTTON)lpList->lpData)->text), ((LPBUTTON)lpList->lpData)->y);
			}

			lpList = lpList->next;
		}
#endif

#ifdef __LABEL__
		lpList = Window->list_label;
		while(lpList) {
			if(1 & ((LPLABEL)lpList->lpData)->flag)
			  PutStr2WindowXY(Window->hWnd, (UCHAR*)((LPLABEL)lpList->lpData)->text, ((LPLABEL)lpList->lpData)->text_color, ((LPLABEL)lpList->lpData)->x, ((LPLABEL)lpList->lpData)->y);

			lpList = lpList->next;
		}
#endif

	  }

	  return 0;
	}

	return 1;
}

void WindowSetOnTop(HWND hWnd) // +
{
	LPWINDOW current;

	if(NULL != DefaultWindowList) {
	  LPWINDOW temp, last;

	  last = temp = DefaultWindowList;
	  while(last->next) {
		 if(hWnd == last->hWnd) current = last;
		 last = last->next;
	  }

	  if(last == DefaultWindowList || hWnd == last->hWnd) return;

	  //if(NULL == current->next) return;

	  if(hWnd == temp->hWnd) { // Set On Top Root Window.
		 last->next = temp;
		 temp->previous = last;

		 if(last->previous == DefaultWindowList) { // Work With 2 Window.
			last->previous = NULL;
			last->next = temp;
			temp->next = NULL;

			DefaultWindowList = last;
			return;
		 }

		 if(last == temp->next->next) { // Work With 3 Window.
			last->previous = temp->next;
			temp->next->previous = NULL;
		 } else temp->next->previous = NULL;

		 DefaultWindowList = temp->next;
		 temp->next = NULL;

		 return;
	  }

	  current->previous->next = current->next;
	  current->next->previous = current->previous;

	  current->next = NULL;
	  current->previous = last;

	  last->next = current;
	}
}

UCHAR ChangeDefWindowFunc(HWND hwnd, CALLBACK (*NewWindowFunc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)) // +
{
	LPWINDOW current;

	if(NULL == NewWindowFunc) return 1;
	if(NULL == (current = GetPtrToWindow(hwnd))) return 2;
	current->WindowFunc = NewWindowFunc;
	return 0;
}

void UpdateWindow(void) // +
{
	LPWINDOW lpCurrent;

	RestoreBackGround();
	lpCurrent = DefaultWindowList;

	while(lpCurrent) {
	  lpCurrent->WindowFunc(lpCurrent->hWnd, MSG_DRAW_WINDOW, 0, 0);
	  lpCurrent->WindowFunc(lpCurrent->hWnd, MSG_PAINT, 0, 0);

	  lpCurrent = lpCurrent->next;
	}

	UpdateVideoMemory();
}

char SetFocusForParentWindow(HWND hChild) // +
{
	LPWINDOW lpChild;

	if(NULL == (lpChild = GetPtrToWindow(hChild))) return 1;
	if(!(8 & lpChild->flag)) return 2;

	KillFocus(lpChild->parent_window, 3);

	return 0;
}

char KillFocusForParentWindow(HWND hChild) // +
{
	LPWINDOW lpChild;

	if(NULL == (lpChild = GetPtrToWindow(hChild))) return 1;
	if(!(8 & lpChild->flag)) return 2;

	KillFocus(lpChild->parent_window, 0);

	return 0;
}

void windowChildSetFlag(HWND hParent, UINT newFlag) // +
{
	LPWINDOW lpParent = GetPtrToWindow(hParent);

	if(16 & lpParent->flag) windowSetFlag(lpParent->child_window, newFlag);
}

void windowParentSetFlag(HWND hChild, UINT newFlag) // +
{
	LPWINDOW lpChild = GetPtrToWindow(hChild);

	if(8 & lpChild->flag) windowSetFlag(lpChild->parent_window, newFlag);
}

UINT windowGetFlag(HWND hWnd) // +
{
	LPWINDOW lpWindow = GetPtrToWindow(hWnd);
	if(NULL == lpWindow) return 0;
	return lpWindow->flag;
}

void windowSetFlag(HWND hWnd, UINT newFlag) // +
{
	LPWINDOW lpWindow = GetPtrToWindow(hWnd);
	lpWindow->flag = newFlag;
}

void SetChildWindow(HWND hParent, HWND hChild) // +
{
	LPWINDOW lpWindow;

	lpWindow = GetPtrToWindow(hParent);
	lpWindow->child_window = hChild;
	lpWindow->flag |= 16;

	SetParentWindow(hChild, hParent);
}

void SetParentWindow(HWND hChild, HWND hParent) // +
{
	LPWINDOW lpWindow;

	lpWindow = GetPtrToWindow(hChild);
	lpWindow->parent_window = hParent;
	lpWindow->flag |= 8;
}

void SetFocus(HWND hwnd, char mode) // +
{
	LPWINDOW current;

	switch(mode) {
	// Set focus for previous window.
	case 4: {
		if(NULL == (current = GetPtrToWindow(hwnd))) return;
		if(current->previous)
		  KillFocus(current->previous->hWnd, 3);
		return;
	 }

	// Set focus for parent window.
	case 5: {
		SetFocusForParentWindow(hwnd);
		return;
	 }
	}

	current = DefaultWindowList;

	while(current) {

	  // Set focus hWnd window.
	  switch(mode) {
	  case 0:
		if(current->hWnd == hwnd) {
		  current->flag |= 4;
		  return;
		}
	   break;

	  // Set focus for all, with out hwnd window.
	  case 1:
		if(current->hWnd != hwnd) {
		  current->flag |= 4;
		  return;
		}
	   break;

	  // Set focus for all window.
	  case 2: current->flag |= 4; break;

	  // Set focus for all and kill focus for hwnd window.
	  case 3:
		if(current->hWnd != hwnd) current->flag |= 4;
		else current->flag &= 4^0xffff;
	   break;

	  default: return;
	  }

	  current = current->next;
	}
}

void KillFocus(HWND hwnd, char mode) // +
{

	WINDOW *current;

	current = DefaultWindowList;

	while(current) {

	  // Kill focus for hwnd window.
	  switch(mode) {
	  case 0:
		if(current->hWnd == hwnd) {
		  current->flag &= 4^0xffff;
		  return;
		}
	   break;

	  // Kill focus for all, with out hwnd window.
	  case 1:
		if(current->hWnd != hwnd) {
		  current->flag &= 4^0xffff;
		  return;
		}
	   break;

	  // Kill focus for all window.
	  case 2: current->flag &= 4^0xffff; break;

	  // Kill focus for all and set focus for hwnd window.
	  case 3:
		if(current->hWnd != hwnd) current->flag &= 4^0xffff;
		else current->flag |= 4;
	   break;

	  default: return;
	  }

	  current = current->next;
	}
}

LPWINDOW GetPtrToWindow(HWND hWnd) // +
{
	LPWINDOW temp;

	temp = DefaultWindowList;
	while(temp) {
	  if(hWnd == temp->hWnd) return temp;
	  temp=temp->next;
	}

	return NULL;
}

void DebugWindowList(WINDOW **root) // +
{
	WINDOW *temp;

	HideMouse
	temp = *root;
	while(temp) {
	  printf("Current Window ID = %d: \n", temp->hWnd);
	  if(temp->previous) printf("\tPrevious Window ID = %d,\tTitle: %s\n", temp->previous->hWnd, temp->previous->Title);
	  if(temp->next) printf("\tNext Window ID = %d, Title: %s\n\n", temp->next->hWnd, temp->next->Title);
	  temp = temp->next;
	}
	ShowMouse
}
/////////////////////////////////////////////////////////////////////////////
void PutStr2WindowXY(HWND hWnd, UCHAR *str, UCHAR color, char x, char y) // +
{
	if(NULL != GetPtrToWindow(hWnd))
	  while(*str) PutChar2WindowXY(hWnd, *str++, color, x++, y);
}

void PutChar2WindowXY(HWND hWnd, UCHAR ch, UCHAR color, char x, char y) // +
{
	LPWINDOW window;

	if(NULL != (window = GetPtrToWindow(hWnd))) {
	  x += window->X+1;     y += window->Y+1;

	  if(x > window->X && x+1 < window->X + window->SIZE_X)
		if(y > window->Y && y+1 < window->Y + window->SIZE_Y)
		  PutCharToBuffer(VideoBuffer, ch, color, x, y);
	}
}
//////////////////////////////////////////////////////////////////////////////
HANDLE add2List(void **root, UCHAR type, void  *data) // +
{
	HANDLE id=0;

	switch(type) {
	case LIST_WINDOW: {
		LPWINDOW temp;

		if(NULL == *root) {
		  DefaultWindowList = (LPWINDOW)data;
		  return 0;
		}

		temp = (LPWINDOW)(*root);
		while(temp) {
		  if(id == temp->hWnd) { id++; temp = (LPWINDOW)(*root); }
		  else temp = temp->next;
		}

		temp = DefaultWindowList;
		while(temp->next) {
		  temp->flag &= 4^255;
		  temp = temp->next;
		}

		temp->flag &= 4^255;
		temp->next = (LPWINDOW)data;
		temp->next->hWnd = id;
		temp->next->previous = temp;
	}
	 break;

	case LIST_WINDOW_MSG: {
		if(NULL != *root) {
		  MSG_LIST *temp;

		  temp = (MSG_LIST*)(*root);
		  while(temp->next) temp = temp->next;

		  temp->next = (MSG_LIST*)data;
		} else (MSG_LIST*)(*root) = (MSG_LIST*)data;

		return 0;
	 }
	}

	return id;
}

void _freeList(void **root, UCHAR type) // +
{
	switch(type) {
	case WINDOW_LIST_LABEL:
	case WINDOW_LIST_BUTTON:
	case WINDOW_LIST_CHAR_MAP:
	case WINDOW_LIST_EDIT_BOX:
	case WINDOW_LIST_TAB_ORDER:
	case WINDOW_LIST_MENU_BUTTON:
		while(*root) {
			LPLIST current = (LPLIST)(*root);
			removeFromList((LPLIST*)root, current->id);
		}
	 break;

	case LIST_WINDOW_MSG: { MSG_LIST *current, *temp;

		current = (MSG_LIST*)(*root);
		while(current) {
		  temp = current->next;

		  free(current->lpMsg);
		  free(current);

		  current = temp;
		}
	 }
	}
}
//////////////////////////////////////////////////////////////////////////////
void PostQuitMsg(HWND hwnd)
{
	SendMessage(hwnd, MSG_QUIT, 0, 0);
}

void SendMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) // +
{
	LPWINDOW window;
	MSG_LIST *temp;

	if(NULL == (window = GetPtrToWindow(hwnd))) return;
	if(NULL == (temp = (MSG_LIST *) malloc(sizeof(MSG_LIST)))) return;
	if(NULL != (temp->lpMsg = (LPMSG) malloc(sizeof(MSG)))) {

	  temp->next = NULL;

	  temp->lpMsg->hwnd = hwnd;
	  temp->lpMsg->message = msg;
	  temp->lpMsg->wParam = wParam;
	  temp->lpMsg->lParam = lParam;

	  Mouse
	  temp->lpMsg->pt.x = mouse.x/8 - window->X;
	  temp->lpMsg->pt.y = mouse.y/8 - window->Y;

	  add2List((void **)&window->MsgList, LIST_WINDOW_MSG, (void *)temp);
	}
}

BOOL GetMessage(LPMSG msg, HWND hwnd, UINT min, UINT max) // +
{
	LPWINDOW window;

	if(NULL != (window = GetPtrToWindow(hwnd))) {
	  MSG_LIST *temp;

	LGetNewMessage:
	  // Get message from input (mouse/keyboard).
	  while(NULL == window->MsgList)
		IORead();

	  temp = window->MsgList->next;

	  *msg = *window->MsgList->lpMsg;

	  free(window->MsgList->lpMsg);
	  free(window->MsgList);

	  window->MsgList = temp;

	  if(min && msg->message < min) goto LGetNewMessage;
	  if(max && msg->message > max) goto LGetNewMessage;

	  if(MSG_QUIT == msg->message) {
		window->WindowFunc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
		return FALSE;
	  }

	} else return FALSE;

	return TRUE;
}

void TranslateMessage(LPMSG msg) // ?
{
	msg->hwnd |= 0;
}

void DispachMessage(LPMSG msg) // +
{
	WINDOW *window;

	if(NULL != (window = GetPtrToWindow(msg->hwnd)))
	  window->WindowFunc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
}
//////////////////////////////////////////////////////////////////////////////
void PutStrToBuffer(UINT *buffer, UCHAR *str, UCHAR color, char x, char y) // +
{
	while(*str) PutCharToBuffer((UINT*)buffer, *str++, color, x++, y);
}

void PutCharToBuffer(UINT *buffer, UCHAR ch, UCHAR color, char x, char y) // +
{
	if((int)x<0 || (int)x>79) return;
	if((int)y<0 || (int)y>24) return;

	buffer[y*80+x] = ch|(color<<8);
}
//////////////////////////////////////////////////////////////////////////////
void FreeMemory(void) // +
{
	free(VideoBuffer);
	free(BackGroundBuffer);
}
//////////////////////////////////////////////////////////////////////////////
//      Direct write to video memory.
/*void PutStrXY(UCHAR *str, UCHAR color, char x, char y) // +
{
	while(*str) PutCharXY(*str++, color, x++, y);
}

char GetChar(void) // +
{
	UCHAR far *videoMem=(UCHAR far *) 0xB8000000;
	return *(videoMem+((xScr*2)+(yScr*160)));
}

char GetColor(void) // +
{
	UCHAR far *video_buffer=(UCHAR far *) 0xB8000000;
	return *(video_buffer+(((xScr*2)+(yScr*160))+1));
}

void PutStr(char *str) // +
{
	while(*str) {
	  switch(*str) {
	  case '\t': str++;
		if((xScr+=4)>79) {
			xScr = 0;
			if(++yScr==25) yScr = 0;
		}
	   break;
	  case '\n': str++; xScr=0; if(++yScr==25) yScr = 0; break;
	  default: PutChar(*str++);
	  }
	}
}

void PutChar(char ch) // +
{
	char far *video_buffer=(char far *) 0xB8000000;
	*(video_buffer+((xScr*2)+(yScr*160)))=ch;
	*(video_buffer+(((xScr*2)+(yScr*160))+1))=Color;

	if(++xScr==80) {
	  xScr = 0;

	  if(++yScr==25) yScr = 0;
	}
}*/
//////////////////////////////////////////////////////////////////////////////
void IORead(void) // +
{
	UCHAR ch;
	MouseHandler();

	if(kbhit()) {
		LPWINDOW lpWindow;

		lpWindow = DefaultWindowList;

		while(lpWindow) {
			if(4 & lpWindow->flag)
				while(kbhit()) {
					ch = getch();
					if(ch) SendMessage(lpWindow->hWnd, MSG_SYSTEM_PRESS_KEY, ch, 0);
					else {
						UINT message;

						switch(getch()) {
						case 75: message = MSG_KB_RKEY; break;
						case 77: message = MSG_KB_LKEY; break;
						case 72: message = MSG_KB_UKEY; break;
						case 80: message = MSG_KB_DKEY;
						}
						SendMessage(lpWindow->hWnd, message, 0, 0);
						break;
					}
				}
			lpWindow = lpWindow->next;
		}
		while(kbhit()) getch();
	}
}

void InitVideo(void) // +
{
#ifdef __OPEN_WATCOM__
	union REGS regs;
#endif

	if(NULL == (VideoBuffer = (UINT *) malloc(2000*sizeof(int)))) {
	  puts("Cannot malloc memory for VideoBuffer /4000 B/...");
	  exit(1);
	}

	if(NULL == (BackGroundBuffer = (UINT *) malloc(2000*sizeof(int)))) {
	  puts("Cannot malloc memory for BackGroundBuffer /4000 B/...");
	  exit(1);
	}

	memset(VideoBuffer, 0, 2000*sizeof(int));

#ifdef __OPEN_WATCOM__
	regs.w.ax = 0x0013;
	int86(0x10, &regs, &regs);
	regs.w.ax = 0x0003;
	int86(0x10, &regs, &regs);
#else
  #ifdef BORLANDC
	clrscr();
	asm {
	  mov ax, 0x0013
	  int 0x10
	  mov ax, 0x0003
	  int 0x10
	}
  #endif
#endif
	InitMouse
	SaveBackGround();
	ShowMouse
}
#endif // __WINDOW_H__
