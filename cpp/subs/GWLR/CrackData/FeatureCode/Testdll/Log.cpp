#include "StdAfx.h"
#include "Log.h"
#include <direct.h>


Log::Log(void)
{
}

Log::~Log(void)
{
}

void GetCurrentMoudlePatch(CStringA& csPatch)
{
	char szPatchName[MAX_PATH] = {0};
	GetModuleFileNameA(NULL,szPatchName,MAX_PATH);
	CStringA cs_thefile;
	cs_thefile = szPatchName;
	int n_Pos = cs_thefile.ReverseFind('\\');
	csPatch = cs_thefile.Left(n_Pos);
}

stLogFlush MakeLogFlush( long line )
{
	stLogFlush tmp = { line };
	return tmp;
}

bool Log::OpenFile(const char* pszPatch)
{
	CFileException ex;

	Init();
	try
	{
		CStringA	csPath;
		CStringA	csFilePath;
		GetCurrentMoudlePatch(csPath);
		csPath += "\\log";
		csFilePath = csPath;
		csPath += "\\";
		csPath += pszPatch;
//		if (!m_theFile.Open(csPath,CFile::shareDenyNone | CFile::modeReadWrite, &ex))
		{
CREATE_FILE:
			if (!m_theFile.Open(CA2CT(csPath), CFile::shareDenyNone | CFile::modeCreate | CFile::modeReadWrite, &ex))
			{
				if( ex.m_lOsError == 3 )
				{
					_mkdir(csFilePath.GetBuffer(0));
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