#include "stdafx.h"
#include <Common/UsefulClass.h>
#include <Net/IoTimer.h>
#include <Common/LogOut.h>
#include <Common/CmnException.h>
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>
#include <Poco/StreamCopier.h>
#include <Common/Toolhelp.h>
#include <wx/wxprec.h>

const std::string& LocalInfo::GetMachineCodeSysFileInfo()
{
	static std::string file_info;
	if (file_info.size() == kUuidStrLenEx)
		return file_info;
	file_info.clear();

	char buffer[MAX_PATH] = { 0 };
	GetSystemDirectoryA(buffer, sizeof(buffer));
	CPathA path_sys = buffer;
	if (!path_sys.IsDirectory())
	{
		assert(false);
		throw CmnExitIoThreadException("jfpqieui");
	}
	auto path_tmp = path_sys;
	path_tmp.Append("win32splcdapi.dll");
	std::fstream fstm(path_tmp);

	if (fstm)
	{
		Poco::StreamCopier::copyToString(fstm, file_info);
	}
	if (file_info.size() != kUuidStrLenEx)
	{
		file_info = RandomUniqueStrEx(kUuidStrLenEx);
		fstm.close();
		fstm.open(path_tmp, std::ios_base::out | std::ios_base::trunc);
		fstm.write(file_info.c_str(), file_info.size());
		fstm.close();
	}
	assert(kUuidStrLenEx == file_info.size());
	return file_info;
}

ModuleReplace::ModuleReplace( HMODULE module )
{
	old_resource_handle_ = AfxGetResourceHandle();
	old_instance_handle_ = wxGetInstance();
	SetNewHandle(module, module);
}

ModuleReplace::ModuleReplace( HMODULE new_resource_handle, HMODULE new_instance_handle )
{
	old_resource_handle_ = AfxGetResourceHandle();
	old_instance_handle_ = wxGetInstance();
	SetNewHandle(new_resource_handle, new_instance_handle);
}

ModuleReplace::~ModuleReplace()
{
	SetNewHandle(old_resource_handle_, old_instance_handle_);
}

void ModuleReplace::SetNewHandle( HMODULE new_resource_handle, HMODULE new_instance_handle )
{
	AfxSetResourceHandle(new_resource_handle);
	afxCurrentInstanceHandle = new_instance_handle;
}

bool RuntimeClassReg::RegisterRuntimeClass( size_t runtime_class_id, const CRuntimeClass* runtime_class )
{
	if ( !runtime_class )
		return false;
	auto it = map_runtime_.find(runtime_class_id);
	if ( it != map_runtime_.end() )
		return false;
	map_runtime_[runtime_class_id] = const_cast<CRuntimeClass*>(runtime_class);
	return true;
}

CRuntimeClass* RuntimeClassReg::FindRuntimeClass( size_t runtime_class_id ) const
{
	auto it = map_runtime_.find(runtime_class_id);
	if ( it != map_runtime_.end() )
		return it->second;
	return nullptr;
}

void RuntimeClassReg::RemoveRuntimeClass( size_t runtime_class_id )
{
	map_runtime_.erase(runtime_class_id);
}

RuntimeClassReg::~RuntimeClassReg()
{
}

FrameCheck FrameCheck::only_one_;


void FrameCheck::TryCheck()
{
	if (FailedCheck_())
	{
		SetIoTimer1(5 * 1000, []( IoTimer& timer ){
			int* i;
			int tmp = 0;
			i = (int*)tmp;
			i += 10;
			tmp = *i;
		});
	}
}

bool FrameCheck::FailedCheck_()
{
	__try{
		Check();
	}
	__except(1){
		return true;
	}
	return false;
}

FrameCheck::FrameCheck()
{

}

SmartCondition::SmartCondition()
{
	cond_true_ = false;
}

void SmartCondition::ResetCondition()
{
	cond_true_ = false;
}

bool SmartCondition::CondtionTrue() const
{
	return cond_true_;
}

void SmartCondition::TouchCond()
{
	cond_true_ = true;
}

bool SmartCondition::WaitTouch()
{
	if (cond_true_)
		return true;
	WaitUntil([this]() -> bool{
		return cond_true_;
	});
	return cond_true_;
}

bool SmartCondition::WaitTouch( pt_dword timeout )
{
	if (cond_true_)
		return true;
	if ( timeout <= 0 )
		return WaitTouch();

	WaitUntil(timeout, [this]() -> bool{
		return cond_true_;
	});
	return cond_true_;
}

