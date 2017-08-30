#pragma once
/*
@author zhp
@date 2014-3-10 14:59
@purpose for game player info
*/
#include <boost/asio.hpp>
#include "json/value.h"
#include "Common/Iterator.h"
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <list>
#include "Facility/GTypes.h"
#include "Net/IoTimer.h"
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormatter.h>
#include <set>
#include <Common/UsefulClass.h>
#include <boost/optional.hpp>
#include <Net/PocoMore.h>

//global functions
//Check result
bool CheckResFromRespond( const Json::Value& respond_msg );

class GPlayer : public Singleton<GPlayer, Singleton_Me>{
	GPlayer();

	//type definition
public:
	//level info
	struct stLevelInfo{
		stLevelInfo();
		stLevelInfo( int level, int exp, int to );
		//get level
		int GetLevel() const;
		//set level
		void SetLevel( int level );
		//get exp
		int GetExp() const;
		//set exp
		void SetExp( int exp );
		//get to
		double GetTo() const;
		//set to
		void SetTo( double to );
		bool ParseFromJsonObj( const Json::Value& json_level_info );

	private:
		int					level_;					//等级
		int					exp_;					//经验
		double				to_;					//升级所需经验
	};

	//球员
	struct stPlayer{
		stPlayer(){
			grade_ = 0;
			price_ = 0;
			life_ = 0;
			player_pos_ = GType::enPlayerPosition_No;
			spid_ = 0;
		}
		typedef std::list<stPlayer> PlayerContT;

		//职位信息
		struct PositionInfo{
			PositionInfo(){
				pos_ = 0;
			}

			bool ParseFromJsonObj( const Json::Value& pos_info );

			int						pos_;
			Json::Value				starting_;

			static const int		kSecondPos = 28;			//替补职位
		};

		const char* GetId() const;
		void SetId( int id );
		int GetGrade() const;
		void SetGrade( int grade );
		double GetPrice() const;
		void SetPrice( double price );
		int GetLife() const;
		void SetLife( int life );
		GType::enPlayerPosition GetPosition() const;
		void SetPosition( GType::enPlayerPosition pos );
		//得到数据库索引ID
		int GetSpid() const;
		//得到能力值
		int GetAbility() const;
		//得到球员名
		const std::string GetPlayerName() const;
		//是否可以交换
		bool CanExchangePlayer() const;
		//交换球员
		void ExchangePlayer( const stPlayer& other );
		//强化球员
		void EnhancePlayer(const stPlayer& other);
		static bool ExchangePlayers( const PlayerContT& players );
		//parse from json obj
		bool ParseFromJsonObj( const Json::Value& player_obj );
		//是否可以交换职位
		bool CanExchangePosition( const stPlayer& other );
		//交换职位
		bool ExchangePostion( stPlayer& other, bool is_send );
		bool ExchangePostionSync( stPlayer& other, bool is_send );
		//是否可以从侯补降为无
		bool CanDegradeFromThirdToNo() const;
		//从侯补降为无
		bool DegradeFromThirdToNo();
		//是否可以从无升级为侯补
		bool CanUpgradeFromNoToThird() const;
		//从无升级为侯补
		bool UpgradeFromNoToThird();
		//是否是一线球员
		bool IsFirstTeamPlayer() const;
		//是否是二线球员
		bool IsSecondTeamPlayer() const;
		//比赛结束后的更新
		void MatchEndUpdate( const stPlayer& new_player );
		//是否可以解雇
		bool CanBeFired() const;
		//发送解雇消息
		bool SendFireMsg() const;
		//拍卖球员
		bool SaleSelfAs( double price );
		//不检查剩余场次
		bool CanBeSale_NoStrict() const;
		bool SaleSelfAs_NoStrict( double price, bool notify_gplayer, bool notify_coor );
		//是否可以被拍卖
		bool CanBeSale() const;
		//调整价格
		static double AdjustPrice( double price );
		//是否是这样的职位之一
		bool IsInPos( GType::enPlayerPosition player_position[], int position_cnt ) const;

