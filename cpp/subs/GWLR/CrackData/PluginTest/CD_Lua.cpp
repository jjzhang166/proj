#include "stdafx.h"
#include "CD_Lua.h"
#include <CD_CommonFiles/Data/CD_Structs.h>
#include <CD_CommonFiles/Data/CD_Traverse.h>
#include <CD_CommonFiles/Data/CD_ProtocolS.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CommonX/CmnX_UsefulFunc.h>

CD_LuaVM::CD_LuaVM() : LuaVM_Base(_T(""))
{
}

void LoadScriptFile( const char* file_name )
{
	if ( !file_name )
	{
		assert(false);
		return;
	}
	std::string str_file("F:\\proj\\GWLR\\CrackData\\PluginTest\\LuaScript\\");
	str_file += file_name;
	str_file += ".lua";

	CD_LuaVM::GetMe().LoadFile(str_file.c_str());
}

void DebugInfo_MsgSize(int msg_id)
{
	char buffer[sizeof(stCD_MsgBase) + 100];
	auto msg = (stCD_MsgBase*)buffer;
	msg->msg_id_ = msg_id;
	LOG_O(Log_trace) << msg->GetMsgLength();
}

void DebugInfo_SayMsg(int say_type, const char* str)
{
	stCD_Msg_SayMsg msg;
	std::memset(&msg, 0, sizeof(msg));
	msg.say_type_ = (stCD_Msg_SayMsg::enSayType)say_type;
	TStrCpy(msg.msg_buffer_, str, sizeof(msg.msg_buffer_));
	msg.array_size_ = 0xa;
	*(int*)&msg.array_[0] = 0xaa;
	*(int*)&msg.array_[4] = 0x3000100a;
	*(short*)&msg.array_[8] = 0xfdd;
	LOG_O(Log_trace) << "res: " << CD_FuncCall::SendT(msg);
}

void L_DebugInfoTheObjs(const luabind::object& types)
{
	using namespace luabind;
	std::vector<CD_En_LocalObjType> the_types;
	for (iterator it(types), it_end; it != it_end; ++it)
	{
		auto v = ObjectCast(*it, (int)CD_En_LocalObjType::kInvalid);
		the_types.push_back((CD_En_LocalObjType)v);
	}
	CD_Traverse::DebugInfo_TheLocalObjs(the_types.data(), the_types.size());
}

void L_HandleSomeUiCtrlMsg(pt_csz ui_cls_name, const luabind::object& msgs)
{
	using namespace luabind;
	std::vector<CD_En_UiCtrlMsg> the_types;
	for (iterator it(msgs), it_end; it != it_end; ++it)
	{
		auto v = ObjectCast(*it, 0);
		the_types.push_back((CD_En_UiCtrlMsg)v);
	}
	std::vector<stCD_UiCtrlMsg> the_msgs;
	for (auto v : the_types)
	{
		stCD_UiCtrlMsg tmp;
		tmp.type_ = v;
		the_msgs.push_back(tmp);
	}
	auto the_ui = CD_FuncCall::FindUiCtrlByFullClsName(ui_cls_name);
	if (!the_ui)
	{
		assert(false);
		return;
	}
	the_ui->HandleSomeMsg(the_msgs.data(), the_msgs.size());
}

void L_UiCtrlEdit_PutStr(pt_csz cls_full_name, pt_csz str)
{
	auto ptr = CD_FuncCall::FindUiCtrlByFullClsName(cls_full_name);
	if (!ptr)
	{
		assert(false);
		return;
	}
	std::wstring tmp = CA2CT(str);
	CD_FuncCall::UiCtrlEdit_PutString(ptr, tmp.c_str());
}

void L_LpkaTest(){
	auto lpka = GetModuleHandleA("lpka.dll");
	if (!lpka)
	{
		LOG_O(Log_error) << "lpka handle error";
		return;
	}
	typedef void(*Fn)();
	auto fn = (Fn)GetProcAddress(lpka, "MyUseGDIWidthCache");
	if (!fn)
	{
		assert(false);
		return;
	}
	fn();
}

luabind::object L_Test(const luabind::object& id)
{
	luabind::object res = luabind::newtable(id.interpreter());
	res[1] = 1;
	res[2] = 2;
	return res;
}

