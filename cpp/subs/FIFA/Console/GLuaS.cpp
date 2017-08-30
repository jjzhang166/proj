#include "stdafx.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include "MainFrm.h"
#include "DlgUnion.h"
#include "protect/Protect.h"
#include "CheckLoginC.h"
#include "CoordinateC.h"
#include "CoordinateS.h"
#include <Communicate/CGL_Defines.h>

GLuaS::GLuaS( FuncObj* parent ) : GLua(parent)
{

}

BC_REG_CLS_DEFINE(LuaScriptS){
	bufcall::class_<LuaScriptS, LuasScriptEchoS>("LuaScriptS")
		;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

void LuaAll::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	typedef std::vector<std::string> StrVectorT;
	typedef void (StrVectorT::*StrVector_PushBack)( const StrVectorT::value_type& );
	module(lstate_)
		[
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

			class_<CoorTrans_CS, JsonEchoTrans>("GCoordinateS"),

			class_<LoginOperS, JsonEchoBase>("LoginOperS")
			.def("AcOrPwdErr", &LoginOperS::AcOrPwdErr)
			.def("AcFreeze", &LoginOperS::AcFreeze)
			.def("oclvr", &LoginOperS::OnCheckLoginVersion)
			.def("LoginUnRespond", &LoginOperS::LoginUnRespond)
		];
}

bool GLuaVM_S::InitThrdData()
{
	if (!__super::InitThrdData())
	{
		assert(false);
		return false;
	}
	login_data_.reset( new CmnPriLuaData(lstate_));

	stSingletonCfg::GetMe().SetLuaData(GetLoginData());
	return true;
}

CmnPriLuaData* GLuaVM_S::GetLoginData() const
{
	return login_data_.get();
}

LuaScriptS::LuaScriptS(FuncObj_NormalS& parent ) : LuasScriptEchoS(parent), func_obj_(parent)
{

}

bool LuaScriptS::OutputScriptInfo( const std::string& script_info )
{
	func_obj_.GetSessionOper().SetScriptOutputInfo(script_info);
	return true;
}
