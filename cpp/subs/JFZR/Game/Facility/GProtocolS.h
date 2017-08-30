#pragma once
/*
@author zhp
@date 2014/7/2 19:04
@purpose protocol of game for send/write
*/
#include "GTypes.h"
#include "GameArray.h"
#include "GProtocolBase.h"

static_assert(sizeof(CMD_HeaderBase) + sizeof(WORD) == sizeof(PACKET_HEADER), "");
struct CMDS_MsgBase : stBStreamOutput{
	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const CMDS_MsgBase& msg );
protected:
	WORD CalcUriHash() const{
		using namespace GFuncCall;
		LOG_O(Log_trace) << "发包,type name->" << GetTypeInfo().name();
		auto uri_hash = CalcHash_WORD(GetUriStr());
		return uri_hash;
	}
	virtual const wchar_t* const GetUriStr() const{
		assert(false);
		return nullptr;
	}
	virtual const std::type_info& GetTypeInfo() const{
		assert(false);
		return typeid(*this);
	};
};

#define CMDS_MsgBase_OUTPUT(derived_t)	friend StrOutputBStream& operator << ( StrOutputBStream& ostm, const derived_t& msg ){	\
	return ostm << static_cast<const CMDS_MsgBase&>(msg);	\
}	\
	const std::type_info& GetTypeInfo() const{		\
	return typeid(derived_t);	\
}	\
/**/
#define CMDS_GET_URI_STR( str ) const wchar_t* const GetUriStr() const{		\
	return str;	\
}	\
/**/

//拾取物品
struct CMDS_Msg_PickUpItem : CMDS_MsgBase{
	DWORD	item_serial_;			//物品序号

	CMDS_Msg_PickUpItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_PickUpItem)
	CMDS_GET_URI_STR(L"76883bc7e7b32d16bdf424a2071c1ee8")
};

//使用技能
struct CMDS_Msg_SkillHit : CMDS_MsgBase{
	//被攻击者项
	struct HittedItem : CMDS_MsgBase{
		DWORD				projectile_serial_;			//抛射者的序号
		DWORD				defender_serial_;			//防御者序号
		DWORD				attacker_serial_;			//攻击者序号
		GType::enHitType	hit_type_;					//HitType
		int					delta_hp_;					//DeltaHp
		int					random_seed_;				//RandomSeed
		float				attack_power_rate_;			//AttackPowerRate,攻击比率
		float				damage_plus_rate_to_super_armor_;
		float				damage_plua_rate_for_counter_;
		int					event_option_serial_;
		int					event_option_damage_reduce_rate_;
		bool				is_counter_attack_;
		bool				is_hit_super_armor_;
		DWORD				projectile_hash_;			//抛射hash，其实是技能ID，也是技能名称的hash值
		BYTE				first_hit_;					//FirstHit
		DWORD				action_hash_;				//行动hash和技能的行动hash值一样,有些是不一样的
		DWORD				skill_hash_;
		DWORD				skill_level_;
		float				projectile_scale_;
		float				float_unkown1_;
		float				float_unkown2_;
		DWORD				timestamp_;

		HittedItem();
		void AcceptParams( DWORD defender_serial, DWORD projectile_serial, DWORD attacker_serial,
			DWORD projectile_hash, DWORD action_hash, DWORD skill_hash, DWORD skill_level, DWORD time_stamp, float attack_power_rate, int delta_hp );
		virtual void Write( StrOutputBStream& ostm ) const;
		B_STREAM_OUTPUT(HittedItem)
	};

	GameArrayT<HittedItem>				hitted_items_;

	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SkillHit)
	CMDS_GET_URI_STR(L"c492f0d9b91babcb562db514f3a9a86b")
	static DWORD GetSkillTimestamp();
};

