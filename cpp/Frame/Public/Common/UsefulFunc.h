#pragma once
/*
@author zhp
@date 2013-12-29
@purpose utilities policy
*/
#include "stdafx.h"
#include <boost/asio/io_service.hpp>
#include <Poco/Types.h>
#include <atlpath.h>
#include <boost/filesystem.hpp>
#include <random>
#include <boost/uuid/uuid.hpp>
#include <Net/PocoMore.h>
#include <Protocol/P_Defines.h>
#include <Net/IoService.h>
#include <CommonX/CmnX_UsefulFunc.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include "../DependSelfLib.h"
#include <AppBase/Cmn_Thrd.h>

//////////////////////////////////////////////////////////////////////////
//******************extern***********************************************
extern TimeDuration& GetGlobalTimeDuration();
//////////////////////////////////////////////////////////////////////////

//挂起除了当前线程之外的线程
void WINAPI SupendProcess( bool is_suspend );

#ifdef UNICODE
#define InjectLib InjectLibW
#define EjectLib  EjectLibW
#else
#define InjectLib InjectLibA
#define EjectLib  EjectLibA
#endif
BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile);
BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR pszLibFile);
BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile);
BOOL WINAPI EjectLibA(DWORD dwProcessId, PCSTR pszLibFile);

//DetourCreateProcessWithDll 创建进程的时候可以完美地注入dll
bool LoadLib( const char* lib_name );
bool FreeLib( const char* lib_name );

//提升当前进程的权限
bool AdjustProcessTokenPrivilege();
//////////////////////////////////////////////////////////////////////////
//选择文件夹，并获得目录
CString GetBrowseFolder( HWND pParent );

//load rcdata resource
//保存资源到文件到
bool SaveRcdataAsFile( HMODULE module, LPCTSTR res_name, const CPathW& full_file_name );
bool SaveRcdataAsFile(LPCTSTR module_name, LPCTSTR res_name, const CPathW& full_file_name);
bool SaveRcdataAsFile(HMODULE module, LPCTSTR res_name, const CPathA& full_file_name);
bool SaveRcdataAsFile(LPCTSTR module_name, LPCTSTR res_name, const CPathA& full_file_name);
//文件有变化的才会保存
bool CopyUnsameFile( const std::string& file_src, const std::string& file_dst );

std::random_device& GetDltRandomDevice();
//某个数值区间的随机数,比如[0, 100]
int RandomNum( int min_num, int max_num );
//根据出现概率判断是否出现
bool IsOccuredByOccurRate( double occur_rate );
//随机生成指定长度范围的字母与数字的组合,[cnt_min, cnt_max]
std::string Random_Str( size_t cnt_min, size_t cnt_max );
std::wstring Random_StrW( size_t cnt_min, size_t cnt_max );
//只生成字母，不包含数字
std::string Random_LetterStr(size_t cnt_min, size_t cnt_max);
//从一个字符串生成一个固定的uuid字符串
std::string GenUuidStr( const std::string& str_src );
std::string GenUuidStr( const void* buffer, pt_dword buffer_size );
std::string GenUuidStr( std::istream& istm );
std::string GenUuidStrOfFile( const std::string& full_file_name );
//根据一个生成器生成一个固定的uuid字符串
std::string GenUuidStrFromGenerator( const std::string& str_src, const std::string& str_generator );
//生成一个新的RSA加密文件
bool GenNewRsaKeyFile( const std::string& str_out_to_pub_file, const std::string& str_out_to_pri_file, const std::string& str_private_key );

//////////////////////////////////////////////////////////////////////////
//sleep,但内部依然会处理消息
bool WaitUntil(const FunctionBool& func_until);
bool WaitUntil(pt_dword timeout, const FunctionBool& func_until);
bool DoWaitUntil(pt_dword timeout, const FunctionBool& func_until, const FunctionVoid& func_do);
bool TimerDoWaitUntil(pt_dword timer_time, pt_dword timeout, const FunctionBool& func_until, const FunctionVoid& func_do);
namespace Details{
	template<>
	struct stTimerDoByPolicy<tagTimerDoDummySleep>{
		static bool TimerDoImpl(pt_dword every_sleep, pt_dword time_out, const FunctionBool& func_do);
	};
};
//////////////////////////////////////////////////////////////////////////

//数值拼接
unsigned __int64 MakeInt64( Poco::UInt32 lower, Poco::UInt32 upper );
//数值分割,first==lower,second==upper
std::pair<Poco::UInt32, Poco::UInt32> SplitInt64( Poco::UInt64 big_value );

