#pragma once
/*
@author zhp
@date 2014/7/6 19:30
@purpose for all game object base
*/
#include <string>
#include "GClass.h"
#include <memory>
#include <boost/shared_ptr.hpp>
#include <Common/UsefulFunc.h>
#include <Common/GameObjBase.h>
#include <Common/TemplateUtility.h>

//game object type
enum GameObjType{
	kGOT_Invalid = 0,
	kGOT_MISC,				//静态对象，副本里的箱子等物品
	kGOT_Npc,
	kGOT_Player,			//玩家
	kGOT_Unknown,			//掉落物品
	kGOT_Monster,
	kGOT_ParseObjType,		//仅用于得到临时游戏对象的type
};
//game object base
class GameObjBase POD_TO_CLASS_UNWIND(: public GGameObjBase){
	//type definitions
public:
	typedef boost::shared_ptr<GameObjBase> SelfPtrT;
	typedef boost::weak_ptr<GameObjBase>	WeakPtrT;

public:
	GameObjBase( GameObjType gameobj_type ){
		obj_type_ =  gameobj_type;
		game_obj_addr_ = nullptr;
		obj_serial_ = 0;
		cur_hp_ = 0;
		hp_limit_ = 0;
		cur_mp_ = 0;
		mp_limit_ = 0;
		npc_type_9d4_ = 0;
	}
	POD_TO_CLASS_VIRTUAL ~GameObjBase(){}
	bool ConverToJson( Json::Value& out_json ) const{
		out_json["type"] = obj_type_;
		out_json["serial"] = (Json::UInt)obj_serial_;
		out_json["job_name"] = WideCharToGbk(obj_job_name_);
		out_json["obj_name"] = WideCharToGbk(obj_name_);
		out_json["type_in_game"] = WideCharToGbk(game_obj_type_name_);
		out_json["hp"] = (Json::UInt)cur_hp_;
		return true;
	}
	bool ParseFromJson( const Json::Value& json_v ){
		const auto& json_type = json_v["type"];
		if ( json_type.isNull() || !json_type.isIntegral() )
			goto LABEL_ERR;
		const auto& json_serial = json_v["serial"];
		if ( json_serial.isNull() || !json_serial.isIntegral() )
			goto LABEL_ERR;
		const auto& json_job_name = json_v["job_name"];
		if ( json_job_name.isNull() || !json_job_name.isString() )
			goto LABEL_ERR;
		const auto& json_obj_name = json_v["obj_name"];
		if ( json_obj_name.isNull() || !json_obj_name.isString() )
			goto LABEL_ERR;
		const auto& json_type_in_game = json_v["type_in_game"];
		if ( json_type_in_game.isNull() || !json_type_in_game.isString() )
			goto LABEL_ERR;
		const auto& json_hp = json_v["hp"];
		if ( json_hp.isNull() || !json_hp.isIntegral() )
			goto LABEL_ERR;
		obj_type_ = (GameObjType)json_type.asUInt();
		obj_serial_ = json_serial.asUInt();
		obj_job_name_ = GbkToWideChar(json_job_name.asString());
		obj_name_ = GbkToWideChar(json_obj_name.asString());
		game_obj_type_name_ = GbkToWideChar(json_type_in_game.asString());
		cur_hp_ = json_hp.asUInt();
		return true;
LABEL_ERR:
		assert(false);
		return false;
	}
	//设置游戏对象地址
	void SetGameObjAddr( void* obj_addr );
	void* GetGameObjAddr() const;
	//set object type
	void SetObjType( GameObjType obj_type );
	GameObjType GetGameObjType() const;
	//set object serial
	void SetObjSerial( DWORD obj_serial );
	DWORD GetObjSerial() const;
	DWORD GetCurHp() const;
	DWORD GetHpLimit() const;
	DWORD GetCurMp() const;
	DWORD GetMpLimit() const;
	const std::wstring& GetJobName() const;
	const GType::stGameVector& GetPos() const;
	const std::wstring& GetObjName() const;
	//更新数据
	bool Update();
	//更新hp、mp
	bool UpdateHpMp();
	//向下转换
	template<typename DerivedT>
	DerivedT* CastTo(){
		return PointerCast<DerivedT>(this);
	}

protected:
	//通过游戏中的字符串来判定游戏对象属于何种类型
	static GameObjType CalcGameObjType( const std::wstring& game_obj_type_name );
	bool UpdateNpcValidTypeData();

protected:
	GameObjType					obj_type_;
	char*						game_obj_addr_;
	DWORD						obj_serial_;			//id
	std::wstring				obj_job_name_;
	std::wstring				obj_name_;
	GType::stGameVector			obj_pos_;
	std::wstring				game_obj_type_name_;	//游戏中对象的类型
	DWORD						cur_hp_;
	DWORD						hp_limit_;
	DWORD						cur_mp_;
	DWORD						mp_limit_;
	BYTE						npc_type_9d4_;
};

//parse game object type
class GameObj_Parser : public GameObjBase{
public:
	GameObj_Parser();
	bool Update();
	bool ObjValid() const;
	
protected:
	virtual bool CalcObjValid();

private:
	bool			obj_valid_;
};

class GameObj_ParserAny : public GameObj_Parser{
public:
	bool CalcObjValid();
};