#pragma once
/*
@author zhp
@date 2014-2-18 13:26
@purpose for game class
*/
#include "GTypes.h"
#include <string>
#include "Common/GObjProxy.h"
#include "Protocol/P_Defines.h"
#include <queue>
#include "json/value.h"
#include "Common/UsefulClass.h"
#include "Net/PocoMore.h"
#include <Common/GameObjBase.h>
#include "GTypes.h"
#include <Net/IoTimer.h>
#include "GFuncCall.h"
#include <CD_CommonFiles/Data/CD_Structs.h>
#include <CD_CommonFiles/Data/CD_Traverse.h>

namespace GClass
{
	class GameObjMgrFilterBase : public GGameObjMgrBase{
	public:
		virtual bool AcceptTheType(CD_En_LocalObjType game_type, GameObjType& obj_type) const = 0;
	};

	//游戏中的二叉树
	template<typename OwnerT>
	class BTreeBaseT : public GGameObjBase{
	public:
		typedef stCD_Node stBTreeNodeBase;
		typedef OwnerT OwnerType;

	public:
		BTreeBaseT(){
			game_cur_node_ = nullptr;
		}
		virtual bool SetCurNode(const stBTreeNodeBase* game_cur_node){
			assert(game_cur_node);
			game_cur_node_ = game_cur_node;
			return true;
		}
		const stBTreeNodeBase* GetCurNode() const{
			return game_cur_node_;
		}
		void SetOwner(OwnerType* owner){
			assert(owner);
			owner_ = owner;
		}
		OwnerType* GetOwner() const{
			return owner_;
		}
		virtual bool ObjValid(){
			return true;
		}		

	private:
		const stBTreeNodeBase*	game_cur_node_;

	protected:
		OwnerType*				owner_;
	};
	typedef BTreeBaseT<GGameObjMgrBase> BTreeBase;

	template<typename T, typename OwnerT>
	class BTreeBasePtrWrapper : public BTreeBaseT<OwnerT>{
	public:
		typedef T RealTypeT;
		static_assert(std::is_base_of<BTreeBaseT<OwnerT>, RealTypeT>::value == true, "");
		typedef boost::shared_ptr<RealTypeT> HandlerPtrT;

	protected:
		bool Update() override{
			if ( !handler_ )
			{
				handler_.reset(new RealTypeT);
				handler_->SetCurNode(GetCurNode());
				handler_->SetOwner(GetOwner());
			}
			return handler_->Update();
		}

		HandlerPtrT			handler_;
	};
	class BTreeBasePlaceholder : public BTreeBaseT<GGameObjMgrBase>{};
	template<typename ContT_, typename BaseT = GGameObjMgrBase>
	class BTreeMgr : public BaseT{
	public:
		typedef BaseT BaseType;
		typedef ContT_ ContT;
		typedef typename BTreeBaseT<BaseT>::stBTreeNodeBase GBTreeNodeBase;
		typedef typename ContT::value_type TreeNodeT;
		typedef TraverseTraits<const GBTreeNodeBase*, const GBTreeNodeBase*> GNodeTraTraits;
		typedef typename GNodeTraTraits::DoAnyTraverse GNodeDoAnyTraverse;

	public:
		BTreeMgr(){
			root_node_ = nullptr;
		}
		bool UpdateAllImpl() override{
			if (btree_.empty())
				return false;
			for ( auto& node : btree_ )
			{
				if ( !node.Update() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "btree node update failed";
					assert(false);
					return false;
				}
			}
			return true;
		}
		bool RebuildAllImpl() override{
			static_assert(std::is_base_of<BTreeBaseT<BaseT>, TreeNodeT>::value == true, "");
			ContT btree_tmp;
			CD_Traverse::NodeTraverse(const_cast<GBTreeNodeBase*>(root_node_), [&btree_tmp, this](GBTreeNodeBase* cur_node){
				TreeNodeT node_tmp;
				node_tmp.SetOwner(this);
				if (!node_tmp.SetCurNode(cur_node))
					return false;
				if (node_tmp.ObjValid())
				{
					if (!node_tmp.Update())
					{
						/*LOG_ENTER_FUNC;
						LOG_O(Log_error) << "node tmp update failed";
						assert(false);*/
						//break;
					}
					else
					{
						btree_tmp.push_back(node_tmp);
						return true;
					}
				}
				return false;
			});
			std::swap(btree_tmp, btree_);
			return true;
		}
		void DoRebuildUpdateClear() override{
			btree_.clear();
			root_node_ = nullptr;
		}
		//设置根结点
		void SetRootNode( const GBTreeNodeBase* root_node ){
			root_node_ = root_node;
		}
		ContT& GetAllNodes(){
			return btree_;
		}
		const ContT& GetAllNodes() const{
			return btree_;
		}

