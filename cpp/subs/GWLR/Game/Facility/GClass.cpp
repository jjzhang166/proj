#include "stdafx.h"
#include "GClass.h"
#include "GConst.h"
#include <Net/IoTimer.h>
#include "../FuncObjC.h"
#include <Common/EnsureCleanup.h>
#include "GItem.h"
#include "../GloFuncs.h"
#include "GPlayer.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include "../GLua/Scmd.h"

using namespace GClass;

bool CD_FuncCall::SendImplSync(stCD_MsgBase& msg)
{
	return GetSendTimeDuration().TimeToDoSync([&msg](){
		return GWndExecSync([&msg](){
			return SendImpl(msg);
		});
	});
}

bool GClass::GAmbient::WaitRetVillage( std::size_t wait_time )
{
	cond_cross_graph_.ResetCondition();
	CancelTimer_FbWin();

	IoTimerPtr io_timer;
	BOOST_SCOPE_EXIT_ALL(&){
		if (io_timer)
			io_timer->Cancel();
		io_timer.reset();
		cond_fb_game_over_win_.ResetCondition();
		cond_fb_game_over_lose_.ResetCondition();
	};
	if ( !WaitUntil([this, &io_timer]() -> bool{
		if ( !io_timer && scmd::GetMe().IsInFb() )
		{
			if ( cond_fb_game_over_lose_.CondtionTrue() || cond_fb_game_over_win_.CondtionTrue() )
			{
				io_timer = CreateIoTimer(1000, []( IoTimerPtr& timer_ptr ){
					/*PressKeyEsc();
					DummySleep(1000);*/
					auto norc = FuncObj_NormalC::GetInstance();
					if ( !norc )
						return;
					//LOG_O(Log_trace) << "副本结束，结束副本";
					norc->LeftButtonClick(505, 647);
					norc->LeftButtonClick(512, 617);
					//点记录的那个框
					norc->LeftButtonClick(517, 462);
					timer_ptr->Continue();
				}, true);
			}
		}
		return cond_cross_graph_.CondtionTrue();
	}, wait_time) )
	{
		//assert(false);
	}

	return cond_cross_graph_.CondtionTrue() && cond_fb_game_over_win_.CondtionTrue();
}

bool GClass::GAmbient::IsGameOver() const
{
	//这里一定只判断IsInVillage
	return /*cond_fb_game_over_win_.CondtionTrue() || */cond_fb_game_over_lose_.CondtionTrue() || scmd::GetMe().IsInVillage();
}

bool GClass::GAmbient::UntilJuQing( const std::string& str_ju_qing, std::size_t wait_time /*= 0 */ )
{
	str_ju_qing_ = str_ju_qing;
	AutoResetCondT cond(cond_ju_qing_);
	if ( !cond(wait_time) )
	{
		cond_ju_qing_.TouchCond();
		return false;
	}
	cond_ju_qing_.TouchCond();
	return true;
}

void GClass::GAmbient::SetJuQingStr( const std::string& str_ju_qing )
{
	if ( str_ju_qing.find(str_ju_qing_) != std::string::npos )
		cond_ju_qing_.TouchCond();
	LOG_O(Log_trace) << "剧情环境字符串：" << str_ju_qing;
}

