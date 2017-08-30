#include "StdAfx.h"
#include "Log.h"
#include <direct.h>


Log::Log(void)
{
}

Log::~Log(void)
{
}

void GetCurrentMoudlePatch(CString& csPatch)
{
	TCHAR szPatchName[MAX_PATH] = {0};
	GetModuleFileName(NULL,szPatchName,MAX_PATH);
	CString cs_thefile;
	cs_thefile = szPatchName;
	int n_Pos = cs_thefile.ReverseFind('\\');
	csPatch = cs_thefile.Left(n_Pos);
}

bool Log::OpenFile(const TCHAR* pszPatch)
{
	CFileException ex;

	Init();
	try
	{
		CString	csPath;
		CString	csFilePath;
		GetCurrentMoudlePatch(csPath);
		csPath += _T("\\log");
		csFilePath = csPath;
		csPath += _T("\\");
		csPath += pszPatch;
//		if (!m_theFile.Open(csPath,CFile::shareDenyNone | CFile::modeReadWrite, &ex))
		{
		CREATE_FILE:
			if (!m_theFile.Open(csPath, CFile::shareDenyNone | CFile::modeCreate | CFile::modeReadWrite, &ex))
			{
				if( ex.m_lOsError == 3 )
				{
					_tmkdir(csFilePath.GetBuffer(0));
					goto CREATE_FILE;
				}
				else
				{
					ASSERT(false);
					m_theFile.Close();
					return false;
				}
			}
			
			return true;
		}
	}
	catch (...)
	{
		ASSERT(false);
	}

	return true;
}

void Log::WriteData()
{
	try
	{
		m_theFile.SeekToEnd();
		strcat_s(m_szLog,"\r\n");
		m_theFile.Write(m_szLog,strlen(m_szLog));
		m_theFile.Flush();
		Init();
	}
	catch (...)
	{
		ASSERT(false);
	}
}