	public:
		//无效能力值
		static const int			kInvalidAbility = -1;

		std::string					id_;
		int							grade_;			//强化等级
		double						price_;			//球员价值
		int							life_;			//剩余场数
		GType::enPlayerPosition		player_pos_;	//球员职位
		PositionInfo				position_info_;	//职位信息
		int							spid_;			//数据库索引ID
	};

	//物品
	struct stItem{
		struct stUseHelper;
		typedef bool (*UseMsgParamT) ( Json::Value& msg, const stUseHelper& use_hlp );
		struct stUseHelper{
			int						item_id_;
			stItem::UseMsgParamT	msg_param_handler_;
			bool					is_encrypt_;
		};

		stItem(){
			itemid_ = 0;
			remains_ = 0;
			expired_ = true;
		}

		//使用
		bool Use();
		bool UseEx( const stUseHelper& use_hlp );
		bool SmartUse();
		//can be use?
		bool CanUse() const;

		const char* GetId() const;
		void SetId( const char* id );
		int GetItemId() const;
		void SetItemId( int item_id );
		int GetRemains() const;
		void SetRemains( int remains );
		bool GetExpired() const;
		void SetExpired( bool expired );
		//parse from json obj
		bool ParseFromJsonObj( const Json::Value& item_obj );

		//蓝色问号卡
		static int ItemId_BlueCard();
		//绿色问号卡
		static int ItemId_GreenCard();
		//红色问号卡
		static int ItemId_RedCard();
		//紫色问号卡
		static int ItemId_PurpleCard();
		//幸运EP卡
		static int ItemId_EpCard();
		//新的幸运EP卡
		static int ItemId_EpCardNew();
		//球员更换卡
		static int ItemId_ChangeCard();
		//自动比赛券
		static int ItemId_AutoMatchTicket();
		//蓝色球探卡
		static int ItemId_BlueBallCard();
		//绿色球探卡
		static int ItemId_GreenBallCard();
		//红色球探卡
		static int ItemId_RedBallCard();
		//13前锋球员卡
		static int ItemId_13ForwardCard();
		//13中场球员卡
		static int ItemId_13MidCard();
		//13后卫球员卡
		static int ItemId_13BackCard();
		//13门将球员卡
		static int ItemId_13GateCard();
		//世界杯32强国家对球员卡
		static int ItemId_32CountryCard();
		//橙色球探卡
		static int ItemId_OrangeCard();
		//w卡
		static int ItemId_WCard();

		//use helper function
	public:
		//指定球员包
		static bool OpenTheSpecificCard100( Json::Value& msg, const stUseHelper& use_hlp ); 
		static bool OpenTheSpecificCard200( Json::Value& msg, const stUseHelper& use_hlp ); 
		static std::pair<const stUseHelper*, int> GetSpecificUse();

		std::string					id_;			//id
		int							itemid_;		//物品ID，表示物品类型与作用	
		int							remains_;		//数量
		bool						expired_;		//是否到期
	};

	//成就
	struct Achievement{
		Achievement();
		//parse from json obj
		bool ParseFromJsonObj( const Json::Value& achievement_item );
		//是否可以领取
		bool CanBeCollect() const;
		//领取
		bool Collect();

		std::string					id_;
		int							grade_;
		bool						available_;		//是否可以领取成就
	};

	//礼包
	struct GiftPackage{
		GiftPackage();
		//parse from json obj
		bool ParseFromJsonObj( const Json::Value& achievement_item );
		//是否可以买
		bool CanBuy() const;
		//买该礼包
		bool Buy();

		int					shop_id_;
		int					product_id_;
		double				price_;
	};

	//出售项
	struct stOfferItem{
		//type definitions
		typedef std::list<stOfferItem> ContT;