GClass::GAmbient::GAmbient()
{
	cond_ju_qing_.TouchCond();
	
	class AmbientStr_BanAc : public AmbientStr_ByDlg{
		void SetUnknownData( DWORD data ){
			DO_EXCEPTION_TRY;
			auto str_ptr = *(const char**)(*(DWORD*)(*(DWORD*)(data + g_so_dlg_info_ban_time)));
			if (IsUnValidPtr(str_ptr))
			{
				assert(false);
				return;
			}
			std::shared_ptr<std::string> ban_time_str(new std::string(str_ptr));
			GetIoService().post([ban_time_str](){
				auto norc = FuncObj_NormalC::GetInstance();
				if ( !norc )
					return;
				norc->GetSessionOper().ToCall(BC_MakeP("SetBanAcTime")<<*ban_time_str);
			});
			DO_CATCH_ALL_MSG_BOX_CODE(
				return;);
		}
	};
	AmbientStrHanderPtrT ac_ban_handle(new AmbientStr_BanAc);
	ac_ban_handle->SetMatchDoFunc([]( const std::string& full_str ){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return;
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "帐号被封：" << full_str;
		norc->BanAccount();
	});
	ac_ban_handle->AddSubMatchStr("帐号已被封停，封停截止时间：");
	dlg_str_handlers_.push_back(ac_ban_handle);

	AmbientStrHanderPtrT game_no_relogin(new AmbientStr_ByDlg);
	game_no_relogin->SetMatchDoFunc([]( const std::string& full_str ){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return;
		norc->GetSessionOper().ToCall(BC_MakeP("NoReLoginAtDisconnect"));
		FuncObj_NormalC::Disconnect_Prompt(full_str);
	});
	game_no_relogin->AddSubMatchStr("帐号出现数据异常");
	game_no_relogin->AddSubMatchStr("您的帐号在其他地方登录");
	game_no_relogin->AddSubMatchStr("您的帐号未激活");
	game_no_relogin->AddSubMatchStr("您的帐号被停封");
	game_no_relogin->AddSubMatchStr("服务器连接失败，错误类型");
	game_no_relogin->AddSubMatchStr("如需解除该模式，请点击下方解除按钮");
	dlg_str_handlers_.push_back(game_no_relogin);

	AmbientStrHanderPtrT offline_handle(new AmbientStr_ByDlg);
	offline_handle->SetMatchDoFunc([]( const std::string& full_str ){
		LOG_O(Log_fatal) << "掉线了";
		FuncObj_NormalC::Disconnect_Prompt(full_str);
	});
	offline_handle->AddSubMatchStr("请重新启动客户端尝试登录");
	offline_handle->AddSubMatchStr("您将被强制退出游戏");
	offline_handle->AddSubMatchStr("您与服务器的连接已中断");
	offline_handle->AddSubMatchStr("连接副本服务器失败");
	offline_handle->AddSubMatchStr("服务器连接失败");
	offline_handle->AddSubMatchStr("与服务器断开连接");
	dlg_str_handlers_.push_back(offline_handle);

	AmbientStrHanderPtrT change_ac_handle(new AmbientStr_ByDlg);
	change_ac_handle->SetMatchDoFunc([]( const std::string& full_str ){
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "需要换号：" << full_str;
		scmd::GetMe().ChangeGameAc(full_str);
	});
	change_ac_handle->AddSubMatchStr("请选择其他服务进行游戏");
	change_ac_handle->AddSubMatchStr("当前线路已经爆满");
	dlg_str_handlers_.push_back(change_ac_handle);

	AmbientStrHanderPtrT pai_dui_handle(new AmbientStr_AmbientStr);
	pai_dui_handle->SetMatchDoFunc([]( const std::string& full_str ){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "正在排队，但需要启动下一个游戏客户端";
		norc->DoInitAfterGameState(GType::kGS_Invalid, true);
	});
	pai_dui_handle->AddSubMatchStr("名玩家正在排队");
	ambient_str_handlers_.push_back(pai_dui_handle);

	AmbientStrHanderPtrT shou_lie_end_handle(new AmbientStr_AmbientStr);
	shou_lie_end_handle->SetMatchDoFunc([=]( const std::string& full_str ){
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "狩猎结束，接下来就有框弹出了";
		//有这个字符串，说明下一步会有弹框结束，此时当作副本失败处理，能够迅速忽略掉后续的命令
		cond_fb_game_over_lose_.TouchCond();
		//BeginTimer_FbWin();
	});
	shou_lie_end_handle->AddSubMatchStr("1秒后狩猎结束");
	//shou_lie_end_handle->AddSubMatchStr("战斗结束");
	ambient_str_handlers_.push_back(shou_lie_end_handle);
}

void GClass::GAmbient::CancelTimer_FbWin()
{
	if (timer_fb_win_)
		timer_fb_win_->Cancel();
}

void GClass::GAmbient::BeginTimer_FbWin()
{
	//LOG_O(Log_trace) << "begin timer of fb win";
	if (timer_fb_win_)
	{
		timer_fb_win_->Continue();
		return;
	}
	timer_fb_win_ = CreateIoTimer(1000 * 55, [this]( const IoTimerPtr& ){
		//LOG_O(Log_trace) << "timeout timer of fb win";
		cond_fb_game_over_lose_.TouchCond();
	}, true);
}

void GClass::GAmbient::MatchDlgInfoStr( const std::shared_ptr<std::string>& full_str, DWORD data )
{
	if ( !full_str )
		return;
	if (full_str->empty())
		return;
	LOG_O(Log_trace) << "弹框内容：" << *full_str;
	for ( auto& v : dlg_str_handlers_ )
	{
		assert(v);
		if (v->CanMatchTheStr(full_str))
		{
			v->SetUnknownData(data);
			v->DoMatched(full_str);
			break;
		}
	}
}

void GClass::GAmbient::MatchAmbientStr( const std::shared_ptr<std::string>& full_str )
{
	if ( !full_str )
		return;
	if (full_str->empty())
		return;
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "MatchAmbientStr：" << *full_str;*/
	for ( auto& v : ambient_str_handlers_ )
	{
		assert(v);
		if (v->CanMatchTheStr(full_str))
		{
			v->DoMatched(full_str);
			break;
		}
	}
}

bool GClass::GAmbient::WaitCrossedGraph( std::size_t wait_time /*= 0 */ )
{
	/*AutoResetCondT cond(cond_cross_graph_);
	if ( !cond(wait_time) )
		return false;*/
	cond_cross_graph_.ResetCondition();

	return WaitUntil([this]() -> bool{
		return cond_cross_graph_.CondtionTrue();
	}, wait_time);
}

