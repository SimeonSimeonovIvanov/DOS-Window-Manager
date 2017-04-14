/*
			The^day^of^DooM

			Borland C++ 3.1
				  &&
			  Open Watcom

	Create Date:	29.03.2007
	Last Update:	25.11.2007
	Full Change:    26.12.2007
	Last Update:    28.12.2007
	Last Update:    04.05.2009
*/

#ifndef __TYPE_H__
	#define __TYPE_H__

  #ifdef __OPEN_WATCOM__
	#include <dos.h>
	#include <i86.h>
  #endif

	#include <mem.h>
	#include <conio.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	typedef char BOOL;
	typedef long LONG;
	typedef int DWORD;
	typedef unsigned int UINT;
	typedef unsigned char UCHAR;
	typedef unsigned long ULONG;

	typedef int HANDLE;
	typedef HANDLE HWND;
	typedef UINT WPARAM;
	typedef LONG LPARAM;
	typedef void CALLBACK;
	typedef HANDLE HINSTANCE;

	#define TRUE 1
	#define FALSE 0

	#define WINDOW_BUTTON		0
	#define WINDOW_MENU_BUTTON	1
	#define WINDOW_EDIT_BOX		2

	#define WINDOW_MENU			0
	#define WINDOW_POP_UP_MENU  1
	////////////////////////////////////////
	// add2list(...)
	#define LIST_WINDOW				0
	#define LIST_WINDOW_MSG			1
	////////////////////////////////////////

#define	WINDOW_LIST_TIMER			2
#define WINDOW_LIST_LABEL           3
#define WINDOW_LIST_BUTTON          4
#define WINDOW_LIST_MENU_BUTTON     5
#define WINDOW_LIST_CHAR_MAP		6
#define WINDOW_LIST_EDIT_BOX		7
#define WINDOW_NULL_LIST			8
#define WINDOW_LIST_TAB_ORDER		9
//////////////////////////////////////////////////////////////////////////////
#define MSG_MOVE_WINDOW                  1
#define MSG_IS_SELECT                    2
#define MSG_RESIZE_WINDOW                3
#define MSG_SELECT_BUTTON                4
#define MSG_CLOSE_BUTTON                 5
#define MSG_MOVE_AND_SELECT              6
#define MSG_KB_PRESS_KEY                 7
#define MSG_QUIT                         8
#define MSG_PAINT                        9
#define MSG_DRAW_WINDOW                 10
#define MSG_WINDOW_MENU_SELECT_BUTTON   11
#define MSG_POP_UP_MENU_SELECT_BUTTON   12
#define MSG_WINDOW_TIMER                13
#define MSG_SET_ON_TOP                  14
#define MSG_SET_FOCUS                   15
#define MSG_WINDOW_FIRST_RUN			16
#define MSG_SELECT_EDIT_BOX				17
#define MSG_USER_DEFINED_MESSAGE		18
#define MSG_EDIT_BOX_PUTC				19
#define MSG_SYSTEM_PRESS_KEY			20
#define MSG_PRESS_BUTTON				21
#define MSG_MOUSE_CLICK					22
#define MSG_MOUSE_CLICK_ON_WINDOW		23

#define MSG_KB_RKEY						40
#define MSG_KB_LKEY                     41
#define MSG_KB_UKEY                     42
#define MSG_KB_DKEY                     43

#define LCLICK							24
#define RCLICK							25

#define CLICK_WINDOW					36
#define LCLICK_WINDOW					26
#define RCLICK_WINDOW					27

#define LCLCIK_LABBEL					28
#define RCLCIK_LABBEL					29

#define	LCLICK_BUTTON					30
#define RCLICK_BUTTON					31

#define LCKICK_MENU_BUTTON				32
#define RCLICK_MENU_BUTTON				33

#define LCLCIK_EDIT_BOX					34
#define RCLCIK_EDIT_BOX					35

//////////////////////////////////////////////////////////////////////////////
typedef struct list {
	void *lpData;
	unsigned char id;
	struct list *next;
	void (*freeData)(void *lpData);
} LIST, *LPLIST;
//////////////////////////////////////////////////////////////////////////////

typedef struct tagPOINT {
	char x, y;
} POINT;
//////////////////////////////////////////////////////////////////////////////
typedef struct tagMSG {
   HWND hwnd;
   UINT message;
   WPARAM wParam;
   LPARAM lParam;

   DWORD time;
   POINT pt;
} MSG;

