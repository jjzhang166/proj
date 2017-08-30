#include "stdafx.h"
#include <CmnMix/Cmn_UsefulFunc.h>
#include <cassert>
#include <boost/locale/encoding.hpp>
#include <dbghelp.h>
#include <ProtocolX/P_DefinesX.h>
#include <boost/scope_exit.hpp>
#include <Poco/LocalDateTime.h>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/tokenizer.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <CommonX/CmnX_DllApi.h>
#include <AppBase/Cmn_App.h>
#include <CmnMix/Cmn_IoTimer.h>
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "DbgHelp.lib")
//////////////////////////////////////////////////////////////////////////
int GetTzd()
{
	static Poco::LocalDateTime lt;
	return lt.tzd();
}

bool GetModulePath( LPCTSTR module_name, LPTSTR path_buffer, pt_dword buffer_size )
{
	if ( !path_buffer )
		return false;

	return GetModulePath(path_buffer, buffer_size, GetModuleHandle(module_name));
}

bool GetModulePath( LPTSTR path_buffer, pt_dword buffer_size, HMODULE module )
{
	if ( !module )
		return false;
	GetModuleFileName(module, path_buffer, buffer_size);
	auto it = _tcsrchr(path_buffer, _T('\\'));
	assert(it);
	if (it)
	{
		*++it = 0;
	}
	assert(PathFileExists(path_buffer));
	return true;
}

CString GetModulePath( LPCTSTR module_name )
{
	TCHAR szbuffer[MAX_PATH];
	szbuffer[0] = _T('\0');
	if (GetModulePath(module_name, szbuffer, sizeof(szbuffer)))
		return szbuffer;
	else
		return _T("error directory");
}

CString GetPathFromFullFilePath( const CString& full_file_path )
{
	TCHAR buffer[MAX_PATH];
	_tcscpy_s(buffer, full_file_path);
	auto it = _tcsrchr(buffer, _T('\\'));
	if (it)
	{
		*++it = 0;
		return CString(buffer);
	}
	return CString();
}

CString GetFileNameFromFullFilePath( const CString& full_file_path )
{
	auto it = _tcsrchr(full_file_path, _T('\\'));
	if (it)
	{
		return CString(++it);
	}
	return CString();
}

std::string Utf8ToGbk( const char* str_utf8 )
{
	static std::string kGbk("GBK");
	static std::string kUtf8("UTF-8");
	return std::move(boost::locale::conv::between(str_utf8, kGbk, kUtf8));
}

std::string GbkToUtf8( const char* str_gbk )
{
	static std::string kGbk("GBK");
	static std::string kUtf8("UTF-8");
	return std::move(boost::locale::conv::between(str_gbk, kUtf8, kGbk));
}

std::wstring GbkToWideChar( const char* str_gbk )
{
	return std::move(boost::locale::conv::to_utf<wchar_t>(str_gbk, "GBK"));
}

std::wstring GbkToWideChar( const std::string& str_gbk )
{
	return std::move(GbkToWideChar(str_gbk.c_str()));
}

std::string WideCharToGbk( const std::wstring& str_wide )
{
	return std::move(boost::locale::conv::from_utf(str_wide, "GBK"));
}

std::string WideCharToGbk(const RStd::wstring& str_wide)
{
	auto ptr = str_wide.c_str();
	if (!ptr)
	{
		assert(false);
		return kEmptyStr;
	}
	return boost::locale::conv::from_utf(ptr, ptr + str_wide.size(), "GBK");
}

std::string WideCharToGbk(const wchar_t* str_wide)
{
	if (!str_wide)
	{
		assert(false);
		return kEmptyStr;
	}
	return boost::locale::conv::from_utf(str_wide, str_wide + StdEx::TStrLen(str_wide), "GBK");
}