bool GClass::GAmbient::PressKeyEscWaitFor( bool is_show_cursor )
{
	if (IsCursorSameAsTheState(is_show_cursor))
		return true;

	for ( int i = 0; i != 10; ++i )
	{
		scmd::GetMe().PressKeyEsc();
		if (IsCursorSameAsTheState(is_show_cursor))
			return true;
		DummySleep(200);
		if (IsCursorSameAsTheState(is_show_cursor))
			return true;
		if ( i > 5 )
		{
			if (cond_fb_game_over_lose_.CondtionTrue())
				return false;
		}
	}
	return IsCursorSameAsTheState(is_show_cursor);
}

bool GClass::GAmbient::IsCursorSameAsTheState( bool is_cursor_show ) const
{
	if ( !is_show_cursor_ )
		return false;
	if ( *(bool*)&*is_show_cursor_ == is_cursor_show )
		return true;
	return false;
}

bool GClass::GAmbient::PressKeyGraveWaitFor( bool is_show_cursor )
{
	if (IsCursorSameAsTheState(is_show_cursor))
		return true;
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
	{
		assert(false);
		return false;
	}
	for ( int i = 0; i != 10; ++i )
	{
		norc->DxPressKeyGrave();
		if (IsCursorSameAsTheState(is_show_cursor))
			return true;
		DummySleep(200);
		if (IsCursorSameAsTheState(is_show_cursor))
			return true;
	}
	return IsCursorSameAsTheState(is_show_cursor);
}

bool GClass::CraftDbMgr::Update()
{
	
	if ( !craft_info_.empty() )
		return true;
	return __super::Update();
}

const GClass::CraftDbMgr::stCraftDb* GClass::CraftDbMgr::FindByDstItemId( int dst_item_id ) const
{
	auto it = std::find_if(craft_info_.begin(), craft_info_.end(), [dst_item_id]( const stCraftDb& craft_item ){
		return dst_item_id == craft_item.dst_item_id_;
	});
	if ( it != craft_info_.end() )
		return &*it;
	return nullptr;
}

const GClass::CraftDbMgr::stCraftDb* GClass::CraftDbMgr::FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const
{
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto it = std::find_if(craft_info_.begin(), craft_info_.end(), [=, &item_mgr]( const stCraftDb& craft_item ){
		for ( auto& v : craft_item.materials_ )
		{
			if ( v.material_item_id_ == material_item_id )
			{
				auto item_template = item_mgr.FindById(craft_item.dst_item_id_);
				if ( !item_template )
				{
					assert(false);
					continue;
				}
				if ( item_template->item_lvl_ >= dst_item_lvl && item_template->item_quality_ >= dst_item_quality )
					return true;
			}
		}
		return false;
	});
	if ( it != craft_info_.end() )
		return &*it;
	return nullptr;
}

void GClass::CraftDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "锻造信息总数：" << craft_info_.size();
	for ( auto& v : craft_info_ )
	{
		LOG_O(Log_debug) << "id:" << v.craft_id_ << " dst item id:" << v.dst_item_id_;
	}
}

bool GClass::CraftDbMgr::UpdateImpl()
{
	craft_info_.clear();
	
	CD_Traverse::TraverseCraftDb([this](stCD_CraftDb* db){
		stCraftDb craft_item;
		craft_item.craft_id_ = db->craft_id_;
		craft_item.dst_item_id_ = db->dst_item_id_;
		db->material_info_.Traverse([&craft_item](stCD_MaterialInfo* v){
			craft_item.materials_.push_back(*v);
			return true;
		});
		craft_info_.push_back(craft_item);
		return true;
	});
	assert(!craft_info_.empty());
	return true;
}

void GClass::CraftDbMgr::DoUpdateClear()
{
	craft_info_.clear();
}

bool GClass::ShopDb::Update()
{
	if (!infos_.empty())
		return true;
	return __super::Update();
}

