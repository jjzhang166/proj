#pragma once
#include "VMProtectSDK.h"



#ifdef __VMP__
template<typename CharT>
const CharT* VMProtectDecryptString_Helper_( const CharT* );
template<>
const char* VMProtectDecryptString_Helper_( const char* str );
template<>
const wchar_t* VMProtectDecryptString_Helper_( const wchar_t* str );
#endif

struct __VMProtectBegin__{
	__VMProtectBegin__( const char* str_id );
	~__VMProtectBegin__();
};
struct __VMProtectBeginUltra__{
	__VMProtectBeginUltra__( const char* str_id );
	~__VMProtectBeginUltra__();
};

#ifdef __VMP__
#ifdef _WIN64
#pragma comment(lib, "VMProtectSDK64.lib")
#else
#pragma comment(lib, "VMProtectSDK32.lib")
#endif

//简单VM
#define VMP_BEGIN VMProtectBegin(__FUNCSIG__)
#define VMP_BEGIN_P(p) VMProtectBegin(p)
//复杂VM
#define VMP_BEGIN_EX VMProtectBeginUltra(__FUNCSIG__)
#define VMP_BEGIN_EX_P(p) VMProtectBeginUltra(p)
void __VMProtectEnd_HELPER__( int* );
#define VMP_END __VMProtectEnd_HELPER__(0), VMProtectEnd()
//保护字符串
#define VMP_PROTECT_STR(procted_str)	VMProtectDecryptString_Helper_(procted_str)
#define VMP_PROTECT_STR_A VMProtectDecryptStringA
//得到当前的机器码
#define VMP_GET_CUR_HWID(buff, buff_len)	VMProtectGetCurrentHWID(buff, buff_len)
//scoped
#define VMP_BEGIN_SCOPE	__VMProtectBegin__(__FUNCSIG__)
#define VMP_BEGIN_SCOPE_EX	__VMProtectBeginUltra__(__FUNCSIG__)
#define VMP_IS_DEBUGGER() VMProtectIsDebuggerPresent(1)
#define VMP_CRC_VALID() VMProtectIsValidImageCRC()

#ifdef UNICODE
#define VMProtectDecryptString VMProtectDecryptStringW
#else
#define VMProtectDecryptString VMProtectDecryptStringA
#endif



#else
#define VMP_BEGIN	 (void*)0
#define VMP_BEGIN_P(p) (void*)0
#define VMP_BEGIN_EX	(void*)0
#define VMP_BEGIN_EX_P(p) (void*)0
#define VMP_END		(void*)0
#define VMP_PROTECT_STR(procted_str)	procted_str
#define VMP_PROTECT_STR_A(procted_str)	procted_str
#define	VMP_GET_CUR_HWID(buff, buff_len)	memset(buff, 'a', buff_len), buff[buff_len - 1] = 0
#define VMP_BEGIN_SCOPE	(void*)0
#define VMP_BEGIN_SCOPE_EX	(void*)0
#define VMP_IS_DEBUGGER() false
#define VMP_CRC_VALID() true

#ifdef UNICODE
#define VMProtectDecryptString 
#else
#define VMProtectDecryptString 
#endif

#endif

#ifdef __DEBUG_PRINT__
#define DEBUG_PRINT(s)	{char szPrint[1024] = {0};	sprintf_s(szPrint,"[check] %s",s);	OutputDebugString(szPrint);}

#else
#define DEBUG_PRINT(s)
#endif