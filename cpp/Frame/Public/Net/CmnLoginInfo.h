#pragma once
/*
@author zhp
@date 2016/5/1 11:19
@purpose for login,login info of console
*/
#include <vector>
#include <string>
#include <utility>
#include <boost/serialization/nvp.hpp>
#include <Common/UsefulFunc.h>
#include <Common/CmnPriLuaData.h>
#include <luabind/luabind.hpp>
#include <CmnMix/Cmn_LuaOper.h>
#include "PocoMore.h"
#include <Common/UsefulClass.h>
#include "IoTimer.h"
//////////////////////////////////////////////////////////////////////////
class CmnLoginInfoMgrBaseImpl : public Singleton<CmnLoginInfoMgrBaseImpl, Singleton_Instance>{
	friend class CmnLoginInfo;
public:
	//first==account,second==server name
	typedef std::pair<std::string, std::string> AcServerInfo;
	typedef std::vector<AcServerInfo> BindAcInfoT;

public:
	CmnLoginInfoMgrBaseImpl();
	void SetLuaData(CmnPriLuaData* lua_data);
	virtual pt_dword GetMaxConnectedCntByAcType(pt_dword ac_type) const;

public:
	template<typename DerivedT>
	static DerivedT* GetDerived(){
		auto this_ptr = GetInstance();
		if (!this_ptr)
			return nullptr;
		auto res = dynamic_cast<DerivedT*>(this_ptr);
		assert(res);
		return res;
	}

public:
	virtual void DoCloseTheSession(const BindAcInfoT& the_clr_ac) = 0;

protected:
	CmnPriLuaData*		lua_data_;
};
//存储着登陆信息
class CmnLoginInfo{
	friend class CmnLoginInfoMgrBase;
	template<typename ConnectInfo, typename BaseT>
	friend class CmnLoginInfoMgr;
	friend class CmnCheckLoginOperC;

public:
	typedef CmnLoginInfoMgrBaseImpl::BindAcInfoT BindAcInfoT;

