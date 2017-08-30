#include "stdafx.h"
#include "GClass.h"
#include <cassert>
#include "Common/LogOut.h"
#include "FuncObjC.h"
#include "GAddr.h"
#include "Protocol/P_BaseStruct.h"
#include "GOffset.h"
#include "GFuncCall.h"
#include "FuncObjC.h"
#include "Common/JsonMore.h"
#include "Socket/PocoMore.h"
#include "json/writer.h"
#include <openssl/ssl.h>
#include <Common/BinaryStream.h>
#include "GPlayer.h"
#include "GMap.h"
#include "../EventsImpl.h"
#include "../EventsName.h"
#include "GCloneMap.h" 

using namespace GClass;
using namespace GAddr;
using namespace GType;
using namespace GOffset;
using namespace GFuncCall;

BOOL _ReadProcessMemory(DWORD dwReadAddr,void* pReadData,DWORD dwReadlen)
{
	return ::ReadProcessMemory((HANDLE)-1,(LPCVOID)dwReadAddr,pReadData,dwReadlen,0);
}
typedef int (*PSENDPACKET)(LPVOID lpParameter,OUT char* pszsendpackebuffer);
typedef struct
{
	DWORD	dwAddr1;
	DWORD	dwAddr2;
	DWORD	dwBufferBeginAddr1;
	DWORD	dwBufferEndAddr;
	DWORD	dwBufferBeginAddr2;
}GAME_BUIILD_OBJ,*PGAME_BUIILD_OBJ;
typedef struct
{
	DWORD dwArgc3;
	DWORD dwArgc4;
}SEND_CALL_ARGC;
typedef struct  
{
	DWORD	str_dwCount;			// 发送伤害封包的数量，代表攻击次数
	char	str_szSkillName[64];	// 技能名称
	char	str_szTypeName[64];		// 攻击对象的类型，如 怪物，箱子
}CMD_HIT,*PCMD_HIT;

/*
bool send(LPVOID lpParameter,PSENDPACKET pFun)
{
	DWORD	dwArgc1 = 0;
	// 读取参数一的值
	_ReadProcessMemory(JFZR_REVERSAL_SEND_PACKET_SOCKET_ADDR,&dwArgc1,sizeof(dwArgc1));
	_ReadProcessMemory(dwArgc1+JFZR_REVERSAL_SEND_PACKET_SOCKET_OFFSET___,&dwArgc1,sizeof(dwArgc1));
	assert(dwArgc1);

	// 组建对象,即调用游戏内部函数申请缓冲区
	GAME_BUIILD_OBJ theBuildObj;
	__asm
	{
		push 0
		lea ecx, theBuildObj
		mov eax, JFZR_REVERSAL_SEND_ALLOC_OBJ_CALL
		call eax
	}

	theBuildObj.dwBufferEndAddr = pFun(lpParameter,(char*)theBuildObj.dwBufferBeginAddr1) + (DWORD)theBuildObj.dwBufferBeginAddr1;
	DWORD	dwSendBufferLen = (DWORD)(WORD)(theBuildObj.dwBufferEndAddr -theBuildObj.dwBufferBeginAddr1);

	if( dwSendBufferLen != 0 )
	{
		*(WORD*)theBuildObj.dwBufferBeginAddr1 = (WORD)dwSendBufferLen;

		SEND_CALL_ARGC theArgc;

		// 基址未获取，暂且先用这个
		DWORD	dwEcx = g_dwSendObjAddr+JFZR_REVERSAL_SEND_PACKET_OFFSET;

		_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_JIAMI_OBJ_OFFSET,&theArgc.dwArgc3,sizeof(theArgc.dwArgc3));
		_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_TASK_OBJ_OFFSET,&theArgc.dwArgc4,sizeof(theArgc.dwArgc4));

		// 发包前任务对象+1
		/ *__asm
		{
			mov EBX, theArgc.dwArgc4
			add EBX, 4
			mov ECX, dword ptr [EBX]
			mov eax, ecx
			add ECX, 1
			LOCK CMPXCHG DWORD PTR DS:[EBX],ECX
		}* /


		__asm
		{
			sub esp, 8
			mov ecx, esp
			lea edx, theArgc		// 加密对象,发包任务对象
			push edx
			mov dword ptr [ecx], 0
			mov dword ptr [ecx+4], 0
			mov eax, JFZR_REVERSAL_SEND_PUSH_ARGC3_ARGC4_CALL
			call eax

			lea eax, theBuildObj	// 明文数据
			push eax				
			push dwArgc1			// socket 对象
			mov eax, JFZR_REVERSAL_SEND_ADDR_CALL
			call eax
			add esp, 0x10
		}

		// 发包结束后任务-1
		/ *__asm
		{
			mov EDX, theArgc.dwArgc4
			add EDX, 4
			OR EAX,0xFFFFFFFF
			LOCK XADD DWORD PTR DS:[EDX],EAX
		}* /
	}


	
	// 释放组建对象时申请的内存
	__asm
	{
		lea ecx, theBuildObj
		mov eax, JFZR_REVERSAL_SEND_FREE_OBJ_CALL
		call eax
	}
	return true;
}
*/

