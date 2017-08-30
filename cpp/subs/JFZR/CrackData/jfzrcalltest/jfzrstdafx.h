
#include "jfzrbase.h"
#include <assert.h>
#include <afxtempl.h>
#include "JFZRSkill.h"
#include "JFZRMonsterObj.h"
#include "JFZRPacket.h"
#include "JFZRRole.h"
#include "MyLock.h"
#include "jfzrbase.h"
#include "JFZRActionSystem.h"
#include "JFZRRoom.h"
#include "JFZRItem.h"
#include "UnPacket.h"
#include "SendPacket.h"
#include "PacketString.h"
#include "JFZRFbData.h"
#include "jfzrstruct.h"
#include "EnumPidWnd.h"
#include "jfzrpacketstruct.h"

#ifdef _SDB_BASE_
#include "Sdb.h"
#define SDB_FUN SdbBase();
#else
#define SDB_FUN
#endif





typedef int (*PACKET)(const char* pszSayBufffer, char* pOutszSendBuffer);
extern BOOL _ReadProcessMemory(DWORD dwReadAddr,void* pReadData,DWORD dwReadlen);
extern BOOL _WriteProcessMemory(DWORD dwWriteAddr,void* pWriteData,DWORD dwWritelen);
extern void Hook(DWORD dwHookAddr,LPVOID pfun);
extern void JFZR_Send(char* szSayBuffer,PACKET pFun);
extern int UserSkill1( const char* pszSayBufffer, char* pOutszSendBuffer);
extern int UserSkill2( const char* pszSayBufffer, char* pOutszSendBuffer);
extern int UserSkill3( const char* pszSayBufffer, char* pOutszSendBuffer);

typedef struct
{
	DWORD	dwAddr1;
	DWORD	dwAddr2;
	DWORD	dwBufferBeginAddr1;
	DWORD	dwBufferEndAddr;
	DWORD	dwBufferBeginAddr2;
}GAME_BUIILD_OBJ,*PGAME_BUIILD_OBJ;

typedef struct
{
	DWORD dwArgc3;
	DWORD dwArgc4;
}SEND_CALL_ARGC;

extern DWORD g_dwSendObjAddr;


#define DEBUG_PRINT(s)	char szPrint[1024] = {0};	sprintf_s(szPrint,"[JFZR] %s",s);	OutputDebugString(szPrint);



/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

