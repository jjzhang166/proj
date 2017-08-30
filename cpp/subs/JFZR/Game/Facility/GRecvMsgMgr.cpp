#include "stdafx.h"
#include "GRecvMsgMgr.h"
#include "GFuncCall.h"

void GRecvMsgMgrBase::AddCmd( const wchar_t* hash_uri, CMDR_MsgBase* cmd_placeholder, WORD ret_offset )
{
	using namespace GFuncCall;
	assert(hash_uri);
	assert(cmd_placeholder);
	AddCmd(CalcHash_WORD(hash_uri), cmd_placeholder, ret_offset);
}

void GRecvMsgMgrBase::AddCmd( WORD hash_uri, CMDR_MsgBase* cmd_placeholder, WORD ret_offset )
{
	assert(hash_uri > 0);
	const auto it = cmd_map_.find(hash_uri);
	if ( it != cmd_map_.end() )
	{
		assert(false);
		return;
	}
	stMsgInfo msg_info = {
		cmd_placeholder, ret_offset
	};
	cmd_map_[hash_uri] = msg_info;
}

GRecvMsgMgr::GRecvMsgMgr()
{
	/*AddTemplateCmd<CMDR_AllowSwitchChannel>(L"Apple::Protocol::FromServer::SessionSystem::AllowSwitchChannel");
	AddTemplateCmd<CMDR_ChannelList>(L"Apple::Protocol::FromServer::SessionSystem::ChannelList");
	AddTemplateCmd<CMDR_PartialListPlayersStaticInfo>(L"PartialListPlayersStaticInfo");
	AddTemplateCmd<CMDR_EndPartialListPlayers>(L"EndPartialListPlayers");
	AddTemplateCmd<CMDR_OnCreatePlayer>(L"Apple::Protocol::FromServer::PlayerManagementSystem::CreatePlayer");
	AddTemplateCmd<CMDR_OnMailList>(L"Apple::Protocol::FromServer::MailSystem::MailList");
	AddTemplateCmd<CMDR_OnMyProductList>(L"Apple::Protocol::FromServer::MarketSystem::RequestMyProductList");
	AddTemplateCmd<CMDR_RecvMailContents>(L"Apple::Protocol::FromServer::MailSystem::MailContents");
	AddTemplateCmd<CMDR_OnGetAttachments>(L"Apple::Protocol::FromServer::MailSystem::GetAttachments");
	AddTemplateCmd<CMDR_OnRegisterProduct>(L"Apple::Protocol::FromServer::MarketSystem::RegisterProduct");
	AddTemplateCmd<CMDR_OnBuyProduct>(L"Apple::Protocol::FromServer::MarketSystem::BuyProduct");
	AddTemplateCmd<CMDR_OnRequestProductInfo>(L"Apple::Protocol::FromServer::MarketSystem::RequestProductInfo");
	AddTemplateCmd<CMDR_OnRequestProductList>(L"Apple::Protocol::FromServer::MarketSystem::RequestProductList");*/
	AddTemplateCmd<CMDR_AllowSwitchChannel>(L"94d50b73864f5682d3e091ef5d884559");
	AddTemplateCmd<CMDR_ChannelList>(L"2bdfa59b5e93cdb9d4a1313aebd0ae25");
	AddTemplateCmd<CMDR_PartialListPlayersStaticInfo>(L"82de8827ee0d06331d1b1d583fb0d2ba");
	AddTemplateCmd<CMDR_EndPartialListPlayers>(L"53b7ee7068b502dd9a867a7e963062a9");
	AddTemplateCmd<CMDR_OnCreatePlayer>(L"b3283ba862b85d061ba4f8cd99ed17a5");
	AddTemplateCmd<CMDR_OnMailList>(L"3d894648a534f8205210c4aaad92974e");
	AddTemplateCmd<CMDR_OnMyProductList>(L"31bc866842a5439e308a170bfa24473b");
	//AddTemplateCmd<CMDR_RecvMailContents>(L"2197cd5a4edff09f1559c717d18ee605");
	AddTemplateCmd<CMDR_OnGetAttachments>(L"329656d9f5a5817482ee735c01652a7e");
	AddTemplateCmd<CMDR_OnRegisterProduct>(L"3d7f0e591dd14f39d41ce8e3e57e9b28");
	AddTemplateCmd<CMDR_OnBuyProduct>(L"79deee93b8fa134771559f02d165f17f");
	AddTemplateCmd<CMDR_OnRequestProductInfo>(L"27a58432f813df2395f896dc77bf6d59");
	AddTemplateCmd<CMDR_OnRequestProductList>(L"90220ab243e8bade2776266fea9260db");
	static CMDR_OnLoginResult static_on_login;
	AddCmd(0x000057B6, &static_on_login, 0);
	AddTemplateCmd<CMDR_OnSellItemResult>(L"97cd2d91540399d862ba54fd2a7c0034");
	AddTemplateCmd<CMDR_OnStartStageResult>(L"d313982cc304fdb314419d2c0cae263a");
	AddTemplateCmd<CMDR_OnCreateNPC>(L"bd1cca3fb041b5a1acc2f523ac85b8a6");
	AddTemplateCmd<CMDR_OnAcceptQuestResult>(L"faac064cc47448f533db91322a3d16e4");
	AddTemplateCmd<CMDR_OnPlayerDead>(L"c85a0cd95fabae899231e8e33d33d831");
	AddTemplateCmd<CMDR_OnChangeVillage>(L"156560dcd0fcee4201d7fbace468b5d2");
}

