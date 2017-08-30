#include "stdafx.h"
#include "SafeCheck.h"
#include <protect/Protect.h>
#include <Common/UsefulFunc.h>
#include <fstream>

SafeCheckLpk::SafeCheckLpk()
{
	handle_file_map_ = nullptr;
	ptr_map_view_ = nullptr;
}

bool SafeCheckLpk::WriteValue( unsigned int value )
{
	VMP_BEGIN_EX;
	if ( !handle_file_map_ || !ptr_map_view_ )
	{
		assert(false);
		return false;
	}
	const auto& kRandStr = Random_Str(50, 66);
	assert(kKeyValueOffset / 2 + 6 >= 0);
	CopyMemory((char*)ptr_map_view_ + kKeyValueOffset / 2 + 6, kRandStr.c_str(), kRandStr.size());
	*(int*)((char*)ptr_map_view_ + kKeyValueOffset) = value ^ kMemMap_ConstValue;
	VMP_END;
	return true;
}

bool SafeCheckLpk::CreateCheckLpkFileMap()
{
	VMP_BEGIN_EX;
	// Create the file mapping object.
	handle_file_map_ = CreateFileMappingA(
		INVALID_HANDLE_VALUE,   // Use paging file - shared memory
		NULL,                   // Default security attributes
		PAGE_READWRITE,         // Allow read and write access
		0,                      // High-order DWORD of file mapping max size
		0x1000,					// Low-order DWORD of file mapping max size
		VMP_PROTECT_STR(kMemMapName_CheckLpk)           // Name of the file mapping object
		);
	if ( !handle_file_map_ )
	{
		assert(false);
		return false;
	}
	// Map a view of the file mapping into the address space of the current 
	// process.
	ptr_map_view_ = MapViewOfFile(
		handle_file_map_,               // Handle of the map object
		FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		0,						// Low-order DWORD of the file offset 
		0						// The number of bytes to map to view
		);
	VMP_END;
	return !!ptr_map_view_;
}

SafeCheckLpk::~SafeCheckLpk()
{
	if (ptr_map_view_)
		::UnmapViewOfFile(ptr_map_view_);
	if (handle_file_map_)
		::CloseHandle(handle_file_map_);
}

bool SafeCheckLpk::SafeCheckLpkValid()
{
	return true;
}

bool SafeCheckLpk::IsValid() const
{
	VMP_BEGIN_EX;
	return handle_file_map_ && ptr_map_view_;
	VMP_END;
}
