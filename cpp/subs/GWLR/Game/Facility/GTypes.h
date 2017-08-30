#pragma once
/*
@author zhp
@date 2014-1-04
@purpose 逆向分析出的游戏中的一些类型、结构等
*/
#include "stdafx.h"
#include <CmnMix/Cmn_BinaryStream.h>
#include <CD_CommonFiles/Data/CD_Structs.h>

namespace GType{
	class GameSendSocket;

	enum enHitType{
		kHT_Unknown = 1,
	};
	struct stDirection2d : stCD_Direction2{
		stDirection2d(){
			dir_x = 0;
			dir_y = 0;
		}
		stDirection2d( float x, float y ){
			dir_x = x;
			dir_y = y;
		}
		stDirection2d& operator = (const stCD_Direction2& rhs){
			*(stCD_Direction2*)this = rhs;
			return *this;
		}
		float GetDirX() const{
			return dir_x;
		}
		void SetDirX( float x ){
			dir_x = x;
		}
		float GetDirY() const{
			return dir_y;
		}
		void SetDirY( float y ){
			dir_y = y;
		}
		friend StrOutputBStream&
			operator << ( StrOutputBStream& ostm, const stDirection2d& dir ){
				ostm << dir.dir_x << dir.dir_y;
				return ostm;
		}
	};
	//游戏中的向量
	struct stGameVector3 : stCD_Vector3{
		stGameVector3(){
			fx = 0.;
			fy = 0.;
			fz = 0.;
		}
		stGameVector3( float x, float y, float z ){
			fx = x;
			fy = y;
			fz = z;
		}
		stGameVector3( double x, double y, double z ){
			fx = (float)x;
			fy = (float)y;
			fz = (float)z;
		}
		stGameVector3& operator = (const stCD_Vector3& rhs){
			*(stCD_Vector3*)this = rhs;
			return *this;
		}
		stGameVector3(const stCD_Vector3& rhs){
			*(stCD_Vector3*)this = rhs;
		}
		float GetPosX() const{
			return fx;
		}
		void SetPosX( float x ){
			fx = x;
		}
		float GetPosY() const{
			return fy;
		}
		void SetPosY( float y ){
			fy = y;
		}
		float GetPosZ() const{
			return fz;
		}
		void SetPosZ( float z ){
			fz = z;
		}
		void ConvertToPos(const stCD_PosDirInfo& actor_vec);
		void FromDirection(const stCD_Direction2& direction){
			fx = direction.dir_x;
			fy = direction.dir_y;
			fz = 0.;
		}
		void SetXYZ( float x, float y, float z );
		double Distance3d( const stGameVector3& rhs ) const;
		double Distance3d_ZoomInZ( const stGameVector3& rhs, int zoom_in_z_rate ) const;
		double Distance2d( const stGameVector3& rhs ) const;
		float GetAngleXY() const;
		//沿原点逆时针旋转
		void RotateXY_Origin( float angle );
		//xy的向量积
		float CrossProductXY( const stGameVector3& rhs ) const;
		//xy的模
		float ModuleXY() const;
		//向量的夹角
		float IncludedAngleXY( const stGameVector3& rhs ) const;
		void PlusXY( const stGameVector3& rhs );
		bool OriginDistLessThan( const stGameVector3& rhs ) const;
		bool ValueEqual( const stGameVector3& rhs ) const;
		bool FloatAllEqual( const stGameVector3& rhs ) const;
		bool IsXYAllEqual( const stGameVector3& rhs ) const;
		friend stGameVector3 operator + ( const stGameVector3& lhs, const stGameVector3& rhs ){
			stGameVector3 pos(lhs);
			pos.fx += rhs.fx;
			pos.fy += rhs.fy;
			pos.fz += rhs.fz;
			return pos;
		}
		friend stGameVector3 operator - ( const stGameVector3& lhs, const stGameVector3& rhs ){
			stGameVector3 pos(lhs);
			pos.fx -= rhs.fx;
			pos.fy -= rhs.fy;
			pos.fz -= rhs.fz;
			return pos;
		}
		friend StrOutputBStream&
			operator << ( StrOutputBStream& ostm, const stGameVector3& vec ){
				ostm << vec.fx << vec.fy << vec.fz;
				return ostm;
		}
		friend std::ostream&
			operator << ( std::ostream& ostm, const stGameVector3& vec ){
				ostm << "（" << vec.fx << ", " << vec.fy << ", " << vec.fz << "）";
				return ostm;
		}
	};
	struct G_PosDirInfo : stCD_PosDirInfo{
		G_PosDirInfo(const stCD_PosDirInfo& rhs){
			*(stCD_PosDirInfo*)this = rhs;
		}
	};
	const stGameVector3 kVecEmpty;
	//装备部位索引
	enum enEquipPartIndex{
		kEPI_Weapon = 0,		//武器
		kEPI_Head,			//头部
		kEPI_Hand,			//手部
		kEPI_Heart,			//胸部
		kEPI_Pocket,			//腰部
		kEPI_Foot,			//足部
		kEPI_Ring,			//戒指
		kEPI_Necklace,		//项链
		kEPI_HuShi = 9,		//护石
		kEPI_Invalid,
	};
	
