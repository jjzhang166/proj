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
#include "GProtocolS.h"
#include <Common/GameObjBase.h>
#include <stack>
#include "GProtocolR.h"
#include "GAddr.h"
#include <Socket/PocoMore.h>

namespace GClass
{
	class TimerSyncExec{
	public:
		TimerSyncExec();
		bool TimerGWndExecSync( const std::function<bool()>& func, int time_out = 80 );
		bool TimeoutToDoUpdate( int time_out = 80 ) const;

	private:
		bool AutoTimeoutToDoUpdate( int time_out );

	private:
		TimeDuration				time_last_update_;
	};

	//发包
	class TcpSend : public Singleton<TcpSend, Singleton_MakeMe>{
		//type definitions
	private:
		//发包辅助对象
		class SendCallHelper{
			//type definitions
		public:
			struct SecretHelper{
				SecretHelper();
				//加密
				void MakeSecret( SendCallHelper* call_helper );

			public:
				void*			unknown_ptr1_;
				void*			unknown_ptr2_;
			};

		public:
			SendCallHelper();
			//索取数据
			bool RetrieveData();

		private:
			void*				secret_obj_;
			void*				task_obj_;
		};
		//组包
		class PackageCombine{
		public:
			PackageCombine();
			~PackageCombine();
			//组建包
			bool CombinePackage( const void* tcp_package, std::size_t package_size );

			DWORD				addr1_;
			DWORD				addr2_;
			void*				buffer_begin_addr1_;
			void*				buffer_end_addr_;
			DWORD				buffer_begin_addr2_;
		};

	public:
		TcpSend();

		//interface
	public:
		bool SendMsg( const CMDS_MsgBase& msg );
		DWORD SendMsgTest( const CMDS_MsgBase& msg, const char* buffer_begin, const char* buffer_end );

	private:
		//底层发包
		static void SendImpl( GType::GameSendSocket* game_socket_obj, PackageCombine* build_package, const SendCallHelper::SecretHelper& secret_hlp );
		//发包
		bool Send( const void* msg, std::size_t msg_size );
	};

	//游戏内存的字符串结构
	struct stGameString{
	private:
		//stGameString();
		stGameString& operator = ( const stGameString& );
		//~stGameString();

	public:
		bool CopyTo( std::wstring& str_out ) const;
		std::wstring Duplicate() const;
		bool EqualTo( const std::wstring& rhs ) const;
		const wchar_t* GetOriginBegin() const;
		const wchar_t* GetOriginEnd() const;
		bool empty() const;

		union{
			const wchar_t*const		wstr_ptr_;
			const wchar_t			wstr_[8];
		};
		const int		str_cnt_;
		const int		char_total_cnt_;			//包括结尾的0字符
		const int		unknown_;
	};
	static_assert(sizeof(stGameString) == sizeof(GAME_STRING_ID), "GAME_STRING_ID");

	struct stGameStringEx{
		void*			vtbl_;
		stGameString	game_str_;
	};
	static_assert(sizeof(stGameStringEx) == sizeof(GAME_STRING_VF_ID), "GAME_STRING_VF_ID");

	struct stGameString_4{
		stGameString	game_str_;
		DWORD			unknown_;
	};
	static_assert(sizeof(stGameString_4) == sizeof(GAME_STRING_ID_), "GAME_STRING_ID_");

	struct stGameVectorCopier : public GType::stGameVector{
	private:
		stGameVectorCopier();
		stGameVectorCopier& operator = ( const stGameVectorCopier& );

	public:
		bool CopyTo( GType::stGameVector& game_vec ) const;
		GType::stGameVector Duplicate() const;
	};

	//游戏中的二叉树
	class BTreeBase : public GGameObjBase{
	public:
		struct stBTreeNodeBase{
			stBTreeNodeBase*			node_left_;
			stBTreeNodeBase*			node_parent_;
			stBTreeNodeBase*			node_right_;

		public:
			//根结点是否有效
			bool RootNodeValid() const;

