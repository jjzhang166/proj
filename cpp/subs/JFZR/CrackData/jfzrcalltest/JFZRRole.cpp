#include "StdAfx.h"
#include "JFZRRole.h"

JFZRRole::JFZRRole(void)
{
}

JFZRRole::~JFZRRole(void)
{
}

bool JFZRRole::UpData(void)
{
	DWORD	dwRoleBaseAddr = 0;
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_BASE_ADDR,&dwRoleBaseAddr,sizeof(dwRoleBaseAddr));
	assert(dwRoleBaseAddr);

	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_SERIAL_OFFSET___,&m_dwSerial,sizeof(m_dwSerial));
	assert(m_dwSerial);

	DWORD	dwLen = 0;
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_JOBLEN_OFFSET___,&dwLen,sizeof(dwLen));

	DWORD dwJobNameAddr = 0;
	if( dwLen > 7 )
	{
		_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOACL_OBJ_JOBNAME_OFFSET___,&dwJobNameAddr,sizeof(dwJobNameAddr));
	}
	else
	{
		dwJobNameAddr = dwRoleBaseAddr+JFZR_REVERSAL_LOACL_OBJ_JOBNAME_OFFSET___;
	}

	char szReadJobData[256] = {0};
	_ReadProcessMemory(dwJobNameAddr,szReadJobData,dwLen*2);

	DWORD dwJobNameLen = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadJobData,-1,NULL,0,NULL,FALSE);

	char* pszObjJobName = NULL;
	pszObjJobName = new char[dwJobNameLen+2];

	if( pszObjJobName != NULL )
	{
		memset(pszObjJobName,0,dwJobNameLen+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadJobData,-1,pszObjJobName,dwJobNameLen,NULL,FALSE);
	}

	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_NAMELEN_OFFSET___,&dwLen,sizeof(dwLen));

	DWORD dwNameAddr = 0;
	if( dwLen > 7 )
	{
		_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOACL_OBJ_NAME_OFFSET___,&dwNameAddr,sizeof(dwNameAddr));
	}
	else
	{
		dwNameAddr = dwRoleBaseAddr+JFZR_REVERSAL_LOACL_OBJ_NAME_OFFSET___;
	}

	char szReadNameData[256] = {0};
	_ReadProcessMemory(dwNameAddr,szReadNameData,dwLen*2);

	DWORD dwObjNameLen = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadNameData,-1,NULL,0,NULL,FALSE);

	char* pszObjName = NULL;
	pszObjName = new char[dwObjNameLen+2];

	if( pszObjName != NULL )
	{
		memset(pszObjName,0,dwObjNameLen+2);
		WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)szReadNameData,-1,pszObjName,dwObjNameLen,NULL,FALSE);
	}

	memcpy(m_szJobName,pszObjJobName,dwJobNameLen+1);
	memcpy(m_szJobNameEx,szReadJobData,(dwJobNameLen+1)*2);
	memcpy(m_szRoleName,pszObjName,dwObjNameLen+1);
	memcpy(m_szRoleNameEx,szReadNameData,(dwObjNameLen+1)*2);

	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_X_OFFSET,&m_fx,sizeof(m_fx));
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_Z_OFFSET,&m_fz,sizeof(m_fz));
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_Y_OFFSET,&m_fy,sizeof(m_fy));
	
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_X_OFFSET___,&m_fDirectionX,sizeof(m_fDirectionX));
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_Z_OFFSET___,&m_fDirectionZ,sizeof(m_fDirectionZ));
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_Y_OFFSET___,&m_fDirectionY,sizeof(m_fDirectionY));

	DWORD dwLevel = 0;
	_ReadProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_ROLE_LEVEL_OFFSET,&dwLevel,sizeof(dwLevel));

	char szPrint[256] = {0};
	//			 	   职业           角色名称
 	sprintf_s(szPrint,"job name = %s  obj name = %s level = %d x = %f y = %f z = %f dx = %f dy = %f dz = %f\r\n",
							m_szJobName,m_szRoleName,dwLevel,m_fx,m_fy,m_fz,m_fDirectionX,m_fDirectionY,m_fDirectionZ);

	csTxt = szPrint;
	return false;
}



bool JFZRRole::UpStaticData(void)
{
	csTxt = "";
	PLAYER_INFO	thePlayerInfo = {0};
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR,&thePlayerInfo,sizeof(thePlayerInfo));

	wchar_t wszPrint[256] = {0};
	swprintf_s(wszPrint,L"name = %s serial = 0x%p class type = 0x%p Gold = %d faigue = %d\r\n",
				GetString(&thePlayerInfo.str_thePlayerName),
				thePlayerInfo.str_dwSerial,
				thePlayerInfo.thePlayerInfo.theStaticPlayerInfo.str_dwClassType,
				thePlayerInfo.thePlayerInfo.theDynamicPlayerInfo.str_dwGold,
				thePlayerInfo.thePlayerInfo.theDynamicPlayerInfo.str_dwFatigue
				);

	csTxt = wszPrint;

	CALSS_CLASSTYPE theClassType = {0};
	_ReadProcessMemory(JFZR_REVERSAL_CLASS_CLASSTYPE_ADDR,&theClassType,sizeof(theClassType));

	for (int i=0; i<20; i++)
	{
		swprintf_s(wszPrint,L"class name = %s class type = %d\r\n",GetString(&theClassType.theClass[i].str_theClass),theClassType.theClass[i].str_dwClassType);
		csTxt += wszPrint;
	}

	return true;
}

void JFZRRole::LocalActionSyncxyz(FLOAT x,FLOAT y, FLOAT z)
{
	DWORD	dwRoleBaseAddr = 0;
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_BASE_ADDR,&dwRoleBaseAddr,sizeof(dwRoleBaseAddr));
	assert(dwRoleBaseAddr);

	_WriteProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_X_OFFSET,&x,sizeof(x));
	_WriteProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_Z_OFFSET,&z,sizeof(z));
	_WriteProcessMemory(dwRoleBaseAddr+JFZR_REVERSAL_LOCAL_OBJ_Y_OFFSET,&y,sizeof(y));
}