const GClass::ShopDb::stDb* GClass::ShopDb::FindByName(const std::string& shop_cate_name) const
{
	if (shop_cate_name.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = std::find_if(infos_.begin(), infos_.end(), [&shop_cate_name](const stDb& v){
		return v.cate_name_ == shop_cate_name;
	});
	if (it != infos_.end())
	{
		return &*it;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "没有这样的商品种类：" << shop_cate_name;
	return nullptr;
}

bool GClass::ShopDb::UpdateImpl()
{
	infos_.clear();
	CD_Traverse::TraverseShopDb([this](stCD_ShopDb* shop_db){
		infos_.push_back(stDb());
		auto& db = infos_.back();
		db.cate_id_ = shop_db->cate_id_;
		db.cate_name_ = CDF_EmptyStr(shop_db->cate_name_);
		shop_db->shop_info_.Traverse([&db](stCD_ShopInfo** ptr){
			auto shop_info = *ptr;
			if (IsUnValidPtr(shop_info))
			{
				assert(false);
				return true;
			}
			stShopItem tmp = {
				shop_info->shop_id_, shop_info->item_id_, shop_info->item_cnt_
			};
			db.items_.push_back(tmp);
			return true;
		});		
		return true;
	});
	assert(!infos_.empty());
	return true;
}

void GClass::ShopDb::DoUpdateClear()
{
	infos_.clear();
}

bool GClass::BuffStateMgr::RebuildAllImpl()
{
	auto mgr = CD_FuncCall::GetBuffMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	SetRootNode(&mgr->buf_root_node1_);
	if ( !__super::RebuildAllImpl() )
	{
		assert(false);
		return false;
	}
	ContT cont_tmp;
	std::swap(cont_tmp, btree_);
	SetRootNode(&mgr->buf_root_node2_);
	if ( !__super::RebuildAllImpl() )
	{
		assert(false);
		return false;
	}
	std::copy(cont_tmp.begin(), cont_tmp.end(), std::back_inserter(btree_));
	return true;
}

bool GClass::BuffStateMgr::RebuildAll()
{
	if (time_elapse_.DoTimeout())
	{
		return __super::RebuildAll();
	}
	return false;
}

GClass::BuffStateMgr::BuffStateMgr() : time_elapse_(2000, true)
{

}

void GClass::BuffStateMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "buff total cnt:" << btree_.size();
	for ( auto& v : btree_ )
	{
		v.DebugInfo();
	}
}

const BuffState* GClass::BuffStateMgr::FindByName( const std::string& buff_name )
{
	if (buff_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& v : btree_ )
	{
		if ( !v.buff_name_.empty() && v.buff_name_ == buff_name )
			return &v;
	}
	return nullptr;
}

bool GClass::BuffState::UpdateImpl()
{
	if (IsUnValidPtr(GetCurNode()))
	{
		assert(false);
		return false;
	}
	auto node = GetCurNode()->CastTo<stCD_BuffNode>();
	auto buf = node->buf_;
	if (IsUnValidPtr(buf))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(buf->buf_db_))
	{
		assert(false);
		return false;
	}
	packet_id_ = buf->dst_packet_id_;
	buff_id_ = buf->buf_db_->buff_id_;
	buff_name_.assign(Utf8ToGbk(CDF_EmptyStr(buf->buf_db_->buff_name_)));
	buff_desc_ = CDF_EmptyStr(buf->buf_db_->buff_desc_);
	return true;
}

GClass::BuffState::BuffState()
{
	packet_id_ = 0;
	buff_id_ = 0;
}

void GClass::BuffState::DebugInfo() const
{
	LOG_O(Log_trace) << "the buff id:" << buff_id_ << " name:" << buff_name_ << " desc:" << buff_desc_;
}

bool GClass::BuffState::MatchDesc( const std::string& sub_desc ) const
{
	if (buff_desc_.empty())
		return false;
	if (sub_desc.empty())
	{
		assert(false);
		return false;
	}
	return buff_desc_.find(sub_desc) != std::string::npos;
}

void GClass::BuffState::DoUpdateClear()
{
	packet_id_ = -1;
	buff_id_ = 0;
	buff_name_.clear();
	buff_desc_.clear();
}

GClass::FarmDbMgr::stDb::stDb()
{
	id_ = 0;
	the_lvl_ = 0;
	jing_suan_dian_ = 0;
	bind_gold_ = 0;
	role_lvl_ = 0;
}

bool GClass::FarmDbMgr::Update()
{
	
	if ( !info_.empty() )
		return true;
	return __super::Update();
}

void GClass::FarmDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "total cnt:" << info_.size();
	for ( auto& v : info_ )
	{
		LOG_O(Log_trace) << "id:" << v.id_ << " the_lvl:" << v.the_lvl_ << " 精算点:" << v.jing_suan_dian_ << " 绑定金：" << v.bind_gold_ << " role lvl:" << v.role_lvl_ << " name:" << v.name_;
	}
}

const GClass::FarmDbMgr::stDb* GClass::FarmDbMgr::FindById( int id ) const
{
	for ( auto& v : info_ )
	{
		if ( v.id_ == id )
			return &v;
	}
	return nullptr;
}

bool GClass::FarmDbMgr::UpdateImpl()
{
	info_.clear();
	CD_Traverse::TraverseFarmDb([this](stCD_FarmDb* farm_db){
		stDb tmp;
		tmp.id_ = farm_db->id_;
		tmp.the_lvl_ = farm_db->lvl_;
		assert(tmp.the_lvl_ >= 0);
		tmp.jing_suan_dian_ = farm_db->point_;
		assert(tmp.jing_suan_dian_ >= 0);
		tmp.bind_gold_ = farm_db->silver_;
		tmp.name_.assign(CDF_EmptyStr(farm_db->name_));
		tmp.role_lvl_ = farm_db->role_lvl_;
		assert(tmp.role_lvl_ >= 0);
		info_.push_back(tmp);
		return true;
	});
	return true;
}

void GClass::FarmDbMgr::DoUpdateClear()
{
	info_.clear();
}

GClass::CurFarmFacilityInfo::stFacility::stFacility()
{
	farm_db_id_ = 0;
	idx_ = CD_En_FarmFacilityIdx::kOre;
	lvl_ = 0;
	left_seconds_ = 0;
}

void GClass::CurFarmFacilityInfo::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "total cnt:" << info_.size();
	for ( auto& v : info_ )
	{
		LOG_O(Log_trace) << "id:" << v.farm_db_id_ << " the_lvl:" << v.idx_ << " lvl:" << v.lvl_ << " left seconds：" << v.left_seconds_;
	}
}

