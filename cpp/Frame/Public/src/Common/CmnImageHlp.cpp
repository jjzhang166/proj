#include "stdafx.h"
#include <Common/CmnImageHlp.h>
#include <cassert>

PIMAGE_NT_HEADERS CmnImageHlp::AnalyaisImage( HMODULE hModule )
{
	PBYTE pImage = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pImageDosHeader;
	PIMAGE_NT_HEADERS pImageNtHeader;
	pImageDosHeader = (PIMAGE_DOS_HEADER)pImage;
	if(pImageDosHeader->e_magic==IMAGE_DOS_SIGNATURE)
	{
		pImageNtHeader = (PIMAGE_NT_HEADERS)&pImage[pImageDosHeader->e_lfanew];
		if(pImageNtHeader->Signature==IMAGE_NT_SIGNATURE)
		{
			return pImageNtHeader;
		}
	}
	assert(false);
	return nullptr;
}

DWORD CmnImageHlp::GetPEImageSize( HMODULE hModule )
{
	PBYTE pInfo = (PBYTE)hModule;
	PIMAGE_DOS_HEADER pImgDos = (PIMAGE_DOS_HEADER)pInfo;
	PIMAGE_NT_HEADERS pImgNt;
	if(pImgDos->e_magic==IMAGE_DOS_SIGNATURE)
	{
		pImgNt = (PIMAGE_NT_HEADERS)&pInfo[pImgDos->e_lfanew];
		if(pImgNt)
		{
			if(pImgNt->Signature==IMAGE_NT_SIGNATURE)
			{
				return pImgNt->OptionalHeader.SizeOfImage;
			}
		}
	}
	assert(false);
	return 0;
}