bool SmartCondition::TimerDoWaitTouch(pt_dword timer_milli_seconds, const DoFuncT& func_do)
{
	if (cond_true_)
		return true;
	if ( timer_milli_seconds <= 0 )
	{
		assert(false);
		return false;
	}
	return TimerDoWaitUntil(timer_milli_seconds, 0, [this]() -> bool{
		return cond_true_;
	}, func_do);
}

bool SmartCondition::TimerDoWaitTouch(pt_dword timer_milli_seconds, pt_dword timeout, const DoFuncT& func_do)
{
	if (cond_true_)
		return true;
	if ( timeout <= 0 )
		return TimerDoWaitTouch(timer_milli_seconds, func_do);
	if ( timer_milli_seconds <= 0 )
	{
		assert(false);
		return false;
	}
	return TimerDoWaitUntil(timer_milli_seconds, timeout, [this]() -> bool{
		return cond_true_;
	}, func_do);
}

SmartCondition::AutoResetDo::AutoResetDo( SmartCondition& cond ) : cond_(cond)
{
	cond.ResetCondition();
}

bool SmartCondition::AutoResetDo::operator()()
{
	return cond_.WaitTouch();
}

bool SmartCondition::AutoResetDo::operator()(pt_dword timeout)
{
	return cond_.WaitTouch(timeout);
}

bool SmartCondition::AutoResetDo::operator()(pt_dword timer_milli_seconds, const DoFuncT& func_do)
{
	return cond_.TimerDoWaitTouch(timer_milli_seconds, func_do);
}

bool SmartCondition::AutoResetDo::operator()(pt_dword timer_milli_seconds, pt_dword timeout, const DoFuncT& func_do)
{
	return cond_.TimerDoWaitTouch(timer_milli_seconds, timeout, func_do);
}

IniProfile::IniProfile( const std::string& profile_full_file_name ) : profile_name_(profile_full_file_name)
{

}

IniProfile::IniProfile()
{

}

void IniProfile::SetProfile( const std::string& profile )
{
	profile_name_ = profile;
}

const std::string& IniProfile::GetProfile() const
{
	return profile_name_;
}

RandomCombineStr::RandomCombineStr()
{
	ClearAllData();
}

void RandomCombineStr::ClearAllData()
{
	strs_pre_.clear();
	strs_middle_.clear();
	strs_suffix_.clear();
	strs_random_.clear();
	/*max_len_ = 1;
	min_len_ = 1;
	occur_rate_pre_ = 0.01;
	occur_rate_middle_ = 0.01;
	occur_rate_suffix_ = 0.01;*/
}

void RandomCombineStr::SetTotalLen_MinMax( int min_len, int max_len )
{
	if ( max_len >= 0 && min_len >= 0 )
	{
		if ( max_len < min_len )
		{
			assert(false);
			return;
		}
		min_len_ = min_len;
		max_len_ = max_len;
	}
}

void RandomCombineStr::SetOccurRate_Pre( double rate_pre )
{
	if ( rate_pre >= 0 )
		occur_rate_pre_ = rate_pre;
}

void RandomCombineStr::SetOccurRate_Middle( double rate_middle )
{
	if ( rate_middle >= 0 )
		occur_rate_middle_ = rate_middle;
}

void RandomCombineStr::SetOccurRate_Suffix( double rate_suffix )
{
	if ( rate_suffix >= 0 )
		occur_rate_suffix_ = rate_suffix;
}

void RandomCombineStr::AddStr_Pre( const std::string& str_pre )
{
	strs_pre_.Add(str_pre);
	//strs_pre_.Add(FilterValidStr(str_pre));
}

void RandomCombineStr::AddStr_Middle( const std::string& str_middle )
{
	strs_middle_.Add(str_middle);
	//strs_middle_.Add(FilterValidStr(str_middle));
}

void RandomCombineStr::AddStr_Suffix( const std::string& str_suffix )
{
	strs_suffix_.Add(str_suffix);
	//strs_suffix_.Add(FilterValidStr(str_suffix));
}

void RandomCombineStr::AddRandomStr( const std::string& str_random )
{
	strs_random_.Add(str_random);
	//strs_random_.Add(FilterValidStr(str_random));
}