	enum enEquipPartSubType_Defense{
		kEPSTDEF_Head = 1,
		kEPSTDEF_Hand,
		kEPSTDEF_Heart,
		kEPSTDEF_Pocket,
		kEPSTDEF_Foot,
		kEPSTDEF_Invalid,
	};
	enum enEquipPartSubType_Decorate{
		kEPSTDEC_Ring = 1,
		kEPSTDEC_Necklace,
		kEPSTDEC_HuShi = 4,
		kEPSTDEC_Invalid,
	};

	//职业类型
	enum enJobCategory{
		kJC_JianDouShi = 1,		//剑斗士,WARRIOR
		kJC_RenWuZhe = 2,		//刃武者,THIEF
		kJC_ZhanKuang,			//斩狂,GLADIBERSERKER
		kJC_QuanSha,			//拳刹,GIGONGSA
		kJC_FengWuXing,			//风舞星,RIPPER
		kJC_LieRenXing,			//猎刃星,ASSASSIN
		kJC_TianLangXing,		//天狼星,MISTRESSOFFENRIR
		kJC_JianMo,				//剑魔,DOOMBLADE
		kJC_KongMoShi = 9,		//控魔师,MAGICGUNNER
		kJC_MoYingShi,			//魔影师,DARKSTALKER
		kJC_ShiKongShuShi,		//时空术士,ARCHITECT
		kJC_ShuangYuZhe,		//霜语者,ICEMAGICIAN
		//觉醒后职业
		kJC_LuanZhan = 13,		//乱战,F_SCYTHE_00
		kJC_XiXueYaoHua,		//吸血妖化,F_SCYTHE_01
		kJC_XieHunShuShi,		//邪魂术士,F_SCYTHE_02
		kJC_KuLuoSha,			//苦罗刹,F_SCYTHE_03
		kJC_Any,				//任意职业，起到占位符的作用
		kJC_Invalid,
	};

	//任务类别
	enum enTaskCategory{
		kTC_InvalidBegin,
		kTC_Normal_KillMonster = 4,	//普通任务之副本杀怪任务
		kTC_Main,					//主线任务
		kTC_CollectMaterial,		//普通任务之收集材料任务
		kTC_InvalidEnd,
	};
	//游戏状态
	enum enGameState{
		kGS_Invalid = 0,
		kGS_SelectRole,
		kGS_InGame,			//角色已进入游戏
	};
	//UI场景
	enum enUiState{
		kUS_StartGame = 0,		//开始游戏按钮
		kUS_OldPlayer,			//我是老猎人
		kUS_RandomSkin,			//随机皮肤和脸型
		kUS_CreateRoleOk,		//创建完成按钮
		kUS_CreateRoleBt6,		//第六个创建新角色按钮
		kUS_Invalid,
	};
	//游戏中的物品品质，数值与游戏中的一致
	enum enItemQualityOfGame{
		kIQOG_Low = 1,
		kIQOG_Middle = 1 << 1,
		kIQOG_High = 1 << 2,
		kIQOG_Rare = 1 << 3,
		kIQOG_Legendary = 1 << 4,
	};
	//物品使用类别
	enum enItemUsableType{
		kIUT_Identify = 0,		//鉴定书
		kIUT_Unseal,			//解封石
		kIUT_Invalid,
	};
	//////////////////////////////////////////////////////////////////////////
	class GameSkillType;