LONG WINAPI HandleCrashImpl(LONG ret_value, PEXCEPTION_POINTERS pExceptPtrs)
{
	if (!Cmn_AppBase::GetAppBase().IsEnableDump())
		return ret_value;
	assert(pExceptPtrs);
	auto cur_time = CTime::GetCurrentTime();

	struct Helper{
		Helper(){
			prefix_ = GetModulePath(nullptr) + _T("CrashDumps_\\");
			::CreateDirectory(prefix_, nullptr);
			prefix_ += _T("dump-");
		}

		CString		prefix_;
	};
	static Helper helper;

	auto dump_file_name = helper.prefix_ + cur_time.Format(_T("%m-%d %I-%M-%S.dmp"));
	HANDLE hDump = ::CreateFile(dump_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,0,0);

	// 写出Dump文件
	MINIDUMP_EXCEPTION_INFORMATION info;
	info.ClientPointers = FALSE;
	info.ExceptionPointers = pExceptPtrs;
	info.ThreadId = ::GetCurrentThreadId();

	MINIDUMP_TYPE dump_type = (MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithDataSegs | MiniDumpWithFullMemory | 
		MiniDumpWithThreadInfo | MiniDumpWithHandleData | MiniDumpWithUnloadedModules);
	::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
		hDump, dump_type, &info, 0, 0);
	::CloseHandle(hDump);
	return ret_value;
}

std::pair<void*, pt_dword>
	LoadRcdataRes( LPCTSTR module_name, LPCTSTR res_name )
{
	return LoadRcdataRes(GetModuleHandle(module_name), res_name);
}

std::pair<void*, pt_dword>
	LoadRcdataRes( HMODULE module, LPCTSTR res_name )
{
	std::pair<void*, pt_dword> result__(nullptr, 0);
	if ( !module )
	{
		assert(false);
		return result__;
	}
	auto res = FindResource(module, res_name, RT_RCDATA);
	if ( !res )
		return result__;

	auto loaded_res = LoadResource(module, res);
	if ( !loaded_res )
		return result__;
	result__.first = LockResource(loaded_res);
	result__.second = SizeofResource(module, res);
	return result__;
}

std::wstring UnsensePerlMetachar( const std::wstring& str_src )
{
	std::wstring str_dst;
	static const wchar_t kMetacharacters[] = L".[{()\\*+?|^$";
	static_assert(13 == _countof(kMetacharacters), "");
	for ( auto& c : str_src )
	{
		for ( int i = 0; i != _countof(kMetacharacters) - sizeof(wchar_t); ++i )
		{
			if ( c == kMetacharacters[i] )
			{
				str_dst += L'\\';
				break;
			}
		}
		str_dst += c;
	}
	return std::move(str_dst);
}

std::string UnsensePerlMetacharA( const std::string& str_src )
{
	std::string str_dst;
	static const char kMetacharacters[] = ".[{()\\*+?|^$";
	static_assert(13 == _countof(kMetacharacters), "");
	for ( auto& c : str_src )
	{
		for ( int i = 0; i != _countof(kMetacharacters) - 1; ++i )
		{
			if ( c == kMetacharacters[i] )
			{
				str_dst += '\\';
				break;
			}
		}
		str_dst += c;
	}
	return std::move(str_dst);
}

HMODULE ModuleFromAddress(pt_pcvoid pv)
{
	MEMORY_BASIC_INFORMATION mbi;
	return((VirtualQuery(pv, &mbi, sizeof(mbi)) != 0) 
		? (HMODULE) mbi.AllocationBase : NULL);
}

std::string Str2Hex( const char* buffer, unsigned int buf_size )
{
	if ( buf_size <= 0 || !buffer )
	{
		assert(false);
		return kEmptyStr;
	}
	
	std::string res;
	char c[3];
	c[2] = 0;
	for ( auto buf_end = buffer + buf_size; buffer < buf_end; )
	{
		if ( !IsHexadecimal(*buffer) )
		{
			++buffer;
			continue;
		}
		for ( auto next_c = buffer + 1; next_c < buf_end; )
		{
			if ( !IsHexadecimal(*next_c) )
			{
				++next_c;
				continue;
			}
			c[0] = *buffer;
			c[1] = *next_c;		
			res.push_back((char)strtol(c, nullptr, 16));
			buffer = next_c + 1;
			goto LableContinue;
		}

		break;
LableContinue:
		continue;
	}
	return res;
}

std::string Hex2str( const char* buffer, unsigned int buf_size )
{
	if ( buf_size <= 0 || !buffer )
	{
		assert(false);
		return kEmptyStr;
	}
	std::string res;
	for ( auto buf_end = buffer + buf_size; buffer < buf_end; ++buffer )
	{
		char buffer[3];
		sprintf_s(buffer, "%x", *buffer);
		if ( strlen(buffer) < 2 )
			res += '0';
		res += buffer;
	}
	return res;
}

