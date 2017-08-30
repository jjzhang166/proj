#include "stdafx.h"
#include "MHOUi.h"
#include "UsefulFns.h"

#define P_D *(DWORD *)
#define P_N *(int *)
#define P_C (char *)
#define P_F *(float *)
#define P_B *(BYTE *)
#define P_W *(WORD *)
#define P_WC (wchar_t *)

MHOUi::MHOUi(void)
{
}


MHOUi::~MHOUi(void)
{
}
//dd [[[[[[[0xEA37458]+0xbc]+0xb8+8]+14]+64]+5c]+3f78]+54


void __cdecl OutputDebugStringF(const char *format, ...)  
{  
	va_list vlArgs;  
	char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);  

	va_start(vlArgs, format);  
	_vsnprintf_s(strBuffer, 4096 - 1, 4095, format, vlArgs);  
	va_end(vlArgs);  
	strcat_s(strBuffer, 4096, "\n");  
	OutputDebugStringA(strBuffer);  
	GlobalFree(strBuffer);  
	return;  
}  

void MHOUi::GetUIClassName(DWORD dwAddr,char *RetName)
{
	//先获得这一层的名字
	__try
	{
			// 如果没有上一层，那么这一层是没有类名的

		DWORD dwFatherWnd = P_N(dwAddr + MMOCLIENT_REVERSAL_UI_INFO_FATHERWND_OFFSET____);
			if (!dwFatherWnd)
				return;
		  
			DWORD dwClassNameAddr=P_N(dwAddr+0x44);//没有特征码
			if (dwClassNameAddr)
			{
				dwClassNameAddr=P_N(dwClassNameAddr+8);
				char *pszCurName=P_C(P_N(dwClassNameAddr));
				if (strlen(pszCurName)>0xff || !strlen(pszCurName))
					return;
				
				char szTmp[0xff]={0};
				strcpy_s(szTmp,pszCurName);
				if (strlen(RetName))
					strcat_s(szTmp,".");
				

				strcat_s(szTmp,RetName);

				strcpy_s(RetName,strlen(szTmp)+1,szTmp);


				DWORD dwFatherIndex=P_N(dwAddr+0x18);

				if (dwFatherIndex)
				{
					GetUIClassName(dwFatherWnd,RetName);
				}
				
			}


	}__except(1)
	{

	}

	


}


BOOL MHOUi::GetUIVisible(DWORD dwAddr)
{
	__try
	{
		BYTE byIsVisible=FALSE;
		__asm
		{
			pushad
				mov ecx,dwAddr
				movzx eax,word ptr ds:[ecx+0x3E]
			shr eax,0xe
				and eax,1
				mov byIsVisible,al
				popad
		}
		if (!byIsVisible)
			return FALSE;//如果当中有一个是不可见，那么这个就是不可见

		DWORD dwFatherWndAddr=P_N(dwAddr+MMOCLIENT_REVERSAL_UI_INFO_FATHERWND_OFFSET____);
		if (!dwFatherWndAddr)
			return TRUE;

		return  GetUIVisible(dwFatherWndAddr);
	}
	__except(1)
	{

	}
	return FALSE;
}

BOOL MHOUi::PutString(DWORD dwObjAddr,const wchar_t *pszChar)
{
	BYTE bRet=FALSE;
	__try
	{
		DWORD dwStringLen=wcslen(pszChar);
		DWORD dwPutChar;
		DWORD dwFocus=P_N(dwObjAddr+MMOCLIENT_REVERSAL_UI_INFO_ISFOCUS_OFFSET_____);
		for (DWORD i=0;i<dwStringLen;i++)
		{
			 dwPutChar=0;
			 memcpy(&dwPutChar,&pszChar[i],2);

			 __asm
			 {
					 mov esi,dwObjAddr
					 mov edx,dword ptr ds:[esi]
					 mov eax,dword ptr ds:[edx+0x188]
					 push dwFocus
					 push dwPutChar
					 mov ecx,esi
					 call eax
					 mov bRet,al

			 }
		}


	}__except(1)
	{

	}
	return bRet;
}