	protected:
		ContT						btree_;

	private:
		const GBTreeNodeBase*		root_node_;
	};
	template<typename TreeNodeT_, typename ContT_ = std::vector<TreeNodeT_>, typename BaseT = GGameObjMgrBase>
	class BTreeMgr_SkipRoot : public BTreeMgr<ContT_, BaseT>{
	public:
		typedef BTreeMgr<ContT, BaseT> BTreeMgrBase;
	};

	class IAmbientStr{
	public:
		typedef std::function<void(const std::string&)> MatchDoFuncT;

	public:
		virtual ~IAmbientStr(){}
		void AddSubMatchStr( const std::string& sub_str );
		void SetMatchDoFunc( const MatchDoFuncT& func );

	public:
		bool CanMatchTheStr( const std::shared_ptr<std::string>& full_str ) const;
		virtual void SetUnknownData( DWORD data );
		virtual void DoMatched( const std::shared_ptr<std::string>& full_str ) const = 0;

	protected:
		NameList<std::string, std::vector<std::string> >	sub_strs_;
		MatchDoFuncT										do_func_;
	};
	class AmbientStr_ByDlg : public IAmbientStr{
		void DoMatched(const std::shared_ptr<std::string>& full_str) const override;
	};
	class AmbientStr_AmbientStr : public IAmbientStr{
		void DoMatched(const std::shared_ptr<std::string>& full_str) const override;
	};
	class GAmbient : public Singleton<GAmbient, Singleton_MakeMe>{
		typedef SmartCondition::AutoResetDo AutoResetCondT;
		typedef std::shared_ptr<IAmbientStr> AmbientStrHanderPtrT;
		typedef std::vector<AmbientStrHanderPtrT> AmbientStrHanderContT;

	public:
		GAmbient();
		bool IsGameOver() const;
		void SetJuQingStr( const std::string& str_ju_qing );
		void BeginTimer_FbWin();
		void CancelTimer_FbWin();
		void MatchDlgInfoStr( const std::shared_ptr<std::string>& full_str, DWORD data );
		void MatchAmbientStr( const std::shared_ptr<std::string>& full_str );
		bool IsCursorSameAsTheState( bool is_cursor_show ) const;

	public:
		bool WaitCrossedGraph( std::size_t wait_time = 0 );
		bool WaitRetVillage( std::size_t wait_time = 0 );
		bool UntilJuQing( const std::string& str_ju_qing, std::size_t wait_time = 0 );
		bool PressKeyEscWaitFor( bool is_show_cursor );
		bool PressKeyGraveWaitFor( bool is_show_cursor );

	public:
		SmartCondition			cond_cross_graph_;
		SmartCondition			cond_fb_game_over_win_;
		SmartCondition			cond_fb_game_over_lose_;
		SmartCondition			cond_ju_qing_;
		boost::optional<BOOL>	is_show_cursor_;

	private:
		std::string				str_ju_qing_;
		IoTimerPtr				timer_fb_win_;
		AmbientStrHanderContT		dlg_str_handlers_;
		AmbientStrHanderContT		ambient_str_handlers_;
	};
	//锻造
	class CraftDbMgr : public GGameObjBase, public Singleton<CraftDbMgr, Singleton_MakeMe>{
	public:
		struct CraftMaterial{
			int		material_item_id_;
			int		item_cnt_;
		};
		struct stCraftDb{
			std::vector<stCD_MaterialInfo>	materials_;
			int							craft_id_;
			int							dst_item_id_;		//锻造成功后
		};
		typedef std::vector<stCraftDb> CraftInfoT;