void CD_LuaVM::BindGlobalFn()
{
	__super::BindGlobalFn();

	using namespace luabind;
	module(lua())
		[
			def("LoadScriptFile", &LoadScriptFile),
			def("DebugInfo_MsgSize", &DebugInfo_MsgSize),
			def("DebugInfo_SayMsg", &DebugInfo_SayMsg),
			def("DebugInfo_NetObjs", &CD_Traverse::DebugInfo_NetObjs),
			def("DebugInfo_LocalObjs", &CD_Traverse::DebugInfo_LocalObjs),
			def("DebugInfo_CurRoleInfo", &CD_Traverse::DebugInfo_CurRoleInfo),
			def("DebugInfo_CurMapInfo", &CD_Traverse::DebugInfo_CurMapInfo),
			def("DebugInfo_AllPackagesMgr", &CD_Traverse::DebugInfo_AllPackagesMgr),
			def("DebugInfo_TheItemDb", &CD_Traverse::DebugInfo_TheItemDb),
			def("DebugInfo_TheTaskDb", &CD_Traverse::DebugInfo_TheTaskDb),
			def("DebugInfo_TheMapDb", &CD_Traverse::DebugInfo_TheMapDb),
			def("DebugInfo_ReceivedTask", &CD_Traverse::DebugInfo_ReceivedTask),
			def("DebugInfo_AccetableTask", &CD_Traverse::DebugInfo_AccetableTask),
			def("DebugInfo_ChapterInfo", &CD_Traverse::DebugInfo_ChapterInfo),
			def("DebugInfo_EnterableFbInfo", &CD_Traverse::DebugInfo_EnterableFbInfo),
			def("DebugInfo_TheFarmDb", &CD_Traverse::DebugInfo_TheFarmDb),
			def("DebugInfo_CurFarmFacility", &CD_Traverse::DebugInfo_CurFarmFacility),
			def("DebugInfo_TheShopDb", &CD_Traverse::DebugInfo_TheShopDb),
			def("DebugInfo_TheSeedDb", &CD_Traverse::DebugInfo_TheSeedDb),
			def("DebugInfo_TheCraftDb", &CD_Traverse::DebugInfo_TheCraftDb),
			def("DebugInfo_TheMenufactureDb", &CD_Traverse::DebugInfo_TheMenufactureDb),
			def("DebugInfo_GiftInfo", &CD_Traverse::DebugInfo_GiftInfo),
			def("DebugInfo_SupplyBox", &CD_Traverse::DebugInfo_SupplyBox),
			def("DebugInfo_Mails", &CD_Traverse::DebugInfo_Mails),
			def("DebugInfo_TheBuffDb", &CD_Traverse::DebugInfo_TheBuffDb),
			def("DebugInfo_CurBuff", &CD_Traverse::DebugInfo_CurBuff),
			def("DebugInfo_AllLocalObjs", &CD_Traverse::DebugInfo_AllLocalObjs),
			def("DebugInfo_CurShangJinInfo", &CD_Traverse::DebugInfo_CurShangJinInfo),
			def("DebugInfo_KingQuestInfo", &CD_Traverse::DebugInfo_KingQuestInfo),
			def("TraTest", &CD_Traverse::TraTest),
			def("L_DebugInfoTheObjs", &L_DebugInfoTheObjs),
			def("DebugInfo_CurUiCtrlInfo", &CD_Traverse::DebugInfo_CurUiCtrlInfo),
			def("UiCtrl_LeftBtClickByFullClsName", &CD_FuncCall::UiCtrl_LeftBtClickByFullClsName),
			def("L_HandleSomeUiCtrlMsg", &L_HandleSomeUiCtrlMsg),
			def("L_UiCtrlEdit_PutStr", &L_UiCtrlEdit_PutStr),
			def("L_LpkaTest", &L_LpkaTest),
			def("L_Test", &L_Test),
			def("DebugInfo_UiMng", &CD_Traverse::DebugInfo_UiMng)
		];
}

void CD_LuaVM::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	module(lua())
		[
			class_<CD_LuaVM, LuaVM_Base>("CD_LuaVM"),
			def("CD_LuaVM_Me", &CD_LuaVM::GetMe)
		];
}

bool CD_LuaVM::DoPrivateLuaFileByFile()
{
	return false;
}