		private:
			stBTreeNodeBase();
			stBTreeNodeBase& operator = ( const stBTreeNodeBase& rhs );
		};
		static_assert(sizeof(stBTreeNodeBase) == sizeof(NODE), "NODE");

		void SetCurNode( const stBTreeNodeBase* game_cur_node );

	public:
		BTreeBase();
		bool Update();
		virtual bool ObjValid(){
			return true;
		}

	protected:
		const stBTreeNodeBase*				game_cur_node_;
	};
	template<typename T>
	class BTreeBasePtrWrapper : public BTreeBase{
	public:
		typedef T RealTypeT;
		static_assert(std::is_base_of<BTreeBase, RealTypeT>::value == true, "");
		typedef std::shared_ptr<RealTypeT> HandlerPtrT;

	public:
		bool Update(){
			if ( !handler_ )
			{
				assert(game_cur_node_);
				handler_.reset(new RealTypeT);
				handler_->SetCurNode(game_cur_node_);
			}
			return handler_->Update();
		}

		HandlerPtrT			handler_;
	};
	template<typename T>
	struct SdbIndex : public BTreeBase::stBTreeNodeBase{
		DWORD			str_hash_id_;
		const T*		real_obj_;
		void*			unknown_obj_ptr_;
		DWORD			unknown_flag_;
	};
	static_assert(sizeof(SdbIndex<int>) == sizeof(SDB_INDEX), "SDB_INDEX");

	class BTreeBasePlaceholder : public BTreeBase{};
	class BTreeMgrBase : public GGameObjMgrBase{
	public:
		struct stGBTreeNodes{
			void*						unknown1_;
			BTreeBase::stBTreeNodeBase*	node_null_;		//空节点，首节点，placeholder
			int							node_cnt_;		//二叉树有效节点个数，不包括空节点
			void*						unknown2_;

		private:
			stGBTreeNodes();
		};
		static_assert(sizeof(stGBTreeNodes) == sizeof(BINARY_TREE_MAP), "");
	};
	template<typename ContT_>
	class BTreeMgr : public BTreeMgrBase{
	public:
		typedef ContT_ ContT;
		typedef BTreeBase::stBTreeNodeBase GBTreeNodeBase;
		typedef typename ContT::value_type TreeNodeT;
		typedef TraverseTraits<const GBTreeNodeBase*, const GBTreeNodeBase*> GNodeTraTraits;
		typedef typename GNodeTraTraits::DoAnyTraverse GNodeDoAnyTraverse;

