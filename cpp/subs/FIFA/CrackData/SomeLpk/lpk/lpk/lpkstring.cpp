#include "lpkstring.h"
#include <assert.h>


lpkstring::lpkstring(const char *str)
{
	try
	{
		memset(m_szdata,0,sizeof(m_szdata));

		if( str != NULL )
		{
			if( strlen(str) >= REVERSAL_SOCK_BUFFER_SIZE )
			{
				assert(false);
			}
			else
			{
				memcpy(m_szdata,str,strlen(str));
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
}

lpkstring::lpkstring(const lpkstring &other)
{
	try
	{
		*this = other;
	}
	catch (...)
	{
		assert(false);
	}
}

lpkstring::~lpkstring(void)
{
	try
	{
		memset(m_szdata,0,sizeof(m_szdata));
	}
	catch (...)
	{
		assert(false);
	}
}

lpkstring& lpkstring::operator=(const lpkstring &other)
{
	try
	{
		memset(m_szdata,0,sizeof(m_szdata));
		memcpy(m_szdata,other.m_szdata,strlen(other.m_szdata));
	}
	catch (...)
	{
		assert(false);
	}
	return *this;
}

lpkstring& lpkstring::operator+=(const lpkstring &other)
{
	try
	{
		int nLen = strlen(m_szdata);
		nLen += strlen(other.m_szdata);
		if( nLen < REVERSAL_SOCK_BUFFER_SIZE )
		{
			strcat_s(m_szdata,other.m_szdata);
		}
		else
		{
			assert(false);
		}

	}
	catch (...)
	{
		assert(false);
	}

	return *this;
}

bool lpkstring::operator==(const lpkstring &other)
{
	int result;
	try
	{
		result = strcmp(m_szdata,other.m_szdata);
	}
	catch (...)
	{
		result = -1;
		assert(false);
	}
	if( result == 0 )
		return true;
	return false;
}