		stOfferItem(){
			price_ = 0;
			spid_ = 0;
			grade_ = 0;
		}
		bool ParseFromJsonObj( const Json::Value& sold_player );
		bool ConvertToJson( Json::Value& sold_player ) const;
		//friend bool operator < ( const stOfferItem& lhs, const stOfferItem& rhs );
		friend bool operator == ( const stOfferItem& lhs, const stOfferItem& rhs );
		//购买该球员
		bool Buy( bool notify_coor ) const;
		//是否可以购买
		bool CanBuy() const;

		std::string			id_;				//拍卖的id
		double				price_;				//出售价格
		int					spid_;
		Poco::DateTime		issued_at_;			//拍卖球员的时间
		int					grade_;				//强化等级
	};
	//我的出售项
	struct stSelfOfferItem : public stOfferItem{
		//type definition
		enum enSoldState{
			kSS_Waiting = 0,			//等待出售
			kSS_Sold,					//已经卖掉
			kSS_Expired,				//过期
			kSS_Invalid,				//无效
		};
		typedef std::list<stSelfOfferItem> ContT;

		stSelfOfferItem(){
			sold_state_ = kSS_Invalid;
		}
		bool ParseFromJsonObj( const Json::Value& sold_player );
		bool ConvertToJson( Json::Value& sold_player ) const;
		//取消拍卖过期球员
		bool CancelExpired( bool notify_gplayer );
		//接受拍卖成功
		bool SoldDone( bool notify_gplayer );
		//拍卖时间太长了
		bool IsWaitSaleTimeTooLong() const;
		//取消正在拍卖的球员
		bool CancelSale_Waiting( bool notify_gplayer );

		std::string					player_id_;		//球员id
		enSoldState					sold_state_;	//拍卖状态
	};

	//正在拍卖的球员的信息
	struct stSellingPlayerInfo{
		//type definitions
		typedef std::multimap<int, stSellingPlayerInfo> ContT;

		stSellingPlayerInfo();
		bool ParseFromJsonObj( const Json::Value& selling_player );
		//购买该球员
		bool Buy() const;
		//是否可以购买
		bool CanBuy() const;
				
		std::string				id_;			//拍卖的id
		int						cnt_;			//拍卖的数据
		int						grade_;			//强化等级
		Poco::DateTime			oldest_date_;	//挂拍卖的时间
		double					price_;			//价钱
		int						spid_;
		double					price_lower_;	//最低价钱
		double					price_upper_;	//最高价钱
		double					price_default_;	//默认价钱
	};
	//球员的最新拍卖价格范围
	struct stSellingPriceRange{
		//type definitions
		typedef std::pair<int, int> KeyT;
		typedef std::map<KeyT, stSellingPriceRange> ContT;

		stSellingPriceRange();
		bool ParseFromJsonObj( const Json::Value& selling_price );
		//是否需要更新
		bool NeedUpdate() const;
		//更新
		bool Update( int spid, int grade );
		//是否在价格范围内
		bool InRange( double price ) const;
		//get/set
		double GetPriceLower() const;
		void SetPriceLower( double price_lower );
		double GetPriceUpper() const;
		void SetPriceUpper( double price_upper );
		double GetPriceDefault() const;
		void SetPriceDefault( double price_default );

		double				price_lower_;		//最低价格
		double				price_upper_;		//最高价格
		double				price_default_;		//默认价格
		//int					price_lowest_;		//未知
		int					reg_fee_;			//登记费
		Poco::DateTime		lastest_time_;		//刷新球员价格的时间
	};

