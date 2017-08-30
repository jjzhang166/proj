#include "stdafx.h"
#include "GStrMap.h"
#include <boost/algorithm/string.hpp>
#include "Facility/GAddr.h"
#include "Facility/GOffset.h"
#include "Facility/GConst.h"

stPTreeEn2ChValue::stPTreeEn2ChValue() : game_ch_str_(nullptr)
{
}

/*
stPTreeEn2ChValue::stPTreeEn2ChValue( const std::wstring& en_node ) : path_en_node_(en_node), game_ch_str_(nullptr)
{
}
*/

stPTreeEn2ChValue::stPTreeEn2ChValue( const stGStrEn2ChBase* game_ch_str ) : game_ch_str_(game_ch_str)
{
}

GameEn2ChNode::GameEn2ChNode()
{
}

bool GameEn2ChNode::Update()
{
	assert(game_cur_node_);
	auto node = (stGStrEn2ChBase*)game_cur_node_;
	auto str_path = node->en_str_.GetOriginBegin();
	if ( !str_path )
	{
		assert(false);
		game_cur_node_ = nullptr;
		return false;
	}
	const auto& filter_cont = init_filter_.GetMatchPatternCont();
	for ( const auto& the_regex : filter_cont )
	{
		if (node->EnMatchPattern(the_regex))
			return true;
	}
	game_cur_node_ = nullptr;
	
	/*boost::split(str_node_cont_, str_path, boost::is_any_of(L"."), boost::token_compress_on);
	assert(str_node_cont_.size() >= 1);*/
	return true;
}

const stGStrEn2ChBase* GameEn2ChNode::GetEn2ChNode() const
{
	if ( !game_cur_node_ )
		return nullptr;
	return (stGStrEn2ChBase*)game_cur_node_;
}

const GameEn2ChNode::InitFilter GameEn2ChNode::init_filter_;

/*
const GameEn2ChNode::StrNodeContT& GameEn2ChNode::GetStrNodeCont() const
{
	return str_node_cont_;
}*/

GameEn2ChNode GameEn2ChNode::empty_placeholder_;

void GameEn2ChNode::ContWrapper::push_back( const GameEn2ChNode& en2ch_node )
{
	auto en2ch_base = en2ch_node.GetEn2ChNode();
	if ( !en2ch_base )
		return;
	/*const auto& str_node_cont = en2ch_node.GetStrNodeCont();
	if (str_node_cont.empty())
		return;*/
	auto str_path = en2ch_base->en_str_.GetOriginBegin();
	if ( !str_path )
	{
		assert(false);
		return;
	}
	auto str_ch = en2ch_base->ch_str_.GetOriginBegin();
	if ( !str_ch )
	{
		assert(false);
		return;
	}
	en2ch_ptree_.add(str_path, En2ChPTreeT::data_type(en2ch_base));
}

bool GameEn2ChNode::ContWrapper::empty() const
{
	return en2ch_ptree_.empty();
}

GameEn2ChNode* GameEn2ChNode::ContWrapper::begin()
{
	return &GameEn2ChNode::empty_placeholder_;
}

GameEn2ChNode* GameEn2ChNode::ContWrapper::end()
{
	return &GameEn2ChNode::empty_placeholder_;
}

void GameEn2ChNode::ContWrapper::clear()
{
	en2ch_ptree_.clear();
}

bool GameEn2ChNodeMgr::UpdateAll()
{
	assert(false);
	return false;
}