	CmnLoginInfo(){
		std::memset(sz_buffer_, 0, sizeof(sz_buffer_));
	}
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & BOOST_SERIALIZATION_NVP(ac_)
			& BOOST_SERIALIZATION_NVP(pwd_);
		const auto& the_str = RandomUniqueStr(_countof(sz_buffer_));
		memcpy_s(sz_buffer_, sizeof(sz_buffer_), the_str.c_str(), the_str.size());
	}
	inline bool IsTheLoginSucceed() const{
		using namespace luabind;
		if (uuid_.empty())
			return false;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return false;
		if (!mgr->lua_data_)
			return false;
		auto& lua_data = *mgr->lua_data_;
		const auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return false;
		const object& the_res = the_tbl[kLoginData_LoginSucceed];
		if (!the_res.is_valid())
			return false;
		return LuaObjCast(the_res, false);
	}
	inline pt_dword GetAcType() const{
		using namespace luabind;
		if (uuid_.empty())
			return -1;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return -2;
		if (!mgr->lua_data_)
			return -3;
		auto& lua_data = *mgr->lua_data_;
		const auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return -4;
		const object& the_res = the_tbl[kLoginData_AcType];
		if (!the_res.is_valid())
			return -5;
		return LuaObjCast(the_res, pt_dword(-6));
	}
	inline std::string GetLoginInfo() const{
		using namespace luabind;
		if (uuid_.empty())
			return kEmptyStr;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return kEmptyStr;
		if (!mgr->lua_data_)
			return kEmptyStr;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return kEmptyStr;
		const object& the_res = the_tbl[kLoginData_LoginInfo];
		if (!the_res.is_valid())
			return kEmptyStr;
		return LuaObjCast(the_res, kEmptyStr);
	}
	inline boost::optional<DateTimeLocal> GetUseEndDayTime() const{
		boost::optional<DateTimeLocal> res;
		using namespace luabind;
		if (uuid_.empty())
			return res;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return res;
		if (!mgr->lua_data_)
			return res;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return res;
		const object& the_res = the_tbl[kLoginData_EndDateTime];
		if (!the_res.is_valid() || LUA_TTABLE != type(the_res))
			return res;
		const object& time_lower = the_res[1];
		if (!the_res.is_valid())
			return res;
		const object& time_upper = the_res[2];
		if (!time_upper.is_valid())
			return res;
		res = Poco::Timestamp::fromEpochTime((time_t)MakeInt64(LuaObjCast(time_lower, 1000), LuaObjCast(time_upper, 2000)));
		return res;
	}
	inline std::vector<DWORD> GetAddrDatas() const{
		std::vector<DWORD> res;
		using namespace luabind;
		if (uuid_.empty())
			return res;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return res;
		if (!mgr->lua_data_)
			return res;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return res;
		const object& the_res = the_tbl[kLoginData_AddrDatas];
		if (!the_res.is_valid() || LUA_TTABLE != type(the_res))
			return res;
		for (iterator it(the_res), it_end; it != it_end; ++it)
		{
			res.push_back(LuaObjCast(*it, (DWORD)0));
		}
		return res;
	}
	inline std::vector<std::string> GetStrDatas() const{
		std::vector<std::string> res;
		using namespace luabind;
		if (uuid_.empty())
			return res;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return res;
		if (!mgr->lua_data_)
			return res;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return res;
		const object& the_res = the_tbl[kLoginData_StrDatas];
		if (!the_res.is_valid() || LUA_TTABLE != type(the_res))
			return res;
		for (iterator it(the_res), it_end; it != it_end; ++it)
		{
			res.push_back(LuaObjCast(*it, kEmptyStr));
		}
		return res;
	}
	inline BindAcInfoT GetBindGameAcInfo() const{
		using namespace luabind;
		BindAcInfoT res;
		if (uuid_.empty())
			return res;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return res;
		if (!mgr->lua_data_)
			return res;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return res;
		const object& the_res = the_tbl[kLoginData_BindGameAcInfo];
		if (!the_res.is_valid() || LUA_TTABLE != type(the_res))
			return res;
		for (iterator it(the_res), it_end; it != it_end; ++it)
		{
			std::pair<std::string, std::string> tmp;
			tmp.first = LuaObjCast((*it)[1], kEmptyStr);
			if (tmp.first.empty())
				continue;
			tmp.second = LuaObjCast((*it)[2], kEmptyStr);
			if (tmp.second.empty())
				continue;
			res.push_back(tmp);
		}
		/*if ( GetAcceptMaxCnt() < res.size() )
		{
		assert(false);
		res.clear();
		}*/
		return res;
	}
	inline bool AddBindGameAcInfo(const std::string& game_ac, const std::string& game_service_name) const{
		using namespace luabind;
		if (game_ac.empty() || game_service_name.empty())
		{
			assert(false);
			return false;
		}
		if (uuid_.empty())
			return false;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return false;
		if (!mgr->lua_data_)
			return false;
		auto& lua_data = *mgr->lua_data_;
		if (GetAcceptMaxCnt() <= (int)GetBindGameAcInfo().size())
		{
			assert(false);
			return false;
		}
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		object tbl_bind = the_tbl[kLoginData_BindGameAcInfo];
		if (!tbl_bind.is_valid() || LUA_TTABLE != type(tbl_bind))
		{
			tbl_bind = newtable(lua_data.lua());
			the_tbl[kLoginData_BindGameAcInfo] = tbl_bind;
		}
		const auto& the_key = GenUuidStr(game_ac + game_service_name);
		object tbl_ac_info = tbl_bind[the_key];
		if (!tbl_ac_info.is_valid() || LUA_TTABLE != type(tbl_ac_info))
		{
			tbl_ac_info = newtable(lua_data.lua());
			tbl_bind[the_key] = tbl_ac_info;
		}
		tbl_ac_info[1] = game_ac;
		tbl_ac_info[2] = game_service_name;
		return true;
	}
	inline bool DeleteTheBindGameAcInfo(const std::string& game_ac, const std::string& game_service_name) const{
		using namespace luabind;
		if (game_ac.empty() || game_service_name.empty())
		{
			assert(false);
			return false;
		}
		if (uuid_.empty())
			return false;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return false;
		if (!mgr->lua_data_)
			return false;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return false;
		const object& tbl_bind = the_tbl[kLoginData_BindGameAcInfo];
		if (!tbl_bind.is_valid() || LUA_TTABLE != type(tbl_bind))
			return false;
		const auto& the_key = GenUuidStr(game_ac + game_service_name);
		object tbl_ac_info = tbl_bind[the_key];
		if (!tbl_ac_info.is_valid() || LUA_TTABLE != type(tbl_ac_info))
			return false;
		tbl_bind[the_key] = object();
		return true;
	}
	inline void GenUuid(){
		if (!uuid_.empty())
			return;
		uuid_ = GenUuidStr(ac_ + pwd_);
	}
	//测试时间是否到期
	inline bool TestTimeEnd(const DateTimeLocal& time_now) const{
		if (!IsTheLoginSucceed())
			return false;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return false;
		const auto& my_time_end = GetUseEndDayTime();
		if (!my_time_end || *my_time_end < time_now || *my_time_end < DateTimeLocal())
		{
			ResetAll();
			return true;
		}
		return false;
	}
	inline void ResetAll() const{
		const auto& the_clr_ac = GetBindGameAcInfo();
		SetLoginRes(false);
		SetLoginInfo(kEmptyStr);
		ClrAllBindGameAcInfo();
		SetUseEndDayTime(DateTimeLocal(1981, 1, 1));
		SetAcceptMaxCnt(0);
		if (!the_clr_ac.empty())
		{
			auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
			if (mgr)
				mgr->DoCloseTheSession(the_clr_ac);
		}
	}

