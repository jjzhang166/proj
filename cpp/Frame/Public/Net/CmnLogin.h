#pragma once
/*
@author zhp
@date 2016/4/24 9:29
@purpose for common login
*/
#include <Net/Bufcall_Socket.h>
#include "client.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <vector>
#include <ctime>
#include <memory>
#include "DbData.h"
#include <protect/Protect.h>
#include <Net/CmnLoginInfo.h>
#include <Net/LoginLocalInfo.h>
#include <CmnMix/Cmn_LogOut.h>
#include <AppBase/Cmn_App.h>


//普通操作
class ICmnLoginSink{
public:
	//for login
	typedef std::vector<std::string> AddrDatasContT;
	typedef std::pair<std::string, AddrDatasContT> AddrDatasT;
	typedef std::vector<std::string> StrDatasT;
	typedef std::time_t TimeT;
	template<typename StrT>
	struct stCheckLoginInfo{
		template<class Archive>
		void serialize(Archive & ar){
			ar & ac_
				& pwd_
				& machine_code_
				& str_mix_
				& cookie_;
		}

		BC_CHECK_SIZE(StrT, kUserNameMaxLen, kUserNameMinLen)	ac_;
		BC_CHECK_SIZE(StrT, kUserPwdMaxLen, kUserPwdMinLen)		pwd_;
		BC_CHECK_SIZE(StrT, kMachineCodeLen, kMachineCodeLen)	machine_code_;
		BC_CHECK_SIZE(StrT, kUuidStrLen, kUuidStrLen)			str_mix_;
		BC_CHECK_SIZE(StrT, kCookieStrLen, 0)					cookie_;
	};
	template<typename StrT>
	using CheckLoginInfoContT = BC_CHECK_SIZE(std::vector<stCheckLoginInfo<StrT> >, 50, 1);
	enum enCSRet_CheckLogin{
		kCSR_CL_invalid = 0,	//无效
		kCSR_CL_failed,			//失败
		kCSR_CL_Succeed			//登陆成功
	};
	//CheckLogin的返回结果
	struct stCheckLoginResInfo{
		stCheckLoginResInfo(){
			ac_ = nullptr;
			res_ = kCSR_CL_failed;
		}
		template<class Archive>
		void serialize(Archive & ar){
			ar & ac_ & res_;
		}

		pt_csz				ac_;
		enCSRet_CheckLogin	res_;
	};
	typedef std::vector<stCheckLoginResInfo> CheckLoginResContT;

public:
	virtual ~ICmnLoginSink(){}
	virtual bool OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, 
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas
		, pt_dword ac_type);
	virtual bool OnRemoveBind( bool res, pt_csz info );
	virtual bool OnRegUser( bool res, pt_csz info );
	virtual bool OnUserPay( bool res, pt_csz info );
	virtual bool OnLeaveMsg( bool res, pt_csz info );
	virtual bool OnCheckLogin(const CheckLoginResContT& res_info);
};

//权限操作
class ICmnLoginGrantSink : public virtual ICmnLoginSink{
protected:
	typedef std::vector<std::pair<std::string, std::string> > CardInfoT;
	typedef std::vector<std::string> RegKeyInfoT;

public:
	virtual bool OnGrant_RemoveBind( bool res, pt_csz info );
	virtual bool OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info );
	virtual bool OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key );
	virtual bool OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state);
	virtual bool OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info);
	virtual bool OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state);
	virtual bool OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info);
	virtual bool OnGrant_RollbackPay( bool res, pt_csz info );
	virtual bool OnGrant_SetAddrData( bool res, pt_csz info );
	virtual bool OnGrant_SetStrData( bool res, pt_csz info );
	virtual bool OnGrant_SetAcState(bool res, pt_csz info);
};

/*最好用public的继承方式，否则dynamic_cast会有可能失败*/
class CmnCheckLoginOperC : public BufCall_SocketExcept, public virtual ICmnLoginSink{
public:
	CmnCheckLoginOperC( ICmnLoginSink& sink, FuncObj* parent );

	//接口
public:
	bool GetLoginRes() const;

