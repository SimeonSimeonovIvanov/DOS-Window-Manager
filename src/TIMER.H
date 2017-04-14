/*
		The^day^of^DooM

		Borland C++ 3.1

	Create Date:	31.03.2007
	Last Update:	21.05.2007
	Full Change:    27.12.2007
	Last Update:    28.12.2007
	Last Update:	12.07.2008
*/

#ifndef __TIMER_H__
	#define __TIMER_H__

#ifndef __WINDOW_H__
	#include "src/window.h"
#endif

LPTIMER GetPtrToTimer(HWND hwnd, HANDLE idTimer);

HANDLE AddTimer( HWND hwnd,ULONG value,
				 void (*UserTimerFunc)(HWND hWnd, HANDLE idTimer)
			   ) // +
{
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hwnd))) {
	  LPTIMER lpNewTimer;

	  if(NULL == (lpNewTimer = (LPTIMER) malloc(sizeof(TIMER))) ) return -1;

	  lpNewTimer->flag = 1;
	  lpNewTimer->temp = value;
	  lpNewTimer->value = value;
	  lpNewTimer->DefTimerFunc = UserTimerFunc;

	  return addToList(&lpWindow->list_timer, (void *)lpNewTimer, NULL);
	}

	return -2;
}

void RemoveTimer( HWND hWnd, HANDLE hTimer) // +
{
	register LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hWnd)))
	  removeFromList(&lpWindow->list_timer, hTimer);
}

char TimerStatus(HWND hwnd, HANDLE idTimer)
{
	LPTIMER lpTimer;

	if(NULL != (lpTimer = GetPtrToTimer(hwnd, idTimer)))
	  return lpTimer->flag;

	return -1;
}

void RunTimer(HWND hwnd, HANDLE idTimer)
{
	LPTIMER lpTimer;

	if(NULL != (lpTimer = GetPtrToTimer(hwnd, idTimer)))
	  lpTimer->flag |= 1;
}

void StopTimer(HWND hwnd, HANDLE idTimer)
{
	LPTIMER lpTimer;

	if(NULL != (lpTimer = GetPtrToTimer(hwnd, idTimer)))
	  lpTimer->flag &= 1^0xff;
}

UCHAR SetTimer(HWND hwnd, HANDLE idTimer, ULONG newValue) // +
{
	LPTIMER lpTimer;

	if(NULL == (lpTimer = GetPtrToTimer(hwnd, idTimer))) return 1;

	lpTimer->value = newValue;	lpTimer->temp = newValue;

	return 0;
}

ULONG GetTimer(HWND hwnd, HANDLE idTimer) // +
{
	LPTIMER lpTimer;

	if(NULL == (lpTimer = GetPtrToTimer(hwnd, idTimer))) return 0;

	return lpTimer->value;
}

UCHAR ResetTimer(HWND hwnd, HANDLE idTimer) // +
{
	LPTIMER lpTimer;

	if(NULL == (lpTimer = GetPtrToTimer(hwnd, idTimer))) return 1;

	lpTimer->value = lpTimer->temp;

	return 0;
}

UCHAR DecTimer(HWND hwnd, HANDLE idTimer)
{
	LPTIMER lpTimer;

	if(NULL != (lpTimer = GetPtrToTimer(hwnd, idTimer)))
	  if(--lpTimer->value > 0)
		return 0;

	return 1;
}

UCHAR IncTimer(HWND hwnd, HANDLE idTimer)
{
	LPTIMER lpTimer;

	if(NULL != (lpTimer = GetPtrToTimer(hwnd, idTimer)))
	  if(4294967295 > ++lpTimer->value)
		return 0;

	return 1;
}

LPTIMER GetPtrToTimer(HWND hwnd, HANDLE idTimer)
{
	LPWINDOW lpWindow;

	if(NULL != (lpWindow = GetPtrToWindow(hwnd)) ) {
	  LPLIST lpList;

	  lpList = lpWindow->list_timer;
	  while(lpList) {

		if(idTimer == lpList->id)
		  return (LPTIMER)lpList->lpData;

		lpList = lpList->next;
	  }
	}

	return NULL;
}
#endif // __TIMER_H__