bool IsHexadecimal( char c )
{
	if ( c >= '0' && c <= '9' )
		return true;
	if ( c >= 'a' && c <= 'f' )
		return true;
	if ( c >= 'A' && c <= 'F' )
		return true;
	return false;
}

bool CopyTextToClipboard( const char* str, size_t str_size )
{
	if ( !str || str_size <= 0 )
	{
		assert(false);
		return false;
	}
	if ( !OpenClipboard(nullptr) )
	{
		assert(false);
		return false;
	}
	BOOST_SCOPE_EXIT_ALL(&){
		CloseClipboard();
	};
	if ( !EmptyClipboard() )
	{
		assert(false);
		return false;
	}
	HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, str_size + 1);
	if ( !clipbuffer )
	{
		assert(false);
		return false;
	}
	BOOST_SCOPE_EXIT_ALL(clipbuffer){
		GlobalFree(clipbuffer);
	};
	auto buffer = (char*)GlobalLock(clipbuffer);
	if ( !buffer )
	{
		assert(false);
		return false;
	}
	BOOST_SCOPE_EXIT_ALL(clipbuffer){
		GlobalUnlock(clipbuffer);
	};
	strcpy_s(buffer, str_size + 1, str);
	SetClipboardData(CF_TEXT, clipbuffer);
	return true;
}

std::string GetTextFromClipboard()
{
	if ( !OpenClipboard(nullptr) )
	{
		assert(false);
		return kEmptyStr;
	}
	BOOST_SCOPE_EXIT_ALL(&){
		CloseClipboard();
	};
	HANDLE hData = GetClipboardData(CF_TEXT);
	if ( !hData )
	{
		//assert(false);
		return kEmptyStr;
	}
	auto str = (const char*)GlobalLock(hData);
	BOOST_SCOPE_EXIT_ALL(hData){
		GlobalUnlock(hData);
	};
	if ( !str )
	{
		assert(false);
		return kEmptyStr;
	}
	std::string res = str;
	return res;
}

boost::uuids::uuid GenUuidFromBuff(const void* buffer, pt_dword buffer_size)
{
	if (!buffer || buffer_size <= 0)
	{
		boost::uuids::uuid uuid = { 0 };
		return uuid;
	}
	struct Helper{
		Helper(){
			boost::uuids::string_generator gen;
			uuid_ = gen("9AF5C6EE-7244-4243-9007-CE8DB643D162");
		}
		boost::uuids::uuid			uuid_;
	};
	static Helper helper;
	boost::uuids::name_generator gen(helper.uuid_);
	return gen(buffer, buffer_size);
}

void SeparateString(const std::string& full_str, const std::string& sep_char, std::vector<std::string>& out_str)
{
	boost::char_separator<char> sep(sep_char.c_str());
	typedef boost::tokenizer<boost::char_separator<char> >
		CustomTokenizer;
	CustomTokenizer tok(full_str, sep);

	for (CustomTokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg)
		out_str.push_back(*beg);
}

std::string RandomUniqueStr(pt_dword str_size)
{
	//如果str_size少于36个的话，生成的字符串就可能不是唯一的了
	//assert(str_size >= boost::uuids::uuid::static_size());
	boost::uuids::uuid u;
	static boost::uuids::random_generator rgen;
	std::string str_res, str_tmp;
	while (str_size > 0)
	{
		u = rgen();
		str_tmp = boost::uuids::to_string(u);
		auto minus_size = std::min(str_tmp.size(), str_size);
		str_res.append(str_tmp.c_str(), minus_size);
		str_size -= minus_size;
	}
	return str_res;
}

std::string RandomUniqueStrEx(pt_dword str_size /*= 32 */)
{
	boost::uuids::uuid u;
	static boost::uuids::random_generator rgen;
	std::string str_res, str_tmp;
	while (str_size > 0)
	{
		u = rgen();
		str_tmp = Uuid2String(u);
		auto minus_size = std::min(str_tmp.size(), str_size);
		str_res.append(str_tmp.c_str(), minus_size);
		str_size -= minus_size;
	}
	return str_res;
}