	//用户操作
public:
	//登陆
	bool Login( bool is_only_login, pt_csz user_name, pt_csz user_pwd, pt_int version );
	//解绑
	bool RemoveBind( pt_csz user_name, pt_csz user_pwd );
	//注册
	bool RegUser( pt_csz user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id );
	//充值
	bool UserPay( pt_csz user_name, pt_csz card_id, pt_csz card_pwd );
	//留言
	bool LeaveMsg( pt_csz msg );
	//验证登陆
	bool CheckLogin(const CheckLoginInfoContT<std::string>& login_infos);
	
public:
	//得到机器码
	static std::string GetMachineCode();

	//用户操作结果
protected:
	//响应登陆
	bool OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, 
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type) override;
	//响应解绑
	bool OnRemoveBind( bool res, pt_csz info ) override;
	//响应注册
	bool OnRegUser( bool res, pt_csz info ) override;
	//响应支付
	bool OnUserPay( bool res, pt_csz info ) override;
	//响应留言
	bool OnLeaveMsg( bool res, pt_csz info ) override;
	//CheckLogin result
	bool OnCheckLogin(const CheckLoginResContT& res_info) override;

private:
	OptionalBool			login_res_;
	OptionalBool			remove_bind_res_;
	OptionalBool			reg_user_res_;
	OptionalBool			user_pay_res_;
	OptionalBool			leave_msg_res_;
	OptionalBool			check_login_res_;

private:
	ICmnLoginSink&		login_sink_;

private:
	std::string			user_account_tmp_;

	BC_REG_CLS_DECLARE(CmnCheckLoginOperC);
};

#pragma warning(push)
#pragma warning(disable:4250)

class CmnCheckLoginGrantOperC : public virtual CmnCheckLoginOperC, public virtual ICmnLoginGrantSink{
public:
	CmnCheckLoginGrantOperC( ICmnLoginGrantSink& sink, FuncObj* parent );

	//操作
public:
	//权限解绑
	bool Grant_RemoveBind( pt_csz user_name_dst );
	//开卡
	bool Grant_OpenCard(pt_word card_cnt, pt_dword card_minutes, pt_qword grant, pt_dword ac_type);
	//生成注册码
	bool Grant_GenRegKey(pt_word reg_key_cnt, pt_dword ac_type);
	//通过卡号查询帐号信息
	bool Grant_PeerAcByCard( pt_csz card_id, pt_csz card_pwd );
	//通过帐号查询卡号信息
	bool Grant_PeerCardByAc( pt_csz ac, pt_csz ac_pwd );
	//查询帐号信息
	bool Grant_PeerAcInfo( pt_csz ac, pt_csz ac_pwd );
	//查询卡号信息
	bool Grant_PeerCardInfo( pt_csz card_id, pt_csz card_pwd );
	//支付回滚
	bool Grant_RollbackPay( pt_csz card_id, pt_csz card_pwd, pt_bool direction );
	//项目数据
	bool Grant_SetAddrData( pt_int id, pt_dword addr_data );
	bool Grant_SetStrData( pt_int id, pt_csz str_data );
	//设置帐号状态
	bool Grant_SetAcState(pt_csz user_name_dst, enAcState ac_state, Poco::UInt32 period);

	//操作结果
protected:
	bool OnGrant_RemoveBind( bool res, pt_csz info ) override;
	bool OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info ) override;
	bool OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key ) override;
	bool OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_RollbackPay( bool res, pt_csz info ) override;
	bool OnGrant_SetAddrData( bool res, pt_csz info ) override;
	bool OnGrant_SetStrData( bool res, pt_csz info ) override;
	bool OnGrant_SetAcState(bool res, pt_csz info) override;

private:
	OptionalBool		grant_remove_bind_res_;
	OptionalBool		grant_open_card_res_;
	OptionalBool		grant_gen_reg_key_res_;
	OptionalBool		grant_peer_ac_by_card_res_;
	OptionalBool		grant_peer_card_by_ac_res_;
	OptionalBool		grant_peer_ac_info_res_;
	OptionalBool		grant_peer_card_info_res_;
	OptionalBool		grant_peer_rool_back_pay_res_;
	OptionalBool		grant_set_addr_data_res_;
	OptionalBool		grant_set_str_data_res_;
	OptionalBool		grant_set_ac_state_;

private:
	ICmnLoginGrantSink&		login_sink_ex_;

	BC_REG_CLS_DECLARE(CmnCheckLoginGrantOperC);
};

#pragma warning(pop)

template<typename T>
struct CmnCheckLoginOperSwitch{
	typedef ICmnLoginSink type;
};
template<>
struct CmnCheckLoginOperSwitch<CmnCheckLoginGrantOperC>{
	typedef ICmnLoginGrantSink type;
};

