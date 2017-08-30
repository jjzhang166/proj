#include "stdafx.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include "MainFrm.h"
#include "DlgUnion.h"
#include "protect/Protect.h"
#include "CheckLoginC.h"
#include "CoordinateC.h"
#include "CoordinateS.h"

GLuaS::GLuaS( FuncObj* parent ) : GLua(parent)
{

}


GLuaVM_S& GLuaVM_S::GetMe()
{
	if ( !lua_vm_ )
	{
		lua_vm_ = new GLuaVM_S;
	}
	return *lua_vm_;
}

GLuaVM_S::GLuaVM_S() : LuaVM(_T("s"))
{

}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void GLuaVM_S::BindGlobalFn()
{
	__super::BindGlobalFn();

	/*using namespace luabind;
	module(lua())
		[
			
		];*/
}

void GLuaVM_S::BindStruct()
{
	__super::BindStruct();
}

void GLuaVM_S::BindClass()
{
	__super::BindClass();

	using namespace luabind;

	typedef std::vector<std::string> StrVectorT;
	typedef void (StrVectorT::*StrVector_PushBack)( const StrVectorT::value_type& );
	module(lua())
		[
			class_<GLuaVM_S, LuaVM>("GLuaVM_S"),
			def("GLuaVM_S_Me", &GLuaVM_S::GetMe),

			class_<ListNormal>("ListNormal"),

			/*class_<PageAccount_List, ListNormal>("PageAccount_List"),
			def("PageAccount_List_Instance", &PageAccount_List::GetInstance),*/

			class_<CTabOption>("_T_O_")
			.property("l_succeed", &CTabOption::IsLoginSucceed, &CTabOption::SetLoginSucceed)
			.def("GetPageAccount", &CTabOption::GetPageAccount),
			def("_T_O_instance", &CTabOption::GetInstance),

			class_<DlgUseful>("DlgUseful")
			.def("SetWndText", &DlgUseful::SetWndText),

			class_<DlgLogin, DlgUseful>("_d_l_"),
			def("_d_l_instance_", &DlgLogin::GetInstance),

			class_<DlgReg, DlgUseful>("_d_r_"),
			def("_d_r_instance", &DlgReg::GetInstance),

			class_<DlgPay, DlgUseful>("_d_p_"),
			def("_d_p_instance", &DlgPay::GetInstance),

			class_<StrVectorT>("StrVectorT")
			.def("PushBack", (StrVector_PushBack)&StrVectorT::push_back),

			class_<PageAccount>("PageAccount")
			.def("GetAccountList", &PageAccount::GetAccountList),

			class_<CheckLoginOperC, JsonEchoBase>("CheckLoginOperC")
			//.def("OnLogin", &CheckLoginOperC::OnLogin)
			.def("ACMI", &CheckLoginOperC::OnLoginAcInfo)
			.def("OnRemoveBind", &CheckLoginOperC::OnRemoveBind)
			.def("OnRegUser", &CheckLoginOperC::OnRegUser)
			.def("OnUserPay", &CheckLoginOperC::OnUserPay)
			.def("occlr", &CheckLoginOperC::OnCheckLoginResult)
			.def("OnLoginInfo", &CheckLoginOperC::OnLoginInfo)
			.def("OnRemoveBindInfo", &CheckLoginOperC::OnRemoveBindInfo)
			.def("OnRegUserInfo", &CheckLoginOperC::OnRegUserInfo)
			.def("OnUserPayInfo", &CheckLoginOperC::OnUserPayInfo)
			.def("OUET", &CheckLoginOperC::OnUserEndTime)
			.def("OnRecvAddrDatas", &CheckLoginOperC::OnRecvAddrDatas),

			class_<SessionOperS, JsonEchoBase>("SessionOperS")
			.def("SetRunState", &SessionOperS::SetRunState)
			.def("SetScriptOutputInfo", &SessionOperS::SetScriptOutputInfo)
			.def("StartRunScript", &SessionOperS::StartRunScript)
			.def("EchoMsg", &SessionOperS::EchoMsg)
			.def("BanAccount", &SessionOperS::BanAccount)
			.def("SetPlayersInfo", &SessionOperS::SetPlayersInfo)
			.def("SetItemsInfo", &SessionOperS::SetItemsInfo)
			.def("SetMyOfferList", &SessionOperS::SetMyOfferList)
			.def("SetOtherOfferItems", &SessionOperS::SetOtherOfferItems)
			.def("ExchangeGameAc", &SessionOperS::ExchangeGameAc)
			.def("SetCurHp", &SessionOperS::SetCurHp)
			.def("SetRoleName", &SessionOperS::SetRoleName)
			.def("SetRoleLvl", &SessionOperS::SetRoleLvl)
			.def("SetFatigueData", &SessionOperS::SetFatigueData)
			.def("SetCurMapRoomName", &SessionOperS::SetCurMapRoomName)
			.def("SetCurGold", &SessionOperS::SetCurGold)
			.def("SetJobName", &SessionOperS::SetJobName)
			.def("SetRoleListInfo", &SessionOperS::SetRoleListInfo)
			.def("AutoCreateOrSelectRole", &SessionOperS::AutoCreateOrSelectRole)
			.def("GameLoginSucceed", &SessionOperS::GameLoginSucceed)
			.def("RoleRestricted", &SessionOperS::RoleRestricted)
			.def("OnCreateRoleFailed", &SessionOperS::OnCreateRoleFailed),

			class_<LoginOperS, JsonEchoBase>("LoginOperS")
			.def("AcOrPwdErr", &LoginOperS::AcOrPwdErr)
			.def("AcFreeze", &LoginOperS::AcFreeze)
			.def("oclvr", &LoginOperS::OnCheckLoginVersion)
		];
}

void GLuaVM_S::BindConst()
{
	__super::BindConst();
}

void GLuaVM_S::BindOther()
{
	__super::BindOther();
}

bool GLuaVM_S::DoMainLuaFileByFile()
{
	return
#ifdef _LUA_PACK_IN_RES
		false
#else
		true
#endif
		;
}

void GLuaVM_S::ReleaseMe()
{
	if (lua_vm_)
		delete lua_vm_;
	lua_vm_ = nullptr;
}

bool GLuaVM_S::InitScript( LPCTSTR module_name, LPCTSTR main_lua_file_name )
{
	if ( !__super::InitScript(module_name, main_lua_file_name) )
	{
		assert(false);
		return false;
	}
	LuaScriptS::RegForLua(*this);
	return true;
}

GLuaVM_S* GLuaVM_S::GetInstance()
{
	return lua_vm_;
}

GLuaVM_S* GLuaVM_S::lua_vm_ = nullptr;

LuaScriptS::LuaScriptS( LuaVM& lua_vm, FuncObj_NormalS& parent ) : LuasScriptEchoS(lua_vm, parent), func_obj_(parent)
{

}

bool LuaScriptS::OutputScriptInfo( const std::string& script_info )
{
	func_obj_.GetSessionOper().SetScriptOutputInfo(script_info);
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "控制台收到脚本输出信息->" << script_info;
	return true;
}