	public:
		bool Update() override;
		const stCraftDb* FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const;
		const stCraftDb* FindByDstItemId( int dst_item_id ) const;
		void DebugInfo() const override;

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	private:
		CraftInfoT			craft_info_;
	};

	class ShopDb : public GGameObjBase, public Singleton<ShopDb, Singleton_MakeMe>{
	public:
		struct stShopItem{
			int		shop_id_;
			int		item_id_;
			int		item_cnt_;
		};
		typedef std::vector<stShopItem> ShopItemsT;
		struct stDb{
			CD_En_ShopInfoCategory		cate_id_;
			std::string					cate_name_;
			ShopItemsT					items_;

			const stShopItem* FindByItemId(int item_id) const;
		};

	public:
		bool Update() override;
		const stDb* FindByName(const std::string& shop_cate_name) const;

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	private:
		std::vector<stDb>	infos_;
	};
	class BuffState : public GClass::BTreeBase{
	public:
		BuffState();
		bool MatchDesc( const std::string& sub_desc ) const;

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

		//test
	public:
		void DebugInfo() const override;

		DWORD			packet_id_;
		int				buff_id_;
		std::string		buff_name_;
		std::string		buff_desc_;
	};

	class BuffStateMgr : public GClass::BTreeMgr_SkipRoot<BuffState>, public Singleton<BuffStateMgr, Singleton_MakeMe>{
	public:
		BuffStateMgr();
		bool RebuildAll() override;
		void DebugInfo() const override;
		const BuffState* FindByName( const std::string& buff_name );

	private:
		bool RebuildAllImpl() override;

	private:
		TimeElapse		time_elapse_;
	};
	class FarmDbMgr : public GGameObjBase, public Singleton<FarmDbMgr, Singleton_MakeMe>{
	public:
		struct stDb{
			int			id_;
			int			the_lvl_;			//采集点等级
			int			jing_suan_dian_;	//精算点
			int			bind_gold_;			//绑定金
			int			role_lvl_;			//角色等级限制
			std::string	name_;

			stDb();
		};

	public:
		bool Update() override;
		void DebugInfo() const override;
		const stDb* FindById( int id ) const;

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:
		std::vector<stDb>		info_;
	};

	class CurFarmFacilityInfo : public GGameObjBase{
	public:
		struct stFacility{
			int						farm_db_id_;
			CD_En_FarmFacilityIdx	idx_;			//索引
			int						lvl_;			//当前等级
			int						left_seconds_;	//剩余时间

			stFacility();
		};

	public:
		CurFarmFacilityInfo();
		void DebugInfo() const override;
		const stFacility* FindByIdx(CD_En_FarmFacilityIdx idx) const;
		bool CalcNextCollectTime();
		void ResetAllCollectTime();

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:
		std::vector<stFacility>		info_;
		std::vector<DateTimeLocal>	time_to_collect_;

		static const Poco::Timespan		kTimeToCollectOfNoLvl;		//如果庄园没有升过级，则每隔一段时间才能去采集一次
	};

	class FarmSeedDbMgr : public GGameObjBase, public Singleton<FarmSeedDbMgr, Singleton_MakeMe>{
	public:
		struct stDb{
			int			item_id_;
			std::string	item_name_;

			stDb();
		};

	public:
		bool Update() override;
		const stDb* FindByItemId( int item_id ) const;
		void DebugInfo() const override;

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:
		std::vector<stDb>		info_;
	};

	class CurFarmFieldInfo : public GGameObjBase{
	public:
		struct stState{
			CD_En_FarmFieldIdx			idx_;
			int			flag_;
			int			seed_id_;
			int			left_seconds_;

			stState();
			//是否可以被种植
			bool CanBePlanted() const;
			//是否可以采集
			bool CanBeCollected() const;
		};

	public:
		CurFarmFieldInfo();
		const stState* FindByIdx(CD_En_FarmFieldIdx idx) const;
		bool CalcNextCollectTime();
		void ResetAllCollectTime();