class CmnLoginConnectCfg : public Connection_Rsa{
	struct stConnectCfgInfo{
		stConnectCfgInfo();
		std::vector<std::string>		ip_info_;
		Crypto_RsaInitor				rsa_;
		pt_word							port_;
	};

public:
	explicit CmnLoginConnectCfg(const std::string& server_ip);

public:
	static bool SetCfgInfo(const std::vector<std::string>& ip_info, pt_word port);
	template<typename T>
	static bool SetCfgInfoT(const T& ip_info, pt_word port){
		std::vector<std::string> ips;
		for (auto& v : ip_info)
		{
			ips.push_back(v);
		}
		return SetCfgInfo(ips, port);
	}
	static bool SetCfgInfo(const luabind::object& tbl_ip_info, pt_word port);
	static const stConnectCfgInfo& GetCfgInfo();

private:
	static stConnectCfgInfo		connect_cfg_info_;
};

template<typename OperT_, typename T>
class CmnCheckLoginC : public CmnLoginConnectCfg, public CmnCheckLoginOperSwitch<OperT_>::type{
public:
	typedef OperT_ OperType;
	typedef T SelfType;
	//function obj必须是boost::shared_ptr
	typedef boost::shared_ptr<SelfType> SelfPtr;

private:
	typedef boost::weak_ptr<SelfType> WeakPtr;

public:
	CmnCheckLoginC( const std::string& check_server_ip) : CmnLoginConnectCfg(check_server_ip), oper_(*this, this){
		//最好在程序启动的时候初始化，也就是在InitInstance函数里。
		GainLocalInfo::TimerCheck();
	}

public:
	OperType& GetUserOper(){
		return oper_;
	}
	static SelfPtr GetSelfPtr(){
		if ( !weak_ptr_.expired() )
		{
			auto self_ptr = weak_ptr_.lock();
			if ( self_ptr && self_ptr->ObjValid() )
				return self_ptr;
		}
		return ToDoConnect_();
	}

protected:
	bool OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie,
		const typename OperType::TimeT& end_day_time, const typename OperType::TimeT& ser_cur_time,
		const typename OperType::AddrDatasT& addr_datas, const typename OperType::StrDatasT& str_addr_datas, pt_dword ac_type) override{
		VMP_BEGIN_EX;
		if (res)
		{
			static bool init_once = false;
			if (init_once)
				return true;
			init_once = true;

			auto mgr_ptr = CmnLoginInfoMgrBaseImpl::GetInstance();
			if (mgr_ptr)
			{
				const pt_dword kTimerTime = 1000 * 60 * 45;
				//const pt_dword kTimerTime = 1000 * 5;
				SetIoTimer1(kTimerTime, [kTimerTime](IoTimer& timer){
					VMP_BEGIN_EX;
					auto timer_time = kTimerTime;
					BOOST_SCOPE_EXIT_ALL(&timer, &timer_time){
						timer.SetDuration(timer_time);
						timer.Continue();
					};
					
					auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
					if (!login_info_mgr)
					{
						assert(false);
						throw CmnExitIoThreadException("fdjask");
					}
					CheckLoginInfoContT<std::string> login_infos;
					const auto& machine_code = OperType::GetMachineCode();

					auto& local_info = GainLocalInfo::GetMe();
					std::vector<std::string> the_uuids;
					for (auto& v : login_info_mgr->GetLoginInfos())
					{
						if (v.uuid_.empty())
							continue;
						CmnLoginInfo login_info_tmp;
						login_info_tmp.ac_ = v.ac_;
						login_info_tmp.pwd_ = v.pwd_;
						login_info_tmp.GenUuid();
						if (login_info_tmp.uuid_ != v.uuid_)
						{
							assert(false);
							throw CmnExitIoThreadException("fjdase");
						}
						the_uuids.push_back(v.uuid_);

						if (!v.IsTheLoginSucceed())
							continue;
						stCheckLoginInfo<std::string> info_tmp;
						info_tmp.ac_ = v.ac_;
						info_tmp.pwd_ = v.pwd_;
						info_tmp.machine_code_ = machine_code;
						std::string str_mix = v.ac_;
						str_mix += v.pwd_;
						str_mix += machine_code;
						str_mix = GenUuidStr(str_mix);
						info_tmp.str_mix_ = str_mix;
						info_tmp.cookie_ = local_info.GetCookie(str_mix);
						//info_tmp.cookie_ = CookieHelper::GetMe().GetCookie(GenUuidStr(str_mix));
						login_infos.value_.push_back(info_tmp);
					}
					if (!the_uuids.empty())
					{
						std::sort(the_uuids.begin(), the_uuids.end());
						auto it = std::unique(the_uuids.begin(), the_uuids.end());
						if (it != the_uuids.end())
						{
							assert(false);
							throw CmnExitIoThreadException("mlell");
						}
					}
					if (login_infos.value_.empty())
						return;
					auto self_ptr = GetSelfPtr();
					if (!self_ptr || !self_ptr->ObjValid())
					{
						timer_time = 1000 * 60 * 5;
						return;
					}
					if (!self_ptr->oper_.CheckLogin(login_infos))
					{
						assert(false);
						throw CmnExitIoThreadException("fdjkaeqald");
					}
					VMP_END;
				});
			}
		}
		VMP_END;
		return true;
	}