WORD GRecvMsgMgrBase::HandleMsg( const CMDR_HeaderBase* cmd_header )
{
	if ( !cmd_header )
	{
		assert(false);
		return 0;
	}
	if ( JFZR_REVERSAL_PACKET_SEND_HEADER != cmd_header->stable_length_ )
		return 0;
	const auto it = cmd_map_.find(cmd_header->hash_uri_);
	if ( it == cmd_map_.end() )
		return 0;
	const auto kFollowSize = cmd_header->GetRealMsgSize();
	if ( kFollowSize < 0 )
	{
		assert(false);
		return 0;
	}

	CMDR_MsgBase::SelfPtr cmd(it->second.handler_msg_->Clone());
	if ( !cmd )
	{
		assert(false);
		return 0;
	}
	StableInputBStream istm((const char*)cmd_header->GetRealMsgAddr(), kFollowSize);
	if ( !cmd->Read(istm) )
	{
		assert(false);
		return 0;
	}
	auto new_ret_offset = cmd->NewRetOffset();
	WORD ret_offset;
	if (new_ret_offset)
		ret_offset = *new_ret_offset;
	else
		ret_offset = it->second.ret_offset_;
	GetIoService().dispatch([cmd](){
		assert(cmd);
		cmd->OnReceived();
	});
	return ret_offset;
}

WORD GRecvMsgMgrBase::RecvPacket( const void* packet )
{
	auto cmd_header = (const CMDR_HeaderBase*)packet;
	/*if ( packet_size != cmd_header->total_length_ )
	{
		assert(false);
		return;
	}*/
	return HandleMsg(cmd_header);
}

GRecvMsgMgr GRecvMsgMgr::msg_mrg_;
GHookGameSendMsgMgr GHookGameSendMsgMgr::msg_mrg_;

GHookGameSendMsgMgr::GHookGameSendMsgMgr()
{
	//AddTemplateCmd<CMDR_Hook_EnteredCloneMap>(L"Apple::Protocol::ToServer::StageSystem::ActorsLoaded");
	AddTemplateCmd<CMDR_Hook_EnteredCloneMap>(L"ae9d2edb864599d0041860fb6c6ad153");
	AddTemplateCmd<CMDR_Hook_RequestSwitchChannel>(L"8bedfbed6e9106a8a208e391c60cfeb3");
	AddTemplateCmd<CMDR_OnCreateNpcByClient>(L"e14a6a7ed25052d68ba65f06039b538a");
	AddTemplateCmd<CMDR_Hook_RequestProjectileHit>(L"c492f0d9b91babcb562db514f3a9a86b");
	//AddTemplateCmd<CMDR_Hook_RequestAddBuff>(L"d3ef9eb8f5b784744e2cefec23510043");
	AddTemplateCmd<CMDR_Hook_RequestChangeHp>(L"7822688b9bdfadbac7d6719c9242c75b");
	AddTemplateCmd<CMDR_Hook_ChangeSector>(L"51d107379e1955235e4e5dc41dd2e391");
}

GHookAllSendMsgMgr GHookAllSendMsgMgr::msg_mrg_;

GHookAllSendMsgMgr::GHookAllSendMsgMgr()
{
	//AddTemplateCmd<CMDR_Hook_SelectPlayer>(L"Apple::Protocol::ToServer::SessionSystem::SelectPlayer");
	AddTemplateCmd<CMDR_Hook_SelectPlayer>(L"5cefcbe0615c112023bbcca2d1cbd8bc");
}