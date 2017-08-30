#include "stdafx.h"
#include "CGL_Defines.h"
#include <protect/Protect.h>
#include <Common/UsefulFunc.h>
#include <Protocol/P_Defines.h>

const std::size_t kCheckAtBufferIdx = 21;

bool CreateCheck_AssistVersion()
{
	VMP_BEGIN_EX;
	// Create the file mapping object.
	HANDLE handle_file_map_ = CreateFileMappingA(
		INVALID_HANDLE_VALUE,   // Use paging file - shared memory
		NULL,                   // Default security attributes
		PAGE_READWRITE,         // Allow read and write access
		0,                      // High-order DWORD of file mapping max size
		0x1000,					// Low-order DWORD of file mapping max size
		VMP_PROTECT_STR(kAssistVersionEventObj)           // Name of the file mapping object
		);
	if ( !handle_file_map_ )
	{
		assert(false);
		return false;
	}
	if ( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		assert(false);
		return false;
	}
	// Map a view of the file mapping into the address space of the current 
	// process.
	LPVOID ptr_map_view_ = MapViewOfFile(
		handle_file_map_,               // Handle of the map object
		FILE_MAP_READ|FILE_MAP_WRITE,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		0,						// Low-order DWORD of the file offset 
		0						// The number of bytes to map to view
		);
	if ( !ptr_map_view_ )
	{
		assert(false);
		return false;
	}
	const auto& kStrUuid = GenUuidStrFromGenerator(std::to_string(kAssistVersion), VMP_PROTECT_STR(kAssistVersionUuidGenerator));
	if (kStrUuid.empty())
	{
		assert(false);
		return false;
	}
	char buffer[100];
	assert(kStrUuid.size() == kUuidStrLen);
	assert(kUuidStrLen < _countof(buffer));
	for ( int i = 0; i != kUuidStrLen; ++i )
	{
		buffer[i + kCheckAtBufferIdx] = kStrUuid[i] ^ kAssistVersionXor;
	}
	CopyMemory((char*)ptr_map_view_, buffer, sizeof(buffer));
	VMP_END;
	return true;
}

bool CheckAssistVersion()
{
	VMP_BEGIN_EX;
	auto handle_file_map = OpenFileMappingA(FILE_MAP_READ, FALSE, VMP_PROTECT_STR(kAssistVersionEventObj));
	if ( !handle_file_map )
	{
		assert(false);
		return false;
	}
	LPVOID ptr_map_view_ = MapViewOfFile(
		handle_file_map,               // Handle of the map object
		FILE_MAP_READ,    // Read and write access
		0,                      // High-order DWORD of the file offset 
		0,						// Low-order DWORD of the file offset 
		0						// The number of bytes to map to view
		);
	if ( !ptr_map_view_ )
	{
		assert(false);
		::CloseHandle(handle_file_map);
		return false;
	}
	const auto& kStrUuid = GenUuidStrFromGenerator(std::to_string(kAssistVersion), VMP_PROTECT_STR(kAssistVersionUuidGenerator));
	const char* ptr_str = (const char*)ptr_map_view_ + kCheckAtBufferIdx;
	for ( int i = 0; i != kUuidStrLen; ++i )
	{
		char c = kStrUuid[i] ^ kAssistVersionXor;
		if ( *(ptr_str + i) != c )
		{
			assert(false);
			::UnmapViewOfFile(ptr_map_view_);
			::CloseHandle(handle_file_map);
			return false;
		}
	}
	::UnmapViewOfFile(ptr_map_view_);
	::CloseHandle(handle_file_map);
	VMP_END;
	return true;
}