private:
	static SelfPtr ToDoConnect_(){
		int i = 0;
		weak_ptr_.reset();
		for (auto& domain : GetCfgInfo().ip_info_)
		{
			const auto& svr_ip = Domain2Ip(domain);
			++i;
			LOG_O(Log_trace) << "domain2svr ip->" << svr_ip << " 解析次数:" << i;
			auto the_ptr = Cmn_AppBase::GetAppFactory().CreateCheckServerConnection(svr_ip);
			if (!the_ptr)
			{
				assert(false);
				return false;
			}
			SelfPtr self_ptr(static_cast<SelfType*>(the_ptr));
			if ( !self_ptr )
			{
				assert(false);
				return SelfPtr();
			}
			if ( !self_ptr->Connect() )
				continue;
			if ( !self_ptr->WaitForConnectDone() )
				continue;
			weak_ptr_ = self_ptr;
			return self_ptr;
		}
		return SelfPtr();
	}

private:
	OperType		oper_;

private:
	static WeakPtr	weak_ptr_;
};

template<typename OperT_, typename T>
p_selectany
typename CmnCheckLoginC<OperT_, T>::WeakPtr CmnCheckLoginC<OperT_, T>::weak_ptr_;


class CmnLoginException : public CmnExceptionBase{
public:
	CmnLoginException(bool res, const std::string& msg);
	CMN_EXCEPTION_CLONE

public:
	bool		res_;
};

class CmnDoLogin : public CmnCheckLoginC<CmnCheckLoginOperC, CmnDoLogin>{
	typedef CmnLoginException OperExceptionT;
	struct stVersionInfo{
		std::string		str_version_;
		pt_int			version_;

		stVersionInfo();
	};

public:
	explicit CmnDoLogin(const std::string& check_server_ip);
	//登陆
	luabind::object Login(const luabind::object& is_only_login, pt_csz user_name, pt_csz user_pwd);
	//解绑
	luabind::object RemoveBind(const luabind::object& user_name, pt_csz user_pwd);
	//注册
	luabind::object RegUser(const luabind::object& user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id);
	//充值
	luabind::object UserPay(const luabind::object& user_name, pt_csz card_id, pt_csz card_pwd);
	//留言
	luabind::object LeaveMsg(const luabind::object& msg);
	static void RegForLua(lua_State* l);
	static void SetVersionInfo(const std::string& str_version, pt_int version);
	static const stVersionInfo& GetVersionInfo();

protected:
	using CmnCheckLoginC::GetUserOper;
	bool OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie,
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type) override;

private:	
	bool OnRemoveBind(bool res, pt_csz info) override final;
	bool OnRegUser(bool res, pt_csz info) override final;
	bool OnUserPay(bool res, pt_csz info) override final;
	bool OnLeaveMsg(bool res, pt_csz info) override final;
	static luabind::object __Login__(const luabind::object& is_only_login, pt_csz user_name, pt_csz user_pwd);
	static luabind::object __RemoveBind__(const luabind::object& user_name, pt_csz user_pwd);
	static luabind::object __RegUser__(const luabind::object& user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id);
	static luabind::object __UserPay__(const luabind::object& user_name, pt_csz card_id, pt_csz card_pwd);
	static luabind::object __LeaveMsg__(const luabind::object& msg);	//暂不支持

private:
	bool					is_doing_;
	static stVersionInfo	g_login_version_info_;
};