bool GameEn2ChNodeMgr::RebuildUpdateAll()
{
	using namespace GClass;
	using namespace GAddr;
	using namespace GOffset;
	if ( !btree_.empty() )
		return true;
	const auto kEn2ChMapObj = *(const char**)g_pEn2ChMapAddr;
	if ( !kEn2ChMapObj )
	{
		assert(false);
		return false;
	}
	const auto nodes = (const BTreeMgr::stGBTreeNodes*)(kEn2ChMapObj + g_o_en2_ch);
	assert(nodes->node_cnt_ > 0);
	assert(nodes->node_null_);
	SetGameNodes(nodes);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	quest_index_.reset(new StrIndex_Quest);
	if ( !quest_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Quest.Quest") )
	{
		assert(false);
		return false;
	}
	quest_begin_desc_index_.reset(new StrIndex_QuestBeginDesc);
	if ( !quest_begin_desc_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Quest.Quest") )
	{
		assert(false);
		return false;
	}
	village_index_.reset(new StrIndex_Village);
	if ( !village_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Village") )
	{
		assert(false);
		return false;
	}
	stage_index_.reset(new StrIndex_Stage);
	if ( !stage_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Stage") )
	{
		assert(false);
		return false;
	}
	skill_index_.reset(new StrIndex_Skill);
	if ( !skill_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Skill") )
	{
		assert(false);
		return false;
	}
	item_index_.reset(new StrIndex_Item);
	if ( !item_index_->ParseFromRoot(btree_.en2ch_ptree_, L"Item") )
	{
		assert(false);
		return false;
	}
	return true;
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetQuestIndex() const
{
	return quest_index_;
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetVillageIndex() const
{
	return village_index_;
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetStageIndex() const
{
	return stage_index_;
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetSkillIndex() const
{
	return skill_index_;
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetQuestBeginDescIndex() const
{
	return quest_begin_desc_index_;
}

std::wstring GameEn2ChNodeMgr::FindChByFullPath( const std::wstring& full_path ) const
{
	if (full_path.empty())
	{
		assert(false);
		return kEmptyWStr;
	}
	const auto& ch_value = btree_.en2ch_ptree_.get_optional<stPTreeEn2ChValue>(full_path);
	if ( !ch_value )
	{
		//assert(false);
		return kEmptyWStr;
	}
	if ( !ch_value->game_ch_str_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	return ch_value->game_ch_str_->ch_str_.Duplicate();
}

const GameEn2ChNodeMgr::StrIndexPtr& GameEn2ChNodeMgr::GetItemIndex() const
{
	return item_index_;
}

bool stGStrEn2ChBase::EnMatchPattern( const boost::wregex& the_regex ) const
{
	using namespace boost;
	auto en_str_ptr = en_str_.GetOriginBegin();
	if ( !en_str_ptr )
	{
		assert(false);
		return false;
	}
	return regex_match(en_str_ptr, en_str_.GetOriginEnd(), the_regex);
}

bool stGStrEn2ChBase::ChMatchPattern( const boost::wregex& the_regex ) const
{
	using namespace boost;
	auto ch_str_ptr = ch_str_.GetOriginBegin();
	if ( !ch_str_ptr )
	{
		assert(false);
		return false;
	}
	return regex_match(ch_str_ptr, ch_str_.GetOriginEnd(), the_regex);
}

const GameEn2ChNode::InitFilter::MatchPatternContT& 
	GameEn2ChNode::InitFilter::GetMatchPatternCont() const
{
	return match_pattern_cont_;
}

void GameEn2ChNode::InitFilter::AddFilter( const std::wstring& en_match_pattern )
{
	boost::wregex ex(en_match_pattern);
#ifdef _DEBUG
	auto it_end = match_pattern_cont_.end();
	auto it = std::find(match_pattern_cont_.begin(), it_end, ex);
	if ( it != it_end )
	{
		assert(false);
		return;
	}
#endif
	match_pattern_cont_.push_back(ex);
}

GameEn2ChNode::InitFilter::InitFilter()
{
	//城镇名
	AddFilter(L"^Village\\.\\w+\\.\\w+\\.Name$");
	//副本名
	AddFilter(L"^Stage\\.Stage\\w+\\.\\w+\\.Name$");
	AddFilter(L"^Stage\\.Rg\\w+\\.\\w+\\.Name$");
	//AddFilter(L"^Stage\\.Rg\\w+\\.\\w+\\.Name$");
	//任务名
	AddFilter(L"^Quest\\.Quest\\.\\w+\\.MainTitle$");
	AddFilter(L"^Quest\\.Quest\\.\\w+\\.BeginDesc$");
	//NPC名
	AddFilter(L"^NpcList\\.FixedNpc\\.\\w+\\.Name$");
	//技能名
	/*AddFilter(L"^Skill\\.Thief\\.\\w+\\.Name$");
	AddFilter(L"^Skill\\.Warrior\\.\\w+\\.Name$");
	AddFilter(L"^Skill\\.MagicGunner\\.\\w+\\.Name$");*/
	//AddFilter(L"^Skill\\.\\w+\\.\\S+Name$");
	AddFilter(L"^Skill\\.\\S+Name$");
	//职业名
	AddFilter(L"^Class\\.\\w+\\.\\w+\\.ClassName$");
	//技能分支名
	AddFilter(L"^Class\\.\\w+\\.\\w+\\.Name$");
	//物品名
	AddFilter(L"^Item\\.\\w+\\.\\w+\\.Name$");
	//region name
	AddFilter(L"^Region\\.\\w+\\.\\w+\\.Name$");
}

GameEn2ChNodeMgr::StrIdexBase::StrIdexBase()
{
	tree_node_ = nullptr;
	for_lvl_ = 1;
}

bool GameEn2ChNodeMgr::StrIdexBase::ParseFromRoot( const En2ChPTreeT& tree_root, const std::wstring& node_path )
{
	if (node_path.empty())
	{
		assert(false);
		return false;
	}
	assert(!tree_node_);
	const auto& quest_node = tree_root.get_child_optional(node_path);
	if (quest_node)
		tree_node_ = &*quest_node;
	return !!tree_node_;
}

std::wstring GameEn2ChNodeMgr::StrIdexBase::En2Ch( const std::wstring& en_id ) const
{
	if ( en_id.empty() || !tree_node_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	assert(for_lvl_ > 0);
	std::wstring str_out;
	auto it_end = tree_node_->end();
	for ( auto it_begin = tree_node_->begin(); it_begin != it_end; ++it_begin )
	{
		if (En2Ch_ForLvl(it_begin, en_id, for_lvl_ - 1, str_out))
			return str_out;
	}
	assert(false);
	return str_out;
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::Ch2En( const std::wstring& ch_name ) const
{
	if ( ch_name.empty() || !tree_node_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	assert(for_lvl_ > 0);
	auto it_end = tree_node_->end();
	for ( auto it_begin = tree_node_->begin(); it_begin != it_end; ++it_begin )
	{
		auto str_out = Ch2En_ForLvl(it_begin, ch_name, for_lvl_ - 1);
		if (str_out)
			return *str_out;
	}
	//assert(false);
	return kEmptyWStr;
}

bool GameEn2ChNodeMgr::StrIdexBase::En2Ch_ForLvl( const PTreeIterT& it, const std::wstring& en_id, int for_lvl, std::wstring& str_out ) const
{
	if ( for_lvl <= 0 )
	{
		assert(0 == for_lvl);
		if ( it->first != en_id )
			return false;
		const auto& ch_value = it->second.get_optional<stPTreeEn2ChValue>(CalcSuffix(en_id));
		if ( !ch_value )
			return false;
		if ( !ch_value->game_ch_str_ )
		{
			assert(false);
			return false;
		}
		str_out = ch_value->game_ch_str_->ch_str_.Duplicate();
		return true;
	}
	else
	{
		auto it_end = it->second.end();
		for ( auto it_begin = it->second.begin(); it_begin != it_end; ++it_begin )
		{
			if (En2Ch_ForLvl(it_begin, en_id, for_lvl - 1, str_out))
				return true;
		}
		return false;
	}
}

const std::wstring* GameEn2ChNodeMgr::StrIdexBase::Ch2En_ForLvl( const PTreeIterT& it, const std::wstring& ch_name, int for_lvl ) const
{
	if ( for_lvl <= 0 )
	{
		assert(0 == for_lvl);
		const auto& ch_value = it->second.get_optional<stPTreeEn2ChValue>(CalcSuffix(it->first));
		if ( !ch_value )
			return nullptr;
		if ( !ch_value->game_ch_str_ )
		{
			assert(false);
			return nullptr;
		}
		if (ch_value->game_ch_str_->ch_str_.EqualTo(ch_name))
			return &it->first;
		return nullptr;
	}
	else
	{
		auto it_end = it->second.end();
		for ( auto it_begin = it->second.begin(); it_begin != it_end; ++it_begin )
		{
			auto str_out = Ch2En_ForLvl(it_begin, ch_name, for_lvl - 1);
			if (str_out)
				return str_out;
		}
		return nullptr;
	}	
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::Ch2EnA( const std::string& ch_name ) const
{
	if (ch_name.empty())
		return kEmptyWStr;
	return Ch2En(GbkToWideChar(ch_name));
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::Ch2EnIf( const Ch2EnCondT& cond ) const
{
	if ( !tree_node_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	assert(for_lvl_ > 0);
	auto it_end = tree_node_->end();
	for ( auto it_begin = tree_node_->begin(); it_begin != it_end; ++it_begin )
	{
		if (Ch2En_ForLvl_If(cond, it_begin, for_lvl_ - 1))
			return it_begin->first;
	}
	//assert(false);
	return kEmptyWStr;
}

bool GameEn2ChNodeMgr::StrIdexBase::Ch2En_ForLvl_If( const Ch2EnCondT& cond, const PTreeIterT& it, int for_lvl ) const
{
	if ( for_lvl <= 0 )
	{
		assert(0 == for_lvl);
		return cond(it);
	}
	else
	{
		auto it_end = it->second.end();
		for ( auto it_begin = it->second.begin(); it_begin != it_end; ++it_begin )
		{
			if (Ch2En_ForLvl_If(cond, it_begin, for_lvl - 1))
				return true;
		}
		return false;
	}
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::TwoCh2En( const std::wstring& ch_lhs, const std::wstring& ch_rhs ) const
{
	assert(false);
	return kEmptyWStr;
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::TwoCh2EnA( const std::string& ch_lhs, const std::string& ch_rhs ) const
{
	if ( ch_lhs.empty() || ch_rhs.empty() )
	{
		assert(false);
		return kEmptyWStr;
	}
	return TwoCh2En(GbkToWideChar(ch_lhs), GbkToWideChar(ch_rhs));
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::ThreeCh2EnA( const std::string& ch_first, const std::string& ch_second, const std::string& ch_third ) const
{
	if (ch_first.empty())
	{
		assert(false);
		return kEmptyWStr;
	}
	const auto& wsecond = ch_second.empty() ? kEmptyWStr : GbkToWideChar(ch_second);
	const auto& wthird = ch_third.empty() ? kEmptyWStr : GbkToWideChar(ch_third);
	return ThreeCh2En(GbkToWideChar(ch_first), wsecond, wthird);
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::ThreeCh2En( const std::wstring& ch_first, const std::wstring& ch_second, const std::wstring& ch_third ) const
{
	assert(false);
	return kEmptyWStr;
}

const std::wstring& GameEn2ChNodeMgr::StrIdexBase::ThreeCh2EnA_( const std::string& ch_first, const std::string& ch_second, const std::wstring& str_third ) const
{
	if (ch_first.empty())
	{
		assert(false);
		return kEmptyWStr;
	}
	const auto& wsecond = ch_second.empty() ? kEmptyWStr : GbkToWideChar(ch_second);
	return ThreeCh2En(GbkToWideChar(ch_first), wsecond, str_third);
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Quest::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_MainTitle;
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Quest::TwoCh2En( const std::wstring& ch_lhs, const std::wstring& ch_rhs ) const
{
	assert(!ch_rhs.empty());
	boost::wregex reg(ch_rhs);
	return Ch2EnIf([&ch_lhs, &reg]( const PTreeIterT& it ){
		const auto& ch_lhs_value = it->second.get_optional<stPTreeEn2ChValue>(kEn2Ch_MainTitle);
		if ( !ch_lhs_value )
			return false;
		if ( !ch_lhs_value->game_ch_str_ )
		{
			assert(false);
			return false;
		}
		if ( !ch_lhs_value->game_ch_str_->ch_str_.EqualTo(ch_lhs) )
			return false;
		const auto& ch_rhs_value = it->second.get_optional<stPTreeEn2ChValue>(kEn2Ch_BeginDesc);
		if ( !ch_rhs_value )
		{
			assert(false);
			return false;
		}
		if ( !ch_rhs_value->game_ch_str_ )
		{
			assert(false);
			return false;
		}
		if ( !ch_rhs_value->game_ch_str_->ChMatchPattern(reg) )
			return false;
		return true;
	});
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Quest::ThreeCh2En( const std::wstring& ch_first, const std::wstring& ch_second, const std::wstring& ch_third ) const
{
	using namespace boost;
	assert(!ch_first.empty());
	wregex reg_second(ch_second);
	wregex reg_third(ch_third, regex_constants::perl | regex_constants::icase);
	wregex* reg_second_ptr;
	wregex* reg_third_ptr;
	if (ch_second.empty())
		reg_second_ptr = nullptr;
	else
		reg_second_ptr = &reg_second;
	if (ch_third.empty())
		reg_third_ptr = nullptr;
	else
		reg_third_ptr = &reg_third;
	
	return Ch2EnIf([&ch_first, reg_second_ptr, reg_third_ptr]( const PTreeIterT& it ){
		const auto& ch_lhs_value = it->second.get_optional<stPTreeEn2ChValue>(kEn2Ch_MainTitle);
		if ( !ch_lhs_value )
			return false;
		if ( !ch_lhs_value->game_ch_str_ )
		{
			assert(false);
			return false;
		}
		if ( !ch_lhs_value->game_ch_str_->ch_str_.EqualTo(ch_first) )
			return false;
		if (reg_second_ptr)
		{
			const auto& ch_rhs_value = it->second.get_optional<stPTreeEn2ChValue>(kEn2Ch_BeginDesc);
			if ( !ch_rhs_value )
			{
				assert(false);
				return false;
			}
			if ( !ch_rhs_value->game_ch_str_ )
			{
				assert(false);
				return false;
			}
			if ( !ch_rhs_value->game_ch_str_->ChMatchPattern(*reg_second_ptr) )
				return false;
		}
		if (reg_third_ptr)
		{
			if ( !regex_match(it->first, *reg_third_ptr) )
				return false;
		}		
		return true;
	});
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Village::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_Name;
}

GameEn2ChNodeMgr::StrIndex_Village::StrIndex_Village()
{
	for_lvl_ = 2;
}

GameEn2ChNodeMgr::StrIndex_Stage::StrIndex_Stage()
{
	for_lvl_ = 2;
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Stage::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_Name;
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Skill::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_Name;
}

GameEn2ChNodeMgr::StrIndex_Skill::StrIndex_Skill()
{
	for_lvl_ = 2;
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_QuestBeginDesc::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_BeginDesc;
}

GameEn2ChNodeMgr::StrIndex_Item::StrIndex_Item()
{
	for_lvl_ = 2;
}

const std::wstring& GameEn2ChNodeMgr::StrIndex_Item::CalcSuffix( const std::wstring& en_id ) const
{
	return kEn2Ch_Name;
}
