#pragma once
/*
@author zhp
@date 2014/7/1 22:31
@purpose room of clone map
*/
#include <Common/GameObjBase.h>
#include <Common/UsefulClass.h>
#include <string>
#include "GTypes.h"
#include <json/value.h>
#include <targetver.h>
#include "GClass.h"

class GPlayerSelf;
class GRoomDoor POD_TO_CLASS_UNWIND(: public GClass::BTreeBase){
public:
	bool ConverToJson( Json::Value& out_json ) const{
		out_json["dn"] = WideCharToGbk(door_name_);
		out_json["x"] = door_pos_.x_;
		out_json["y"] = door_pos_.y_;
		out_json["z"] = door_pos_.z_;
		return true;
	}
	bool ParseFromJson( const Json::Value& json_v ){
		const auto& json_dn = json_v["dn"];
		if ( json_dn.isNull() || !json_dn.isString() )
			goto LABLE_ERR;
		const auto& json_x = json_v["x"];
		if ( json_x.isNull() || !json_x.isDouble() )
			goto LABLE_ERR;
		const auto& json_y = json_v["y"];
		if ( json_y.isNull() || !json_y.isDouble() )
			goto LABLE_ERR;
		const auto& json_z = json_v["z"];
		if ( json_z.isNull() || !json_z.isDouble() )
			goto LABLE_ERR;
		door_name_ = GbkToWideChar(json_dn.asString());
		door_pos_.x_ = (float)json_x.asDouble();
		door_pos_.y_ = (float)json_y.asDouble();
		door_pos_.z_ = (float)json_z.asDouble();
		return true;
LABLE_ERR:
		assert(false);
		return false;
	}
	const std::wstring& GetGateName() const;
	const GType::stGameVector& GetGatePos() const;
	const void* GetDoorObjAddr() const;

	//interface
public:
	bool Update();

protected:
	std::wstring				door_name_;
	GType::stGameVector			door_pos_;
};

class GDoorsMgr : public GClass::BTreeMgr_SkipRoot<GRoomDoor>{};

class GRoomBase POD_TO_CLASS_UNWIND(: public GGameObjBase){
public:
	GRoomBase(){
		room_obj_base_ = 0;
	}
	bool ConverToJson( Json::Value& out_json ) const{
		/*out_json["mapn"] = WideCharToGbk(room_name_);
		out_json["mapid"] = WideCharToGbk(room_id_);
		Json::Value json_tmp, json_array;
		for ( auto i = 0; i != _countof(doors_); ++i )
		{
			if ( !doors_[i].ConverToJson(json_tmp) )
			{
				assert(false);
				return false;
			}
			json_array.append(json_tmp);
		}
		out_json["doors"] = json_array;*/
		return true;
	}
	bool ParseFromJson( const Json::Value& json_v ){
		/*const auto& json_mapn = json_v["mapn"];
		if ( json_mapn.isNull() || !json_mapn.isString() )
			goto LABLE_ERR;
		const auto& json_mapid = json_v["mapid"];
		if ( json_mapid.isNull() || !json_mapid.isString() )
			goto LABLE_ERR;
		const auto& json_door_array = json_v["doors"];
		if ( json_door_array.isNull() || !json_door_array.isArray() )
			goto LABLE_ERR;
		if ( json_door_array.size() != _countof(doors_) )
			goto LABLE_ERR;
		room_name_ = GbkToWideChar(json_mapn.asString());
		room_id_ = GbkToWideChar(json_mapid.asString());
		for ( auto i = 0; i != _countof(doors_); ++i )
		{
			if ( !doors_[i].ParseFromJson(json_door_array[i]) )
				goto LABLE_ERR;
		}
		return true;
LABLE_ERR:
		assert(false);
		return false;
		*/
	}

	//interface
public:
	bool Update();
	GRoomDoor& GetTheDoor( int door_idx );
	const std::wstring& GetRoomId() const;
	bool UnsameDoorCheck( int door_idx ) const;
	GRoomDoor* GetDoorExcept( const std::vector<std::wstring>& door_name );
	GRoomDoor* GetDoorById( const std::wstring& wrap_gate_id );

private:
	bool DoUpdateImpl();

	//static
public:
	//static const int	 kMaxDoorCnt = 3;

protected:
	std::wstring			room_name_;
	std::wstring			room_id_;
	//GRoomDoor				doors_[kMaxDoorCnt];
	POD_TO_CLASS_UNWIND(GClass::TimerSyncExec	timer_sync_exec_;)
	int						room_obj_base_;
	GDoorsMgr				the_doors_;
};

class GTownRoom : public GRoomBase, public Singleton<GTownRoom, Singleton_MakeMe>{
public:
	bool Update();
};