std::string RandomCombineStr::GenRandomStr( const std::string& str_base ) const
{
	if (strs_pre_.empty() && strs_middle_.empty() && strs_suffix_.empty() && strs_random_.empty())
		return kEmptyStr;
	std::string str_base_tmp;
	const std::string* str_base_ptr = &str_base_tmp;
	if ( str_base.size() < max_len_ )
		str_base_ptr = &str_base;

	int cur_char_max_cnt = 0;
	std::string str_res;
	if (str_base_ptr->empty())
	{
		cur_char_max_cnt = RandomNum(min_len_, max_len_);
		OptionalString str_pre;
		if (IsOccuredByOccurRate(occur_rate_pre_))
			str_pre = GetRandomOneStrBy(strs_pre_);
		if ( str_pre && !str_pre->empty() && (int)str_pre->size() <= cur_char_max_cnt )
		{
			cur_char_max_cnt -= str_pre->size();
			str_res += *str_pre;
		}
	}
	else
	{
		if ( max_len_ <= str_base_ptr->size() )
		{
			assert(max_len_ == str_base_ptr->size());
			str_base_ptr = &str_base_tmp;
			cur_char_max_cnt = RandomNum(min_len_, max_len_);
		}
		else
		{
			auto the_min_len = std::max(1, (int)min_len_ - (int)str_base_ptr->size() - 1);
			auto the_max_len = (int)max_len_ - (int)str_base_ptr->size() - 1;
			if ( the_min_len > the_max_len )
			{
				str_base_ptr = &str_base_tmp;
				cur_char_max_cnt = RandomNum(min_len_, max_len_);
			}
			else
			{
				cur_char_max_cnt = RandomNum(the_min_len, the_max_len);
				str_res += *str_base_ptr;
				str_res += Random_Str(1, 1);
			}
		}
	}

	OptionalString str_middle;
	if (IsOccuredByOccurRate(occur_rate_middle_))
		str_middle = GetRandomOneStrBy(strs_middle_);

	OptionalString str_suffix;
	std::string the_str_suffix;
	if (IsOccuredByOccurRate(occur_rate_suffix_))
		str_suffix = GetRandomOneStrBy(strs_suffix_);
	if ( str_suffix && !str_suffix->empty() && (int)str_suffix->size() <= cur_char_max_cnt )
	{
		cur_char_max_cnt -= str_suffix->size();
		the_str_suffix = *str_suffix;
	}
	
	const auto& the_random_str = GetRandomSomeStrBy(strs_random_, str_middle, cur_char_max_cnt);
	cur_char_max_cnt -= the_random_str.size();
	str_res += the_random_str;
	str_res += the_str_suffix;
	
	if ( str_res.size() < min_len_ )
	{
		while ( str_res.size() < min_len_ )
		{
			auto& the_suffix = GetRandomOneStrBy(strs_suffix_);
			if (the_suffix.empty())
				break;
			if ( str_res.size() + the_suffix.size() > max_len_ )
				break;
			str_res += the_suffix;
		}
		if ( str_res.size() < min_len_ )
		{
			auto the_str_size = min_len_ - str_res.size();
			const auto& the_str_tmp = Random_Str(the_str_size, the_str_size);
			assert(the_str_tmp.size() == the_str_size);
			str_res += the_str_tmp;
		}		
	}
	if ( str_res.size() <= max_len_ )
		return str_res;

	assert(false);
	return kEmptyStr;
}

std::vector<std::string> RandomCombineStr::SeparateTheStr( const std::string& the_str )
{
	std::vector<std::string> res;
	SeparateString(the_str, " ,，", res);
	//Regex_FindAllTheStr(GbkToWideChar(the_str), L"\\<\\w+\\>");
	return res;
}

void RandomCombineStr::AddSomeStr_Pre( const std::string& str_some_pre )
{
	for ( auto& v : SeparateTheStr(str_some_pre) )
	{
		AddStr_Pre(v);
	}	
}

void RandomCombineStr::AddSomeStr_Middle( const std::string& str_some_middle )
{
	for ( auto& v : SeparateTheStr(str_some_middle) )
	{
		AddStr_Middle(v);
	}
}

void RandomCombineStr::AddSomeStr_Suffix( const std::string& str_some_suffix )
{
	for ( auto& v : SeparateTheStr(str_some_suffix) )
	{
		AddStr_Suffix(v);
	}
}

void RandomCombineStr::AddSomeRandomStr( const std::string& str_some_random )
{
	for ( auto& v : SeparateTheStr(str_some_random) )
	{
		AddRandomStr(v);
	}
}

const std::string& RandomCombineStr::GetRandomOneStrBy( const StrT& strs )
{
	auto& str_values = strs.GetValues();
	if (str_values.empty())
		return kEmptyStr;
	auto the_num = RandomNum(0, str_values.size() - 1);
	return str_values[the_num];
}

