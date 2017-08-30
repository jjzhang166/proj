#pragma once


typedef struct
{
	wchar_t	str_szJobName[256];
	wchar_t	str_szObjName[256];
	wchar_t	str_szObjType[256];
	DWORD	str_dwSerial;
	float	str_fx;
	float	str_fy;
	float	str_fz;
	float	str_fDirectionX;
	float	str_fDirectionY;
	float	str_fDirectionZ;
	DWORD	str_dwCurrentHp;
	DWORD	str_dwCeilingHp;
	DWORD	str_dwCurrentMp;
	DWORD	str_dwCeilingMp;
	wchar_t	str_szDefaultActionId[256];
}JFZR_MONSTER_OBJ,*PJFZR_MONSTER_OBJ;

typedef struct
{
	wchar_t	str_szType[256];
	int		str_nCount;
	JFZR_MONSTER_OBJ	str_theMonsterObj[64];
}JFZR_MONSTER_OBJ_STRUCT,*PJFZR_MONSTER_OBJ_STRUCT;

typedef struct
{
	wchar_t		str_szNpcId[256];
}JFZR_NPC_ID,*PJFZR_NPC_ID;


class JFZRMonsterObj
{
public:
	JFZRMonsterObj(void);
	~JFZRMonsterObj(void);
public:
	bool TraverseObj(bool bPrintFlag);

public:
	CList<JFZR_NPC_ID,JFZR_NPC_ID> m_str_NpcIdList;

	bool SetUnHitNpcId(const wchar_t* pwszNpcId);
	bool IsHitNpcId(const wchar_t* pwszNpcId);

private:
	JFZR_MONSTER_OBJ	m_MonsterObj[0x100];
	DWORD				m_dwMonsterObjIndex;

// ≤‚ ‘◊®”√
public:
	CString csTxt;
	bool	bPrintcsTxtFlag;
	int GetObjCount(wchar_t* pszObjType);
	int GetArrayObj(PJFZR_MONSTER_OBJ_STRUCT ptheObj, int nCount);
};

extern JFZRMonsterObj	theMonsterObj;
extern CString  g_csTraverseObjTxt;