//副本中，步行到、移动到
struct CMDS_Msg_FbWalk : CMDS_MsgBase{
	CMDS_Msg_FbWalk();
	//站立
	void AcceptStand( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos, float pre_action_time, DWORD action_serial, DWORD rand_seed, float client_absolute_time );
	//快速移动
	void AcceptFastRun( DWORD role_serial, const std::wstring& job_name, 
		const GType::stGameVector& action_start_position, const GType::stGameVector& direction, float pre_action_time, DWORD action_serial, DWORD rand_seed, float client_absolute_time );
	void AcceptRealize( DWORD role_serial, const std::wstring& job_name, 
		const GType::stGameVector& action_start_position, float pre_action_time, DWORD action_serial, DWORD pre_action_serial, DWORD rand_seed, float client_absolute_time );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_FbWalk)
	CMDS_GET_URI_STR(L"038f2071d608b0d6942d097326ccbb27")

private:
	void AcceptParams( DWORD role_serial, const std::wstring& job_name, const std::wstring& cmd_name, 
		const GType::stGameVector& action_start_position, const GType::stGameVector& direction, bool skip_validation );

	DWORD					role_serial_;
	std::wstring			role_job_name_;
	std::wstring			cmd_name_;
	GType::stGameVector		action_start_position_;
	GType::stGameVector		direction_;
	bool					skip_validation_;
	GType::stDirection2d	direction2d_;
	GType::stDirection2d	moving_direction_;
	float					pre_action_play_time_;
	DWORD					action_serial_;
	DWORD					rand_seed_;
	float					client_absolute_time_;
	bool					exist_params_;
	DWORD					pre_action_serial_;
};

//城镇中，步行到、移动到
struct CMDS_Msg_VillageWalk : CMDS_MsgBase{
	CMDS_Msg_VillageWalk();
	//fast run
	void AcceptFastRun( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos, const GType::stGameVector& direction );
	//站立
	void AcceptStand( DWORD role_serial, const std::wstring& job_name, const GType::stGameVector& pos );

private:
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_VillageWalk)
	CMDS_GET_URI_STR(L"982d94fcd2d7650a650a2644ff41d9ac")

private:
	DWORD					role_serial_;
	std::wstring			role_job_name_;
	const std::wstring*		cmd_name_;
	GType::stGameVector		action_start_position_;
	GType::stDirection2d	direction2d_;
	GType::stDirection2d	moving_direction_;
	GType::stGameVector		direction_;
};

//进入副本
struct CMDS_Msg_EnterCloneMap : CMDS_MsgBase{
	CMDS_Msg_EnterCloneMap();
	void AcceptEnter( const std::wstring& clonemap_id, GType::enFbDifficulty difficulty );
	void AcceptRestart( const std::wstring& clonemap_id, GType::enFbDifficulty difficulty );
	void AcceptChangeJob( const std::wstring& clonemap_id );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_EnterCloneMap)
	CMDS_GET_URI_STR(L"dd246d8973092d9a60c0bddb6f8f39b0")

	std::wstring			clone_map_name_;
	GType::enFbDifficulty	difficulty_;
	std::wstring			wrap_gate_id_;
	bool					is_first_player_;
	bool					is_restart_;
};

//步入传送门
struct CMDS_Msg_StepInWarpGate : CMDS_MsgBase{
	CMDS_Msg_StepInWarpGate( const std::wstring& role_name, const std::wstring& gate_name );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_StepInWarpGate)
	CMDS_GET_URI_STR(L"6490af8101d3c41132b3366b2a46a72d")

	const std::wstring&				role_name_;
	const std::wstring&				gate_name_;
};

//告诉服务器，当前角色坐标信息
struct CMDS_Msg_DirectionSyncPacket : CMDS_MsgBase{
	CMDS_Msg_DirectionSyncPacket( DWORD role_serial, const GType::stGameVector& role_pos );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_DirectionSyncPacket)
	CMDS_GET_URI_STR(L"e1ddc887f465dd79053399f012a3cac9")

	DWORD					role_serial_;
	GType::stGameVector		role_pos_;
	GType::stDirection2d	direction_;
	GType::stDirection2d	moving_direction_;
};