bool GClass::CurFarmFacilityInfo::UpdateImpl()
{
	info_.clear();
	
	auto farm_facility = CD_FuncCall::GetCurFarmFacility();
	if (!farm_facility)
	{
		assert(false);
		return false;
	}
	CD_En_FarmFacilityIdx idx = CD_En_FarmFacilityIdx::kOre;
	for (auto& v : farm_facility->facility_info_)
	{
		auto db_id = CD_FuncCall::GetFarmDbId(idx, v.cur_lvl_);
		stFacility tmp;
		tmp.idx_ = idx;
		tmp.farm_db_id_ = db_id;
		tmp.lvl_ = v.cur_lvl_;
		tmp.left_seconds_ = CD_FuncCall::GetFarmLeftTime(db_id, v.cur_elapse_time_);
		info_.push_back(tmp);
		idx = CD_En_FarmFacilityIdx((int)idx + 1);
	}
	return true;
}

const GClass::CurFarmFacilityInfo::stFacility* GClass::CurFarmFacilityInfo::FindByIdx(CD_En_FarmFacilityIdx idx) const
{
	for ( auto& v : info_ )
	{
		if ( v.idx_ == idx )
			return &v;
	}
	return nullptr;
}

GClass::CurFarmFacilityInfo::CurFarmFacilityInfo() : time_to_collect_(kCurJingSuanDianMax)
{

}

bool GClass::CurFarmFacilityInfo::CalcNextCollectTime()
{
	using namespace Poco;
	DateTimeLocal time_now;
	for ( auto& v : info_ )
	{
		if (v.idx_ < CD_En_FarmFacilityIdx::kOre || v.idx_ >= CD_En_FarmFacilityIdx::kInvalid)
		{
			assert(false);
			continue;
		}
		assert((int)v.idx_ < (int)time_to_collect_.size());
		if ( v.lvl_ > 0 )
		{
			time_to_collect_[(int)v.idx_] = time_now + Timespan(v.left_seconds_, 0);
		}
		else
		{
			time_to_collect_[(int)v.idx_] = time_now + kTimeToCollectOfNoLvl;
		}
	}

	return true;
}

void GClass::CurFarmFacilityInfo::ResetAllCollectTime()
{
	using namespace Poco;
	DateTimeLocal time_now;
	for ( auto& v : time_to_collect_ )
	{
		v = time_now;
	}
}

void GClass::CurFarmFacilityInfo::DoUpdateClear()
{
	info_.clear();
}

const Poco::Timespan GClass::CurFarmFacilityInfo::kTimeToCollectOfNoLvl(60 * 60 * 3, 0);

bool GClass::FarmSeedDbMgr::Update()
{
	
	if ( !info_.empty() )
		return true;
	return __super::Update();
}

const GClass::FarmSeedDbMgr::stDb* GClass::FarmSeedDbMgr::FindByItemId( int item_id ) const
{
	for ( auto& v : info_ )
	{
		if ( v.item_id_ == item_id )
			return &v;
	}
	return nullptr;
}

void GClass::FarmSeedDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "FarmSeed总数：" << info_.size();
	for ( auto& v : info_ )
	{
		LOG_O(Log_debug) << "item id:" << v.item_id_ << " name:" << v.item_name_;
	}
}

bool GClass::FarmSeedDbMgr::UpdateImpl()
{
	info_.clear();
	CD_Traverse::TraverseSeedDb([this](stCD_SeedDb* db){
		stDb tmp;
		tmp.item_id_ = db->item_id_;
		tmp.item_name_.assign(CDF_EmptyStr(db->item_name_));
		info_.push_back(tmp);
		return true;
	});
	assert(!info_.empty());
	return true;
}

void GClass::FarmSeedDbMgr::DoUpdateClear()
{
	info_.clear();
}

GClass::FarmSeedDbMgr::stDb::stDb()
{
	item_id_ = 0;
}

GClass::CurFarmFieldInfo::stState::stState()
{
	idx_ = CD_En_FarmFieldIdx::kInvalid;
	flag_ = 0;
	seed_id_ = 0;
	left_seconds_ = 0;
}

bool GClass::CurFarmFieldInfo::stState::CanBePlanted() const
{
	if ( 0 == flag_ )
		return false;
	if ( seed_id_ != 0 )
		return false;
	assert(0 == left_seconds_);
	return true;
}

bool GClass::CurFarmFieldInfo::stState::CanBeCollected() const
{
	if ( 0 == flag_ )
		return false;
	if ( left_seconds_ > 0 )
		return false;
	//assert(0 != seed_id_);
	return true;
}