	/*enum enShopInfoCategory{
		kSIC_LieShouBuJiPin = MMOCLIENT_SHOP_INFO_LIE_SHOU_BU_JI_PIN,				//狩猎补给品
		kSIC_XinShouLieShouEquip = MMOCLIENT_SHOP_INFO_XIN_SHOU_LIE_SHOU_EQUIP,		//新手猎守装备
		kSIC_DanYaoBuJi = MMOCLIENT_SHOP_INFO_DAN_YAO_BU_JI,						//弹药补给
		kSIC_GongJianPingTeMai = MMOCLIENT_SHOP_INFO_GONG_JIAN_PING_TE_MAI,			//弓箭瓶特卖
		kSIC_EquipRuMenXuKe = MMOCLIENT_SHOP_INFO_RU_MEN_XU_KE,						//猎守武器入门许可
		kSIC_EquipSkill = MMOCLIENT_SHOP_INFO_EQUIP_SKILL,							//武器技能
		kSIC_LiWei = MMOCLIENT_SHOP_INFO_LI_WEI,									//神秘的商人
		kSIC_AiLuMaoGuYong = MMOCLIENT_SHOP_INFO_AI_LU_MAO_GU_YONG,					//艾路猫雇佣
		kSIC_AiLuNongYeTeGong = MMOCLIENT_SHOP_INFO_AI_LU_NONG_YE_TE_GONG,			//艾露农业特供
		kSIC_ZhuangYuanBiXuPin = MMOCLIENT_SHOP_INFO_ZHUANG_YUAN_BI_XU_PIN,			//庄园必须品
		kSIC_TeChanShangPin = MMOCLIENT_SHOP_INFO_TE_CAN_SHANG_DIAN,				//特产商品
	};*/

	enum enPlayerLineType{
		kPLT_Smooth = 1,		//流畅
		kPLT_Fine = 2,			//良好
		kPLT_Yellow = 3,		//黄色繁忙
		kPLT_Red = 4,			//红色
		kPLT_Unknown = 5,		//未知
	};

	//公会委托等级
	enum enSociatyTaskLvl{
		kSTL_Invalid = 0,
		kSTL_Lvl1 = 0x64,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL1___,
		kSTL_Lvl2 = 0x65,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL2___,
		kSTL_Lvl3 = 0x66,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL3___,
		kSTL_Lvl4 = 0x67,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL4___,
		kSTL_Lvl5 = 0x67,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL5___,
		kSTL_Lvl6 = 0x67,	//MMOCLIENT_REVERSAL_ACCEPT_GONG_HUI_FB_LEVEL6___,
	};
	struct stSociatyTaskLvlMap{
		const char*			task_lvl_name_;
		enSociatyTaskLvl	task_lvl_;
	};
	const stSociatyTaskLvlMap g_sociaty_task_name2lvl[] = {
		{ "委托*", kSTL_Lvl1 },
		{ "委托**", kSTL_Lvl2 },
		{ "委托***", kSTL_Lvl3 },
		{ "委托****", kSTL_Lvl4 },
		{ "委托*****", kSTL_Lvl5 },
		{ "委托******", kSTL_Lvl6 },
	};

	struct stFbTypeKeyMap{
		CD_En_FbType	fb_type_;
		DWORD			fb_key_flag_;
	};
	const stFbTypeKeyMap g_fb_type_key_map[] = {
		{ CD_En_FbType::kFT_Sociaty, 0x0000985E/*MMOCLIENT_REVERSAL_PACKET_MSG_ID_GONG_HUI_KEY___*/ },
		{ CD_En_FbType::kFT_Cat, 0x000075C2/*MMOCLIENT_REVERSAL_PACKET_MSG_ID_MIAO_MIAO_KEY___*/ },
		{ CD_En_FbType::kFT_QianShou, 0x000075B6/*MMOCLIENT_REVERSAL_PACKET_MSG_ID_QIAN_SHOU_KEY___*/ }
	};

	//npc打开类型
	enum enNpcOpenType{
		kNOT_Normal = 2,
		kNOT_JiaoNaXiang = 0x0a,		//交纳箱
	};

	//副本消费模式
	enum enFbConsumeMode{
		kFCM_ShouLieQuan = 0,		//只使用狩猎券
		kFCM_Silver,				//只使用银币
		kFCM_All,					//先使用狩猎券，再使用银币
	};
	//////////////////////////////////////////////////////////////////////////
}

enum class AcceptTaskType{
	kNormal = 0x01,		//MMOCLIENT_REVERSAL_ACCEPT_TASK_TYPE_PU_TONG___
};

//game object type
enum GameObjType{
	kGOT_Invalid = 0,
	//kGOT_MISC,				//静态对象，副本里的箱子等物品
	kGOT_Npc,
	kGOT_Player,			//玩家
	//kGOT_Unknown,			//掉落物品
	kGOT_Monster,
	kGOT_Door,
	kGOT_Collect,
	kGOT_Static,
	kGOT_StaticBeatable,
	kGOT_AnyObj,			//任意对象
	kGOT_ParseObjType,		//仅用于得到临时游戏对象的type
};
