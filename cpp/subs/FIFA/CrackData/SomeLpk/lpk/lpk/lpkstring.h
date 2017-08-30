#pragma once
#include <memory.h>
#include <string.h>
#include <CD_CommonFiles/CD_CmnDefines.h>

class lpkstring
{
private:
	char	m_szdata[REVERSAL_SOCK_BUFFER_SIZE];
public:
	lpkstring(const char *str = NULL);
	lpkstring(const lpkstring &other);
	~lpkstring(void);
public:
	lpkstring& operator=(const lpkstring &other);
	lpkstring& operator+=(const lpkstring &other);
	bool operator==(const lpkstring &other);
public:
	size_t length(){return strlen(m_szdata);}
	size_t size(){return strlen(m_szdata);}
	const char* c_str(){return m_szdata;}
};