private:
	inline void SetLoginRes(bool res) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		the_tbl[kLoginData_LoginSucceed] = res;
	}
	inline void SetAcType(pt_dword ac_type) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		the_tbl[kLoginData_AcType] = ac_type;
	}
	inline void SetLoginInfo(const std::string& login_info) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		the_tbl[kLoginData_LoginInfo] = login_info;
	}
	inline void SetUseEndDayTime(const DateTimeLocal& use_end_time) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		auto ts = use_end_time.timestamp();
		auto& end_time = SplitInt64(ts.epochTime());
		object the_res = the_tbl[kLoginData_EndDateTime];
		if (!the_res.is_valid() || LUA_TNIL == type(the_res))
		{
			the_res = newtable(lua_data.lua());
			the_tbl[kLoginData_EndDateTime] = the_res;
		}
		the_res[1] = end_time.first;
		the_res[2] = end_time.second;
	}
	inline void SetAddrDatas(const std::vector<DWORD>& addr_datas) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		object the_res = the_tbl[kLoginData_AddrDatas];
		if (!the_res.is_valid() || LUA_TNIL == type(the_res))
		{
			the_res = newtable(lua_data.lua());
			the_tbl[kLoginData_AddrDatas] = the_res;
		}
		for (int i = 0; i != addr_datas.size(); ++i)
			the_res[i + 1] = addr_datas[i];
	}
	inline void SetStrDatas(const std::vector<std::string>& str_datas) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TNIL == type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		object the_res = the_tbl[kLoginData_StrDatas];
		if (!the_res.is_valid() || LUA_TNIL == type(the_res))
		{
			the_res = newtable(lua_data.lua());
			the_tbl[kLoginData_StrDatas] = the_res;
		}
		for (int i = 0; i != str_datas.size(); ++i)
		{
			the_res[i + 1] = str_datas[i];
		}
	}