bool GClass::TcpSend::Send( const void* msg, std::size_t msg_size )
{
	if ( !g_pSendObjAddr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "发包对象还不存在，忽略发包";
		//assert(false);
		return false;
	}
	if ( !msg || msg_size < 1 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "param判断不通过";
		assert(false);
		return false;
	}
	
	//游戏发包套接字对象
	GameSendSocket* game_socket_obj = nullptr;
	if ( !MemOperApplyR(game_socket_obj, g_pSendCallSocket, 0, g_o_SendCallSocket) || !game_socket_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "读取游戏发包套接字对象失败";
		assert(false);
		return false;
	}
	PackageCombine build_package;
	if ( !build_package.CombinePackage(msg, msg_size) )
	{
		assert(false);
		return false;
	}
	SendCallHelper helper_obj;
	if ( !helper_obj.RetrieveData() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "索取加密对象或任务对象失败";
		assert(false);
		return false;
	}
	SendCallHelper::SecretHelper secret_hlp;
	secret_hlp.MakeSecret(&helper_obj);

	SendImpl(game_socket_obj, &build_package, secret_hlp);
	return true;
	//////////////////////////////////////////////////////////////////////////
	/*
	DWORD	dwArgc1 = 0;
	// 读取参数一的值
	_ReadProcessMemory(JFZR_REVERSAL_SEND_PACKET_SOCKET_ADDR,&dwArgc1,sizeof(dwArgc1));
	_ReadProcessMemory(dwArgc1+JFZR_REVERSAL_SEND_PACKET_SOCKET_OFFSET___,&dwArgc1,sizeof(dwArgc1));
	assert(dwArgc1);
	
	// 组建对象,即调用游戏内部函数申请缓冲区
	GAME_BUIILD_OBJ theBuildObj;
	__asm
	{
		push 0
		lea ecx, theBuildObj
		mov eax, JFZR_REVERSAL_SEND_ALLOC_OBJ_CALL
		call eax
	}
	CopyMemory((char*)theBuildObj.dwBufferBeginAddr1 + 4, msg, msg_size);
	theBuildObj.dwBufferEndAddr = theBuildObj.dwBufferBeginAddr1 + 4 + msg_size;
	*(WORD*)theBuildObj.dwBufferBeginAddr1 = msg_size + 4;
	*(WORD*)(theBuildObj.dwBufferBeginAddr1 + 2) = 0x55e0;
	
	SEND_CALL_ARGC theArgc;

		// 基址未获取，暂且先用这个
		DWORD	dwEcx = (int)g_pSendObjAddr+JFZR_REVERSAL_SEND_PACKET_OFFSET;

		_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_JIAMI_OBJ_OFFSET,&theArgc.dwArgc3,sizeof(theArgc.dwArgc3));
		_ReadProcessMemory(dwEcx+JFZR_REVERSAL_SEND_PACKET_TASK_OBJ_OFFSET,&theArgc.dwArgc4,sizeof(theArgc.dwArgc4));

		__asm
		{
			sub esp, 8
			mov ecx, esp
			lea edx, theArgc		// 加密对象,发包任务对象
			push edx
			mov dword ptr [ecx], 0
			mov dword ptr [ecx+4], 0
			mov eax, JFZR_REVERSAL_SEND_PUSH_ARGC3_ARGC4_CALL
			call eax

			lea eax, theBuildObj	// 明文数据
			push eax				
			push dwArgc1			// socket 对象
			mov eax, JFZR_REVERSAL_SEND_ADDR_CALL
			call eax
			add esp, 0x10
		}


	
	// 释放组建对象时申请的内存
	__asm
	{
		lea ecx, theBuildObj
		mov eax, JFZR_REVERSAL_SEND_FREE_OBJ_CALL
		call eax
	}
	return true;*/
}

void GClass::TcpSend::SendImpl( GType::GameSendSocket* game_socket_obj, PackageCombine* build_package, const SendCallHelper::SecretHelper& secret_hlp )
{
	g_pOriginalSendCallAddr_.Call<Oper_Void>(game_socket_obj, build_package, secret_hlp.unknown_ptr1_, secret_hlp.unknown_ptr2_);
}

bool GClass::TcpSend::SendMsg( const CMDS_MsgBase& msg )
{
	StrOutputBStream ostm;
	ostm << msg;
	const auto& str_msg = ostm.GetStrBuffer();
	if (str_msg.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "序列化消息失败";
		assert(false);
		return false;
	}
	//LOG_O(Log_trace) << "发送了一个包";
	return Send(str_msg.c_str(), str_msg.size());
}

GClass::TcpSend::TcpSend()
{

}

DWORD GClass::TcpSend::SendMsgTest( const CMDS_MsgBase& msg, const char* buffer_begin, const char* buffer_end )
{
	StrOutputBStream ostm;
	ostm << msg;
	const auto& str_msg = ostm.GetStrBuffer();
	if (str_msg.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "序列化消息失败";
		assert(false);
		return 1;
	}
	buffer_begin += 4;
	auto test_cnt = std::distance(buffer_begin, buffer_end);
	if ( str_msg.size() != test_cnt )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "发包长度与测试缓冲区中的长度不同";
		assert(false);
		return 1;
	}
	auto it_old = str_msg.c_str();
	int cnt = 0;
	auto buffer_old = buffer_begin;
	for ( auto it = str_msg.begin(); it != str_msg.end(); ++it, ++cnt, ++buffer_begin )
	{
		auto selfptr = &*it;
		if ( *selfptr != *buffer_begin )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "测试发包失败，发包内容不同";
			assert(false);
			return DWORD(buffer_begin);
		}
	}
	return 0;
}

GClass::TcpSend::PackageCombine::PackageCombine()
{
	addr1_ = 0;
	addr2_ = 0;
	buffer_begin_addr1_ = 0;
	buffer_end_addr_ = 0;
	buffer_begin_addr2_ = 0;
	g_pSendCallConstructObj_.Call<Oper_Void>(this, 0);
}

GClass::TcpSend::PackageCombine::~PackageCombine()
{
	g_pSendCallDestroyObj_.Call<Oper_Void>(this);
}

bool GClass::TcpSend::PackageCombine::CombinePackage( const void* msg, std::size_t msg_size )
{
	assert(msg && msg_size > 0);
	assert(buffer_begin_addr1_);

	auto* header_ptr = (CMD_HeaderBase*)buffer_begin_addr1_;
	header_ptr->total_length_ = msg_size + sizeof(*header_ptr);
	header_ptr->stable_length_ = JFZR_REVERSAL_PACKET_SEND_HEADER;
	CopyMemory(header_ptr->GetFollowAddr(), msg, msg_size);
	buffer_end_addr_ = (char*)buffer_begin_addr1_ + header_ptr->total_length_;
	return true;
}

GClass::TcpSend::SendCallHelper::SendCallHelper()
{
	secret_obj_ = nullptr;
	task_obj_ = nullptr;
}

bool GClass::TcpSend::SendCallHelper::RetrieveData()
{
	if ( !g_pSendObjAddr )
		return false;

	void* secret_obj_base = (char*)g_pSendObjAddr + g_o_SendCallNeedObj1;
	if ( !MemOperApplyR(secret_obj_, (int)secret_obj_base, g_o_SendCallSecretObj) || !secret_obj_ )
		return false;
	if ( !MemOperApplyR(task_obj_, (int)secret_obj_base, g_o_SendCallTaskObj) || !task_obj_ )
		return false;
	return true;
}

GClass::TcpSend::SendCallHelper::SecretHelper::SecretHelper()
{
	unknown_ptr1_ = nullptr;
	unknown_ptr2_ = nullptr;
}

void GClass::TcpSend::SendCallHelper::SecretHelper::MakeSecret( SendCallHelper* call_helper )
{
	assert(call_helper);
	g_pSendCallAddr_.Call<Oper_Void>(this, call_helper);
}

