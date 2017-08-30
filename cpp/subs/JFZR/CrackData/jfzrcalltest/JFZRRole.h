#pragma once

class JFZRRole
{
public:
	JFZRRole(void);
	~JFZRRole(void);
public:
	char	m_szRoleName[256];
	wchar_t	m_szRoleNameEx[256];
	char	m_szJobName[256];
	wchar_t	m_szJobNameEx[256];
	DWORD	m_dwSerial;
	float	m_fx;
	float	m_fy;
	float	m_fz;
	DWORD	m_dwHp;
	DWORD	m_dwMp;
	float	m_fDirectionX;
	float	m_fDirectionY;
	float	m_fDirectionZ;
public:
	bool UpData(void);
	bool UpStaticData(void);
	void LocalActionSyncxyz(FLOAT x,FLOAT y, FLOAT z);
public:
	CString	csTxt;
};