std::string RandomCombineStr::GetRandomSomeStrBy(const StrT& strs, OptionalString str_middle, pt_dword max_char_size)
{
	if ( max_char_size <= 0 )
		return kEmptyStr;
	auto& the_strs_values = strs.GetValues();
	if (the_strs_values.empty())
		return kEmptyStr;
	auto need_cnt = max_char_size;
	std::string res;
	while ( need_cnt > 0 )
	{
		const std::string* str_tmp_ptr = &GetRandomOneStrBy(strs);
		if ( str_tmp_ptr->size() > need_cnt )
		{
			str_tmp_ptr = &GetRandomOneStrBy(strs);
			if ( str_tmp_ptr->size() > need_cnt )
				break;
		}
		if ( str_middle && !res.empty() )
		{
			if ( str_tmp_ptr->size() + str_middle->size() <= need_cnt )
			{
				res += *str_middle;
				need_cnt -= str_middle->size();
				str_middle.reset();
			}
		}

		if ( need_cnt < str_tmp_ptr->size() )
		{
			assert(false);
			break;
		}

		res += *str_tmp_ptr;
		need_cnt -= str_tmp_ptr->size();
		assert(need_cnt >= 0);
		//break;
	}
	return res;
}

std::string RandomCombineStr::FilterValidStr( const std::string& str )
{
	const auto& the_strs = Regex_FindAllTheStr(GbkToWideChar(str), L"\\<\\w+\\>");
	if (the_strs.empty())
		return kEmptyStr;
	return WideCharToGbk(the_strs[0]);
}

RandomCombineStr& RandomCombineStr::GetDefMe()
{
	static RandomCombineStr me;
	return me;
}

void RandomCombineStr::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<RandomCombineStr>("RandomCombineStr")
		.def("设置组合后的总字符最大最小长度", &RandomCombineStr::SetTotalLen_MinMax)
		.def("设置前缀字符出现几率", &RandomCombineStr::SetOccurRate_Pre)
		.def("设置间隔字符出现几率", &RandomCombineStr::SetOccurRate_Middle)
		.def("设置后缀字符出现几率", &RandomCombineStr::SetOccurRate_Suffix)
		.def("添加前缀字符串", &RandomCombineStr::AddSomeStr_Pre)
		.def("添加间隔字符串", &RandomCombineStr::AddSomeStr_Middle)
		.def("添加后缀字符串", &RandomCombineStr::AddSomeStr_Suffix)
		.def("添加随机字符串", &RandomCombineStr::AddSomeRandomStr)
		.def("清空", &RandomCombineStr::ClearAllData),
		def("中文角色名支持", &RandomCombineStr::GetDefMe)
	];
}

const std::string& CookieHelper::GetCookie( const std::string& whose )
{
	auto it = cookies_.find(whose);
	if ( it != cookies_.end() )
		return it->second;
	const auto& the_cookie = LocalInfo::GetTheCookie(whose);
	if ( !whose.empty() && !the_cookie.empty() )
	{
		cookies_[whose] = the_cookie;
		return cookies_[whose];
	}
	return kEmptyStr;
}

bool CookieHelper::UpdateCookie( const std::string& whose, const std::string& new_cookie )
{
	if (new_cookie.empty())
		return false;
	if (whose.empty())
		return false;
	auto it = cookies_.find(whose);
	if ( it != cookies_.end() )
	{
		if ( it->second == new_cookie )
			return true;
		it->second = new_cookie;
	}
	else
		cookies_[whose] = new_cookie;
	LocalInfo::SetNewCookie(whose, new_cookie);
	return true;
}

std::string LocalInfo::GetTheMachineCode()
{
	//防止修改内存数据，注释掉吧
	static std::string the_mcode;
	if (!the_mcode.empty())
		return GenUuidStr(the_mcode);
	//std::string the_mcode;
	char buffer[kUuidStrLen * 5 + 1] = { 0 };
#ifndef _DEBUG
	if (!GetMixHWID(buffer, kUuidStrLen * 5))
#endif
		VMP_GET_CUR_HWID(buffer, sizeof(buffer));
	the_mcode = buffer;
	the_mcode += GetMachineCodeSysFileInfo();
	the_mcode = GenUuidStr(the_mcode);
	return GenUuidStr(the_mcode);
}