private:
	inline bool ClrAllBindGameAcInfo() const{
		using namespace luabind;
		if (uuid_.empty())
			return false;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return false;
		if (!mgr->lua_data_)
			return false;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return false;
		the_tbl[kLoginData_BindGameAcInfo] = object();
		return true;
	}
	inline int GetAcceptMaxCnt() const{
		using namespace luabind;
		if (uuid_.empty())
			return 0;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return 0;
		if (!mgr->lua_data_)
			return 0;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
			return 0;
		const object& the_res = the_tbl[kLoginData_AcceptMaxCnt];
		int max_cnt = mgr->GetMaxConnectedCntByAcType(GetAcType());
		return std::min(max_cnt, LuaObjCast(the_res, 0));
	}
	inline void SetAcceptMaxCnt(int max_cnt) const{
		using namespace luabind;
		if (uuid_.empty())
			return;
		auto mgr = CmnLoginInfoMgrBaseImpl::GetInstance();
		if (!mgr)
			return;
		if (!mgr->lua_data_)
			return;
		auto& lua_data = *mgr->lua_data_;
		auto& the_tbl = lua_data.GetLuaObj(uuid_);
		if (!the_tbl.is_valid() || LUA_TTABLE != type(the_tbl))
		{
			the_tbl = newtable(lua_data.lua());
			lua_data.SetLuaObj(uuid_, the_tbl);
		}
		the_tbl[kLoginData_AcceptMaxCnt] = max_cnt;
	}

public:
	std::string		ac_;				//辅助登陆帐号
	char			sz_buffer_[50];		//这个用来骗人
	std::string		pwd_;				//辅助登陆密码
	std::string		uuid_;
	

private:
	static const char* const kLoginData_LoginSucceed;
	static const char* const kLoginData_AcType;
	static const char* const kLoginData_LoginInfo;
	static const char* const kLoginData_EndDateTime;
	static const char* const kLoginData_AddrDatas;
	static const char* const kLoginData_StrDatas;
	static const char* const kLoginData_AcceptMaxCnt;
	static const char* const kLoginData_BindGameAcInfo;
};

class CmnLoginInfoMgrBase : public CmnLoginInfoMgrBaseImpl{
	friend class CmnLoginInfo;
public:
	CmnLoginInfo* FindByAc(const std::string& ac);
	bool DelByAc(const std::string& ac);
	void DelAllByAc(const std::string& ac);
	void UniqueLoginInfo();
	const std::vector<CmnLoginInfo>& GetLoginInfos() const;
	void ClearAllLoginInfos();
	void AddLoginInfo(const std::string& ac, const std::string& pwd);
	//得到连接总数量
	pt_dword GetConnectedMaxCnt() const;
	inline CmnLoginInfo* FindFreeOne(){
		const DateTimeLocal& time_now = G_GetSerCurTime();
		for (auto& v : login_info_)
		{
			if (v.TestTimeEnd(time_now))
				continue;
			if (!v.IsTheLoginSucceed())
				continue;
			auto& res = v.GetBindGameAcInfo();
			if (v.GetAcceptMaxCnt() <= (int)res.size())
				continue;
			auto& my_end_day_time = v.GetUseEndDayTime();
			if (my_end_day_time && *my_end_day_time > time_now)
				return &v;
		}
		return nullptr;
	}
	inline CmnLoginInfo* FindByAc_Ser(const std::string& game_ac, const std::string& game_service_name){
		for (auto& v : login_info_)
		{
			auto& res = v.GetBindGameAcInfo();
			for (auto& ac_info : res)
			{
				if (ac_info.first == game_ac && ac_info.second == game_service_name)
					return &v;
			}
		}
		return nullptr;
	}
	inline int GetAllowMaxLoginCnt() const{
		auto res = 0;
		for (auto& v : login_info_)
			res += v.GetAcceptMaxCnt();
		return res;
	}
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version){
		ar & BOOST_SERIALIZATION_NVP(login_info_);
	}
	virtual void RectifyAcceptMaxCnt() = 0;