std::string Uuid2String(boost::uuids::uuid const& u)
{
	using namespace boost::uuids;
	std::string result;
	for (uuid::const_iterator it_data = u.begin(); it_data != u.end(); ++it_data)
	{
		const size_t hi = ((*it_data) >> 4) & 0x0F;
		result += detail::to_char(hi);

		const size_t lo = (*it_data) & 0x0F;
		result += detail::to_char(lo);
	}
	return result;
}

void XorBufferData(void* out_buffer, const void* buffer_src, pt_dword buffer_size, unsigned char xor_data)
{
	if (IsUnValidPtr(out_buffer))
	{
		assert(false);
		return;
	}
	if (IsUnValidPtr(buffer_src) || buffer_size <= 0)
	{
		assert(false);
		return;
	}
	auto out_c = (unsigned char*)out_buffer;
	auto src_c = (const unsigned char*)buffer_src;
	for (pt_dword i = 0; i != buffer_size; ++i)
	{
		*(out_c + i) = *(src_c + i) ^ xor_data;
	}
}

bool CheckOptionalBoolRes(const OptionalBool& res)
{
	if (!res || !*res)
		return false;
	return true;
}

void* GetThreadStartAddr(HANDLE handle_thread)
{
	if (!handle_thread)
	{
		assert(false);
		return nullptr;
	}
	auto query_information = GetNtQueryInformationThread();
	if (!query_information)
	{
		assert(false);
		return nullptr;
	}
	DWORD dwStaAddr = 0;
	DWORD dwReturnLength = 0;
	if (query_information(handle_thread, ThreadQuerySetWin32StartAddress, &dwStaAddr, sizeof(dwStaAddr), &dwReturnLength))
	{
		assert(false);
		return nullptr;
	}
	return (void*)dwStaAddr;
}

void* GetThreadStartAddr(DWORD thread_id)
{
	auto handle_thread = ::OpenThread(THREAD_QUERY_INFORMATION, FALSE, thread_id);
	if (!handle_thread)
	{
		//assert(false);
		return nullptr;
	}
	BOOST_SCOPE_EXIT_ALL(handle_thread){
		::CloseHandle(handle_thread);
	};
	return GetThreadStartAddr(handle_thread);
}

bool GetModuleName(HMODULE module, pt_sz buffer, pt_dword buffer_size)
{
	if (!module || !buffer || buffer_size <= 10)
	{
		assert(false);
		return false;
	}
	if (GetModuleFileNameA(module, buffer, buffer_size) <= 0)
	{
		assert(false);
		return false;
	}
	auto it_c = strrchr(buffer, '\\');
	if (!it_c)
	{
		assert(false);
		return false;
	}
	++it_c;
	strcpy_s(buffer, buffer_size, it_c);
	return true;
}

bool GetAddressModuleName(pt_pcvoid address, pt_sz buffer, pt_dword buffer_size)
{
	auto module = ModuleFromAddress(address);
	if (!module)
		return false;
	return GetModuleName(module, buffer, buffer_size);
}

void TerminateTheProcess(DWORD process_id)
{
	if (0 == process_id)
	{
		assert(false);
		return;
	}
	auto the_handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
	if (!the_handle)
		return;

	BOOST_SCOPE_EXIT_ALL(the_handle){
		::CloseHandle(the_handle);
	};
	::TerminateProcess(the_handle, 0);
}

void TerminateSelfProcess()
{
	TerminateTheProcess(GetCurrentProcessId());
}