/*
std::string LocalInfo::GetTheCookie(const std::string& whose)
{
	std::string the_cookie;
	HKEY the_key = nullptr;
	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, kCookieRegKey, 0, KEY_QUERY_VALUE, &the_key))
	{
		assert(false);
		return the_cookie;
	}
	if (!the_key)
	{
		assert(false);
		return the_cookie;
	}
	char szbuffer[MAX_PATH] = { 0 };

	DWORD value_type = REG_SZ;
	DWORD the_size = _countof(szbuffer);

	std::string whose_tmp = whose;
	whose_tmp += "123";
	auto str_tmp = GenUuidStr(whose_tmp);
	if (ERROR_SUCCESS == RegQueryValueExA(the_key, str_tmp.c_str(), nullptr, &value_type, (BYTE*)szbuffer, &the_size))
		the_cookie = szbuffer;
	RegCloseKey(the_key);

	ZeroMemory(szbuffer, sizeof(szbuffer));
	GetSystemDirectoryA(szbuffer, sizeof(szbuffer));
	CPathA path_sys = szbuffer;
	if (!path_sys.IsDirectory())
	{
		assert(false);
		throw CmnExitIoThreadException("euiqcnjlkqldq");
	}
	auto path_tmp = path_sys;
	path_tmp.Append(kCookieFileName);

	IniProfile pro_file;
	pro_file.SetProfile(std::string(path_tmp));
	std::string str_tmp;
	whose_tmp = whose;
	whose_tmp += "%^$";
	pro_file.Read("1", GenUuidStr(whose_tmp), str_tmp, kEmptyStr);

	the_cookie += str_tmp;

	return the_cookie;
}

void LocalInfo::SetNewCookie(const std::string& whose, const std::string& str_cookie)
{
	std::string cookie_tmp(str_cookie.begin(), str_cookie.begin() + str_cookie.size() / 2);
	HKEY the_key = nullptr;
	if (ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE, kCookieRegKey, 0, KEY_SET_VALUE, &the_key))
	{
		assert(false);
		return;
	}
	if (!the_key)
	{
		assert(false);
		return;
	}
	char szbuffer[MAX_PATH] = { 0 };
	strcat_s(szbuffer, cookie_tmp.c_str());

	std::string whose_tmp = whose;
	whose_tmp += "123";
	auto str_tmp = GenUuidStr(whose_tmp);
	VERIFY(ERROR_SUCCESS == RegSetValueExA(the_key, str_tmp.c_str(), 0, REG_SZ, (const BYTE*)szbuffer, (strlen(szbuffer) + 1) * sizeof(char)));
	RegCloseKey(the_key);

	ZeroMemory(szbuffer, sizeof(szbuffer));
	GetSystemDirectoryA(szbuffer, sizeof(szbuffer));
	CPathA path_sys = szbuffer;
	if (!path_sys.IsDirectory())
	{
		assert(false);
		throw CmnExitIoThreadException("euiqcnjlkqldq");
	}
	auto path_tmp = path_sys;
	path_tmp.Append(kCookieFileName);

	cookie_tmp.assign(str_cookie.begin() + str_cookie.size() / 2, str_cookie.end());
	IniProfile pro_file;
	pro_file.SetProfile(std::string(path_tmp));
	whose_tmp = whose;
	whose_tmp += "%^$";
	auto str_tmp = GenUuidStr(whose_tmp);
	pro_file.Write("1", str_tmp.c_str(), cookie_tmp);
}*/

std::string LocalInfo::GetTheCookie(const std::string& whose)
{
	boost::filesystem::path file_path = GetMyDocumentsFolderPath();
	file_path /= kCookieFileName;
	if (!AS_Node::MakeDirs(file_path))
	{
		assert(false);
		throw CmnExitIoThreadException("euiqcnjlkqldq");
	}

	IniProfile pro_file;
	pro_file.SetProfile(file_path.string());
	std::string str_tmp;
	std::string whose_tmp = whose;
	whose_tmp += "%^$";
	pro_file.Read("1", GenUuidStr(whose_tmp), str_tmp, kEmptyStr);
	return str_tmp;
}

void LocalInfo::SetNewCookie(const std::string& whose, const std::string& str_cookie)
{
	boost::filesystem::path file_path = GetMyDocumentsFolderPath();
	file_path /= kCookieFileName;
	if (!AS_Node::MakeDirs(file_path))
	{
		assert(false);
		throw CmnExitIoThreadException("euiqcnjlkqldq");
	}

	IniProfile pro_file;
	pro_file.SetProfile(file_path.string());
	auto whose_tmp = whose;
	whose_tmp += "%^$";
	pro_file.Write("1", GenUuidStr(whose_tmp), str_cookie);
}

const wchar_t*const LocalInfo::kCookieFileName = L"Tencent Files/All Users/datf.zip";