bool GClass::CurFarmFieldInfo::UpdateImpl()
{
	info_.clear();

	auto farm_facility = CD_FuncCall::GetCurFarmFacility();
	if (!farm_facility)
	{
		assert(false);
		return false;
	}
	CD_En_FarmFieldIdx idx = CD_En_FarmFieldIdx::kField1;
	for (auto& v : farm_facility->field_info_)
	{
		stState tmp;
		tmp.idx_ = idx;
		tmp.flag_ = v.flag_;
		assert(0 == tmp.flag_ || 1 == tmp.flag_);
		tmp.seed_id_ = v.seed_id_;
		tmp.left_seconds_ = CD_FuncCall::GetFarmFieldLeftTime(farm_facility, idx);
		assert(tmp.left_seconds_ >= 0);
		info_.push_back(tmp);

		idx = CD_En_FarmFieldIdx((int)idx + 1);
	}
	return true;
}

const GClass::CurFarmFieldInfo::stState* GClass::CurFarmFieldInfo::FindByIdx(CD_En_FarmFieldIdx idx) const
{
	for ( auto& v : info_ )
	{
		if ( v.idx_ == idx )
			return &v;
	}
	return nullptr;
}

GClass::CurFarmFieldInfo::CurFarmFieldInfo() : time_to_collect_(kCaoYaoTianMax)
{

}

void GClass::CurFarmFieldInfo::ResetAllCollectTime()
{
	using namespace Poco;
	DateTimeLocal time_now;
	for ( auto& v : time_to_collect_ )
	{
		v = time_now;
	}
}

bool GClass::CurFarmFieldInfo::CalcNextCollectTime()
{
	using namespace Poco;
	DateTimeLocal time_now;
	for ( auto& v : info_ )
	{
		if (v.idx_ < CD_En_FarmFieldIdx::kField1 || v.idx_ >= CD_En_FarmFieldIdx::kInvalid)
		{
			assert(false);
			continue;
		}
		assert((int)v.idx_ < (int)time_to_collect_.size());
		if ( v.flag_ != 0 )
		{
			time_to_collect_[(int)v.idx_] = time_now + Timespan(v.left_seconds_, 0);
		}
		else
		{
			time_to_collect_[(int)v.idx_] = time_now + CurFarmFacilityInfo::kTimeToCollectOfNoLvl;
		}
	}

	return true;
}

void GClass::CurFarmFieldInfo::DoUpdateClear()
{
	info_.clear();
}

GClass::GFarmCollectNameMap::GFarmCollectNameMap()
{
	stFarmCollectInfo tmp;
	tmp.cao_yao_tian_idx_ = CD_En_FarmFieldIdx::kInvalid;
	tmp.collect_en_name_[0] = "MineLVMax1";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kOre;
	map_["矿场"] = tmp;

	tmp.collect_en_name_[0] = "FishLV1";
	tmp.collect_en_name_[1] = "FishLV2";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kFish;
	map_["鱼塘"] = tmp;
	tmp.collect_en_name_[1].clear();

	tmp.collect_en_name_[0] = "BugLVMax";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kInsect;
	map_["虫栖地"] = tmp;

	tmp.collect_en_name_[0] = "MushRoomLVMax";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kBed;
	map_["育菇木床"] = tmp;

	tmp.collect_en_name_[0] = "Honey_Collector_All";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kHouse;
	map_["蜂房"] = tmp;

	tmp.collect_en_name_[0] = "LandLV1";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kField;
	tmp.cao_yao_tian_idx_ = CD_En_FarmFieldIdx::kField1;
	map_["草药田"] = tmp;

	tmp.collect_en_name_[0] = "LandLV1";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kField;
	tmp.cao_yao_tian_idx_ = CD_En_FarmFieldIdx::kField1;
	map_["草药田1"] = tmp;

	tmp.collect_en_name_[0] = "LandLV2";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kField;
	tmp.cao_yao_tian_idx_ = CD_En_FarmFieldIdx::kField2;
	map_["草药田2"] = tmp;

	tmp.collect_en_name_[0] = "LandLV3";
	tmp.collect_idx_ = CD_En_FarmFacilityIdx::kField;
	tmp.cao_yao_tian_idx_ = CD_En_FarmFieldIdx::kField3;
	map_["草药田3"] = tmp;
}

const GClass::GFarmCollectNameMap::stFarmCollectInfo* GClass::GFarmCollectNameMap::FindByCnName( const std::string& name ) const
{
	for ( auto& v : map_ )
	{
		if ( v.first == name )
			return &v.second;
	}
	return nullptr;
}

GClass::GFarmCollectNameMap::CollectInfoIterPtr GClass::GFarmCollectNameMap::TraverseById(CD_En_FarmFacilityIdx collect_idx)
{
	return MakeIterator<ItemTraTraits>(map_, [collect_idx]( const MapT::value_type& v ){
		return v.second.collect_idx_ == collect_idx;
	});
}

void GClass::GFarmCollectNameMap::DebugInfo_CnName() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "只支持以下庄园采集名：";
	for ( auto& v : map_ )
	{
		LOG_O(Log_info) << v.first;
	}
}

void GClass::IAmbientStr::AddSubMatchStr( const std::string& sub_str )
{
	assert(!sub_str.empty());
	sub_strs_.Add(sub_str);
}

void GClass::IAmbientStr::SetMatchDoFunc( const MatchDoFuncT& func )
{
	do_func_ = func;
}