//告诉服务器选择改变房间
struct CMDS_Msg_ChangeSector : CMDS_MsgBase{
	CMDS_Msg_ChangeSector( const std::wstring& gate_name );
	void AcceptParam( DWORD valid_data_x, DWORD valid_data_y, bool is_dummy_exist, float attack_speed, float move_speed, float evade_rating );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_ChangeSector)
	CMDS_GET_URI_STR(L"51d107379e1955235e4e5dc41dd2e391")

	const std::wstring&				gate_name_;
	DWORD							valid_data_x_;
	DWORD							valid_data_y_;
	bool							is_dummy_exist_;
	float							attack_speed_;
	float							move_speed_;
	float							evade_rating_;
};

//返回城镇
struct CMDS_Msg_ReturnCity : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_ReturnCity)
	CMDS_GET_URI_STR(L"85ac893b6ce6de3a6dee371c56807d8e")
};

//再次挑战副本
struct CMDS_Msg_RestartStage : CMDS_MsgBase{
	CMDS_Msg_RestartStage();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RestartStage)
	CMDS_GET_URI_STR(L"dd246d8973092d9a60c0bddb6f8f39b0")

	std::wstring			clone_map_name_;
	GType::enFbDifficulty	difficulty_;
};

//修理所有已穿装备
struct CMDS_Msg_RepairWearEquip : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RepairWearEquip)
	CMDS_GET_URI_STR(L"a1cf156de551bb58d0271c523b5ad578")
};

//穿戴装备
struct CMDS_Msg_EquipItem : CMDS_MsgBase{
	CMDS_Msg_EquipItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_EquipItem)
	CMDS_GET_URI_STR(L"cebca38adfe1a51de79f579873e41133")

	GType::enEquipPart			equip_part_;			//装备部位
	WORD						inven_slot_;			//背包槽
};

//脱掉装备
struct CMDS_Msg_UnEquipItem : CMDS_MsgBase{
	CMDS_Msg_UnEquipItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_UnEquipItem)
	CMDS_GET_URI_STR(L"d5b24ed93d2a59535d3291aca7173c8e")

	GType::enEquipPart			equip_part_;			//装备部位
	WORD						inven_slot_;			//背包槽
};

//使用物品用于另一物品
struct CMDS_Msg_UseForItem : CMDS_MsgBase{
	CMDS_Msg_UseForItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_UseForItem)
	CMDS_GET_URI_STR(L"fcf22fe368eaea7f7a2433ab1fea257a")

	WORD						using_slot_;			//使用的东西位置
	WORD						target_slot_;			//目标物品位置
};

//打开箱子
struct CMDS_Msg_OpenItemPackage : CMDS_MsgBase{
	CMDS_Msg_OpenItemPackage();
	void AcceptSingleUse( WORD main_slot );
	void AcceptRequireUse( WORD main_slot, WORD require_slot );
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_OpenItemPackage)
	CMDS_GET_URI_STR(L"415c2a704e50d150c6a26919d9c32405")

	WORD						main_slot_;			//使用的东西位置
	int							require_tab_;
	WORD						require_slot_;		//辅助物品位置
};

//使用消耗品
struct CMDS_Msg_UseItem : CMDS_MsgBase{
	CMDS_Msg_UseItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_UseItem)
	CMDS_GET_URI_STR(L"3ed486eb9fcc46121a77b379e97f3b61")

	WORD						item_slot_;			//使用的东西位置
};

//改变城镇/村庄，从一个村庄到另一个村庄
struct CMDS_Msg_ChangeVillage : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_ChangeVillage)
	CMDS_GET_URI_STR(L"8700c48e9de9aa244ec92d34c498fe78")

	std::wstring			village_gate_id_;		//村庄门id
};

//卖物品
struct CMDS_Msg_SellItem : CMDS_MsgBase{
	CMDS_Msg_SellItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SellItem)
	CMDS_GET_URI_STR(L"2f6401c12fc6a415ea86fada78c3617c")

	std::wstring						fixed_npc_name_;
	GType::enTabCategory				tab_category_;			//目录种类
	int									slot_index_;
	DWORD								item_cnt_;				//物品数量(可迭加物品才有数量)
};

//购买物品
struct CMDS_Msg_BuyItem : CMDS_MsgBase{
	CMDS_Msg_BuyItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_BuyItem)
	CMDS_GET_URI_STR(L"35bf1b03b248e6642008740c7cca0f01")

	std::wstring				fixed_npc_name_;
	WORD						item_id_;
	WORD						item_cnt_;				//物品数量(可迭加物品才有数量)
};