//机器码相关
//获得机器码
bool GetHWID( char* out_buffer, pt_dword buffer_size );
//得到混淆的机器码
bool GetMixHWID( char* out_buffer, pt_dword buffer_size );
//硬盘的物理序列号
bool GetPhyHardDiskId( std::string& str_buffer );
//得到CPU id
bool GetCpuId( std::string& str_buffer );
//得到网卡mac
bool GetMac( std::string& str_buffer );
//得到磁盘逻辑序列号
bool GetAllDiskSubareaSN( std::string& str_buffer );
//得到硬盘分区的容量
bool GetAllDiskSubareaSize( std::string& str_buffer );
//得到操作系统版本号
bool GetOsVersionStr( std::string& str_buffer );
//得到指定硬件的序列号
bool GetTheSerialNumber( const char* const class_name, std::string& out_str );
//得到bios编号
bool GetBiosNumber( std::string& str_buffer );
//得到主板编号
bool GetBaseBoardNumber( std::string& str_buffer );
//得到硬盘编号
bool GetPhysicalMediaNumber( std::string& str_buffer );

//显示一个弹出式菜单
void ShowPopupMenu( UINT resource_id, CWnd* parent );

//验证唯一实例
bool CheckSingletonHandle( const std::string& str_handle_name );

//迭代文件
bool DoFileItem( const std::string& path_file, const std::function<void( const boost::filesystem::directory_iterator& )>& func );
bool DoExtensionFileItem( const std::string& path_file, const std::string& file_extension, const std::function<void( const boost::filesystem::directory_iterator& )>& func );
//创建多级文件夹
int CreateDepthDirectory( const CPath& dir_path );
void CopyAllFilesOfDirectory( const boost::filesystem::path &src, const boost::filesystem::path &dst, const std::function<bool( const boost::filesystem::directory_iterator& )>* but_func );
std::string GetSystemFolderPath(int folder);
std::string GetMyDocumentsFolderPath();

//域名解析
std::string Domain2Ip( const std::string& domain, unsigned short int port = 0 );
//////////////////////////////////////////////////////////////////////////
//*******************algorithm extern*************************************
//与标准库的unique不同，它不会更改重复元素的内容
template<class _FwdIt> inline
	_FwdIt Unique(_FwdIt _First, _FwdIt _Last)
{
	if (_First != _Last)
		for (_FwdIt _Firstb; (_Firstb = _First), ++_First != _Last; )
			if (*_Firstb == *_First)
			{	// copy down
				for (; ++_First != _Last; )
					if (!(*_Firstb == *_First))
					{
						++_Firstb;
						std::swap(*_Firstb, *_First);
					}
					return (++_Firstb);
			}
	return (_Last);
}
//////////////////////////////////////////////////////////////////////////
//注册表操作
void AutoRegLpk();
//重启运行程序
void AutoRunAtSystemBegin( const std::string& reg_value, const std::string& run_param );
void DeleteAutoRunAtSystemBegin( const std::string& reg_value );
//////////////////////////////////////////////////////////////////////////
//重启系统
void RestartSystem();
//////////////////////////////////////////////////////////////////////////
boost::optional<std::wstring> Regex_SearchFirstSubStr( const std::wstring& full_str, const std::wstring& str_pattern );
std::vector<std::wstring> Regex_FindAllTheStr( const std::wstring& full_str, const std::wstring& str_pattern );
OptionalBool Regex_MatchAllSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& matched_str );
std::wstring Regex_ReplaceSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& str_fmt );
boost::optional<std::wstring> Regex_ReplaceUnsameSubStr( const std::wstring& full_str, const std::wstring& str_pattern, const std::wstring& str_fmt );
bool Regex_ModifySubStrOfFile( const std::string& full_file_name, const std::string& str_pattern, const std::string& str_fmt );
bool Regex_ModifySubStrOfFileByStrTag( const std::string& full_file_name, const std::string& str_tag, const std::string& new_str );
bool Regex_ModifySubStrOfFileByStrTagIncInt( const std::string& full_file_name, const std::string& str_tag );
//////////////////////////////////////////////////////////////////////////
//拨号宽带是否连接
bool IsDialBroadbandConnecting();
//断开拨号宽带连接
int CloseDialBroadband();
//重连拨号宽带
bool ReDialBroadband();
//////////////////////////////////////////////////////////////////////////
DateTimeLocal G_GetSerCurTime();
void G_SetCurTime(const std::time_t& time);
//////////////////////////////////////////////////////////////////////////
void WriteQqLoginRecord(const std::string& qq_num, pt_dword service_id, const std::string& full_file_name_path);