bool GClass::IAmbientStr::CanMatchTheStr( const std::shared_ptr<std::string>& full_str ) const
{
	if ( !full_str )
	{
		assert(false);
		return false;
	}
	if (sub_strs_.IsInByMatch(*full_str))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "matched the string：" << *full_str;
		return true;
	}
	return false;
}

void GClass::IAmbientStr::SetUnknownData( DWORD data )
{

}

void GClass::AmbientStr_ByDlg::DoMatched( const std::shared_ptr<std::string>& full_str ) const
{
	assert(do_func_);
	GetIoService().post([this, full_str](){
		do_func_(*full_str);
	});
}

void GClass::AmbientStr_AmbientStr::DoMatched( const std::shared_ptr<std::string>& full_str ) const
{
	do_func_(*full_str);
}

bool GClass::GHitMonsterMode_Mouse::DoHitOnce()
{
	using namespace GType;
	auto& gpm = GPlayerMe::GetMe();
	auto& grid_equip = gpm.GetEquipPackage();
	//这个更新可以去掉，因为每次过图均会更新一次，而在副本里，是不允许换武器的。
	//grid_equip.RebuildAll();
	auto item_ptr = grid_equip.FindBySlotIndex(GType::kEPI_Weapon);
	if (item_ptr)
	{
		auto weapon_type = item_ptr->EquipSubType2Weapon();
		if (CD_En_EquipPartSubType_Weapon::kEPSTW_TaiDao == weapon_type)
		{
			if (!gpm.is_right_bt_hit_ && kRightHit != cur_time_hit_mode_)
				return gpm.HitByLeftBt(1);

			if (gpm.UpdateLianQiCaoSync())
			{
				if (gpm.lian_qi_cao_ <= 0)
					return gpm.HitByLeftBt(1);
				else if (gpm.lian_qi_cao_ >= 50)
					return gpm.HitByRightBt(1);
				else if (gpm.SetLianQiCao())
					return gpm.HitByRightBt(1);
			}
		}
		else if (CD_En_EquipPartSubType_Weapon::kEPSTW_DaJian == weapon_type)
		{
			if (kRightHit == cur_time_hit_mode_)
				return gpm.HitByRightBt(1);
			++hit_cnt_;
			if (hit_cnt_ % 3 == 0)
				return gpm.HitByRightBt(1);
			else
				return gpm.HitByLeftBt(1);
		}
	}
	else
	{
		LOG_O(Log_error) << "当前身上没有武器，无法攻击";
		return false;
	}


	/*if ( role_lvl_ > 20 && HasAoYiJueXing() )
	{
		return UseSkill(GType::kSC_KeyF, 1);
	}*/
	
	if (kRightHit == cur_time_hit_mode_)
		return gpm.HitByRightBt(1);

	return gpm.HitByLeftBt(1);
}

int GClass::GHitMonsterMode_Mouse::GetSteps() const
{
	return 1;
}

void GClass::GHitMonsterMode_Mouse::SetCurTimeHitMode(enHitMode hit_mode)
{
	cur_time_hit_mode_ = hit_mode;
}

bool GClass::GHitMonsterMode_UsePacketItem::DoHitOnce()
{
	return GPlayerMe::GetMe().UseItemByName(item_name_);
}

void GClass::GHitMonsterMode_UsePacketItem::SetCurItem( const std::string& item_name, int steps )
{
	CMN_ASSERT(!item_name.empty());
	item_name_ = item_name;
	steps_ = steps;
}

GClass::GHitMonsterMode_UsePacketItem::GHitMonsterMode_UsePacketItem()
{
	steps_ = 1;
}

int GClass::GHitMonsterMode_UsePacketItem::GetSteps() const
{
	return steps_;
}

void GClass::GCurShangJinInfo::DoUpdateClear()
{
	info_.clear();
}

bool GClass::GCurShangJinInfo::UpdateImpl()
{
	info_.clear();
	CD_Traverse::TraverseCurShangJinInfo([this](stCD_ShangJinInfo* info){
		if (IsUnValidPtr(info->task_db_))
		{
			assert(false);
			return false;
		}
		stShangeJinItem tmp;
		tmp.task_id_ = info->task_id_;
		tmp.task_name_.assign(CDF_NilStr(info->task_db_->task_name_));
		info_.push_back(tmp);
		return true;
	});
	return true;
}

bool GClass::GTaskFbInfo::Update()
{
	if ( !info_.empty() )
		return true;
	return __super::Update();
}

void GClass::GTaskFbInfo::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "总数：" << info_.size();
	LOG_O(Log_debug) << "王立总数：" << king_info_.size();

	for ( auto& v : info_ )
	{
		LOG_O(Log_trace) /*<< "章节的字符串:" << v.chapter_str_*/ << " 章节内任务的序号:" << v.task_index_ <<  " 任务副本ID:" << v.task_fb_id_ << " 任务所在城镇ID：" << v.village_id_ << " show flag:" << v.show_flag_;
	}
	for ( auto& v : king_info_ )
	{
		LOG_O(Log_trace) << "标题：" << v.fb_title_ << " 任务所在城镇ID：" << v.village_id_ << " 任务副本ID：" << v.task_fb_id_;
	}
}

