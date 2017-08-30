#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GFuncCall.h"
#include "Facility/GClass.h"
#include <boost/thread/thread.hpp>
#include "FuncObjC.h"
#include "Facility/GClass.h"
#include "Facility/GPlayer.h"
#include "GloFuncs.h"

using namespace GFuncCall;
using namespace GClass;

static bool is_running = false;

bool is_terminate_script_run = false;
//是否结束脚本
bool IsTerminateScriptRun()
{
	return is_terminate_script_run;
}

void TerminateScriptRun()
{
	is_terminate_script_run = true;
}

//设置脚本输出信息
void SetScriptOutputInfo( const char* output_info )
{
	if ( !output_info )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "output_info为空";
		return;
	}
	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP("SetScriptOutputInfo")<<output_info);
	}
}

//echo message
void EchoMsg( const char* msg )
{
	if ( !msg )
		return;
	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP("EchoMsg")<<msg);
	}
}

//换号
void ExchangeGameAc()
{
	auto func_obj = FuncObj_NormalC::GetInstance();
	if ( !func_obj )
		return;
	func_obj->GetSessionOper().ToCall(BC_MakeP("ExchangeGameAc"));
}

void LetET( int e_t )
{
}

int UseSomeItem( int item_cnt )
{
	return GPlayer::GetMe().UseSomeItem(item_cnt);
}

void HandleElapseTimers()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	norc->ElapseTimers();
}

/*
void OutputScriptInfo( const std::string& str )
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	norc->GetLuaScript().OutputScriptInfo(str);
}*/

void L_EnhanceThePlayer(int player_id, int dst_grade_min, int dst_grade_max, int src_grade_min, int src_grade_max)
{
	return GPlayer::GetMe().EnhanceThePlayer(player_id, dst_grade_min, dst_grade_max, src_grade_min, src_grade_max);
}

void L_HandlePressControlSetting()
{
	LeftBtClick(949, 552);
}
void L_HandleControlSetting()
{
	using namespace GType;
	LOG_O(Log_trace) << "处理控制设定";
	auto nor = FuncObj_NormalC::GetInstance();
	if (!nor)
		return;
	if (enGameState_TrainerSelect != nor->GetGameState())
	{
		LOG_O(Log_debug) << "不在教练选择状态中";
		return;
	}
	for (int i = 0; i < 15; ++i)
	{
		try{
			L_HandlePressControlSetting();
			DummySleep(800);
		}
		DO_CATCH_ALL_EXCEPTION({
			LOG_O(Log_debug) << "处理控制设定中出现了异常";
		})
	}
	LOG_O(Log_trace) << "处理完控制设定，就要关闭游戏了";
	nor->Disconnect();
}

void L_Test(pt_csz str){
	Json::Value respond;
	if (!GClass::HttpPackageSend::Post_JsonMsgSync(str, nullptr, respond))
	{
		LOG_O(Log_warning) << "刷新我的出售列表出错";
		return;
	}
}

void SetOutResult(pt_csz res)
{
	if (!res)
		return;
	if (!*res)
		return;

	auto nor = FuncObj_NormalC::GetInstance();
	if (nor)
		nor->GetSessionOper().ToCall(BC_MakeP("SetOutRes") << res);
}

double L_ScanHideCard(pt_dword spid, int grade, double price_lower, double price_high, double price_addition)
{
	if (price_addition < 0)
	{
		assert(false);
		SetOutResult("扫隐藏卡第五个参数错误");
		return -1;
	}
	Json::Value respond;
	CStringA uri;
	for (; price_lower <= price_high; price_lower += price_addition)
	{
		auto real_price = (unsigned __int64)price_lower;
		uri.Format("/transfer/purchase/%u/%u/%I64u/purchase", spid, grade, real_price);
		respond.clear();
		if (!GClass::HttpPackageSend::Post_JsonMsgSync(uri, nullptr, respond))
		{
			uri.Format("扫卡出错,请点下界面，此时出错的价格：%u", real_price);
			SetOutResult(uri);
			return -1;
		}
		if (CheckResFromRespond(respond))
		{
			uri.Format("成功价格:%u", real_price);
			SetOutResult(uri);
			return price_lower;
		}
		DummySleep(100);
	}
	SetOutResult("该价格范围未找到");
	return -1;
}

int L_ScanHideCardAsPrice(pt_dword spid, int grade, double price, int cnt)
{
	if (price <= 0)
	{
		assert(false);
		return 0;
	}
	Json::Value respond;
	CStringA uri;
	auto real_price = (unsigned __int64)price;
	uri.Format("/transfer/purchase/%u/%u/%I64u/purchase", spid, grade, real_price);
	for (int i = 0; i < cnt; ++i)
	{
		respond.clear();
		if (!GClass::HttpPackageSend::Post_JsonMsgSync(uri, nullptr, respond))
		{
			LOG_O(Log_warning) << "按指定的价格扫隐藏卡出错";
			return i;
		}
		if (!CheckResFromRespond(respond))
		{
			return i;
		}
		DummySleep(100);
	}
	return cnt;
}

void L_HandleScanSoldPlayer(const HTTPRequest_RFB& quest)
{
	auto& uri = quest.getURI();
	auto the_pos = uri.find_last_of('/');
	if (the_pos > 0)
	{
		++the_pos;
		if (the_pos >= uri.size())
		{
			assert(false);
			return;
		}
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->GetSessionOper().ToCall(BC_MakeP("SetOutRes") << (uri.c_str() + the_pos));
	}
}

int L_GetThePlayerCnt(int player_id, int grade_min, int grade_max)
{
	return GPlayer::GetMe().GetThePlayerCnt(player_id, grade_min, grade_max);
}

void GLuaC::BindGlobalFn()
{
	__super::BindGlobalFn();
	using namespace luabind;

	module(lua())
		[
			def("SendHttpPackageAsync", &HttpPackageSend::SendHttpPackageAsync),
			def("Post_JsonMsgAsync", &HttpPackageSend::Post_JsonMsgAsync),
			def("Post_JsonMsgSync", &HttpPackageSend::Post_JsonMsgSync),
			def("Get_JsonMsgAsync", &HttpPackageSend::Get_JsonMsgAsync),
			def("Get_JsonMsgSync", &HttpPackageSend::Get_JsonMsgSync),
			def("SendNormalMsgAsync", &HttpPackageSend::SendNormalMsgAsync),
			def("SendGetMsgAsync", &HttpPackageSend::SendGetMsgAsync),
			def("LeftBtClick", &LeftBtClick),
			def("PressEsc", &PressEsc),
			def("PressReturn", &PressReturn),
			def("PressBackSpace", &PressBackSpace),
			def("SetScriptOutputInfo", &SetScriptOutputInfo),
			def("EchoMsg", &EchoMsg),
			def("ExchangeGameAc", &ExchangeGameAc),
			def("LetET", &LetET),
			def("UseSomeItem", &UseSomeItem),
			def("HandleElapseTimers", &HandleElapseTimers),
			//def("OutputScriptInfo", &OutputScriptInfo),
			def("L_EnhanceThePlayer", &L_EnhanceThePlayer),
			def("L_HandlePressControlSetting", &L_HandlePressControlSetting),
			def("L_HandleControlSetting", &L_HandleControlSetting),
			def("L_Test", &L_Test),
			def("L_ScanHideCard", &L_ScanHideCard),
			def("L_ScanHideCardAsPrice", &L_ScanHideCardAsPrice),
			def("L_HandleScanSoldPlayer", &L_HandleScanSoldPlayer),
			def("L_GetThePlayerCnt", &L_GetThePlayerCnt)
		];
}
