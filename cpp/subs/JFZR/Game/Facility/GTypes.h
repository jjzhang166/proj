#pragma once
/*
@author zhp
@date 2014-1-04
@purpose 逆向分析出的游戏中的一些类型、结构等
*/
#include "GAddr.h"
#include <Common/BinaryStream.h>
#include <set>
#include <json/value.h>
#include <Communicate/CG_Defines.h>
#include <map>
#include <Common/CmnUuid.h>

namespace GType{
	class GameSendSocket;
	
	enum enHitType{
		kHT_Unknown = 1,
	};
	typedef char (GameSkillObjT)[JFZR_REVERSAL_SKILL_OBJ_SIZE___];
	struct GameSkillObjPtr{
	private:
		GameSkillObjPtr();
	public:
		GameSkillObjT*			skill_begin_addr_;
		GameSkillObjT*			skill_end_addr_;
	};
	struct stDirection2d{
		WORD				dir_x_;
		WORD				dir_y_;

		stDirection2d(){
			dir_x_ = 0;
			dir_y_ = 0;
		}
		friend StrOutputBStream&
			operator << ( StrOutputBStream& ostm, const stDirection2d& dir ){
				ostm << dir.dir_x_ << dir.dir_y_;
				return ostm;
		}
	};
	//游戏中的向量
	struct stGameVector{
		float			x_;
		float			z_;
		float			y_;

		stGameVector(){
			x_ = 0.;
			z_ = 0.;
			y_ = 0.;
		}
		stGameVector( float x, float y, float z ){
			x_ = x;
			y_ = y;
			z_ = z;
		}
		float GetPosX() const{
			return x_;
		}
		void SetPosX( float x ){
			x_ = x;
		}
		float GetPosY() const{
			return y_;
		}
		void SetPosY( float y ){
			y_ = y;
		}
		float GetPosZ() const{
			return z_;
		}
		void SetPosZ( float z ){
			z_ = z;
		}
		void FromDirection( const stDirection2d& direction ){
			x_ = direction.dir_x_;
			y_ = direction.dir_y_;
			z_ = 0.;
		}
		friend StrOutputBStream&
			operator << ( StrOutputBStream& ostm, const stGameVector& vec ){
				ostm << vec.x_ << vec.z_ << vec.y_;
			return ostm;
		}
	};
	static_assert(sizeof(stGameVector) == sizeof(XYZ), "");

	//装备部位
	enum enEquipPart{
		kEP_Weapon = 0,		//武器，剑斗士武器：MAIN_WEAPON_Warrior,控魔师武器：MAIN_WEAPON_MagicGunner,刃武者武器：MAIN_WEAPON_Thief
		kEP_Head,			//头部
		kEP_Upper,			//上半身
		kEP_Lower,			//下半身
		kEP_Hand,			//手部
		kEP_Foot,			//足部
		kEP_Necklace,		//项链
		kEP_Earring,		//耳环
		kEP_Ring = 8,		//戒指
		//以下是时装,avatar
		kEP_A_Face,			//面具
		kEP_A_Neck,			//带脖子上的
		kEP_A_Back,			//披身上的，披风等
		kEP_A_Lower,		//下面的，腰包等
		kEP_A_Head,			//头饰
		kEP_A_HasNo = 14,	//这个数值游戏里未用到
		kEP_A_Hand,			//戴在手上的
		kEP_Unknonw,
		kEP_Invalid,		//无效部位
	};

	//物品栏种类
	enum enTabCategory{
		kTC_Normal = 0,		//普通栏
		kTC_Consume,		//消耗品栏
		kTC_Task,			//任务栏
		kTC_Material,		//材料栏
		kTC_Avatar,			//时装1,2,3(下面的)
		//上面与游戏中的对应，下面的是自己定义的
		kTC_Equiped,		//装备，穿在身上的
		kTC_AvatarEquiped,	//时装(上面的)，穿在身上的
		kTC_Invalid
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

	//副本难度
	enum enFbDifficulty{
		kFD_Zero = 0,			//城镇中，房间等级为0
		kFD_Normal,				//普通
		kFD_Fight,				//挑战
		kFD_Master,				//大师
		kFD_Overmaster,			//征服
		kFD_OrangeFb = 9,			//橙本
		kFD_Invalid,
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
		kGS_SelectChannel,
		kGS_SelectRole,
		kGS_InGame,			//角色已进入游戏
	};
	//物品等级，物品品质
	enum enItemQuality{
		kIQ_Invalid = 0,
		kIQ_Low,
		kIQ_Middle,
		kIQ_High,
		kIQ_Rare,		//罕见的
		kIQ_Legendary,	//传说的
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

	struct stProductInfo{
		LONGLONG		serial_;
		LONGLONG		price_;
		int				item_cnt_;

		stProductInfo();
		bool FromJson( const Json::Value& json_v );
		bool ConverToJson( Json::Value& json_v ) const;

		friend bool operator < ( const stProductInfo& lhs, const stProductInfo& rhs ){
			return lhs.serial_ < rhs.serial_;
		}
	};
	typedef std::set<stProductInfo> ProductInfoContT;
	typedef std::map<CmnUuid, ProductInfoContT> RoleProductInfoContT;
	bool IsPrivateProduct( LONGLONG price );
	enGameAcType Price2GameAcType( LONGLONG price );
	LONGLONG MakePriceByAcType( LONGLONG price, enGameAcType game_ac_type );
}