typedef MSG* LPMSG;
//////////////////////////////////////////////////////////////////////////////
typedef struct charMap {
	UCHAR x, y, size_x, size_y, *buffer;
	struct charMap *next;
} CHAR_MAP;
//////////////////////////////////////////////////////////////////////////////
typedef struct MsgList {
	LPMSG lpMsg;
	struct MsgList *next;
} MSG_LIST;
//////////////////////////////////////////////////////////////////////////////
typedef struct tagTIMER {
	char flag;
	unsigned long value, temp;
	void (*DefTimerFunc)(HWND hWnd, HANDLE idTimer);
} TIMER, *LPTIMER;
//////////////////////////////////////////////////////////////////////////////
typedef struct Label {
	char *text;
	UCHAR x, y, text_color, flag;
} LABEL, *LPLABEL;
//////////////////////////////////////////////////////////////////////////////
/*
	BUTTON -> flag:
	bit 7 - N/A
	bit 6 - N/A
	bit 5 - N/A
	bit 4 - N/A
	bit 3 - Draw selct indicator (>_text_<).
	bit 2 - Button is select.
	bit 1 - Button is active.
	bit 0 - Button is visible.
*/
typedef struct Button {
	char *text;
	UCHAR x, y, text_color, flag;
	void (*DefButtonFunc)(HWND hWnd, HANDLE id);
} BUTTON, *LPBUTTON, LABEL_AND_BUTTON;
//////////////////////////////////////////////////////////////////////////////
/*
	EDIT_BOX -> flag:
	bit 7 - N/A
	bit 6 - N/A
	bit 5 - N/A
	bit 4 - N/A
	bit 3 - N/A
	bit 2 - EditBox is select.
	bit 1 - EditBox is active.
	bit 0 - EditBox is visible.
*/
typedef struct EditBox {
	char count;
	UCHAR x, y, *text, lenght, text_color, flag;
	void (*DefEditBoxFunc)(HWND hWnd, HANDLE id);
} EDIT_BOX, *LPEDIT_BOX;
//////////////////////////////////////////////////////////////////////////////
typedef struct TabOrder {
	HANDLE id;
	UCHAR type, flag;
} TAB_ORDER, *LPTAB_ORDER;
//////////////////////////////////////////////////////////////////////////////
/*
	WINDOW -> flag:
	bit 15 - N/A
	bit 14 - N/A
	bit 13 - N/A
	bit 12 - N/A
	bit 11 - N/A
	bit 10 - N/A
	bit  9 - N/A
	bit  8 - N/A
	bit  7 - Is active ? (recive message)
	bit  6 - Have a main menu ?
	bit  5 - N/A
	bit  4 - Child window ?
	bit  3 - Parent window ?
	bit  2 - Is Select (In Focus) ?
	bit  1 - Add close button ?
	bit  0 - Window is visible ?
*/
typedef struct Window {
	 UINT flag;
	 char *Title;
	 char X, Y, SIZE_X, SIZE_Y;

	 struct Window *previous, *next;

	 HWND hWnd, parent_window, child_window;

	 UCHAR border_color, window_color,  title_color;

	 CALLBACK (*WindowFunc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	 MSG_LIST *MsgList;

#ifdef __LABEL__
	 LPLIST list_label;
#endif

#ifdef __MENU__
	 LPLIST list_button, list_menu_button;
#else
	#ifdef __BUTTON__
	#endif
#endif

#ifdef __TIMER__
	LPLIST list_timer;
#endif

#ifdef __CHAR_MAP__
	LPLIST list_char_map;
#endif

#ifdef __EDIT_BOX__
	LPLIST list_edit_box;
#endif

#ifdef __TAB_ORDER__
	LPLIST list_tab_order;
#endif
} WINDOW, *LPWINDOW;

//////////////////////////////////////////////////////////////////////////////
/* 					  	Global functions.									*/
//////////////////////////////////////////////////////////////////////////////
// "scr/window.h"
void UpdateWindow(void);
void SetFocus(HWND hwnd, char mode);
void KillFocus(HWND hwnd, char mode);
void PostQuitMsg(HWND hwnd);
LPWINDOW GetPtrToWindow(HWND hWnd);
char SetFocusForParentWindow(HWND hChild);
UCHAR GetWindowPos(HWND hwnd, int *x, int *y);
UCHAR SetFocusFor(HWND hWnd, UCHAR type, HANDLE id);
HANDLE add2List(void **root, UCHAR type, void  *data);
UCHAR GetControlInFocus(HWND hWnd, UCHAR *type, HANDLE *id);

void SetChildWindow(HWND hParent, HWND hChild);
void SetParentWindow(HWND hChild, HWND hParent);

BOOL GetMessage(LPMSG msg, HWND hwnd, UINT min, UINT max);
void SendMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void PutChar2WindowXY(HWND hWnd, UCHAR ch, UCHAR color, char x, char y);

HWND CreateWindow( char *Title,char x,char y,char size_x,
				   char size_y, char border_color, char window_color,
				   char title_color
				 );

UINT windowGetFlag(HWND hWnd);
void windowSetFlag(HWND hWnd, UINT newFlag);
void windowChildSetFlag(HWND hParent, UINT newFlag);
void windowParentSetFlag(HWND hChild, UINT newFlag);

CALLBACK DefWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//////////////////////////////////////////////////////////////////////////////// "scr/label.h"
// "src/label.h"
LPLABEL GetPtrToLabel(HWND hWnd, HANDLE id);
LPLABEL GetPtrToLabelInList(LABEL **root, HANDLE id);
//////////////////////////////////////////////////////////////////////////////
// "scr/button.h"
LPBUTTON GetPtrToButtonInList(LPLIST root, HANDLE id);
LPBUTTON GetPtrToButton(HWND hWnd, UCHAR type, HANDLE id);
//////////////////////////////////////////////////////////////////////////////
// "scr/editbox.h"
LPEDIT_BOX GetPtrToEditBox(HWND hWnd, HANDLE id);
LPEDIT_BOX GetPtrToEditBoxInList(LPLIST root, HANDLE id);
//////////////////////////////////////////////////////////////////////////////
// "scr/taborder.h"
void tabOrderProcess(HWND hWnd);
void tabOrderAdd(HWND hWnd, UCHAR type, HANDLE id);
UCHAR tabOrderSetFocus(HWND hWnd, UCHAR type, HANDLE id);
//////////////////////////////////////////////////////////////////////////////
#endif // __TYPE_H__