bool GClass::stGameString::CopyTo( std::wstring& str_out ) const
{
	if ( str_cnt_ <= 0 )
	{
		assert(0 == str_cnt_);
		str_out = kEmptyWStr;
		return true;
	}
	static_assert(8 == _countof(wstr_), "");
	if ( char_total_cnt_ >= _countof(wstr_) )
	{
		str_out = wstr_ptr_;
		assert(str_out.size() == str_cnt_);
		return true;
	}
	str_out = wstr_;
	assert(str_out.size() == str_cnt_);
	return true;
}

std::wstring GClass::stGameString::Duplicate() const
{
	std::wstring str_out;
	VERIFY(CopyTo(str_out));
	return str_out;
}

bool GClass::stGameString::EqualTo( const std::wstring& rhs ) const
{
	if ( str_cnt_ < 0 )
	{
		assert(false);
		return false;
	}
	if ( str_cnt_ != rhs.size() )
		return false;
	if ( char_total_cnt_ >= _countof(wstr_) )
	{
		if ( !wstr_ptr_ )
		{
			assert(false);
			return false;
		}
		return rhs == wstr_ptr_;
	}
	return rhs == wstr_;
}

const wchar_t* GClass::stGameString::GetOriginBegin() const
{
	if ( str_cnt_ <= 0 )
	{
		assert(0 == str_cnt_);
		return nullptr;
	}
	if ( char_total_cnt_ >= _countof(wstr_) )
		return wstr_ptr_;
	else
		return wstr_;
}

const wchar_t* GClass::stGameString::GetOriginEnd() const
{
	if ( str_cnt_ <= 0 )
		return nullptr;
	return GetOriginBegin() + str_cnt_;
}

bool GClass::stGameString::empty() const
{
	if ( str_cnt_ <= 0 )
	{
		assert(0 == str_cnt_);
		return true;
	}
	return false;
}

bool GClass::stGameVectorCopier::CopyTo( GType::stGameVector& game_vec ) const
{
	game_vec = *this;
	return true;
}

GType::stGameVector GClass::stGameVectorCopier::Duplicate() const
{
	GType::stGameVector vec_tmp;
	VERIFY(CopyTo(vec_tmp));
	return vec_tmp;
}

GClass::BTreeBase::BTreeBase()
{
	game_cur_node_ = nullptr;
}

void GClass::BTreeBase::SetCurNode( const stBTreeNodeBase* game_cur_node )
{
	assert(game_cur_node);
	game_cur_node_ = game_cur_node;
}

bool GClass::BTreeBase::Update()
{
	assert(false);
	return false;
}

GClass::AllowedVillageCloneMap::AllowedVillageCloneMap()
{
	level_ = GType::kFD_Invalid;
}

struct stGameAllowedStage : GClass::AllowedVillageCloneMap::stBTreeNodeBase{
	DWORD					unknown1_;
	GClass::stGameString			village_clonemap_name_;
	DWORD					unknown2_[2];
	GType::enFbDifficulty	level_;			//城镇或副本等级
};
static_assert(sizeof(stGameAllowedStage) == sizeof(FB_LEVEL), "FB_LEVEL");

bool GClass::AllowedVillageCloneMap::Update()
{
	using namespace GType;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto node = (stGameAllowedStage*)game_cur_node_;
	if ( !node->village_clonemap_name_.wstr_ptr_ )
		return false;
	if ( !node->village_clonemap_name_.CopyTo(village_clonemap_name_) )
	{
		//assert(false);
		return false;
	}
	if (village_clonemap_name_.empty())
		return false;
	if ( node->level_ < kFD_Zero || node->level_ >= kFD_Invalid )
	{
		assert(false);
		return false;
	}
	level_ = node->level_;
	return true;
}

const std::wstring& GClass::AllowedVillageCloneMap::GetMapId() const
{
	return village_clonemap_name_;
}

GType::enFbDifficulty GClass::AllowedVillageCloneMap::GetLvl() const
{
	return level_;
}

bool GClass::AllowedVillageCloneMap::IsCloneMap() const
{
	using namespace GType;
	return level_ >= kFD_Normal && level_ < kFD_Invalid;
}

bool GClass::AllowedVillageCloneMap::IsVillage() const
{
	if (IsCloneMap())
		return false;
	auto& map_mgr = GStageTemplateMgr::GetMe();
	auto stage_template = map_mgr.FindStage(village_clonemap_name_);
	if ( !stage_template )
	{
		if ( !map_mgr.RebuildUpdateAll() )
		{
			assert(false);
			return false;
		}
		stage_template = map_mgr.FindStage(village_clonemap_name_);
		if ( !stage_template )
			return false;
	}
	return stage_template->IsTown();
}

bool GClass::AllowedVillageCloneMapMgr::RebuildUpdateAll()
{
	auto nodes = (const BTreeMgrBase::stGBTreeNodes*)(g_pRoleObjAddr + g_o_CloneMapOffset1 + g_o_CloneMapOffset2 + g_o_CloneMapOffset3);
	/*if ( nodes->node_cnt_ == btree_.size() )
		return true;*/
	return timer_sync_exec_.TimerGWndExecSync([this](){
		return DoUpdateImpl();
	}, 1000 * 10);
}

const AllowedVillageCloneMap* GClass::AllowedVillageCloneMapMgr::FindByName( const std::wstring& map_name ) const
{
	for ( auto& node : btree_ )
	{
		if ( node.GetMapId() == map_name )
			return &node;
	}
	return nullptr;
}

GClass::AllowedVillageCloneMapMgr::IterBasePtr GClass::AllowedVillageCloneMapMgr::TraverseIf( const DoAnyTraverse& func )
{
	return MakeIterator<TraTraits>(btree_, func);
}

void GClass::AllowedVillageCloneMapMgr::TraverseAll( const DoEveryTraverse& func )
{
	for ( auto& node : btree_ )
	{
		func(node);
	}
}

GClass::AllowedVillageCloneMapMgr::IterBasePtr GClass::AllowedVillageCloneMapMgr::TraverseCloneMap()
{
	return MakeIterator<TraTraits>(btree_, []( const TraTraits::DoFuncItemT node ){
		return node.IsCloneMap();
	});
}

GClass::AllowedVillageCloneMapMgr::IterBasePtr GClass::AllowedVillageCloneMapMgr::TraverseVillage()
{
	return MakeIterator<TraTraits>(btree_, []( const TraTraits::DoFuncItemT node ){
		return node.IsVillage();
	});
}

