#pragma once
/*
@author zhp
@date 2014/8/15 13:06
@purpose 游戏中英文映射
*/
#include "GClass.h"
//#include <boost/flyweight.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>

//游戏中，中项文映射的底层数据结构
struct stGStrEn2ChBase : public GClass::BTreeBase::stBTreeNodeBase{
	const GClass::stGameString		en_str_;
	const GClass::stGameString		ch_str_;

public:
	bool EnMatchPattern( const boost::wregex& the_regex ) const;
	bool ChMatchPattern( const boost::wregex& the_regex ) const;
};

/*
struct stEn2ChTag{};
typedef boost::flyweight<std::wstring, boost::flyweights::tag<stEn2ChTag> > SharedEn2ChStrT;*/
struct stPTreeEn2ChValue{
	//SharedEn2ChStrT					path_en_node_;
	const stGStrEn2ChBase*			game_ch_str_;

	stPTreeEn2ChValue();
	//stPTreeEn2ChValue( const std::wstring& en_node );
	stPTreeEn2ChValue( const stGStrEn2ChBase* game_ch_str );
};
typedef boost::property_tree::basic_ptree<std::wstring, stPTreeEn2ChValue> En2ChPTreeT;

class GameEn2ChNode : public GClass::BTreeBase{
public:
	//typedef std::vector<std::wstring> StrNodeContT;
	struct ContWrapper{
		typedef GameEn2ChNode value_type;
		void push_back( const GameEn2ChNode& en2ch_node );
		bool empty() const;
		GameEn2ChNode* begin();
		GameEn2ChNode* end();
		void clear();

		En2ChPTreeT		en2ch_ptree_;
	};
	//过滤器
	struct InitFilter{
		typedef std::vector<boost::wregex> MatchPatternContT;
		InitFilter();
		void AddFilter( const std::wstring& en_match_pattern );
		const MatchPatternContT& GetMatchPatternCont() const;

	private:
		MatchPatternContT		match_pattern_cont_;
	};

public:
	GameEn2ChNode();
	bool Update();
	const stGStrEn2ChBase* GetEn2ChNode() const;
	//const StrNodeContT& GetStrNodeCont() const;

private:
	//StrNodeContT			str_node_cont_;

private:
	static GameEn2ChNode		empty_placeholder_;
	static const InitFilter		init_filter_;
};

class GameEn2ChNodeMgr : public GClass::BTreeMgr_SkipRoot<GameEn2ChNode, GameEn2ChNode::ContWrapper>
	, public Singleton<GameEn2ChNodeMgr, Singleton_MakeMe>{
public:
	class StrIdexBase{
	public:
		typedef En2ChPTreeT::const_iterator PTreeIterT;
		typedef std::function<bool(const PTreeIterT&)> Ch2EnCondT;

	public:
		StrIdexBase();
		virtual ~StrIdexBase(){}
		bool ParseFromRoot( const En2ChPTreeT& tree_root, const std::wstring& node_path );
		std::wstring En2Ch( const std::wstring& en_id ) const;
		const std::wstring& Ch2En( const std::wstring& ch_name ) const;
		const std::wstring& Ch2EnA( const std::string& ch_name ) const;
		const std::wstring& Ch2EnIf( const Ch2EnCondT& cond ) const;
		virtual const std::wstring& TwoCh2En( const std::wstring& ch_lhs, const std::wstring& ch_rhs ) const;
		const std::wstring& TwoCh2EnA( const std::string& ch_lhs, const std::string& ch_rhs ) const;
		virtual const std::wstring& ThreeCh2En( const std::wstring& ch_first, const std::wstring& ch_second, const std::wstring& ch_third ) const;
		const std::wstring& ThreeCh2EnA( const std::string& ch_first, const std::string& ch_second, const std::string& ch_third ) const;
		const std::wstring& ThreeCh2EnA_( const std::string& ch_first, const std::string& ch_second, const std::wstring& str_third ) const;

	protected:
		virtual const std::wstring& CalcSuffix( const std::wstring& en_id ) const = 0;

	private:
		bool En2Ch_ForLvl( const PTreeIterT& it, const std::wstring& en_id, int for_lvl, std::wstring& str_out ) const;
		const std::wstring* Ch2En_ForLvl( const PTreeIterT& it, const std::wstring& ch_name, int for_lvl ) const;
		bool Ch2En_ForLvl_If( const Ch2EnCondT& cond, const PTreeIterT& it, int for_lvl ) const;

	protected:
		const En2ChPTreeT*		tree_node_;
		int						for_lvl_;
	};
	typedef boost::shared_ptr<StrIdexBase> StrIndexPtr;

	class StrIndex_Quest : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
		const std::wstring& TwoCh2En( const std::wstring& ch_lhs, const std::wstring& ch_rhs ) const;
		const std::wstring& ThreeCh2En( const std::wstring& ch_first, const std::wstring& ch_second, const std::wstring& ch_third ) const;
	};
	class StrIndex_QuestBeginDesc : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
	};
	class StrIndex_Village : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
	public:
		StrIndex_Village();
	};
	class StrIndex_Stage : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
	public:
		StrIndex_Stage();
	};
	class StrIndex_Skill : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
	public:
		StrIndex_Skill();
	};
	class StrIndex_Item : public StrIdexBase{
		const std::wstring& CalcSuffix( const std::wstring& en_id ) const;
	public:
		StrIndex_Item();
	};

public:
	bool UpdateAll();
	bool RebuildUpdateAll();

public:
	const std::wstring& Quest_Ch2En( const std::wstring& quest_ch_name );
	const StrIndexPtr& GetQuestIndex() const;
	const StrIndexPtr& GetQuestBeginDescIndex() const;
	const StrIndexPtr& GetVillageIndex() const;
	const StrIndexPtr& GetStageIndex() const;
	const StrIndexPtr& GetSkillIndex() const;
	const StrIndexPtr& GetItemIndex() const;
	std::wstring FindChByFullPath( const std::wstring& full_path ) const;

private:
	StrIndexPtr				quest_index_;
	StrIndexPtr				quest_begin_desc_index_;
	StrIndexPtr				village_index_;
	StrIndexPtr				stage_index_;		//副本
	StrIndexPtr				skill_index_;
	StrIndexPtr				item_index_;
};