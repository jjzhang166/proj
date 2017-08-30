#include "StdAfx.h"
#include "CheckLpk.h"
#include "protect.h"
#include <CD_CommonFiles/CD_CmnDefines.h>

#define XOR_ENCRYPT(data,nlen) 	for (size_t  i = 0; i<nlen; i++)	\
								{									\
								if( i%10 == 0 )					\
									{								\
									continue;					\
									}								\
									data[i] ^= 0x7f;				\
									if( i%3 == 0 )					\
									data[i] += 1;				\
								}

			

CheckLpk	g_theCheckLpk;

bool OpenCheckLpk(LPVOID lpParameter)
{
	VMP_BEGIN;

	if( false == g_theCheckLpk.LpkCreateEvent(LPK_CHECK_EVENT) )
	{
		VMP_END;
		return false;
	}

	if( false == g_theCheckLpk.LpkCreateFileMapping(LPK_CHEKC_FILEMAPPING) )
	{
		VMP_END;
		return false;
	}

	const char* pszdata = VMP_PROTECT_STR_A(LPK_CHECK_VIEWOFFILE);
	char szdata[1024] = {0};
	auto str_len = strlen(pszdata);
	memcpy_s(szdata, sizeof(szdata), pszdata, str_len);
	XOR_ENCRYPT(szdata,str_len);
	if( false == g_theCheckLpk.LpkFlushViewOfFile(szdata, str_len) )
	{
		VMP_END;
		return false;
	}

	VMP_END;
	return true;
}

CheckLpk::CheckLpk(void)
{
	m_hEvent		= NULL;
	m_hMapFile		= NULL;
	m_pView			= NULL;
	
}

CheckLpk::~CheckLpk(void)
{
	if (m_hMapFile)
	{
		if (m_pView)
		{
			// Unmap the file view.
			UnmapViewOfFile(m_pView);
			m_pView = NULL;
		}
		// Close the file mapping object.
		CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}

	if( m_hEvent != NULL )
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
}

bool CheckLpk::LpkCreateEvent(const char* pszObjEvent)
{
	VMP_BEGIN;
	SECURITY_ATTRIBUTES EventAttributes = {0};

	char szObjEvent[256] = {0};
	strcat_s(szObjEvent,"Global\\");
	strcat_s(szObjEvent,pszObjEvent);
	m_hEvent = CreateEventA(&EventAttributes,FALSE,FALSE,szObjEvent);

	if( m_hEvent )
	{
		return true;
	}
	VMP_END;
	return false;
}

void CheckLpk::LpkCloseEvent()
{
	if( m_hEvent != NULL )
	{
		if( CloseHandle(m_hEvent) )
		{
			m_hEvent = NULL;
		}
	}
}

bool CheckLpk::LpkOpenEvent(const char* pszObjEvent)
{
	VMP_BEGIN;
	char szObjEvent[256] = {0};
	strcat_s(szObjEvent,"Global\\");
	strcat_s(szObjEvent,pszObjEvent);
	HANDLE hEvent = OpenEventA(EVENT_ALL_ACCESS,FALSE,szObjEvent);

	if( hEvent )
	{
		return true;
	}
	VMP_END;
	return false;
}

bool CheckLpk::LpkCreateFileMapping(const char* pszFileMappingObj)
{
	VMP_BEGIN;
	// Create the file mapping object.
	m_hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,   // Use paging file - shared memory
		NULL,                   // Default security attributes
		PAGE_READWRITE,         // Allow read and write access
		0,                      // High-order DWORD of file mapping max size
		0x1000,					// Low-order DWORD of file mapping max size
		pszFileMappingObj           // Name of the file mapping object
		);

	if (m_hMapFile != NULL) 
	{
		// Map a view of the file mapping into the address space of the current 
		// process.
		m_pView = MapViewOfFile(
			m_hMapFile,               // Handle of the map object
			FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
			0,                      // High-order DWORD of the file offset 
			0,						// Low-order DWORD of the file offset 
			0						// The number of bytes to map to view
			);
		if (m_pView != NULL)
		{
			return true;

		}
	}
	VMP_END;
	return false;
}

bool CheckLpk::LpkFlushViewOfFile( const char* pszData, int str_len )
{
	VMP_BEGIN;
	char szOutDesData[REVERSAL_SOCK_BUFFER_SIZE] = {0};
	xor(pszData,szOutDesData);
	memcpy_s((char*)m_pView,REVERSAL_SOCK_BUFFER_SIZE,szOutDesData, str_len);
	if( NULL == FlushViewOfFile(m_pView,str_len) )
	{
		return false;
	}
	VMP_END;
	return true;
}

bool CheckLpk::LpkOpenFileMapping(const char* pszFileMapping,OUT char* pszOutData)
{
	VMP_BEGIN;
	HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS,FALSE,pszFileMapping);
	
	if( hMapFile )
	{
		PVOID pView = MapViewOfFile(
			hMapFile,               // Handle of the map object
			FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
			0,                      // High-order DWORD of the file offset 
			0,						// Low-order DWORD of the file offset 
			0						// The number of bytes to map to view
			);

		if( pView )
		{
			char szOutDesData[REVERSAL_SOCK_BUFFER_SIZE] = {0};
			xor((char*)pView,szOutDesData);
			strcpy_s(pszOutData,REVERSAL_SOCK_BUFFER_SIZE,szOutDesData);

			return true;
		}
	}
	VMP_END;
	return false;
}
PVOID CheckLpk::LpkOpenConsoleFileMapping(const char* pszFileMapping)
{
	VMP_BEGIN;
	HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS,FALSE,pszFileMapping);

	if( hMapFile )
	{
		PVOID pView = MapViewOfFile(
			hMapFile,               // Handle of the map object
			FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
			0,                      // High-order DWORD of the file offset 
			0,						// Low-order DWORD of the file offset 
			0						// The number of bytes to map to view
			);

		return pView;
	}
	VMP_END;
	return NULL;
}

bool CheckLpk::xor(const char* pszData, char* pszOutData)
{
	VMP_BEGIN;
	for (int i=0; i<(int)strlen(pszData); i++)
	{
		pszOutData[i] = pszData[i] ^ 0x77;
	}
	VMP_END;
	return false;
}