bool GClass::AllowedVillageCloneMapMgr::DoUpdateImpl()
{
	SetGameNodes((const BTreeMgrBase::stGBTreeNodes*)(g_pRoleObjAddr + g_o_CloneMapOffset1 + g_o_CloneMapOffset2 + g_o_CloneMapOffset3));
	return __super::RebuildUpdateAll();
}

GClass::AllowedVillageCloneMapMgr::IterBasePtr GClass::AllowedVillageCloneMapMgr::TraverseRecommandLvlCloneMap( int the_lvl )
{
	assert(the_lvl > 0);
	return MakeIterator<TraTraits>(btree_, [the_lvl]( const TraTraits::DoFuncItemT node ){
		if ( !node.IsCloneMap() )
			return false;
		auto the_stage = GStageTemplateMgr::GetMe().FindStage(node.GetMapId());
		if ( !the_stage )
			return false;
		if ( !the_stage->IsNormalRecommandLvlFb() )
			return false;
		if ( !the_stage->ValidLvlBetweenRecommandLvl(the_lvl) )
			return false;
		return true;
	});
}

bool GClass::BTreeBase::stBTreeNodeBase::RootNodeValid() const
{
	return this != node_parent_;
}

GClass::GGameTypeBase::GGameTypeBase()
{
	cur_obj_addr_ = nullptr;
}

void GClass::GGameTypeBase::SetCurObj( const void* cur_obj_addr )
{
	assert(cur_obj_addr);
	cur_obj_addr_ = cur_obj_addr;
}

SmartCondition& GClass::GAmbient::GetFirstAnimateCond()
{
	return cond_process_first_animate_;
}

SmartCondition& GClass::GAmbient::GetChooseChannelCond()
{
	return cond_choose_channel_;
}

SmartCondition& GClass::GAmbient::GetChooseRoleCond()
{
	return cond_choose_role_;
}

SmartCondition& GClass::GAmbient::GetGameClockCond()
{
	return cond_game_clock_;
}

SmartCondition& GClass::GAmbient::GetFbRetCityCond()
{
	return cond_ret_city_when_end_clone_map_;
}

SmartCondition& GClass::GAmbient::GetCityUiCond()
{
	return cond_city_ui_now_;
}