bool GClass::GTaskFbInfo::UpdateImpl()
{
	auto chapter_info = CD_FuncCall::GetChapterInfo();
	if (IsUnValidPtr(chapter_info))
	{
		assert(false);
		return false;
	}
	info_.clear();
	for (auto& v : *chapter_info)
	{
		v.fb_info_.Traverse([this](stCD_ChapterFbInfo* chapter_fb_info){
			if (IsUnValidPtr(chapter_fb_info->str_chapter_))
			{
				return true;
			}
			stTaskFbInfo tmp;
			//tmp.chapter_str_.assign(chapter_fb_info->str_chapter_);
			tmp.task_index_ = chapter_fb_info->chapter_task_idx_;
			assert(tmp.task_index_ >= 0);
			tmp.task_fb_id_ = chapter_fb_info->task_fb_id_;
			assert(tmp.task_fb_id_ >= 0);
			tmp.village_id_ = chapter_fb_info->task_owner_map_id_;
			assert(tmp.village_id_ >= 0);
			tmp.show_flag_ = chapter_fb_info->show_flag_;
			assert(tmp.show_flag_ >= 0);
			info_.push_back(tmp);
			return true;
		});
	}

	king_info_.clear();
	CD_Traverse::TraverseKingQuestInfo([this](stCD_KingQuestInfo* info){
		stKingQuestInfo tmp;
		tmp.fb_title_.assign(CDF_EmptyStr(info->title_));
		tmp.task_fb_id_ = info->king_fb_id_;
		tmp.village_id_ = info->city_id_;
		king_info_.push_back(tmp);
		return true;
	});
	assert(!king_info_.empty());
	return !info_.empty();
}

void GClass::GTaskFbInfo::DoUpdateClear()
{
	info_.clear();
	king_info_.clear();
}

const GClass::GTaskFbInfo::stTaskFbInfo* GClass::GTaskFbInfo::FindByFbId( int fb_id ) const
{
	auto it = std::find_if(info_.begin(), info_.end(), [fb_id]( const stTaskFbInfo& v ){
		return v.task_fb_id_ == fb_id;
	});
	if ( it == info_.end() )
		return nullptr;
	return &*it;
}

bool GClass::GTaskFbInfo::IsTheFbIdJuQingDone( int fb_id ) const
{
	auto chapter_info = FindByFbId(fb_id);
	if ( !chapter_info )
		return false;
	if ( 1 != chapter_info->show_flag_ )
		return false;
	if ( !GWndExecSync([fb_id](){
		return CD_FuncCall::IsTheFbDone(fb_id);
	}) )
		return false;
	
	return true;
}

bool GClass::GTaskFbInfo::IsTheFbIdJuQingTask( int fb_id ) const
{
	auto chapter_info = FindByFbId(fb_id);
	if ( !chapter_info )
		return false;
	if ( 1 != chapter_info->show_flag_ )
		return false;
	return true;
}

const GClass::GTaskFbInfo::stKingQuestInfo* GClass::GTaskFbInfo::FindByFbId_King( int fb_id ) const
{
	auto it = std::find_if(king_info_.begin(), king_info_.end(), [fb_id]( const stKingQuestInfo& v ){
		return v.task_fb_id_ == fb_id;
	});
	if ( it == king_info_.end() )
		return nullptr;
	return &*it;
}

GClass::GTaskFbInfo::stTaskFbInfo::stTaskFbInfo()
{
	task_index_ = -1;
	task_fb_id_ = -1;
	village_id_ = -1;
	show_flag_ = -1;
}

GClass::GTaskFbInfo::stKingQuestInfo::stKingQuestInfo()
{
	village_id_ = -1;
	task_fb_id_ = -1;
}

GClass::GVillage2RoleLvlInfo::GVillage2RoleLvlInfo()
{
	village_lvl_info_[kVillageName_XiMeiLunShanLu] = 1;
	village_lvl_info_[kVillageName_LongZhiHu] = 5;
	village_lvl_info_[kVillageName_MiLaDeCun] = 1;
	village_lvl_info_[kVillageName_MeiJiePoErTan] = 15;
	//这个20级是猜的
	village_lvl_info_[kVillageName_YaoYueLin] = 20;
	//这个25级是猜的
	village_lvl_info_[kVillageName_YinBaiAiKou] = 25;
}

int GClass::GVillage2RoleLvlInfo::Village2RoleLvl( const std::string& village_name ) const
{
	auto it = village_lvl_info_.find(village_name);
	if ( it == village_lvl_info_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的城镇：" << village_name;
		assert(false);
		return 10000;
	}
	return it->second;
}

const GClass::ShopDb::stShopItem* GClass::ShopDb::stDb::FindByItemId(int item_id) const
{
	auto it = std::find_if(items_.begin(), items_.end(), [item_id](const stShopItem& v){
		return v.item_id_ == item_id;
	});
	if (it != items_.end())
	{
		return &*it;
	}
	return nullptr;
}