	public:
		BTreeMgr(){
			root_node_ = nullptr;
			node_placeholder_ = nullptr;
		}
		bool UpdateAll(){
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
		bool RebuildUpdateAll(){
			static_assert(std::is_base_of<BTreeBase, TreeNodeT>::value == true, "");
			if ( !root_node_ || !node_placeholder_ )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "root node or placeholder is empty";
				assert(false);
				return false;
			}
			if ( root_node_ == node_placeholder_ )
			{
				btree_.clear();
				return true;
			}
			ContT btree_tmp;
			std::stack<const GBTreeNodeBase*> btree_stack;
			btree_stack.push(root_node_);
			while ( !btree_stack.empty() )
			{
				auto cur_node_ptr = btree_stack.top();
				while (true)
				{
					if ( cur_node_ptr == node_placeholder_ )
						break;
					TreeNodeT node_tmp;
					node_tmp.SetCurNode(cur_node_ptr);
					if (node_tmp.ObjValid())
					{
						if ( !node_tmp.Update() )
						{
							LOG_ENTER_FUNC;
							LOG_O(Log_error) << "node tmp update failed";
							assert(false);
							break;
						}
						btree_tmp.push_back(node_tmp);
					}					

					btree_stack.push(cur_node_ptr->node_left_);
					cur_node_ptr = cur_node_ptr->node_left_;
				}
				btree_stack.pop();
				if ( !btree_stack.empty() )
				{
					cur_node_ptr = btree_stack.top();
					btree_stack.pop();
					btree_stack.push(cur_node_ptr->node_right_);
				}
			}
			std::swap(btree_tmp, btree_);
			return true;
		}
		bool DoUntil( const GNodeDoAnyTraverse& func ){
			if ( !root_node_ || !node_placeholder_ )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "root node or placeholder is empty";
				assert(false);
				return false;
			}
			if ( root_node_ == node_placeholder_ )
				return false;
			std::stack<const GBTreeNodeBase*> btree_stack;
			btree_stack.push(root_node_);
			while ( !btree_stack.empty() )
			{
				auto cur_node_ptr = btree_stack.top();
				while (true)
				{
					if ( cur_node_ptr == node_placeholder_ )
						break;
					if (func(cur_node_ptr))
						return true;

					btree_stack.push(cur_node_ptr->node_left_);
					cur_node_ptr = cur_node_ptr->node_left_;
				}
				btree_stack.pop();
				if ( !btree_stack.empty() )
				{
					cur_node_ptr = btree_stack.top();
					btree_stack.pop();
					btree_stack.push(cur_node_ptr->node_right_);
				}
			}
			return false;
		}
		//设置根结点
		virtual void SetRootNode( const GBTreeNodeBase* root_node ){
			assert(root_node);
			root_node_ = root_node;
		}
		void SetNodePlaceholder( const GBTreeNodeBase* node_placeholder ){
			assert(node_placeholder);
			node_placeholder_ = node_placeholder;
		}
		void SetGameNodes( const stGBTreeNodes* nodes ){
			if ( !nodes )
			{
				assert(false);
				return;
			}
			if ( nodes->node_cnt_ < 0 )
			{
				assert(false);
				return;
			}
			auto node_placeholder = nodes->node_null_;
			if ( !node_placeholder )
			{
				assert(false);
				return;
			}
			auto node_root = node_placeholder->node_parent_;
			if ( !node_root )
			{
				assert(false);
				return;
			}
			root_node_ = 0 == nodes->node_cnt_ ? node_placeholder : node_root;
			node_placeholder_ = node_placeholder;
		}
		void SetRootNodeAddr( const void* root_node_addr ){
			if ( !root_node_addr )
			{
				assert(false);
				return;
			}
			root_node_ = (decltype(root_node_))root_node_addr;
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
		const GBTreeNodeBase*		node_placeholder_;
	};
	template<typename TreeNodeT_, typename ContT_ = std::vector<TreeNodeT_> >
	class BTreeMgr_SkipRoot : public BTreeMgr<ContT_>{
	public:
		typedef BTreeMgr<ContT> BTreeMgrBase;

	public:
		void SetRootNode( const GBTreeNodeBase* root_node ){
			if ( !root_node )
			{
				assert(false);
				return;
			}
			__super::SetRootNode(root_node->node_parent_);
			__super::SetNodePlaceholder(root_node);
		}

	private:
		using BTreeMgrBase::SetNodePlaceholder;
	};
	typedef BTreeMgr_SkipRoot<BTreeBasePlaceholder> BTreeMgrPlaceholder;
	class GGameTypeBase : public GGameObjBase{
	public:
		GGameTypeBase();
		void SetCurObj( const void* cur_obj_addr );

	protected:
		const void*		cur_obj_addr_;
	};
	//游戏对象数组
	template<typename ContT_>
	class GGameTArray : public GGameObjMgrBase{
	public:
		typedef ContT_ ContT;
		typedef typename ContT::value_type GGameType;
		typedef typename GGameType::GameType GameType;
		static_assert(std::is_base_of<GGameTypeBase, GGameType>::value == true, "");
		struct stGameTArray{
			const GameType*		obj_begin_;
			const GameType*		obj_end_;
			const GameType*		obj_end1_;		//这个值与obj_end_相同
			const DWORD			null_addr_;		//一般为0

		private:
			stGameTArray();
		};
		static_assert(sizeof(stGameTArray) == sizeof(GAME_STRUCT_ARRAY_POINT), "GAME_STRUCT_ARRAY_POINT");

