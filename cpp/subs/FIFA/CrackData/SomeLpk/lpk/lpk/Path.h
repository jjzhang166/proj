#pragma once
#include <CD_CommonFiles/CD_CmnDefines.h>

BOOL GetProcessHostFullName(char* pszFullFileName)
{
	DWORD dwResult = 0;
	::ZeroMemory((PBYTE)pszFullFileName, MAX_PATH);
	if (TRUE != ::IsBadReadPtr((PBYTE)pszFullFileName, MAX_PATH))
		dwResult = ::GetModuleFileNameA(
		NULL,                   // handle to module
		pszFullFileName,        // file name of module
		MAX_PATH                // size of buffer
		);

	return (dwResult != 0);
}

BOOL GetProcessHostName(char* pszFullFileName)
{
	BOOL  bResult;
	char  *pdest;

	int   ch = '\\';

	bResult = GetProcessHostFullName(pszFullFileName);
	if (bResult)
	{
		// Search backward 
		pdest = strrchr(pszFullFileName, ch);
		if( pdest != NULL )
			strcpy_s(pszFullFileName, REVERSAL_SOCK_BUFFER_SIZE, &pdest[1]);
	} // if

	return bResult;
}

BOOL GetProcessPatch(char* pszPatch)
{
	BOOL  bResult;
	char  *pdest;

	int   ch = '\\';

	bResult = GetProcessHostFullName(pszPatch);
	if (bResult)
	{
		// Search backward 
		pdest = strrchr(pszPatch, ch);
		if( pdest != NULL )
		{
			pdest[1] = 0;
		}
	} // if

	return bResult;
}