	//plaer iterator
	typedef Iterator<stPlayer*> stPlayerIter;
	typedef boost::shared_ptr<stPlayerIter> SharedPlayerIter;
	typedef stPlayer::PlayerContT PlayerContT;
	typedef std::vector<stItem> ItemContT;
public:
	static GPlayer& GetMe();
	//set ep
	void SetEp( double ep );
	//get ep
	double GetEp() const;
	//get gp
	int GetGp() const;
	//set gp
	void SetGp( int gp );
	//得到所有的球员
	PlayerContT& GetAllPlayers();
	//得到所有的物品
	ItemContT& GetAllItems();
	//set bag items
	bool SetBagItems( const Json::Value& bag_items_array );
	//find item by id
	stItem* FindItemById( const char* id );
	//find item by itemid
	stItem* FindItemByItemId( int item_id );
	//get level info
	const stLevelInfo& GetLevelInfo() const;
	//set level info
	void SetLevelInfo( const stLevelInfo& level_info );
	bool SetJsonLevelInfo( const Json::Value& json_level_info );
	//得到教练名称
	const char* GetName() const;
	//设置教练名称
	void SetName( const char* name );
	//设置比赛次数上限
	void SetMatchLimitCnt( int cnt );
	int GetMatchLimitCnt() const;
	//通过ID得到球员
	stPlayer* FindPlayerById( const char* id ) const;
	//通过json对象增加一个球员
	bool AddPlayerFromJsonObj( const Json::Value& player_obj, PlayerContT& out_players_cont );
	//设置所有的球员
	bool Init_SetAllPlayers( const Json::Value& players_array );
	//比赛结束后更新球员
	bool MatchEnd_UpdatePlayers( const Json::Value& players_obj );
	//是否可以交换球员
	bool IsCanExchangePlayer() const;
	//创建可以交换的球员的迭代器
	SharedPlayerIter CreateExchangePlayerIter() const;
	void ErasePlayerById( const char* id );
	//删除两个
	void ErasePlayerById( const char* id_1, const char* id_2 );
	//是否需要自动选拔球员
	bool IsNeedAutoSelectPlayer() const;
	//通过发包自动选择球员
	bool AutoSelectPlayerBySend();
	//设置首发和替补
	bool SetFirstSecondPlayers( const Json::Value& players_array );
	//设置球员总数量
	bool SetTotalPlayersCnt( const Json::Value& json_players_cnt );
	//设置首发
	void SetFirstPlayers( const Json::Value& players_array );
	//设置替补
	void SetSecondPlayers( const Json::Value& players_array );
	//创建具有某种职位的球员的迭代器
	SharedPlayerIter CreatePlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const;
	//创建具有某种职位而且剩余比赛场数为0的球员的迭代器
	SharedPlayerIter CreateRubbishPlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const;
	//创建具有某种职位而且剩余比赛场数不为0的球员的迭代器
	SharedPlayerIter CreateNoRubbishPlayerIterByPosition( GType::enPlayerPosition player_position[], int position_cnt ) const;
	//续约指定职位的需要续约的球员
	bool ContractPlayersByPosition( GType::enPlayerPosition player_position[], int position_cnt );
	//续约首发和替补的需要续约的球员
	bool ContractFirstAndSecondPlayers();
	//发送自动赛程
	void SendAutoMatch();
	//是否有自动券
	bool HasAutoMatchTicket();
	//自动比赛券的数量
	int AutoMatchTicketCnt();
	//自动使用指定数量的自动比赛券
	void UseSomeAutoMatchTicket( int cnt );
	//一线球员的人数
	int GetFirstTeamCnt() const;
	//二线球员的人数
	int GetSecondTeamCnt() const;
	//得到侯补球员的人数
	int GetThirdPlayerCnt() const;
	//得到指定职位的人数
	int GetThePosPlayersCnt( GType::enPlayerPosition pos ) const;
	//一线球员的迭代器
	SharedPlayerIter CreateFirstTeamIter() const;
	//发送首发和替补
	bool SendFirstAndSecondPlayersInfo() const;
	bool SendFirstAndSecondPlayersSync( const PlayerContT& first_second_players );
	//把首发和替补中剩余比赛为0的给降级
	std::pair<bool, bool> DegradeFirstAndSecond();
	std::pair<bool, bool> DegradeFirstAndSecondSync();
	void LuaDegradeFirstAndSecondSync();
	//得到球员的数量
	int GetAllPlayersCnt() const;
	//是否需要解雇球员
	bool IsNeedFirePlayer() const;
	//自动解雇球员
	bool AutoFirePlayers();
	//是否选择了弱队
	bool IsSelectedSickEnemy();
	//自动选择弱队
	void AutoSelectSickEnemy();
	//设置对手信息
	void SetEnemyInfo( const Json::Value& json_enemy_info );
	//所有球员中，剩余比赛场次不为0的人数，即可参加比赛的人数
	int CanJoinedPlayersCnt() const;
	//降级所有侯补中剩余比赛为0的球员
	bool DegradeAllThirdRubbishPlayers();
	//降级所有候补中同名的球员
	bool DegradeAllThirdSamePlayers();
	//填满侯补球员
	bool FillFullThirdPlayers();
	//是否需要补充候补球员
	bool NeedSupplyThirdPlayers() const;
	//在比赛房间的时候是否超时
	bool IsTimeoutAtMatchRoom() const;
	//重置比赛房间是否超时的标志
	void ResetTimeoutAtMatchRoom();
	//得到比赛房间超时的计时器
	IoTimerPtr GetTimer_AtMatchRoom( std::size_t milli_seconds );
	//寻找指定spid的球员
	stPlayer* FindPlayerBySpid( int spid );
	//设置成就信息
	void SetAchievementInfo( const Json::Value& achievements );
	//设置礼包信息
	bool SetGiftPackageInfo( const Json::Value& json_gp );
	//买第几个GP礼包
	bool BuyGiftPackageByIdx( int idx );
	//是否可以买某个索引的礼包
	bool CanBuyGiftPackage( int idx ) const;
	//设置疲劳
	void SetMatchAddict( bool is_addict );
	//比赛界面，发送球队信息
	void SendSquadInfoAtMatch();
	//得到某个职位的第一个球员
	stPlayer* GetFirstPlayerByPosition( GType::enPlayerPosition pos );
	//拍卖球员成功
	void OnSellPlayerSucceed( const std::string& player_id );
	//得到我的拍卖列表
	stSelfOfferItem::ContT& GetSelfOfferList();
	//刷新我的拍卖列表
	void RefreshSelfOfferList();
	//是否需要刷新我的拍卖列表
	bool NeedRefreshSelfOffserList() const;
	//取消拍卖球员
	void CancelSale( const std::string& id );
	//找到一个指定id的被拍卖的球员
	stSelfOfferItem* FindSoldPlayer( const std::string& player_id );
	//发送拍卖信息
	void SendSelfSoldInfo() const;
	//生成拍卖的球员的信息
	bool GenSoldPlayersInfo( Json::Value& sold_players_info, bool against_offer_price ) const;
	//生成所有的拍卖信息
	bool GenAllSoldPlayersInfo( Json::Value& sold_players_info ) const;
	//生成其他号出售项的信息
	bool GenOtherOfferItemsInfo( Json::Value& sold_players_info ) const;
	//开卡补足指定数量的球员
	int OpenSomePlayersAs( int cnt );
	//使用一些物品
	int UseSomeItem( int cnt );
	//指定的职位中是否有同名的球员
	static bool HasSamePlayerAtPos( GType::enPlayerPosition player_position[], int position_cnt, const stPlayer& player, const PlayerContT& players );
	static bool HasSamePlayerAtPos( const stPlayer& player, const PlayerContT& players );
	//确认选择球员
	bool ChooseExchPlayer();
	//无职位球员中交换一些球员
	bool ExchPlayersInPosNo( int price_filter, int cnt );
	//接收保管箱物品
	bool UnpackAllBags();
	//主动获取教练的所有信息
	bool RefreshCoachMe();
	bool RefreshCoachMeImpl(bool force_refresh);
	//处理下架
	void HandleTimeCancelSale();