	public:
		GGameTArray(){
			game_array_ = nullptr;
		}
		bool UpdateAll(){
			if (cont_.empty())
				return false;
			for ( auto& item : cont_ )
			{
				if ( !item.Update() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "game type item update failed";
					assert(false);
					return false;
				}
			}
			return true;
		}
		bool RebuildUpdateAll(){
			if ( !game_array_ )
			{
				assert(false);
				return false;
			}
			if ( !game_array_->obj_begin_ || !game_array_->obj_end_ )
			{
				cont_.clear();
				return true;
			}
			if ( game_array_->obj_begin_ > game_array_->obj_end_ )
			{
				assert(false);
				return false;
			}
			ContT cont_tmp;
			GGameType ggame_tmp;
			for ( auto cur_ptr = game_array_->obj_begin_; cur_ptr < game_array_->obj_end_; ++cur_ptr )
			{
				ggame_tmp.SetCurObj(cur_ptr);
				if ( !ObjValid() )
					continue;
				if ( !ggame_tmp.Update() )
				{
					assert(false);
					return false;
				}
				cont_tmp.push_back(ggame_tmp);
			}
			std::swap(cont_tmp, cont_);
			return true;
		}
		void SetGameTArray( const stGameTArray* game_array ){
			assert(game_array);
			game_array_ = game_array;
		}
		ContT& GetCont(){
			return cont_;
		}
		const ContT& GetCont() const{
			return cont_;
		}
		bool empty() const{
			return cont_.empty();
		}
		std::size_t size() const{
			return cont_.size();
		}
		virtual bool ObjValid(){
			return true;
		}

	protected:
		ContT							cont_;
		const stGameTArray*				game_array_;
	};
	template<typename T>
	class GGameTypePlaceholder : public GGameTypeBase{
	public:
		typedef T GameType;
		bool Update();
	};
	typedef GGameTArray<std::vector<GGameTypePlaceholder<int> > >::stGameTArray GGameTArrayPlaceholder;
	//所有可以去的城镇或副本
	class AllowedVillageCloneMap : public BTreeBase{
	public:
		AllowedVillageCloneMap();
		bool Update();
		const std::wstring& GetMapId() const;
		GType::enFbDifficulty GetLvl() const;
		//是否是副本
		bool IsCloneMap() const;
		bool IsVillage() const;

	private:
		std::wstring				village_clonemap_name_;
		GType::enFbDifficulty		level_;						//城镇或副本等级，当前可接受的等级	
	};
	class AllowedVillageCloneMapMgr : public BTreeMgr_SkipRoot<AllowedVillageCloneMap>, public Singleton<AllowedVillageCloneMapMgr, Singleton_MakeMe>{
	public:
		typedef TraverseTraits<TreeNodeT&, TreeNodeT&> TraTraits;
		typedef TraTraits::IteratorBasePtr IterBasePtr;
		typedef TraTraits::DoEveryTraverse DoEveryTraverse;
		typedef TraTraits::DoAnyTraverse DoAnyTraverse;

	public:
		bool RebuildUpdateAll();
		const AllowedVillageCloneMap* FindByName( const std::wstring& map_name ) const;
		IterBasePtr TraverseIf( const DoAnyTraverse& func );
		//遍历所有
		void TraverseAll( const DoEveryTraverse& func );
		//遍历副本
		IterBasePtr TraverseCloneMap();
		//遍历城镇
		IterBasePtr TraverseVillage();
		//遍历有效等级的副本
		IterBasePtr TraverseRecommandLvlCloneMap( int the_lvl );

	private:
		bool DoUpdateImpl();

	private:
		TimerSyncExec	timer_sync_exec_;
	};
	//环境
	class GAmbient : public Singleton<GAmbient, Singleton_Me>{
		typedef SmartCondition::AutoResetDo AutoResetCondT;

	public:
		GAmbient();
		void HandleAmbient_String( const std::string& game_str );
		void HandleEnteredSelectChannel();
		void HandleEnteredFb();
		void HandleEnteredSelectRole();
		SmartCondition& GetFirstAnimateCond();
		SmartCondition& GetChooseChannelCond();
		SmartCondition& GetChooseRoleCond();
		SmartCondition& GetGameClockCond();
		SmartCondition& GetFbRetCityCond();
		SmartCondition& GetCityUiCond();
		SmartCondition& GetEnteredFbCond();
		SmartCondition& GetOpenedGameMenuCond();
		bool IsInitBefore() const;