//学习技能
struct CMDS_Msg_LearnSkill : CMDS_MsgBase{
	CMDS_Msg_LearnSkill();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_LearnSkill)
	CMDS_GET_URI_STR(L"a31bfe5025bd74f8cfd945c56fb74251")

	std::wstring				skill_name_;
	DWORD						skill_lvl_;				//技能等级
};

//创建角色
struct CMDS_Msg_CreatePlayer : CMDS_MsgBase{
	CMDS_Msg_CreatePlayer();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_CreatePlayer)
	CMDS_GET_URI_STR(L"cc4c3e9a5e0b08ccae365cba61f5e5c3")

	struct stPlayerFeature : CMDS_MsgBase{
		stPlayerFeature( WORD feature_uri, const std::wstring& body_part_name,
			BYTE selected_mesh, BYTE selected_part1, BYTE selected_part2 );
		virtual void Write( StrOutputBStream& ostm ) const;
		B_STREAM_OUTPUT(stPlayerFeature)

		WORD					hash_feature_uri_;
		const std::wstring&		body_part_name_;				//身体部位
		BYTE					selected_mesh_info_index_;		//SelectedMeshInfoIndex
		BYTE					selected_part1_color_index_;	//SelectedPart1ColorIndex
		BYTE					selected_part2_color_index_;	//SelectedPart2ColorIndex
	};
	struct stPlayerFeatureHlp{
		stPlayerFeatureHlp();
		GameArrayT<stPlayerFeature>			player_features_;
	};

	std::wstring					role_name_;
	GType::enJobCategory			job_category_;

private:
	static const stPlayerFeatureHlp	player_feature_hlp_;
	DWORD							selected_skin_color_index_;	//SelectedSkinColorIndex
};

//选择角色
struct CMDS_Msg_SelectPlayer : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SelectPlayer)
	CMDS_GET_URI_STR(L"5cefcbe0615c112023bbcca2d1cbd8bc")

	std::wstring					role_name_;
};

//请求切换频道
struct CMDS_Msg_RequestSwitchChannel : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RequestSwitchChannel)
	CMDS_GET_URI_STR(L"8bedfbed6e9106a8a208e391c60cfeb3")

	std::wstring				channel_name_;		//频道名
};

//切换频道
struct CMDS_Msg_SwitchChannel : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SwitchChannel)
	CMDS_GET_URI_STR(L"827d94dc425544c8bf32122e7d3f0f84")

	std::wstring			channel_name_;		//频道名称
	std::wstring			account_id_;		//帐号/qq帐号
	std::wstring			session_key_;		//KEY
};

//发送邮件
struct CMDS_Msg_SendMail : CMDS_MsgBase{
	CMDS_Msg_SendMail();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SendMail)
	CMDS_GET_URI_STR(L"5fc62cc2d7a859c3e039eb81b3235d3c")

	struct stMailItems : CMDS_MsgBase{
		stMailItems( const std::wstring& tab_name, int item_slot_index, int item_cnt );
		virtual void Write( StrOutputBStream& ostm ) const;
		B_STREAM_OUTPUT(stMailItems)

		const std::wstring&			tab_name_;			//栏目名称
		int							item_slot_index_;
		int							item_cnt_;
	};

	std::wstring			receiver_role_name_;
	std::wstring			mail_title_;			//邮件标题
	std::wstring			mail_content_;			//邮件内容
	DWORD					gold_;					//邮寄的金币
	GameArrayT<stMailItems>	items_;
};

//接收附件
struct CMDS_Msg_GetAttachments : CMDS_MsgBase{
	CMDS_Msg_GetAttachments();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_GetAttachments)
	CMDS_GET_URI_STR(L"f8260de7249e4d60ab7ee464e5f499e1")

	LONGLONG			mail_serial_;
};

//删除邮件
struct CMDS_Msg_RemoveMail : CMDS_MsgBase{
	CMDS_Msg_RemoveMail();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RemoveMail)
	CMDS_GET_URI_STR(L"246578b8220355a2c542e01995fe3eff")

	LONGLONG			mail_serial_;
};

