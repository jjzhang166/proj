#pragma once

typedef struct  
{
	char	str_szSkillName[64];		// ASICC 技能名称
	wchar_t	str_szSkillNameEx[64];		// UNICODE 技能名称
	DWORD	str_dwSkillLevel;			// 技能等级
	DWORD	str_dwSkillId;				// 技能ID
}JFZR_SKILL_OBJ,*PJFZR_SKILL_OBJ;


class JFZRSkill
{
public:
	JFZRSkill(void);
	virtual ~JFZRSkill(void);
public:
	virtual bool TraverseSkill(void);
	PJFZR_SKILL_OBJ GetSkillObj(char* pszskillname);
public:
	JFZR_SKILL_OBJ	m_SkillObj[64];			// 默认64个技能
	DWORD			m_dwSkillObjIndex;

// 测试专用
public:		
	CString csTxt;
	
};

class JFZRTHIEFSkill : public JFZRSkill
{
public:
	JFZRTHIEFSkill(void);
	virtual ~JFZRTHIEFSkill(void);
};