	public:
		void FailToEnterFb();

	public:
		bool UntilCityUiNow( std::size_t wait_time = 0 );
		bool UntilEnteredFb( std::size_t wait_time = 0 );
		bool UntilCanRetCity( std::size_t wait_time = 0 );
		bool UntilChooseRole( std::size_t wait_time = 0 );
		bool UntilOpenedGameMenu( std::size_t wait_time = 0 );

	public:
		//角色未进入到游戏前的初始化
		void Init_BeforeRoleEnteredGame();
		//角色进入游戏后的初始化
		void Init_RoleEnteredGame();
		
	private:
		SmartCondition			cond_process_first_animate_;			//游戏进程正常打开到动画界面
		SmartCondition			cond_choose_channel_;					//频道选择界面
		SmartCondition			cond_choose_role_;						//角色选择界面
		SmartCondition			cond_game_clock_;						//游戏计时器条件，比如打完副本选择奖励
		SmartCondition			cond_ret_city_when_end_clone_map_;		//打完副本可以返回城镇了
		SmartCondition			cond_city_ui_now_;						//城镇界面，过图用
		SmartCondition			cond_entered_fb_;						//是否进入了副本
		SmartCondition			cond_fail_enter_fb_;
		SmartCondition			cond_opened_game_menu_;					//是否打开了游戏菜单界面
		bool					init_before_role_enter_game_;

	private:
		static GAmbient		ambient_instance_;
	};
	//所有角色信息
	class GAllPlayersStaticInfo : public Singleton<GAllPlayersStaticInfo, Singleton_Me>{
	public:
		typedef std::vector<CMDR_PartialListPlayersStaticInfo> PlayersContT;
		
	public:
		const PlayersContT& GetPlayersInfo() const;
		void ClearPlayersInfo();
		void AddPlayersInfo( const CMDR_PartialListPlayersStaticInfo& player_info );
		const CMDR_PartialListPlayersStaticInfo* FindByRoleName( const std::wstring& role_name ) const;
		bool GenRolesInfoToJson( Json::Value& json_array ) const;
		bool RolesInfoEmpty() const;

	private:
		PlayersContT					players_info_;

	private:
		static GAllPlayersStaticInfo		players_static_info_;
	};

	//游戏窗口类型
	class GWnd{
		GWnd();

	public:
		LRESULT WndProc( UINT message, WPARAM wParam, LPARAM lParam ) const;
	};

	//job name map
	class JobNameMap : public GGameObjBase, public Singleton<JobNameMap, Singleton_MakeMe>{
		typedef std::map<GType::enJobCategory, std::wstring> JobNameMapT;

	public:
		JobNameMap();
		const std::wstring& GetChName( GType::enJobCategory job ) const;
		const std::wstring& GetEnName( GType::enJobCategory job ) const;
		GType::enJobCategory FromChName( const std::wstring& ch_name ) const;
		GType::enJobCategory FromEnName( const std::wstring& en_name ) const;
		const std::wstring& En2ChName( const std::wstring& en_name ) const;
		bool Update();

	private:
		bool AddMap( GType::enJobCategory job, const std::wstring& ch_name );

	private:
		JobNameMapT				job_name_map_;
	};

	//职业class模板
	class GJobClassTemplate : public GGameObjBase{
	public:
		struct stGameJobClassType{
			GType::enJobCategory		str_dwClassType;		// 这里就是对应的class类型
			GClass::stGameString		str_theClass;	
		};
		static_assert(sizeof(stGameJobClassType) == sizeof(CALSS_CLASSTYPE_), "CALSS_CLASSTYPE_");

	public:
		GJobClassTemplate();
		bool Update();
		void SetGameJobClsType( const stGameJobClassType* game_job_cls_type );
		GType::enJobCategory GetJobType() const;
		const std::wstring& GetJobClsId() const;