void ForegroundWnd(HWND hWnd)
{
	if (!::IsWindow(hWnd))
	{
		assert(false);
		return;
	}
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	::SetActiveWindow(hWnd);
	::SetForegroundWindow(hWnd);
	::BringWindowToTop(hWnd);

	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

std::pair<bool, char> Ascii2VK(char c){
	typedef std::pair<bool, char> PairT;
	if (std::isdigit(c))
	{
		return PairT(false, c);
	}
	else if (std::islower(c))
	{
		return PairT(false, std::toupper(c));
	}
	else if (std::isupper(c))
	{
		return PairT(true, c);
	}
	struct stHelp{
		stHelp(){
			map_[';'] = PairT(false, VK_OEM_1);
			map_[':'] = PairT(true, VK_OEM_1);
			map_['='] = PairT(false, VK_OEM_PLUS);
			map_['+'] = PairT(true, VK_OEM_PLUS);
			map_[','] = PairT(false, VK_OEM_COMMA);
			map_['<'] = PairT(true, VK_OEM_COMMA);
			map_['-'] = PairT(false, VK_OEM_MINUS);
			map_['_'] = PairT(true, VK_OEM_MINUS);
			map_['.'] = PairT(false, VK_OEM_PERIOD);
			map_['>'] = PairT(true, VK_OEM_PERIOD);
			map_['/'] = PairT(false, VK_OEM_2);
			map_['?'] = PairT(true, VK_OEM_2);
			map_['`'] = PairT(false, VK_OEM_3);
			map_['~'] = PairT(true, VK_OEM_3);
			map_['['] = PairT(false, VK_OEM_4);
			map_['{'] = PairT(true, VK_OEM_4);
			map_['\\'] = PairT(false, VK_OEM_5);
			map_['|'] = PairT(true, VK_OEM_5);
			map_[']'] = PairT(false, VK_OEM_6);
			map_['}'] = PairT(true, VK_OEM_6);
			map_['\''] = PairT(false, VK_OEM_7);
			map_['"'] = PairT(true, VK_OEM_7);
			map_['!'] = PairT(true, VK_1);
			map_['@'] = PairT(true, VK_2);
			map_['#'] = PairT(true, VK_3);
			map_['$'] = PairT(true, VK_4);
			map_['%'] = PairT(true, VK_5);
			map_['^'] = PairT(true, VK_6);
			map_['&'] = PairT(true, VK_7);
			map_['*'] = PairT(true, VK_8);
			map_['('] = PairT(true, VK_9);
			map_[')'] = PairT(true, VK_0);
		}

		std::map<char, PairT>	map_;
	};
	static stHelp hlp;
	auto it = hlp.map_.find(c);
	if (it != hlp.map_.end())
		return it->second;

	//LOG_O(Log_debug) << "unkonw char->" << c;
	assert(false);
	return std::pair<bool, char>(false, c);
}
void InputKeybdEventChar(char c)
{
	if (LOBYTE(GetKeyState(VK_CAPITAL)))
		//关闭大小写锁
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
	const auto& c_info = Ascii2VK(c);
	if (c_info.first)
	{
		//按下Shift键
		keybd_event(VK_SHIFT, 0, 0, 0);
	}
	//按下键
	keybd_event(c_info.second, 0, 0, 0);
	//松开键
	keybd_event(c_info.second, 0, KEYEVENTF_KEYUP, 0);

	if (c_info.first)
	{
		//松开Shift键
		keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	}
}

void InputKeybdEvent(const std::string& str)
{
	if (LOBYTE(GetKeyState(VK_CAPITAL)))
		//关闭大小写锁
	{
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		auto key = *it;
		if (std::isupper(key))
		{
			//按下Shift键
			keybd_event(VK_SHIFT, 0, 0, 0);
			//按下大写的字母键
			keybd_event(key, 0, 0, 0);
			//松开大写的字母键
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
			//松开Shift键
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
			continue;
		}
		else if (std::islower(key))
		{
			key = std::toupper(key);
		}
		//按下键
		keybd_event(key, 0, 0, 0);
		//松开键
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
	}
}

void __stdcall DummySleep(unsigned long milli_seconds)
{
	if (milli_seconds <= 0)
	{
		assert(false);
		return;
	}
#ifdef _DEBUG
	auto& app = Cmn_AppBase::GetAppBase();
	if (!app.FindThrd(wxThread::GetCurrentId()))
	{
		assert(false);
		return;
	}
#endif
	DEFINE_DEFAULT_PAUSE_ID(milli_seconds);
	try{
		auto timer = CreateIoTimer(milli_seconds, false, [pause_id](const IoTimerPtr&){
			throw CmnPauseLogicException(pause_id, "dummysleep");
		});
		if (!timer)
		{
			assert(false);
			return;
		}
		timer->Continue();
		BOOST_SCOPE_EXIT_ALL(&timer){
			assert(timer);
			timer->Cancel();
		};
		auto& io_ser = GetIoService();
		while (io_ser.RunOne() > 0) {}
	} DO_CATCH_DEFAULT_PAUSE_ID_DO_NOTHING();
}

Cmn_Threadable::ThrdIoService& GetIoService(){
	return Cmn_Threadable::GetCurThreadable().GetIoService();
}