	private:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:

		std::vector<stState> info_;
		std::vector<DateTimeLocal>	time_to_collect_;
	};

	//庄园采集名称映射
	class GFarmCollectNameMap : public Singleton<GFarmCollectNameMap, Singleton_MakeMe>{
	public:
		struct stFarmCollectInfo{
			std::string		collect_en_name_[2];
			CD_En_FarmFacilityIdx	collect_idx_;
			CD_En_FarmFieldIdx		cao_yao_tian_idx_;
		};
		typedef std::map<std::string, stFarmCollectInfo> MapT;
		typedef TraverseTraits<const MapT::value_type&> ItemTraTraits;
		typedef ItemTraTraits::IteratorBasePtr CollectInfoIterPtr;

	public:
		GFarmCollectNameMap();
		const stFarmCollectInfo* FindByCnName( const std::string& name ) const;
		CollectInfoIterPtr TraverseById(CD_En_FarmFacilityIdx collect_idx);
		void DebugInfo_CnName() const;

	private:
		MapT		map_;
	};

	class GHitMonsterModeBase{
	public:
		virtual ~GHitMonsterModeBase(){}
		virtual bool DoHitOnce() = 0;
		virtual int GetSteps() const = 0;
	};
	class GHitMonsterMode_Mouse : public GHitMonsterModeBase{
	public:
		enum enHitMode{
			kRightHit,
			kInvalid
		};

	public:
		void SetCurTimeHitMode(enHitMode hit_mode);

	private:
		bool DoHitOnce() override;
		int GetSteps() const override;

	private:
		int			hit_cnt_ = 0;
		enHitMode	cur_time_hit_mode_ = kInvalid;
	};
	class GHitMonsterMode_UsePacketItem : public GHitMonsterModeBase{
	public:
		GHitMonsterMode_UsePacketItem();
		bool DoHitOnce() override;
		int GetSteps() const override;

	public:
		void SetCurItem( const std::string& item_name, int steps );

	private:
		std::string		item_name_;
		int				steps_;
	};

	class GCurShangJinInfo : public GGameObjBase, public Singleton<GCurShangJinInfo, Singleton_MakeMe>{
	public:
		struct stShangeJinItem{
			std::string	task_name_;
			int			task_id_;
		};

	protected:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:
		std::vector<stShangeJinItem>		info_;
	};

	class GTaskFbInfo : public GGameObjBase, public Singleton<GTaskFbInfo, Singleton_MakeMe>{
	public:
	public:
		struct stTaskFbInfo{
			//std::string	chapter_str_;		//章节的字符串
			int			task_index_;		//章节内任务的序号
			int			task_fb_id_;		//任务副本ID
			int			village_id_;		//任务所在城镇ID
			int			show_flag_;			//1为显示出来的主线任务副本，2为正常副本

			stTaskFbInfo();
		};
		struct stKingQuestInfo{
			std::string		fb_title_;		//title == null 的是王立精英
			int				village_id_;
			int				task_fb_id_;

			stKingQuestInfo();
		};

	public:
		bool Update() override;
		void DebugInfo() const override;
		const stTaskFbInfo* FindByFbId( int fb_id ) const;
		const stKingQuestInfo* FindByFbId_King( int fb_id ) const;
		//剧情已完成
		bool IsTheFbIdJuQingDone( int fb_id ) const;
		//是否属于剧情任务
		bool IsTheFbIdJuQingTask( int fb_id ) const;

	protected:
		bool UpdateImpl() override;
		void DoUpdateClear() override;

	public:
		std::vector<stTaskFbInfo>		info_;
		std::vector<stKingQuestInfo>	king_info_;
	};

	class GVillage2RoleLvlInfo : public Singleton<GVillage2RoleLvlInfo, Singleton_MakeMe>{
	public:
		GVillage2RoleLvlInfo();

	public:
		int Village2RoleLvl( const std::string& village_name ) const;

	private:
		std::map<std::string, int>		village_lvl_info_;
	};
	struct stItemInfo{
		std::string		item_name_;
		int				item_cnt_;
	};
}