	//正在拍卖的球员相关
public:
	//得到拍卖的信息
	stSellingPlayerInfo::ContT& GetSellingPlayers();
	//得到最新价格
	stSellingPriceRange::ContT& GetSellingPrice();
	//寻找一个拍卖项
	stSellingPlayerInfo* FindSellingPlayer( const stOfferItem& offer_item );
	//得到球员价格范围
	stSellingPriceRange* FindSellingPrice( int spid, int grade );
	//更新指定球员的拍卖信息
	bool UpdateSpecifySellingPlayer( int spid, stSellingPlayerInfo::ContT& out_selling_players );
	//更新指定球员和强化等级的价格
	bool UpdateSellingPrice( int spid, int grade );
	//增加其他人的拍卖信息
	bool AddOtherOffers( const Json::Value& sold_players );
	//删除其他人的拍卖信息
	bool DeleteOtherOffers( const Json::Value& sold_players );
	//保持我的拍卖列表里有指定价格的几个球员在拍卖
	bool KeepSellingPlayerAs( double price, int cnt );
	//保持我的球员数不少于几个
	bool KeepPlayersCntMoreThan( int cnt, double price_limit );
	//保持拍卖一个垃圾球员
	bool KeepSellingRubbishPlayer( double price_min, double price_max );
	//保持拍卖一些高价值的球员
	bool KeepSellingGoodPlayers( int cnt, double price_limit );
	//购买该球员
	static bool Buy( int spid, int grade, double price );
	//是否可以购买
	static bool CanBuy( double price );
	//购买其他人的球员
	bool BuyOtherPlayers();
	//清空对方的拍卖信息
	void ClearOtherSoldInfo();
	//按默认价格拍卖球员
	bool SalePlayersAsDefault( double price_lower, double price_upper );
	//以最高价拍卖一些球员
	bool SaleSomePlayersAsMaxPrice( int player_cnt );
	//以最高价拍卖指定的球员
	bool SaleThePlayersAsMaxPrice( int player_cnt, int player_id );
	//以最低价拍卖一些球员
	bool SaleSomePlayersAsMinPrice( int player_cnt );
	//以最低价拍卖指定的球员
	bool SaleThePlayersAsMinPrice( int player_cnt, int player_id );
	//以指定的价格拍卖一些指定的球员
	bool SaleThePlayersAsPrice_Cnt( int player_id, int player_cnt , double price );
	//以指定的价格拍卖指定的球员
	bool SaleThePlayersSpecific(int player_id, int grade_min, int grade_max, int player_cnt, double price);
	//以指定的价格买一个指定的球员
	bool BuyThePlayerAsPrice( int player_id, int grade, double price_lower, double price_upper );
	//是第一次开始计时双点
	bool IsFirstCalcDoubleTime() const;
	//到了双点
	bool IsAtDoubleTime() const;
	//更新双点计时
	void UpdateDoubleTime();
	//到了指定的时间间隔
	bool IsAtTheTimeSpan( int time_minutes );
	//到了截卡指定的时间间隔
	bool IsJieCardTheTimeSpan( int time_minutes );
	bool IsJieCardTheTime_Sec( int time_seconds );
	//对指定的球员进行截卡
	bool JieCardThePlayer( int player_id, int grade, double price );
	//强化指定的球员
	void EnhanceThePlayer(int player_id, int dst_grade_min, int dst_grade_max, int src_grade_min, int src_grade_max);
	//指定球员的数量
	int GetThePlayerCnt(int player_id, int grade_min, int grade_max);