	private:
		const stGameJobClassType*			game_job_cls_type_;
		GType::enJobCategory				job_type_;
		std::wstring						job_cls_id_;
	};
	class GJobClassTemplateMgr : public GGameObjMgrBase, public Singleton<GJobClassTemplateMgr, Singleton_MakeMe>{
	public:
		typedef std::vector<GJobClassTemplate> ContT;

	public:
		bool RebuildUpdateAll();
		bool UpdateAll();
		const ContT& GetCont() const;
		const GJobClassTemplate* FindByJobType( GType::enJobCategory job ) const;
		const GJobClassTemplate* FindByJobClsId( const std::wstring& job_cls_id ) const;

	private:
		ContT			job_classes_;
	};

	//游戏对象遍历辅助对象
	class GGameObjTraverse{
		GGameObjTraverse();

	public:
		static void NextGameObj( const GGameObjTraverse** next_game_obj );
	};

#pragma pack(push)
#pragma pack(1)
	//apple data time
	struct AppleDateTime{
		AppleDateTime();
		friend StrOutputBStream&
			operator << ( StrOutputBStream& ostm, const AppleDateTime& recv_value ){
				ostm << recv_value.wApple_DateTime << recv_value.wYear << recv_value.wMonth
					<< recv_value.wDay << recv_value.dwHour << recv_value.dwMinute
					<< recv_value.dwSecond << recv_value.dwMilliSec << recv_value.IsInfinity;
				return ostm;
		}
		friend StrInputBStreamBase& operator >> ( StrInputBStreamBase& istm, AppleDateTime& recv_value ){
			istm >> recv_value.wApple_DateTime >> recv_value.wYear >> recv_value.wMonth
				>> recv_value.wDay >> recv_value.dwHour >> recv_value.dwMinute
				>> recv_value.dwSecond >> recv_value.dwMilliSec >> recv_value.IsInfinity;
			return istm;
		}

		WORD			wApple_DateTime;
		WORD			wYear;
		WORD			wMonth;
		WORD			wDay;
		DWORD			dwHour;
		DWORD			dwMinute;
		DWORD			dwSecond;
		DWORD			dwMilliSec;
		BYTE			IsInfinity;
	};
#pragma pack(pop)
	class LocalRoomPos : public GClass::GGameTypeBase{
	public:
		struct stPosInfo{
			DWORD						dwFunAddr;
			GAME_STRUCT_ARRAY_POINT		theArray1;
			GAME_STRUCT_ARRAY_POINT		theArray2;
			DWORD						dwUnKnow;
			GType::stGameVector			theRoomPosition;
		};
		struct GameType{
			stGameString_4	the_id_;
			const stPosInfo* pos_ptr_;
		};

	public:
		bool Update();

	public:
		GType::stGameVector			room_pos_;
	};
	class LocalRoomPosMgr : public GClass::GGameTArray<std::vector<LocalRoomPos> >, public Singleton<LocalRoomPosMgr, Singleton_MakeMe>{
	public:
		bool RebuildUpdateAll();
		bool FindByXY( float x, float y, float& z );

	private:
		bool UpdateImpl();
		bool FindByXY_Impl( float x, float y, float& z );
	};
	struct stPreActionPlayTime : GClass::BTreeBase{
		struct stPapt : BTreeBase{
			stPapt(){
				action_play_time = 0.;
				pre_action_serial_ = 0;
			}
			bool Update();

			float		action_play_time;
			DWORD		pre_action_serial_;
		};
		struct stPaptMgr : BTreeMgr_SkipRoot<stPapt>{};

		stPreActionPlayTime(){
			serial_ = 0;
			next_node_ = nullptr;
		}
		bool Update();
		const stPapt* GetPapt() const;

		DWORD							serial_;
		const PREV_ACTION_PLAY_TIME_2*	next_node_;
		stPaptMgr						papt_mgr_;
	};
	struct stPreActionPlayTimeMgr : BTreeMgr_SkipRoot<stPreActionPlayTime>, public Singleton<stPreActionPlayTimeMgr, Singleton_MakeMe>{
		const stPreActionPlayTime* FindBySerial( DWORD serial );
		bool RebuildUpdateAll();

	private:
		bool UpdateImpl();
	};
}