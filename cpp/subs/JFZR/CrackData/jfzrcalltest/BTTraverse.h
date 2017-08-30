#pragma once

CString TraverseAllTask(void);
CString TraverseReceivedTask(void);
CString TraverseJobTask(void);
CString TraverseTaskNpc(void);
CString TraverseChineseEnglish(void);

CString TraverseSdbSkill(void);
CString TraverseSdbItem(void);
CString TraverseSdbStage(void);
std::wstring TraverseShopInfo(void);
CString TraverseClassInfo(void);
CString TraverseRegionInfo(void);
CString TraverseActioncommands(void);

CString TraverseSkillHitHash();
CString TraverseChapter(void);

FLOAT TraversePrevActionPlayTime();

typedef struct
{
	OUT FLOAT		fPrevActionPlayTime;
	OUT DWORD		dwPrevActionSerial;
	IN  DWORD		dwActionSerial;
	IN  DWORD		dwSerial;
}PREV_ACTION_DATA,*PPREV_ACTION_DATA;

bool TraversePrevActionPlayTime_(PPREV_ACTION_DATA pthePrevActionData);

FLOAT GetSdbSkillInfo(wchar_t* pszSkillId,DWORD dwSkillLevel);