void GClass::GAmbient::HandleAmbient_String( const std::string& game_str )
{
	using namespace GType;
	/*if ( game_str == "/Global/Movie/G_IntroMovie" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "第一次的动画";
		cond_process_first_animate_.TouchCond();
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		if ( !nor->KeyClick(VK_ESCAPE) )
		{
			assert(false);
			return;
		}
		return;
	}*/
	// 频道界面
	/*static bool bChannelFlag = false;
	if( game_str == "UiAs.ChannelPage.ChannelListResult.NORMAL" )
	{
		bChannelFlag = true;
		return;
	}
	if(bChannelFlag)
	{
		if( game_str == "/UI/Window_CloseDefault" )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "频道选择界面";
			bChannelFlag = false;
			cond_choose_channel_.TouchCond();
			Init_BeforeRoleEnteredGame();
			auto nor = FuncObj_NormalC::GetInstance();
			if ( !nor )
			{
				assert(false);
				return;
			}
			nor->SetGameState(GType::kGS_SelectChannel);
			return;
		}
	}*/
	/*if ( game_str == "VirtualSetAnimation" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "进入角色选择界面";
		cond_choose_role_.TouchCond();
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		nor->SetGameState(GType::kGS_SelectRole);
		return;
	}*/
	if( game_str == "UiAs.StageSelectList.IntrusionOccur" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "boss乱入";
		GPlayerSelf::GetMe().PlayerDied();
		return;
	}
	if ( game_str == "UiAs.StageClear_RewardInfo_actions.CountTime.second" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "开始读秒";
		cond_game_clock_.TouchCond();
		auto& gps = GPlayerSelf::GetMe();
		VERIFY(gps.FbBossRoomReward());
		cond_ret_city_when_end_clone_map_.ResetCondition();
		return;
	}
	if ( game_str == "UiAs.StageClear_Buttons.StageClearButtonShow.JoinVillage" || game_str == "UiAs.StageClear_Buttons.RestartStage" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "可以从副本中的BOSS房间返回城镇了";
		cond_ret_city_when_end_clone_map_.TouchCond();
		return;
	}
	if ( game_str == "GetEventAlarmInfos" )
	{
		if (cond_choose_role_.CondtionTrue())
		{
			auto& gm = GMap::GetMe();
			if ( !gm.Update() )
			{
				assert(false);
				return;
			}
			auto& obj_mgr = GameObjMgr::GetMe();
			if ( !obj_mgr.RebuildUpdateAll() )
			{
				assert(false);
				return;
			}
			auto& gps = GPlayerSelf::GetMe();
			if ( !gps.Update() )
			{
				assert(false);
				return;
			}
			Init_RoleEnteredGame();
			auto nor = FuncObj_NormalC::GetInstance();
			if ( !nor )
			{
				assert(false);
				return;
			}
			GAllPlayersStaticInfo::GetMe().ClearPlayersInfo();
			nor->SetGameState(GType::kGS_InGame);
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "进入了城镇界面";
		cond_choose_role_.ResetCondition();
		cond_city_ui_now_.TouchCond();
		LOG_O(Log_trace) << "清空对象黑名单";
		GameObjMgr::GetMe().ResetNpcBlacklist();
		return;
	}
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	auto game_stage = nor->GetGameState();
	if ( kGS_InGame == game_stage && game_str == "UiAs.ShortcutList_ShortcutList_actions.ShortcutListInitUi.cashExchangeMarket" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "打开了游戏菜单界面";
		cond_opened_game_menu_.TouchCond();
		return;
	}
	if ( kGS_SelectRole == game_stage && game_str == "/Global/Lobby/G_Story_World01_1" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "故事动画";
		//1级新号进入游戏的故事动画
		CMDS_Msg_SkipStoryAnimate msg;
		TcpSend::GetMe().SendMsg(msg);
		return;
	}
	/*if ( game_str == "/Global/Movie/G_ST01_01_CinemaMovie" || game_str == "/Media/ST01_03_CinemaMovie" || game_str == "/Global/Movie/G_ST01_02_CinemaMovie" )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "副本内动画";
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		if ( !nor->KeyClick(VK_ESCAPE) )
		{
			assert(false);
			return;
		}
		return;
	}*/

	//游戏掉线验证
	static bool bLoginStatusFlag = false;
	if( game_str == "UiAs.WorldSelectPage.LoginResult.NONE" )
	{
		bLoginStatusFlag = true;
		return;
	}
	else if( game_str == "UiAs.WorldSelectPage.LoginResult.defaultMsg" )
	{
		bLoginStatusFlag = true;
		return;
	}
	else
	{
		if( bLoginStatusFlag == true )
		{
			if( game_str == "/UI/Lobby/MenuSlide" )
			{
				//return;
				//MessageBox(NULL,"游戏掉线","",MB_OK);
				bLoginStatusFlag = false;

				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "游戏掉线了";
				auto nor = FuncObj_NormalC::GetInstance();
				if (nor)
					nor->Disconnect();
				return;
			}
		}
	}	
}

bool GClass::GAmbient::UntilCityUiNow( std::size_t wait_time /*= 0 */ )
{
	AutoResetCondT cond(cond_city_ui_now_);
	if ( !cond(wait_time) )
		return false;
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if (events_mgr)
		events_mgr->ActiveEvent(kPEN_RetCity);
	return true;
}

SmartCondition& GClass::GAmbient::GetEnteredFbCond()
{
	return cond_entered_fb_;
}

void GClass::GAmbient::HandleEnteredFb()
{
	using namespace GAddr;
	g_pLocalRoomObjAddr = nullptr;
	cond_entered_fb_.TouchCond();
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "进入了副本";*/

	//if (cond_choose_role_.CondtionTrue())
	{
		auto nor = FuncObj_NormalC::GetInstance();
		if ( !nor )
		{
			assert(false);
			return;
		}
		if ( GType::kGS_SelectRole == nor->GetGameState() )
		{
			auto& gm = GMap::GetMe();
			if ( !gm.Update() )
			{
				assert(false);
				return;
			}
			auto& obj_mgr = GameObjMgr::GetMe();
			if ( !obj_mgr.RebuildUpdateAll() )
			{
				assert(false);
				return;
			}
			auto& gps = GPlayerSelf::GetMe();
			if ( !gps.Update() )
			{
				assert(false);
				return;
			}
			Init_RoleEnteredGame();
			GAllPlayersStaticInfo::GetMe().ClearPlayersInfo();
			nor->SetGameState(GType::kGS_InGame);
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "从选择角色场景直接进入了副本";
			cond_choose_role_.ResetCondition();
		}
		/*else
			assert(false);*/
	}
}

bool GClass::GAmbient::UntilEnteredFb( std::size_t wait_time /*= 0 */ )
{
	static auto fn_cond = [this](){
		using namespace GAddr;
		if (cond_fail_enter_fb_.CondtionTrue())
			return true;
		if ( !cond_entered_fb_.CondtionTrue() )
			return false;
		if ( !g_pLocalRoomObjAddr )
			return false;
		auto& gcm = GCloneMap::GetMe();
		if (gcm.GetRoomId().empty())
		{
			if ( !gcm.Update() )
			{
				assert(false);
				return false;
			}
			if (gcm.GetRoomId().empty())
				return false;
		}
		return true;
	};
	cond_entered_fb_.ResetCondition();
	cond_fail_enter_fb_.ResetCondition();
	if (wait_time)
		WaitUntil(fn_cond, wait_time);
	else
		WaitUntil(fn_cond);
	if ( !cond_entered_fb_.CondtionTrue() )
		return false;
	auto events_mgr = CmnEventsMgr_Impl::GetInstance();
	if (events_mgr)
		events_mgr->ActiveEvent(kPEN_EnterToFb);
	return true;
}

bool GClass::GAmbient::UntilCanRetCity( std::size_t wait_time /*= 0 */ )
{
	AutoResetCondT cond(cond_ret_city_when_end_clone_map_);
	auto& gom = GameObjMgr::GetMe();
	auto& gps = GPlayerSelf::GetMe();
	return cond([&gom, &gps](){
		gom.RebuildUpdateAll();
		gps.KillAllMonster();
	}, 2000, wait_time);
}

void GClass::GAmbient::Init_BeforeRoleEnteredGame()
{
	if (init_before_role_enter_game_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "选择频道场景重复进入，不再初始化了";
		return;
	}

	static GGameObjMgrBase* update_templates[] = {
		&GItemTemplateMgr::GetMe(), &GShopTemplageMgr::GetMe(), &GStageTemplateMgr::GetMe(),
		&GSkillTemplateMgr::GetMe(), &GameEn2ChNodeMgr::GetMe(), &GJobClassTemplateMgr::GetMe(),
		&GClassInfoTemplateMgr::GetMe(), &GRegionStageMapTemplateMgr::GetMe()
	};
	for ( int i = 0; i != _countof(update_templates); ++i )
	{
		if ( !update_templates[i]->RebuildUpdateAll() )
		{
			assert(false);
			continue;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	static GGameObjBase* update_gameobjs[] = {
		&JobNameMap::GetMe(),
	};
	for ( int i = 0;i != _countof(update_gameobjs); ++i )
	{
		if ( !update_gameobjs[i]->Update() )
		{
			assert(false);
			continue;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if ( !GClassInfoTemplateMgr::GetMe().BuildBranchsJobType() )
	{
		assert(false);
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "选择角色前的初始化成功";
	init_before_role_enter_game_ = true;
}

void GClass::GAmbient::Init_RoleEnteredGame()
{
	static GGameObjMgrBase* update_templates[] = {
		&TaskCurJobTemplateMgr::GetMe(), &GSkillTemplateMgr::GetMe()
	};
	for ( int i = 0; i != _countof(update_templates); ++i )
	{
		if ( !update_templates[i]->RebuildUpdateAll() )
		{
			DummySleep(1000);
			VERIFY(update_templates[i]->RebuildUpdateAll());
			continue;
		}
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "角色进入游戏后的初始化完成";

	//////////////////////////////////////////////////////////////////////////
	//*******************改写代码*********************************************
	//static bool wu_di_once = false;
	//无敌
	/*if ( !wu_di_once )
	{
		wu_di_once = true;
		BYTE code_jmp[] = { 0x90, 0x90, 0x90 };
		WirteMemory((void*)g_pWuDiAddr, _countof(code_jmp), code_jmp);
		LOG_O(Log_trace) << "改写无敌";
	}*/
	//////////////////////////////////////////////////////////////////////////
}

void GClass::GAmbient::HandleEnteredSelectRole()
{
	if ( !cond_choose_role_.CondtionTrue() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "进入角色选择界面";
	}	
	cond_choose_role_.TouchCond();
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	nor->SetGameState(GType::kGS_SelectRole);
}

void GClass::GAmbient::HandleEnteredSelectChannel()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "频道选择界面";
	cond_choose_channel_.TouchCond();
	Init_BeforeRoleEnteredGame();
	auto nor = FuncObj_NormalC::GetInstance();
	if ( !nor )
	{
		assert(false);
		return;
	}
	nor->SetGameState(GType::kGS_SelectChannel);
}

bool GClass::GAmbient::UntilChooseRole( std::size_t wait_time /*= 0 */ )
{
	AutoResetCondT cond(cond_choose_role_);
	return cond(wait_time);
}

SmartCondition& GClass::GAmbient::GetOpenedGameMenuCond()
{
	return cond_opened_game_menu_;
}

bool GClass::GAmbient::UntilOpenedGameMenu( std::size_t wait_time /*= 0 */ )
{
	AutoResetCondT cond(cond_opened_game_menu_);
	return cond(wait_time);
}

GClass::GAmbient::GAmbient()
{
	init_before_role_enter_game_ = false;
}

bool GClass::GAmbient::IsInitBefore() const
{
	return init_before_role_enter_game_;
}

void GClass::GAmbient::FailToEnterFb()
{
	cond_fail_enter_fb_.TouchCond();
}

GClass::GAmbient GClass::GAmbient::ambient_instance_;

GClass::GAllPlayersStaticInfo GClass::GAllPlayersStaticInfo::players_static_info_;

const GClass::GAllPlayersStaticInfo::PlayersContT& GClass::GAllPlayersStaticInfo::GetPlayersInfo() const
{
	return players_info_;
}

void GClass::GAllPlayersStaticInfo::ClearPlayersInfo()
{
	players_info_.clear();
}

void GClass::GAllPlayersStaticInfo::AddPlayersInfo( const CMDR_PartialListPlayersStaticInfo& player_info )
{
	auto it = std::find_if(players_info_.begin(), players_info_.end(), [&player_info]( const CMDR_PartialListPlayersStaticInfo& player ){
		return player.player_name_ == player_info.player_name_;
	});
	if ( it != players_info_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << player_info.player_name_ << "\t已经存在所有角色信息里了，手动创建教练才有这样的信息";
		//assert(false);
		return;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "add player info->" << player_info.player_name_;
	players_info_.push_back(player_info);
}

const CMDR_PartialListPlayersStaticInfo* GClass::GAllPlayersStaticInfo::FindByRoleName( const std::wstring& role_name ) const
{
	for ( auto& v : players_info_ )
	{
		if ( v.player_name_ == role_name )
			return &v;
	}
	return nullptr;
}

bool GClass::GAllPlayersStaticInfo::GenRolesInfoToJson( Json::Value& json_array ) const
{
	if (players_info_.empty())
	{
		json_array.clear();
		return true;
	}
	CG_RoleInfo role_info;
	Json::Value json_v;
	for ( const auto& role : players_info_ )
	{
		if ( !role.ConvertToRoleInfo(role_info) )
		{
			assert(false);
			return false;
		}
		if ( !role_info.ConvertToJson(json_v) )
		{
			assert(false);
			return false;
		}
		json_array.append(json_v);
	}
	return true;
}

bool GClass::GAllPlayersStaticInfo::RolesInfoEmpty() const
{
	return players_info_.empty();
}

LRESULT GClass::GWnd::WndProc( UINT message, WPARAM wParam, LPARAM lParam ) const
{
	using namespace GAddr;
	return g_pGameWndProcCal.Call<LRESULT>(this, message, wParam, lParam);
}

GClass::JobNameMap::JobNameMap()
{
	/*
	using namespace GType;
	AddMap(kJC_JianDouShi, L"WARRIOR", L"剑斗士");
	AddMap(kJC_RenWuZhe, L"THIEF", L"刃武者");
	AddMap(kJC_ZhanKuang, L"GLADIBERSERKER", L"斩狂");
	AddMap(kJC_QuanSha, L"GIGONGSA", L"拳刹");
	AddMap(kJC_FengWuXing, L"RIPPER", L"风舞星");
	AddMap(kJC_LieRenXing, L"ASSASSIN", L"猎刃星");
	AddMap(kJC_TianLangXing, L"MISTRESSOFFENRIR", L"天狼星");
	AddMap(kJC_JianMo, L"DOOMBLADE", L"剑魔");
	AddMap(kJC_KongMoShi, L"MAGICGUNNER", L"控魔师");
	AddMap(kJC_MoYingShi, L"DARKSTALKER", L"魔影师");
	AddMap(kJC_ShiKongShuShi, L"ARCHITECT", L"时空术士");
	AddMap(kJC_ShuangYuZhe, L"ICEMAGICIAN", L"霜语者");
	AddMap(kJC_LuanZhan, L"F_SCYTHE_00", L"乱战");
	AddMap(kJC_XiXueYaoHua, L"F_SCYTHE_01", L"吸血妖化");
	AddMap(kJC_XieHunShuShi, L"F_SCYTHE_02", L"邪魂术士");
	AddMap(kJC_KuLuoSha, L"F_SCYTHE_03", L"苦罗刹");*/
}

bool GClass::JobNameMap::AddMap( GType::enJobCategory job, const std::wstring& ch_name )
{
	using namespace GType;
	if ( job < kJC_JianDouShi || job >= kJC_Any )
	{
		assert(false);
		return false;
	}
	if ( job_name_map_.find(job) != job_name_map_.end() )
	{
		assert(false);
		return false;
	}
	for ( const auto& v : job_name_map_ )
	{
		if ( v.second == ch_name )
		{
			assert(false);
			return false;
		}
	}
	job_name_map_[job] = ch_name;
	return true;
}

const std::wstring& GClass::JobNameMap::GetChName( GType::enJobCategory job ) const
{
	auto it = job_name_map_.find(job);
	if ( it == job_name_map_.end() )
	{
		assert(false);
		return kEmptyWStr;
	}
	return it->second;
}

const std::wstring& GClass::JobNameMap::GetEnName( GType::enJobCategory job ) const
{
	const auto& cls_mgr = GJobClassTemplateMgr::GetMe();
	auto cls_template = cls_mgr.FindByJobType(job);
	if ( !cls_template )
	{
		assert(false);
		return kEmptyWStr;
	}
	return cls_template->GetJobClsId();
}

GType::enJobCategory GClass::JobNameMap::FromChName( const std::wstring& ch_name ) const
{
	for ( const auto& v : job_name_map_ )
	{
		if ( v.second == ch_name )
			return v.first;
	}
	return GType::kJC_Invalid;
}

GType::enJobCategory GClass::JobNameMap::FromEnName( const std::wstring& en_name ) const
{
	const auto& cls_mgr = GJobClassTemplateMgr::GetMe();
	auto cls_template = cls_mgr.FindByJobClsId(en_name);
	if ( !cls_template )
	{
		assert(false);
		return GType::kJC_Invalid;
	}
	return cls_template->GetJobType();
}

bool GClass::JobNameMap::Update()
{
	if ( !job_name_map_.empty() )
		return true;
	const auto& cls_mgr = GClassInfoTemplateMgr::GetMe();
	const auto& branch_skill_cont = cls_mgr.GetAllNodes();
	if (branch_skill_cont.empty())
	{
		assert(false);
		return false;
	}
	const auto& job_cls_mgr = GJobClassTemplateMgr::GetMe();
	const auto& job_cls_cont = job_cls_mgr.GetCont();
	if (job_cls_cont.empty())
	{
		assert(false);
		return false;
	}
	if ( branch_skill_cont.size() != job_cls_cont.size() )
	{
		assert(false);
		return false;
	}
	auto branch_it_ = branch_skill_cont.begin();
	auto branch_it_end = branch_skill_cont.end();
	auto job_cls_it = job_cls_cont.begin();
	auto job_cls_it_end = job_cls_cont.end();
	const auto& en2ch = GameEn2ChNodeMgr::GetMe();
	for ( ; branch_it_ != branch_it_end && job_cls_it != job_cls_it_end; ++branch_it_, ++job_cls_it )
	{
		auto cls_template = job_cls_mgr.FindByJobClsId(branch_it_->GetJobId());
		if ( !cls_template )
		{
			assert(false);
			return false;
		}
		const auto& job_ch_name = en2ch.FindChByFullPath(branch_it_->GetJobClassName());
		if (job_ch_name.empty())
		{
			assert(false);
			return false;
		}
		if ( !AddMap(cls_template->GetJobType(), job_ch_name) )
		{
			assert(false);
			return false;
		}
	}
	return true;
}

const std::wstring& GClass::JobNameMap::En2ChName( const std::wstring& en_name ) const
{
	const auto& ch_name = GetChName(FromEnName(en_name));
	if (ch_name.empty())
	{
		assert(false);
		return kEmptyWStr;
	}
	return ch_name;
}

bool GClass::GJobClassTemplateMgr::RebuildUpdateAll()
{
	using namespace GClass;
	using namespace GAddr;
	using namespace GOffset;
	using namespace GType;
	if ( !job_classes_.empty() )
		return true;
	const auto kBranchSize = (int)(kJC_Any - 1);
	GJobClassTemplate job_cls_template_tmp;
	auto job_cls_begin = (const GJobClassTemplate::stGameJobClassType*)g_pJobClassNameTemplate;
	const auto job_cls_end = job_cls_begin + kBranchSize;
	for ( ;job_cls_begin < job_cls_end; ++job_cls_begin )
	{
		job_cls_template_tmp.SetGameJobClsType(job_cls_begin);
		if ( !job_cls_template_tmp.Update() )
		{
			assert(false);
			return false;
		}
		job_classes_.push_back(job_cls_template_tmp);
	}
	assert(!job_classes_.empty());
	return true;
}

bool GClass::GJobClassTemplateMgr::UpdateAll()
{
	if ( !job_classes_.empty() )
		return true;
	assert(false);
	return false;
}

const GClass::GJobClassTemplateMgr::ContT& GClass::GJobClassTemplateMgr::GetCont() const
{
	return job_classes_;
}

const GJobClassTemplate* GClass::GJobClassTemplateMgr::FindByJobType( GType::enJobCategory job ) const
{
	for ( const auto& v : job_classes_ )
	{
		if ( v.GetJobType() == job )
			return &v;
	}
	return nullptr;
}

const GJobClassTemplate* GClass::GJobClassTemplateMgr::FindByJobClsId( const std::wstring& job_cls_id ) const
{
	if (job_cls_id.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( const auto& v : job_classes_ )
	{
		if ( v.GetJobClsId() == job_cls_id )
			return &v;
	}
	return nullptr;
}

GClass::GJobClassTemplate::GJobClassTemplate()
{
	game_job_cls_type_ = nullptr;
	job_type_ = GType::kJC_Invalid;
}

void GClass::GJobClassTemplate::SetGameJobClsType( const stGameJobClassType* game_job_cls_type )
{
	assert(game_job_cls_type);
	game_job_cls_type_ = game_job_cls_type;
}

GType::enJobCategory GClass::GJobClassTemplate::GetJobType() const
{
	return job_type_;
}

const std::wstring& GClass::GJobClassTemplate::GetJobClsId() const
{
	return job_cls_id_;
}

bool GClass::GJobClassTemplate::Update()
{
	using namespace GType;
	if ( !game_job_cls_type_ )
	{
		assert(false);
		return false;
	}
	if ( game_job_cls_type_->str_dwClassType < kJC_JianDouShi || game_job_cls_type_->str_dwClassType >= kJC_Any )
	{
		assert(false);
		return false;
	}
	job_type_ = game_job_cls_type_->str_dwClassType;
	if ( !game_job_cls_type_->str_theClass.CopyTo(job_cls_id_) )
	{
		assert(false);
		return false;
	}
	return true;
}

// 遍历对象，包括 副本中的 玩家，怪物，阻碍物，掉落物品  城镇中 NPC，玩家
_declspec(naked) void TraverseLocalObj()	// 00850310
{
	__asm
	{
		MOV EAX,ECX
			MOV EDX,DWORD PTR DS:[EAX]                        ; oldnode
			CMP BYTE PTR DS:[EDX+0x19],0x0                    ; cmp byte ptr[oldnode+19],0
			JNZ SHORT local_end
			MOV ECX,DWORD PTR DS:[EDX+0x8]                    ; R
			CMP BYTE PTR DS:[ECX+0x19],0x0
			JNZ SHORT local_r
			MOV EDX,DWORD PTR DS:[ECX]                        ; P
			CMP BYTE PTR DS:[EDX+0x19],0x0
			JNZ SHORT local_p
			JMP SHORT local_1
			LEA ECX,DWORD PTR DS:[ECX]
local_1:
		MOV ECX,EDX
			MOV EDX,DWORD PTR DS:[ECX]
		CMP BYTE PTR DS:[EDX+0x19],0x0
			JE SHORT local_1
local_p:
		MOV DWORD PTR DS:[EAX],ECX
			RETN
local_r:
		MOV EDX,DWORD PTR DS:[EDX+0x4]                    ; L
			CMP BYTE PTR DS:[EDX+0x19],0x0
			JNZ SHORT local_2
local_3:
		MOV ECX,DWORD PTR DS:[EAX]                        ; mov ecx, oldnode
			CMP ECX,DWORD PTR DS:[EDX+0x8]                    ; cmp oldnode, [[oldnode+4]+8]
		JNZ SHORT local_2
			MOV DWORD PTR DS:[EAX],EDX
			MOV EDX,DWORD PTR DS:[EDX+0x4]
		CMP BYTE PTR DS:[EDX+0x19],0x0
			JE SHORT local_3
local_2:
		MOV DWORD PTR DS:[EAX],EDX
local_end:
		RETN
	}
}

void GClass::GGameObjTraverse::NextGameObj( const GGameObjTraverse** next_game_obj )
{
	__asm
	{
		mov ecx, next_game_obj
		mov eax, TraverseLocalObj
		call eax
	}
}

bool GClass::TimerSyncExec::TimeoutToDoUpdate( int time_out ) const
{
	return time_last_update_.IsTimeout(time_out);
}

bool GClass::TimerSyncExec::AutoTimeoutToDoUpdate( int time_out )
{
	return time_last_update_.DoTimeout(time_out);
}

bool GClass::TimerSyncExec::TimerGWndExecSync( const std::function<bool()>& func, int time_out )
{
	if ( !AutoTimeoutToDoUpdate(time_out) )
		return true;
	return GWndExecSync(func);
}

GClass::TimerSyncExec::TimerSyncExec() : time_last_update_(80)
{
}

GClass::AppleDateTime::AppleDateTime()
{
	ZeroMemory(this, sizeof(*this));
}

bool GClass::LocalRoomPos::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
		return false;
	}
	auto room_ptr = (const GameType*)cur_obj_addr_;
	if (IsUnValidPtr(room_ptr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(room_ptr->pos_ptr_))
	{
		assert(false);
		return false;
	}
	room_pos_ = room_ptr->pos_ptr_->theRoomPosition;
	return true;
}

bool GClass::LocalRoomPosMgr::RebuildUpdateAll()
{
	return GWndExecSync([this](){
		return UpdateImpl();
	});
}

bool GClass::LocalRoomPosMgr::FindByXY( float x, float y, float& z )
{
	if (FindByXY_Impl(x, y, z))
		return true;
	if ( !RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return FindByXY_Impl(x, y, z);
}

bool GClass::LocalRoomPosMgr::UpdateImpl()
{
	using namespace GAddr;
	using namespace GOffset;
	if (IsUnValidPtr(g_pLocalRoomPos))
	{
		assert(false);
		return false;
	}
	SetGameTArray((const LocalRoomPosMgr::stGameTArray*)(int(g_pLocalRoomPos) + g_o_local_room_pos));
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GClass::LocalRoomPosMgr::FindByXY_Impl( float x, float y, float& z )
{
	for ( auto& v : cont_ )
	{
		if ( (int)v.room_pos_.x_ == (int)x && (int)v.room_pos_.y_ == (int)y )
		{
			z = v.room_pos_.z_;
			return true;
		}
	}
	return false;
}

bool GClass::stPreActionPlayTime::stPapt::Update()
{
	auto pdata = (const PREV_ACTION_PLAY_TIME_3*)game_cur_node_;
	if ( pdata->dwLocalObjAddr < WORD(-1) )
	{
		assert(false);
		return false;
	}
	pre_action_serial_ = *(DWORD*)(pdata->dwLocalObjAddr);
	action_play_time = *(float*)(pdata->dwLocalObjAddr + g_o_pre_action_play_time3);
	return true;
}

bool GClass::stPreActionPlayTime::Update()
{
	assert(0 == serial_);
	assert(!next_node_);
	auto thePrevActionPlayTime_1 = (const PREV_ACTION_PLAY_TIME_1*)game_cur_node_;
	if( !thePrevActionPlayTime_1->dwLocalObjAddr )
		return true;
	assert(thePrevActionPlayTime_1->dwLocalObjAddr > WORD(-1));
	next_node_ = (const PREV_ACTION_PLAY_TIME_2*)thePrevActionPlayTime_1->dwLocalObjAddr;
	serial_ = next_node_->dwSerial;
	BTreeMgrBase::stGBTreeNodes* the_node = (BTreeMgrBase::stGBTreeNodes*)&next_node_->theMapAddr;
	papt_mgr_.SetGameNodes(the_node);
	if ( !papt_mgr_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

const GClass::stPreActionPlayTime::stPapt* GClass::stPreActionPlayTime::GetPapt() const
{
	const auto& the_cont = papt_mgr_.GetAllNodes();
	if ( 1 != the_cont.size() )
	{
		assert(false);
		return nullptr;
	}
	return &the_cont[0];
}

const stPreActionPlayTime* GClass::stPreActionPlayTimeMgr::FindBySerial( DWORD serial )
{
	assert(serial);
	auto it = std::find_if(btree_.begin(), btree_.end(), [serial]( const stPreActionPlayTime& pa ){
		return pa.serial_ == serial;
	});
	if ( it == btree_.end() )
		return nullptr;
	return &*it;
}

bool GClass::stPreActionPlayTimeMgr::RebuildUpdateAll()
{
	return GWndExecSync([this](){
		return UpdateImpl();
	});
}

bool GClass::stPreActionPlayTimeMgr::UpdateImpl()
{
	using namespace GAddr;
	using namespace GOffset;
	if ( !g_pPreActionPlayTimeAddr )
	{
		assert(false);
		return 0.;
	}
	BTreeMgrBase::stGBTreeNodes* the_node = nullptr;
	if ( !MemOperApplyR(the_node, (int)g_pPreActionPlayTimeAddr, g_o_pre_action_play_time1) )
	{
		assert(false);
		return 0.;
	}
	if ( !the_node )
	{
		assert(false);
		return 0.;
	}
	the_node = (BTreeMgrBase::stGBTreeNodes*)((char*)the_node + g_o_pre_action_play_time2);
	SetGameNodes(the_node);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}