	//能力值相关
public:
	//拍卖指定的球员
	void SaleSpecifyPlayers( int ability, double price );
	//大于等于这个能力值的球员不会被交换
	int GetAbilityFilter() const;
	//设置
	void SetAbilityFilter( int ability_filter );
	//大于等于这个价值的球员不会被交换
	double GetExchPriceFilter() const;
	//设置不会被交换的球员价值
	void SetExchPriceFilter( double price_filter );
	//高价值提示
	double GetHighPriceFilter() const;
	void SetHighPriceFilter( double price_filter );
	//对方阵营不会接收到拍卖信息的价格范围
	double GetAgainstOfferPrice() const;
	void SetAgainstOfferPrice( double price );
	//得到等待拍卖球员的超时时间
	int GetTimeWaitSalePlayer() const;
	//设置等待拍卖球员的时间，超时则下架
	void SetTimeWaitSalePlayer( int time_hour );

	//创建教练相关
public:
	//得到创建教练子状态
	GType::enCreateCoach_SubState GetCC_SubState() const;
	//设置创建教练子状态
	void SetCC_SubState( GType::enCreateCoach_SubState cc_state );
	//是否是创建教练流程
	bool IsCreateCoachProcedure() const;
	//随机生成教练名字
	std::string GenCoachName();
	//是否创建教练成功
	bool IsCreateCoachSucceed() const;
	//设置创建教练成功
	void SetCreateCoachSucceed( bool is_succeed );