private:
	std::vector<CmnLoginInfo>		login_info_;		//必须是顺序容器才成，否则算法有问题
};
/*
需求一个基类，里面有以下静态成员变量
//最大连接数量
const pt_int kGameConnectionMaxCnt = 10;
//每个帐号可接受的连接数量
const pt_int kAcceptableGameConnectionCnt[] = {
1, 1, 1, 5, 1
};
需求 auto ConnectItemCnt() -> decltype(kAcceptableGameConnectionCnt)& { return kAcceptableGameConnectionCnt; };
//不在kAcceptableGameConnectionCnt此列的，也就是超出的，全部设为此连接数
const pt_int kDefConnectCntAtOverflow = 2;
*/
template<typename ConnectInfo, typename BaseT = CmnLoginInfoMgrBase>
class CmnLoginInfoMgr : public ConnectInfo, public BaseT{
public:
	typedef ConnectInfo ConnectInfoT;

public:	
	//调整每个ac_pwd可接受的连接数
	void RectifyAcceptMaxCnt() override final{
		struct stHelper{
			const CmnLoginInfo*	ac_pwd_;
			int					cur_max_cnt_;
			int					cur_bind_cnt_;

			stHelper(){
				ac_pwd_ = nullptr;
				cur_max_cnt_ = 0;
				cur_bind_cnt_ = 0;
			}
		};

		std::vector<stHelper>	the_ac_pwd_info;
		const DateTimeLocal& time_now = G_GetSerCurTime();
		int total_max_cnt = 0;
		for (auto& v : GetLoginInfos())
		{
			if (!v.IsTheLoginSucceed())
				continue;
			if (v.TestTimeEnd(time_now))
				continue;
			stHelper tmp;
			tmp.ac_pwd_ = &v;
			tmp.cur_max_cnt_ = v.GetAcceptMaxCnt();
			tmp.cur_bind_cnt_ = v.GetBindGameAcInfo().size();
			total_max_cnt += tmp.cur_max_cnt_;
			if (total_max_cnt > ConnectInfoT::kGameConnectionMaxCnt || tmp.cur_max_cnt_ < tmp.cur_bind_cnt_)
			{
				assert(false);
				GetIoService().post([](){
					throw CmnExitIoThreadException("fjdklasie");
				});
				return;
			}
			the_ac_pwd_info.push_back(tmp);
		}
		if (the_ac_pwd_info.empty())
			return;

		const auto& connect_item_cnt = ConnectInfoT::ConnectItemCnt();
		auto it = the_ac_pwd_info.begin();
		for (int idx = 0; it != the_ac_pwd_info.end(); ++it, ++idx)
		{
			if (idx >= _countof(connect_item_cnt))
				break;
		}
		if (it != the_ac_pwd_info.end())
		{
			//超过_countof(connect_item_cnt) 这个数量的，全部设置为ConnectInfoT::kDefConnectCntAtOverflow
			for (auto it_cpy = it; it_cpy != the_ac_pwd_info.end(); ++it_cpy)
			{
				assert(it_cpy->ac_pwd_);
				if (it_cpy->cur_max_cnt_ > ConnectInfoT::kDefConnectCntAtOverflow || it_cpy->cur_max_cnt_ < it_cpy->cur_bind_cnt_)
				{
					assert(false);
					throw CmnExitIoThreadException("fioeqpvm/");
				}
				if (it_cpy->cur_max_cnt_ != ConnectInfoT::kDefConnectCntAtOverflow)
					it_cpy->ac_pwd_->SetAcceptMaxCnt(ConnectInfoT::kDefConnectCntAtOverflow);
			}
			the_ac_pwd_info.erase(it, the_ac_pwd_info.end());
		}
		assert(the_ac_pwd_info.size() <= _countof(connect_item_cnt));

		it = the_ac_pwd_info.begin();
		for (auto idx = 0; it != the_ac_pwd_info.end() && idx < _countof(connect_item_cnt); ++it, ++idx)
		{
			assert(it->ac_pwd_);
			if (it->cur_max_cnt_ < it->cur_bind_cnt_)
			{
				assert(false);
				throw CmnExitIoThreadException("eiuqp7431");
			}
			if (it->cur_bind_cnt_ > connect_item_cnt[idx])
			{
				auto bind_ac_info = it->ac_pwd_->GetBindGameAcInfo();
				CmnLoginInfo::BindAcInfoT close_ac_info;
				for (auto i = connect_item_cnt[idx]; i < it->cur_bind_cnt_ && !bind_ac_info.empty(); ++i)
				{
					close_ac_info.push_back(bind_ac_info.back());
					bind_ac_info.pop_back();
				}
				assert(!close_ac_info.empty());
				DoCloseTheSession(close_ac_info);
			}
			if (it->cur_max_cnt_ != connect_item_cnt[idx])
				it->ac_pwd_->SetAcceptMaxCnt(connect_item_cnt[idx]);
		}
		assert(it == the_ac_pwd_info.end());
	}
};