BOOL MHOUi::ClickButton(const char*szClassName)
{
	DWORD dwObjAddr=  TraUi(szClassName);
	if (dwObjAddr)
	 return	ClickButton(dwObjAddr);
	assert(false);
	return FALSE;
}




BOOL MHOUi::ClickButton(DWORD dwObjAddr)
{

	 //char szTest[]="ClickButton";
	struct _TMP
	{
		DWORD dwType;
		DWORD dwTmp2;
		DWORD dwTmp4;
		DWORD dwTmp5;
		DWORD dwTmp6;
		DWORD dwTmp7;
	};

	_TMP Step[4]={0};
	Step[0].dwType=0x10;
	Step[1].dwType=0x1000;
	Step[2].dwType=0x20;
	Step[3].dwType=0x0100000C;
	__try
	{
		DWORD dwTempAddr=0;
		for (int i=0;i<4;i++)
		{
			dwTempAddr=(DWORD)&Step[i];
			__asm
			{
				
				mov ecx,dwObjAddr
					mov eax,[ecx]
				mov eax,[eax+0x194]
				mov ebx,dwTempAddr
					push ebx
					call eax
			}
		}
	}__except(1)
	{

	}

	return TRUE;
}



BOOL MHOUi::PutString(const char*pszClassName,const wchar_t*pszPutString)
{
	DWORD dwObjAddr=  TraUi(pszClassName);
	if (dwObjAddr)
	{
		if (CleanString(dwObjAddr))
			return PutString(dwObjAddr,pszPutString);
	}
	return FALSE;
}

BOOL MHOUi::CleanString(DWORD dwObjAddr)
{

	__try
	{
		__asm
		{
			mov esi,dwObjAddr
			mov ecx,[esi+0x84]
			mov ecx,[ecx+8]
			push 0x100
			push 0
			mov ebx,UPDATES_REVERSAL_UI_CLAENSTRING_CALL
			call ebx
		}

		return TRUE;
	}
	__except(1)
	{
		return  FALSE;
	}
	
}

DWORD MHOUi::TraUi(const char *szClassName)
{
	__try
	{
			DWORD dwUiBase= (DWORD)GetCryActionBaseAddr()+MMOCLIENT_REVERSAL_UI_BASE_ADDR;
			dwUiBase=P_N(dwUiBase);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET1____);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET2____);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET3____);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET4____);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET5____);
			dwUiBase=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_OFFSET6);

			DWORD dwNextAddr=P_N(dwUiBase+MMOCLIENT_REVERSAL_UI_INFO_NEXTPOINT_OFFSET____);
			
			char szCurClassName[0xff];

			for (int i=0;i<10000;i++)
			{
				if (!dwNextAddr)
					break;

				ZeroMemory(szCurClassName,0xff);
				BYTE byIsVisible=FALSE;
				
				BOOL bVisible= GetUIVisible(dwNextAddr);
				if (bVisible)
				{
					GetUIClassName(dwNextAddr,szCurClassName);
#if _DEBUG
					if(bVisible && strlen(szCurClassName))
						OutputDebugStringF("flog index=%d Addr=%p Value=%p  Value1=%p Value20=%p Value68=%p IsVisiBle=%d  className=%s",
						i,dwNextAddr,P_N(dwNextAddr+0x14),P_N(dwNextAddr+0x18),P_N(dwNextAddr+0x20),P_N(dwNextAddr+0x68),bVisible,
						szCurClassName
						);
#endif
					if (strlen(szCurClassName)  && strstr(szCurClassName,szClassName))
					{
						return dwNextAddr;
					}
				}


				dwNextAddr=P_N(dwNextAddr+MMOCLIENT_REVERSAL_UI_INFO_NEXTPOINT_OFFSET____);
			}
	}
	__except(1)
	{

	}
	
	return 0;
}