	//请求标志相关
public:
	//是否正在请求礼包数据
	bool IsGettingGpData() const;
	//设置请求礼包的标志
	void SetGettingGpDataTag( bool is_getting );

	//测试相关
public:
	//得到一个可以被拍卖的球员
	stPlayer* GetCanBeSoldPlayer();
	void Test();

private:
	//设置所有的一线球员为侯补
	void ResetFirstTeamPlayersToThird();
	//组合球队信息
	bool CombineSquadInfo( Json::Value& squad_info ) const;
	//购买一定的球员
	int BuySomeOtherPlayers( int cnt, double price_limit );

private:
	struct stPlayersNormalIter : public GPlayer::stPlayerIter{
		template<typename T>
		stPlayersNormalIter( const T& filter_fn ) : filter_fn_(filter_fn){
			it_ = GPlayer::GetMe().GetAllPlayers().begin();
			NextValid();
		}
		void First() override;
		void Next() override;
		bool IsDone() const override;
		value_type CurItem() const override;

	private:
		void NextValid();

	private:
		GPlayer::PlayerContT::iterator							it_;
		boost::function<bool( const GPlayer::stPlayer& )>		filter_fn_;
	};

private:
	double					ep_;
	stLevelInfo				level_info_;					//等级信息
	std::string				name_;							//教练名称
	PlayerContT				players_;						//所有球员
	ItemContT				items_;							//背包物品
	Json::Value				enemy_info_;					//对手信息
	int						select_enemy_cnt_;				//选择对手信息次数
	bool					is_timeout_at_match_room_;		//在比赛房间是否超时
	IoTimerPtr				timer_at_match_room_;			//比赛房间中的计时
	GType::enCreateCoach_SubState		cc_state_;			//创建教练子状态
	bool					create_coach_succeed_;			//是否创建教练成功
	int						ability_filter_;				//大于等于这个能力值的球员不会被交换
	int						gp_;
	std::vector<GiftPackage>	gift_packages_;				//GP礼包
	bool					request_gp_data_;			//是否正在请求获取GP礼包
	int						max_players_cnt_;

private:
	stSelfOfferItem::ContT	self_offer_list_;			//我的拍卖列表
	stOfferItem::ContT		others_offers_;				//其他人的拍卖项
	stSellingPlayerInfo::ContT	selling_players_;		//拍卖球员信息
	stSellingPriceRange::ContT	selling_price_;			//拍卖价格
	Poco::DateTime				time_refresh_offer_list_;	//刷新我的球员列表计时
	Poco::DateTime				time_cancel_sale_wait_players_;		//下架计时

private:
	double					exchange_price_filter_;				//设置不会被交换的最低价格
	double					high_price_filter_;					//高价值提示
	double					against_offer_price_;				//对方阵营不会接收到拍卖信息的价格
	int						error_sick_enemy_cnt_;				//弱队次数
	Poco::DateTime			time_refresh_coach_me_;				//允许主动刷新教练所有信息的计时时间
	int						hour_wait_sale_player_;				//等待拍卖的小时数
	int						match_cnt_limit_;					//比赛次数上限了就换号
	boost::optional<DateTimeLocal>	double_time_;				//是否是双点
	boost::optional<DateTimeLocal>	time_back_select_coach_;	//返回到选择教练的时间
	boost::optional<DateTimeLocal>	time_jie_card_;				//截卡计时
};