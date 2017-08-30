#include "stdafx.h"
#include "UsefulFuncs.h"
#include <CD_CommonFiles/CD_Updates.h>

void PressKey(DWORD dwVk,UINT Msg )
{
	__asm
	{
		mov eax, UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER
			call eax

			push dwVk
			push Msg

			MOV ECX,EAX
			MOV EDX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[EDX+0x5C]
	}
}

//WM_LBUTTONDOWN WM_LBUTTONUP 
void FIFA_MouseLeftClick(int x,int y)
{
	__asm
	{
		mov eax, UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER
			call eax

			push y
			push x
			push 0
			push 1

			MOV EDX,DWORD PTR DS:[EAX]
		MOV ECX,EAX
			CALL DWORD PTR DS:[EDX+0x50]				// 鼠标按下

		mov eax, UPDATES_FIFA_REVERSAL_MEMOPER_GET_MOUSE_KEY_EVENT_HANDLER
			call eax
			push y
			push x
			push 0
			push 0

			MOV EDX,DWORD PTR DS:[EAX]
		MOV ECX,EAX
			CALL DWORD PTR DS:[EDX+0x50]				//鼠标弹起

	}
}