//跳过新建1级新号的故事动画
struct CMDS_Msg_SkipStoryAnimate : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SkipStoryAnimate)
	CMDS_GET_URI_STR(L"e56a7bfbcf0df1471b9c59d6b0444703")
};

//接受任务
struct CMDS_Msg_AcceptTask : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_AcceptTask)
	CMDS_GET_URI_STR(L"6c32a7184cafa0cfd343ecc1a6ea3a8d")

	std::wstring			quest_id_;			//任务id
};

//完成任务
struct CMDS_Msg_CompleteTask : CMDS_MsgBase{
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_CompleteTask)
	CMDS_GET_URI_STR(L"7824a9c8b4fc5eff77a1a20c43fe6135")

	std::wstring			quest_id_;			//任务id
	std::wstring			reward_item_id_;	//奖励的物品id
};

//分解物品
struct CMDS_Msg_DisassemblyItem : CMDS_MsgBase{
	CMDS_Msg_DisassemblyItem();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_DisassemblyItem)
	CMDS_GET_URI_STR(L"920f9bdd034c400bd59dc8c770e73d5a")

	GType::enTabCategory	tab_category_;
	int						slot_index_;
};

//副本BOSS房间选择奖励
struct CMDS_Msg_StageReward : CMDS_MsgBase{
	CMDS_Msg_StageReward();
	virtual void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_StageReward)
	CMDS_GET_URI_STR(L"a8d7b003267a116610630b3c583ee7b3")

	int				reward_index;
};

//连击
struct CMDS_Msg_SkillActivity : CMDS_MsgBase{
	CMDS_Msg_SkillActivity();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_SkillActivity)
	CMDS_GET_URI_STR(L"1127a4bae67312f0d7a2fd8ffdf915dd")

	int			activity_count_;			//连击次数
};

//购买拍卖物品
struct CMDS_Msg_BuyProduct : CMDS_MsgBase{
	CMDS_Msg_BuyProduct();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_BuyProduct)
	CMDS_GET_URI_STR(L"9a6e248c23b03341309edf0af3d02c93")

	LONGLONG		product_market_oid_;
	int				item_cnt_;
};

//挂物品到拍卖
struct CMDS_Msg_RegisterProduct : CMDS_MsgBase{
	CMDS_Msg_RegisterProduct();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RegisterProduct)
	CMDS_GET_URI_STR(L"c9c08a120d4964bd79fe1ee858d6a665")

	std::wstring			item_cn_name_;
	GType::enTabCategory	tab_category_;
	int						slot_index_;
	LONGLONG				price_;
	int						period_;		//单位->天数
	int						item_cnt_;		//物品数量
};

//查询我的拍卖列表
struct CMDS_Msg_RequestMyProductList : CMDS_MsgBase{
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RequestMyProductList)
	CMDS_GET_URI_STR(L"7fe1f4a833cc612bbef1a9e14aecdc2c")
};

//查询物品的拍卖信息
struct CMDS_Msg_RequestProductInfo : CMDS_MsgBase{
	CMDS_Msg_RequestProductInfo();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RequestProductInfo)
	CMDS_GET_URI_STR(L"71cc57ea3c00703152f84cc428ae97ef")

	GType::enTabCategory	tab_category_;
	int						slot_index_;
};

//查询物品拍卖列表
struct CMDS_Msg_RequestProductList : CMDS_MsgBase{
	CMDS_Msg_RequestProductList();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_RequestProductList)
	CMDS_GET_URI_STR(L"f93c6af2752f096fd94bde35d0422520")

	int				page_num_;
	std::wstring	market_category_;
	std::wstring	item_ch_name_;
};

//销毁对象
struct CMDS_Msg_DestroyActors : CMDS_MsgBase{
	CMDS_Msg_DestroyActors();
	void Write( StrOutputBStream& ostm ) const;
	CMDS_MsgBase_OUTPUT(CMDS_Msg_DestroyActors)
	CMDS_GET_URI_STR(L"99a17b89ddaa7cddb22110d75f526fa0")

	DWORD		npc_serial_;
};
